#include <iostream>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包
#include <unistd.h>
using namespace std;

int main()
{
	struct sockaddr_in addr;

	//网络初始化：判断是否可以搭建网络
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

			pid_t pid = fork();
			if (0 == pid)
			{
				//子进程
				while (true)
				{
					char resBuf[50] = { 0 };
					int res = read(acceptfd, resBuf, sizeof(resBuf));
					cout << "服务器接收长度 res = " << res << endl;
					cout << "客户端 acceptfd = " << acceptfd << " 发送数据 resBuf = " << resBuf << endl;
				}
			}
		}
	}

	return 0;
}