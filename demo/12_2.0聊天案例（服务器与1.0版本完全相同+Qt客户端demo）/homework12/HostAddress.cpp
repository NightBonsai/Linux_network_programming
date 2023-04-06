#include "HostAddress.h"

HostAddress::HostAddress(unsigned short port)
{
	//初始化
	this->port = port;

	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = INADDR_ANY;
	this->addr.sin_port = htons(this->port);

	this->length = sizeof(this->addr);
}

HostAddress::~HostAddress()
{
}

sockaddr_in HostAddress::getAddr_in()
{
	return this->addr;
}

sockaddr* HostAddress::getAddr()
{
	return (struct sockaddr*)&(this->addr);
}

unsigned short HostAddress::getPort()
{
	return this->port;
}

void HostAddress::setPort(unsigned short port)
{
	this->port = port;
}

int HostAddress::getLength()
{
	return this->length;
}
