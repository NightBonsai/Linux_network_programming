#pragma once

#include <string.h>

//业务基类
class BaseTask
{
public:
	BaseTask(char* data);
	virtual~BaseTask();			//虚析构函数——执行子类析构时会执行父类析构

	virtual void work() = 0;	//纯虚函数——具体业务子类实现
protected:
	char data[1024];			//业务数据
};

