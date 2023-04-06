#include "BaseTask.h"

BaseTask::BaseTask(char* data)
{
	bzero(this->data, sizeof(this->data));
	mempcpy(this->data, data, sizeof(this->data));
}

BaseTask::~BaseTask()
{
	delete this->data;
}
