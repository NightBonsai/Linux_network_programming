#include <iostream>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包
#include <unistd.h>
#include <pthread.h>	//线程函数库
#include <string.h>
#include <map>
using namespace std;

//************************************昨晚讲了很多，但今天只写了这么多，还是有些地方具体实现不太清楚*********************************
//传输聊天数据协议
typedef struct chatProtocol
{
	int send;		//发送方ID
	int recv;		//接收方ID
	char data[100];	//要发送的数据
}CHATP;
CHATP resBuf;	//接收数据

void* getChat(void* arg);	//线程启动执行函数：获取聊天内容
void* sendChat(void* arg);	//线程启动执行函数：发送聊天内容

int main()
{
	struct sockaddr_in addr;

	//网络初始化：判断是否可以搭建网络
	//协议族	TCP协议		默认
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > socketfd)
	{
		perror("socket error");
	}
	else
	{
		cout << "网络初始化成功：socketfd = " << socketfd << endl;

		addr.sin_family = AF_INET;			//网络连接协议族 IPv4
		addr.sin_addr.s_addr = INADDR_ANY;	//服务器IP地址不会轻易变动，开放服务器IP和端提供给客户端主动来连接
											//系统自动获取本地联网IP
		addr.sin_port = htons(10001);		//开放服务器端口，提供给客户端来主动连接，因为内存保存数据有大小端区分，所以用htons转换	

		int len = sizeof(addr);

		//绑定端口和IP地址
		if (-1 == bind(socketfd, (struct sockaddr*)&addr, len))
		{
			perror("bind error");
		}
		//监听是否有用户上线
		if (-1 == listen(socketfd, 10))
		{
			perror("listen error");
		}
		cout << "网络搭建成功" << endl;

		while (true)
		{
			//阻塞函数：等待用户连接
			int acceptfd = accept(socketfd, NULL, NULL);
			if (-1 != acceptfd)
			{
				cout << "用户连接成功：" << acceptfd << endl;
			}

			//登录验证

			//创建接收聊天线程
			pthread_t getChatThreadID;
			pthread_create(&getChatThreadID, NULL, getChat, &acceptfd);

			//创建发送聊天线程
			pthread_t sendChatThreadID;
			pthread_create(&sendChatThreadID, NULL, sendChat, &socketfd);
		}
	}

	return 0;
}

//线程启动执行函数：获取聊天内容
void* getChat(void* arg)
{
	int acceptfd = *((int*)arg);

	while (true)
	{
		int res = read(acceptfd, &resBuf, sizeof(resBuf));
		cout << "服务器接收数据 发送方：" << resBuf.send << endl;
	}
}

//线程启动执行函数：发送聊天内容
void* sendChat(void* arg)
{
	int socketfd = *((int*)arg);

	while (true)
	{
		int res = write(resBuf.recv, &(resBuf.data), sizeof(resBuf.data));
		cout << "服务器发送数据 接收方：" << resBuf.recv << endl;
		
		//初始化
		memset(&resBuf, 0, sizeof(resBuf));
	}
}