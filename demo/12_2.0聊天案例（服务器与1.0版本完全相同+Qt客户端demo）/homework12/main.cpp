#include <iostream>
#include "TCPServer.h"
using namespace std;

int main()
{
	//****************************时间赶，没做完*****************************
	TCPServer* tcpServer = new TCPServer(10086);
	tcpServer->startConnect();

	return 0;
}