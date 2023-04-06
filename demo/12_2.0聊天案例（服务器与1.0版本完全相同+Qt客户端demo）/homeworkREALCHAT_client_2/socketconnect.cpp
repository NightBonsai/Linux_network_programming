#include "socketconnect.h"

//静态数据成员初始化
SocketConnect* SocketConnect::socketInstance = nullptr;
SocketConnect::SocketConnect()
{
    //网络初始化             协议族	TCP协议		默认
    this->socketfd = socket(AF_INET,SOCK_STREAM,0);
    this->len = sizeof(addr);
}

SocketConnect::~SocketConnect()
{

}

SocketConnect *SocketConnect::getInstance()
{
    //懒汉模式
    if(nullptr == SocketConnect::socketInstance)
    {
        SocketConnect::socketInstance = new SocketConnect();
    }
    return SocketConnect::socketInstance;
}

void SocketConnect::creatConnect()
{
    if(-1 == socketfd)
    {
        perror("socket error");
    }
    else
    {
        addr.sin_family = AF_INET;							//网络连接协议族 IPv4
        addr.sin_addr.s_addr = inet_addr("192.168.9.128");	//客户端：查找服务器IP		inet_ addr()能够把一个用数字和点表示IP地址的字符串转换成一个无符号长整型
        addr.sin_port = htons(10086);						//客户端：查找服务器端口	主机字节顺序转换为网络字节顺序(对无符号短型进行操作2bytes)

        this->len = sizeof(addr);

        //客户端连接服务器
        if(-1 == connect(socketfd,(struct sockaddr*)&addr,len))
        {
            perror("connect error");
        }
    }
}

void SocketConnect::closeConnect()
{
    if(-1 == socketfd)
    {
        perror("socket error");
    }
    else
    {
        //断开客户端连接
        close(socketfd);
    }
}

int SocketConnect::getSocketfd() const
{
    return socketfd;
}

void SocketConnect::setSocketfd(int value)
{
    socketfd = value;
}
