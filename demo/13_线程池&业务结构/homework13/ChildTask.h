#pragma once

#include <iostream>
#include <unistd.h>
#include "BaseTask.h"
using namespace std;

class ChildTask :
    public BaseTask
{
public:
    ChildTask(char* data);
    ~ChildTask();

    void work();
};

