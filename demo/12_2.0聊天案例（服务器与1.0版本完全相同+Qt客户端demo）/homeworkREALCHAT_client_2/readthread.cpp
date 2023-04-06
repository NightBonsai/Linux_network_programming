#include "readthread.h"

ReadThread *ReadThread::readThread = nullptr;
ReadThread::ReadThread()
{

}

ReadThread::~ReadThread()
{

}

ReadThread *ReadThread::getInstance()
{
    if(nullptr == ReadThread::readThread)
    {
        ReadThread::readThread = new ReadThread();
    }
    return ReadThread::readThread;
}

void ReadThread::run()
{
    while (true)
    {
        //读入协议包
        //读取协议头
        int rres = read(this->socketfd, &head, sizeof(HEAD));
        qDebug() << "客户端读入协议头 rres = " << rres;
        qDebug() << "客户端读入协议头 head.businessType = " << head.businessType;
        qDebug() << "客户端读入协议头 head.businessLength = " << head.businessLength;

        //根据协议头判断业务类型i
        if (LOG_B == head.businessType)				//若为登录业务
        {
            //读取协议体
            rres = read(this->socketfd, &backMsg, sizeof(BACKMSG));
            qDebug() << "客户端读入协议体 rres = " << rres;
            qDebug() << "客户端读入协议体 backMsg.flag = " << backMsg.flag;
            qDebug() << "客户端读入协议体 backMsg.message = " << backMsg.message;

            //发送登录信息给登录窗口
            emit send2LogWin(backMsg.flag);
        }
        else if (CHAT_B == head.businessType)		//若为聊天业务
        {
            //读取协议体
            rres = read(this->socketfd, &chatMsg, sizeof(CHATMSG));
            qDebug() << "客户端读入协议体 rres = " << rres;
            qDebug() << "客户端读入协议体 chatMsg.sendID = " << chatMsg.sendID;
            qDebug() << "客户端读入协议体 chatMsg.recvID = " << chatMsg.recvID;
            qDebug() << "客户端读入协议体 chatMsg.context = " << chatMsg.context;

            //发送聊天信息给聊天窗口
            emit send2ChatWin(&chatMsg);
        }
    }
}

void ReadThread::setSocketfd(int value)
{
    socketfd = value;
}
