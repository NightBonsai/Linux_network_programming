#include "writethread.h"

WriteThread *WriteThread::writeThread = nullptr;
WriteThread::WriteThread()
{
    //数据成员初始化
    this->length = 0;
    memset(this->data,0,sizeof(this->data));
}

WriteThread::~WriteThread()
{

}

WriteThread *WriteThread::getInstance()
{
    if(nullptr == WriteThread::writeThread)
    {
        WriteThread::writeThread = new WriteThread();
    }
    return WriteThread::writeThread;
}

void WriteThread::run()
{
    while (true)
    {
        if (this->length > 0)
        {
            //写出，数据发送给服务器端
            int wres = write(this->socketfd, this->data, this->length);	//发送有效长度的数据给服务器端
            qDebug() << "客户端写出数据 wres = " << wres;

            //数据初始化
            bzero(this->data,sizeof(this->data));
            this->length = 0;
        }
    }
}

void WriteThread::setSocketfd(int value)
{
    socketfd = value;
}

void WriteThread::setData(void *value, int size)
{
    this->length = size;
    memcpy(this->data,value,size);
}
