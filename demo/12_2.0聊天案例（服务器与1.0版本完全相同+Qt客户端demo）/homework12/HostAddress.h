#pragma once

#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包

//地址类
class HostAddress
{
public:
	HostAddress(unsigned short port);
	~HostAddress();

	struct sockaddr_in getAddr_in();
	struct sockaddr* getAddr();
	unsigned short getPort();
	void setPort(unsigned short port);
	int getLength();

private:
	struct sockaddr_in addr;	//IP地址、协议族
	unsigned short port;		//端口号
	int length;
};

