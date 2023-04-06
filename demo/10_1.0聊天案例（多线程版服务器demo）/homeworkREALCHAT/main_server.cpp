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

#include "protocol.h"	//自定义协议
using namespace std;

//服务器端线程执行函数
void* pthread_function(void* arg);

//用户登录信息—map键值不能为指针
map<int, int>onlineMap;	

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
		addr.sin_port = htons(10006);		//开放服务器端口，提供给客户端来主动连接，因为内存保存数据有大小端区分，所以用htons转换	

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
			cout << "客户端连接成功：" << acceptfd << endl;

			//开启客户端线程
			pthread_t threadID;
			pthread_create(&threadID, NULL, pthread_function, &acceptfd);
		}
	}

	return 0;
}

//服务器端线程执行函数
void* pthread_function(void* arg)
{
	int fd = *((int*)arg);
	cout << "线程里客户端 acceptfd = " << fd << endl;

	while (true)
	{
		//读入客户端发送协议包
		//读取协议头
		char buf[200] = { 0 };
		int rres = read(fd, buf, sizeof(HEAD));
		cout << "服务器读入协议头 rres = " << rres << endl;

		HEAD head;
		memcpy(&head, buf, sizeof(HEAD));
		cout << "服务器读入协议头 head.businessType = " << head.businessType << endl;
		cout << "服务器读入协议头 head.businessLength = " << head.businessLength << endl;

		//根据协议头判断业务类型
		if (LOG_B == head.businessType)			//若为登录业务
		{
			//读取协议体
			rres = read(fd, buf, head.businessLength);
			cout << "服务器读入协议体 rres = " << rres << endl;

			USER user;
			memcpy(&user, buf, sizeof(USER));
			cout << "服务器读入协议体 user.name = " << user.name << endl;
			cout << "服务器读入协议体 user.pass = " << user.pass << endl;

			//数据库查询进行登录验证
			//保存成功登录用户信息
			onlineMap[user.name] = fd;
			cout << "在线用户人数 onlineMap.size = " << onlineMap.size() << endl;

			//发送返回协议包给客户端
			//设置返回协议体
			BACKMSG back;
			back.flag = LOG_SUCCESS;
			strcpy(back.message, "登录成功");

			//设置协议头
			head.businessLength = sizeof(BACKMSG);

			//生成协议包
			char wbuf[200] = { 0 };
			memcpy(wbuf, &head, sizeof(HEAD));						//添加协议头
			memcpy(wbuf + sizeof(HEAD), &back, sizeof(BACKMSG));	//协议头后添加协议体

			//写出，数据返回给客户端
			int wres = write(fd, wbuf, sizeof(HEAD) + sizeof(BACKMSG));	//发送有效长度的数据给服务器端
			cout << "服务器写出数据 wres = " << wres << endl;

		}
		else if (CHAT_B == head.businessType)	//若为聊天业务
		{
			//读取协议体
			rres = read(fd, buf, head.businessLength);
			cout << "服务器读入协议体 rres = " << rres << endl;

			CHATMSG msg;
			memcpy(&msg, buf, sizeof(CHATMSG));
			cout << "服务器读入协议体 msg.sendID = " << msg.sendID << endl;
			cout << "服务器读入协议体 msg.recvID = " << msg.recvID << endl;
			cout << "服务器读入协议体 msg.context = " << msg.context << endl;

			//发送聊天协议包给对应接收客户端
			//生成协议包
			char wbuf[200] = { 0 };
			memcpy(wbuf, &head, sizeof(HEAD));
			memcpy(wbuf + sizeof(HEAD), &msg, sizeof(CHATMSG));

			//写出，数据发给对应发送和接收客户端
			int wres = write(fd, wbuf, sizeof(HEAD) + sizeof(CHATMSG));
			cout << "客户端发给发送者 wres = " << wres << endl;
			wres = write(onlineMap[msg.recvID], wbuf, sizeof(HEAD) + sizeof(CHATMSG));
			cout << "客户端发给接收者 wres = " << wres << endl;
			
		}
		//数据初始化
		bzero(buf, sizeof(buf));
		bzero(&head, sizeof(HEAD));
	}
}