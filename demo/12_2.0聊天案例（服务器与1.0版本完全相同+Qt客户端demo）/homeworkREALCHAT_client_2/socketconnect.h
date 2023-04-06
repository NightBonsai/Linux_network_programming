#ifndef SOCKETCONNECT_H
#define SOCKETCONNECT_H

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>         //sockaddr_in包
#include <arpa/inet.h>          //sockaddr_in包
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//网络连接单例类
class SocketConnect
{
public:
    static SocketConnect* getInstance();    //外部接口——获取该类唯一实例

    void creatConnect();                    //客户端连接服务器
    void closeConnect();                    //关闭客户端连接
    int getSocketfd() const;
    void setSocketfd(int value);
private:
    SocketConnect();                        //私有构造函数——保证该类只有一个实例——网络初始化
    ~SocketConnect();

    static SocketConnect* socketInstance;   //唯一实例

    struct sockaddr_in addr;
    int socketfd;
    int len;
};

#endif // SOCKETCONNECT_H
