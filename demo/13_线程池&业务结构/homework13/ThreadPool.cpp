#include "ThreadPool.h"

ThreadPool::ThreadPool(const int num)
{
	//初始化
	this->threadMinNum = num;

	pthread_mutex_init(&this->mutexLock, NULL);	//互斥量锁
	pthread_cond_init(&this->cond, NULL);		//线程条件变量
	for (int i = 0; i < this->threadMinNum; i++)
	{
		pthread_t id;
		pthread_create(&id, NULL, run, this);	//创建线程：类指针作为参数传入

		this->idleList.push_back(id);			//线程加入空闲链表中
	}
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::addTask(BaseTask* task)
{
	this->lock();				//关锁

	this->taskQueue.push(task);	//任务队列内添加业务

	this->unlock();				//解锁
	this->wakeUp();				//唤醒线程
}

BaseTask* ThreadPool::popTask()
{
	BaseTask* task = this->taskQueue.front();	//获取任务队列队头业务
	this->taskQueue.pop();						//销毁任务队列队头业务

	return task;				//返回获取业务
}

void ThreadPool::moveToIdle(pthread_t id)
{
	list<pthread_t>::iterator iter;

	//检索对应要操作的线程
	iter = find(this->busyList.begin(), this->busyList.end(), id);
	if (iter != this->busyList.end())
	{
		this->busyList.erase(iter);			//忙碌链表删除对应线程
		this->idleList.push_back(*iter);	//空闲链表添加对应线程
	}
}

void ThreadPool::moveToBusy(pthread_t id)
{
	list<pthread_t>::iterator iter;

	//检索对应要操作的线程
	iter = find(this->idleList.begin(), this->idleList.end(), id);
	if (iter != this->idleList.end())
	{
		this->idleList.erase(iter);			//空闲链表删除对应线程
		this->busyList.push_back(*iter);	//忙碌链表添加对应线程
	}
}

bool ThreadPool::queueIsEmpty()
{
	return this->taskQueue.empty();
}

void ThreadPool::lock()
{
	pthread_mutex_lock(&this->mutexLock);
}

void ThreadPool::unlock()
{
	pthread_mutex_unlock(&this->mutexLock);
}

void ThreadPool::wait()
{
	pthread_cond_wait(&this->cond, &this->mutexLock);
}

void ThreadPool::wakeUp()
{
	pthread_cond_signal(&this->cond);
}

void* ThreadPool::run(void* arg)
{
	//确保主线程与当前执行线程逻辑完全分离，当前执行线程结束后，id会自动释放
	//分离目的：为了声明这个线程不会阻塞主线程的逻辑，pthread_detach不会终止当前执行线程
	pthread_t id = pthread_self();
	pthread_detach(id);

	//接收传入的类指针
	ThreadPool* threadPool = (ThreadPool*)arg;

	while (true)
	{
		threadPool->lock();		//关锁

		while (threadPool->queueIsEmpty())	//若任务队列一直为空，没有业务要处理
		{
			threadPool->wait();				//线程一直阻塞休眠
		}

		cout << "业务执行前	任务数：" << threadPool->taskQueue.size() << endl;
		cout << "业务执行前	busy：" << threadPool->busyList.size() << endl;
		cout << "业务执行前	idle：" << threadPool->idleList.size() << endl;
		cout << "------------------------------------------------" << endl;

		//执行业务线程传入忙碌链表
		threadPool->moveToBusy(id);
		BaseTask* task = threadPool->popTask();

		threadPool->unlock();	//解锁

		//开始工作
		task->work();

		threadPool->lock();

		//执行业务线程传入空闲链表
		threadPool->moveToIdle(id);	

		threadPool->unlock();

		cout << "业务执行后	任务数：" << threadPool->taskQueue.size() << endl;
		cout << "业务执行后	busy：" << threadPool->busyList.size() << endl;
		cout << "业务执行后	idle：" << threadPool->idleList.size() << endl;
		cout << "------------------------------------------------" << endl;
	}
}
