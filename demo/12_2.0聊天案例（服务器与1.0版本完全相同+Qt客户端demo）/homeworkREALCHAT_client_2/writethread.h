#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QThread>
#include <QDebug>
#include <unistd.h>

//写线程单例类
class WriteThread : public QThread
{
    Q_OBJECT
public:
    static WriteThread *getInstance();

    void run();

    void setSocketfd(int value);
    void setData(void *value,int size);
private:
    WriteThread();
    ~WriteThread();

    static WriteThread *writeThread;

    int socketfd;
    char data[1024];
    int length;
signals:

public slots:

};

#endif // WRITETHREAD_H
