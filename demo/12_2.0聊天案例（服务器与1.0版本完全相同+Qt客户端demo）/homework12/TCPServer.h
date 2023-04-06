#pragma once

#include "BaseSocket.h"
#include "HostAddress.h"
#include <iostream>
#include <unistd.h>

#define SOCKET_MAX_NUM 10

using namespace std;

//TCP服务器类
class TCPServer :
    public BaseSocket
{
public:
    TCPServer(unsigned short port);
    ~TCPServer();

    //纯虚函数重定义
    void stopConnect(); //断开连接
    void run();         //

    HostAddress* getAddress();
    void setAddress(HostAddress* address);
private:
    HostAddress* address;   //网络地址
};

