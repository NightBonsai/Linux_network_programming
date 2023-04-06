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
	//协议族	TCP协议		默认
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > socketfd)
	{
		perror("socket error");
	}
	else
	{
		addr.sin_family = AF_INET;							//网络连接协议族 IPv4
		addr.sin_addr.s_addr = inet_addr("192.168.9.128");	//客户端：查找服务器IP
		addr.sin_port = htons(10001);						//客户端：查找服务器端口

		int len = sizeof(addr);

		//客户端连接服务器
		if (-1 == connect(socketfd, (struct sockaddr*)&addr, len))
		{
			perror("connect error");
		}
		else
		{
			cout << "客户端连接服务器成功" << endl;
			while (true)
			{
				char buf[50] = { 0 };

				cin >> buf;
				int res = write(socketfd, buf, sizeof(buf));
				cout << "发送长度 res = " << res << endl;

			}
		}
	}

	return 0;
}