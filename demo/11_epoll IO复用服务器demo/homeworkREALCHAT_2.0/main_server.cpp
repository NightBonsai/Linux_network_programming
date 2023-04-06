#include <iostream>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包
#include <unistd.h>
#include <pthread.h>	//线程函数库
#include <string.h>
#include <sys/epoll.h>

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
		cout << "网络初始化成功：socketfd = " << socketfd << endl;

		addr.sin_family = AF_INET;			//网络连接协议族 IPv4
		addr.sin_addr.s_addr = INADDR_ANY;	//服务器IP地址不会轻易变动，开放服务器IP和端提供给客户端主动来连接
											//系统自动获取本地联网IP
		addr.sin_port = htons(10006);		//开放服务器端口，提供给客户端来主动连接，因为内存保存数据有大小端区分，所以用htons转换	

		int len = sizeof(addr);

		//端口复用—避免出现address already be used
		int optval = 1;
		setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(optval));

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


		struct epoll_event epollEvent;				//epoll事件结构体
		bzero(&epollEvent, sizeof(epollEvent));
		struct epoll_event epollEventArr[5];		//epollEventArr用来保存已经触发事件的fd

		//创建epoll
		int epollfd = epoll_create(5);

		epollEvent.data.fd = socketfd;	//设置网络搭建成功socketfd
		epollEvent.events = EPOLLIN;	//网络搭建成功socketfd可能会发送的EPOLLIN事件

		//epoll事件队列中需要添加网络通道socketfd，有客户端上线才会有事件发生
		epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &epollEvent);

		while (true)
		{
			cout << "epoll wait..." << endl;

			//epoll_wait阻塞函数
			//epollWaitNum已经发生的事件个数
			int epollWaitNum = epoll_wait(epollfd, epollEventArr, 5, -1);

			if (0 > epollWaitNum)
			{
				perror("epoll_wait error");
			}

			for (int i = 0; i < epollWaitNum; i++)
			{
				if (epollEventArr[i].data.fd == socketfd)
				{
					cout << "有客户端来连接..." << endl;

					//阻塞函数：等待用户连接
					int acceptfd = accept(socketfd, NULL, NULL);
					cout << "客户端连接成功：" << acceptfd << endl;

					bzero(&epollEvent, sizeof(epollEvent));

					epollEvent.data.fd = acceptfd;	//设置完成了三次握手的acceptfd
					epollEvent.events = EPOLLIN;	//网络搭建成功socketfd可能会发送的EPOLLIN事件

					epoll_ctl(epollfd, EPOLL_CTL_ADD, acceptfd, &epollEvent);
				}
				else if (epollEventArr[i].data.fd & EPOLLIN)
				{
					cout << "有事件发生，但不是socketfd，是客户端" << endl;

					char resBuf[200] = { 0 };
					int res = read(epollEventArr[i].data.fd, resBuf, sizeof(resBuf));
					if (0 < res)
					{
						cout << "是客户端数据发来的" << endl;
					}
					else
					{
						cout << "是客户端下线..." << endl;

						bzero(&epollEvent, sizeof(epollEvent));

						epollEvent.data.fd = epollEventArr[i].data.fd;	//设置 发生了事件 但不是socketfd 也没有读到数据的fd
						epollEvent.events = EPOLLIN;					//网络搭建成功socketfd可能会发送的EPOLLIN事件

						//从epoll中删除
						epoll_ctl(epollfd, EPOLL_CTL_DEL, epollEventArr[i].data.fd, &epollEvent);
					}
				}
			}
		}
	}

	return 0;
}