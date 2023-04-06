#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包
#include "chatwidget.h"
#include "protocol.h"	//自定义协议

namespace Ui {
class LogWidget;
}

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = nullptr);
    ~LogWidget();
    
    void initNet();     //网络初始化，连接客户端
private:
    Ui::LogWidget *ui;  //UI

    ChatWidget *chatWin;
    struct sockaddr_in addr;
    int len;
signals:
    void toChatWinSIGNAL();
public slots:
    void toChatWin();   //跳转至聊天界面
    void logJudge();    //登录判断
};

#endif // LOGWIDGET_H
