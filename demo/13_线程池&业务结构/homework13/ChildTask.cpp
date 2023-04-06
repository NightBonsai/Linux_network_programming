#include "ChildTask.h"

ChildTask::ChildTask(char* data):BaseTask(data)
{
}

ChildTask::~ChildTask()
{
}

void ChildTask::work()
{
	cout << "业务执行中......" << this->data << endl;
	sleep(3);
}
