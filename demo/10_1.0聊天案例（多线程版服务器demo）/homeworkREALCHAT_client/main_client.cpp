#include <iostream>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包
#include <unistd.h>
#include <pthread.h>	//线程函数库
#include <string.h>

#include "protocol.h"	//自定义协议
using namespace std;

void* readThread(void* arg);	//读线程
void* writeThread(void* arg);	//写线程

bool isLogin = false;			//标志位—判断客户端是否已经登录
int userID = 0;					//客户端用户ID

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
		addr.sin_family = AF_INET;							//网络连接协议族 IPv4
		addr.sin_addr.s_addr = inet_addr("192.168.9.128");	//客户端：查找服务器IP		inet_ addr()能够把一个用数字和点表示IP地址的字符串转换成一个无符号长整型
		addr.sin_port = htons(10006);						//客户端：查找服务器端口	主机字节顺序转换为网络字节顺序(对无符号短型进行操作2bytes)

		int len = sizeof(addr);

		//客户端连接服务器
		if (-1 == connect(socketfd, (struct sockaddr*)&addr, len))
		{
			perror("connect error");
		}
		else
		{
			cout << "客户端连接服务器成功" << endl;
			
			//开启读线程和写线程
			pthread_t readThreadID;
			pthread_t writeThreadID;
			pthread_create(&readThreadID, NULL, readThread, &socketfd);
			pthread_create(&writeThreadID, NULL, writeThread, &socketfd);

			//死循环，防止创建线程销毁
			while (true) {}
		}
	}

	return 0;
}

void* readThread(void* arg)
{
	int fd = *((int*)arg);
	cout << "读线程里 socketfd = " << fd << endl;

	while (true)
	{
		//读入客户端发送协议包
		//读取协议头
		char buf[200] = { 0 };
		int rres = read(fd, buf, sizeof(HEAD));
		cout << "客户端读入协议头 rres = " << rres << endl;

		HEAD head;
		memcpy(&head, buf, sizeof(HEAD));
		cout << "客户端读入协议头 head.businessType = " << head.businessType << endl;
		cout << "客户端读入协议头 head.businessLength = " << head.businessLength << endl;

		//根据协议头判断业务类型i
		if (LOG_B == head.businessType)				//若为登录业务
		{
			//读取协议体
			rres = read(fd, buf, sizeof(BACKMSG));
			cout << "客户端读入协议体 rres = " << rres << endl;

			BACKMSG back;
			memcpy(&back, buf, sizeof(BACKMSG));
			cout << "客户端读入协议体 back.flag = " << back.flag << endl;
			cout << "客户端读入协议体 back.message = " << back.message << endl;

			//修改标志位—登录成功
			isLogin = true;
		}
		else if (CHAT_B == head.businessType)		//若为聊天业务
		{
			//读取协议体
			rres = read(fd, buf, sizeof(CHATMSG));
			cout << "客户端读入协议体 rres = " << rres << endl;

			CHATMSG msg;
			memcpy(&msg, buf, sizeof(CHATMSG));
			cout << "客户端读入协议体 msg.sendID = " << msg.sendID << endl;
			cout << "客户端读入协议体 msg.recvID = " << msg.recvID << endl;
			cout << "客户端读入协议体 msg.context = " << msg.context << endl;

			if (msg.sendID == userID)
			{
				cout << "我说：" << msg.context << endl;
			}
			else
			{
				cout << msg.sendID << " 对我说：" << msg.context << endl;
			}
		}
	}
}

void* writeThread(void* arg)
{
	int fd = *((int*)arg);
	cout << "写线程里 socketfd = " << fd << endl;

	while (true)
	{
		if (false == isLogin)		//若客户端未登录
		{
			//登录业务
			//设置登录协议体
			USER user;
			cout << "请输入账号：" << endl;
			cin >> user.name;
			userID = user.name;
			cout << "请输入密码：" << endl;
			cin >> user.pass;

			//设置协议头
			HEAD head;
			head.businessType = LOG_B;			//业务类型
			head.businessLength = sizeof(USER);	//协议体长度

			//生成协议包
			char buf[200] = { 0 };
			memcpy(buf, &head, sizeof(HEAD));					//添加协议头
			memcpy(buf + sizeof(HEAD), &user, sizeof(USER));	//协议头后添加协议体

			//写出，数据发送给服务器端
			int wres = write(fd, buf, sizeof(HEAD) + sizeof(USER));	//发送有效长度的数据给服务器端
			cout << "客户端写出数据 wres = " << wres << endl;
		}
		else 						//若客户端已登录
		{
			//聊天业务
			//设置聊天协议体
			CHATMSG msg;
			msg.sendID = userID;
			cout << "请输入发送对象ID：" << endl;
			cin >> msg.recvID;
			cout << "请输入聊天内容：" << endl;
			cin >> msg.context;

			//设置协议头
			HEAD head;
			head.businessType = CHAT_B;				//业务类型
			head.businessLength = sizeof(CHATMSG);	//协议体长度

			//生成协议包
			char buf[200] = { 0 };
			memcpy(buf, &head, sizeof(HEAD));					//添加协议头
			memcpy(buf + sizeof(HEAD), &msg, sizeof(CHATMSG));	//协议头后添加协议体

			//写出，数据发送给服务器端
			int wres = write(fd, buf, sizeof(HEAD) + sizeof(CHATMSG));	//发送有效长度的数据给服务器端
			cout << "客户端写出数据 wres = " << wres << endl;
		}

		sleep(3);
	}
}