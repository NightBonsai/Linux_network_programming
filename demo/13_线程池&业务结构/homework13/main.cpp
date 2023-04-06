#include <stdio.h>
#include "ThreadPool.h"
#include "ChildTask.h"

int main()
{
	ThreadPool* threadPool = new ThreadPool();	//线程池

	for (int i = 0; i < 30; i++)
	{
		char buf[40] = { 0 };
		sprintf(buf, "%s%d", "任务", i);

		BaseTask* task = new ChildTask(buf);	//业务

		threadPool->addTask(task);				//线程池添加业务
	}
	
	while (true) {}

	return 0;
}