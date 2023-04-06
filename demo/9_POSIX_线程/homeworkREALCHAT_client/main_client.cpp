#include <iostream>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包
#include <unistd.h>
#include <pthread.h>	//线程函数库
using namespace std;

//传输聊天数据协议
typedef struct chatProtocol
{
	int send;		//发送方ID
	int recv;		//接收方ID
	char data[100];	//要发送的数据
}CHATP;

void* setChat(void* arg);	//线程启动执行函数：写入聊天内容
void* getChat(void* arg);	//线程启动执行函数：读出聊天记录

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
		addr.sin_port = htons(10001);						//客户端：查找服务器端口	主机字节顺序转换为网络字节顺序(对无符号短型进行操作2bytes)

		int len = sizeof(addr);

		//客户端连接服务器
		if (-1 == connect(socketfd, (struct sockaddr*)&addr, len))
		{
			perror("connect error");
		}
		else
		{
			cout << "客户端连接服务器成功" << endl;
			
			//登录验证

			//创建发送聊天线程
			pthread_t sendChatThreadID;
			pthread_create(&sendChatThreadID, NULL, setChat, &socketfd);

			//创建接收聊天线程
		}
	}

	return 0;
}

//线程启动执行函数：写入聊天内容
void* setChat(void* arg)
{
	int socketfd = *((int*)arg);
	CHATP buf;

	buf.send = socketfd;				//确定发送方ID
	cout << "输入聊天对象ID" << endl;
	cin >> buf.recv;					//输入接收方ID
	cout << "输入聊天内容" << endl;
	cin >> buf.data;					//写入聊天内容
	int res = write(socketfd, &buf, sizeof(buf));
	cout << "发送长度 res = " << res << endl;
}

//线程启动执行函数：读出聊天记录
void* getChat(void* arg)
{
	
}