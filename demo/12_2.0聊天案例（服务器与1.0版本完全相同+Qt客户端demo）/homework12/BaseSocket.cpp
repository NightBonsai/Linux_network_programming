#include "BaseSocket.h"

BaseSocket::BaseSocket(unsigned short port)
{
	//初始化
	this->socketfd = 0;
}

BaseSocket::~BaseSocket()
{
}

void BaseSocket::startConnect()
{
	this->socketfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (0 > this->socketfd)
	{
		perror("socket error");
	}
	else
	{
		this->run();
	}
}

int BaseSocket::getSocketfd()
{
	return this->socketfd;
}
