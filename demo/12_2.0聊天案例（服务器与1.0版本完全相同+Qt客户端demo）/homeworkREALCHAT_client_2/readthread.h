#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <QDebug>
#include <unistd.h>
#include "protocol.h"

//读线程单例类
class ReadThread : public QThread
{
    Q_OBJECT
public:
    static ReadThread *getInstance();

    void run();     //线程run函数重定义

    void setSocketfd(int value);
private:
    ReadThread();
    ~ReadThread();

    static ReadThread* readThread;

    int socketfd;       //网络文件描述符
    HEAD head;          //读到的协议头
    USER user;          //读到的协议体
    CHATMSG chatMsg;
    BACKMSG backMsg;
signals:
    void send2ChatWin(CHATMSG *chatMsg);    //将聊天内容发送给聊天窗口
    void send2LogWin(int flag);         //将登录成功信息发给登录窗口
public slots:

};

#endif // READTHREAD_H
