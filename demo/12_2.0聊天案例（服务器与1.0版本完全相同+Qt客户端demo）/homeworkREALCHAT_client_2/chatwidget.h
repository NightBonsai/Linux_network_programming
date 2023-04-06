#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QDebug>
#include "protocol.h"
#include "readthread.h"
#include "writethread.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    void setUserName(int value);
private:
    Ui::ChatWidget *ui; //UI

    WriteThread *wt;    //写线程
    ReadThread *rt;     //读线程

    CHATMSG chatMsg;    //聊天协议体
    HEAD head;          //协议头

    char buf[1024];
    int userName;       //当前登录用户
signals:

public slots:
    void sendChatMsg();                 //发送聊天数据给服务器
    void showChatMsg(CHATMSG *msg);      //从服务器接收聊天数据
};

#endif // CHATWIDGET_H
