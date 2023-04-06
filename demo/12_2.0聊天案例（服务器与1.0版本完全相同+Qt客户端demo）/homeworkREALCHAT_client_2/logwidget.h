#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include "chatwidget.h"
#include "protocol.h"
#include "readthread.h"
#include "writethread.h"
#include "socketconnect.h"

namespace Ui {
class LogWidget;
}

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = nullptr);
    ~LogWidget();

private:
    Ui::LogWidget *ui;  //UI

    ChatWidget *chatWin;//聊天界面

    SocketConnect *sc;  //网络连接线程
    ReadThread *rt;     //读线程
    WriteThread *wt;    //写线程

    HEAD head;          //协议头
    USER user;          //协议体
    char buf[1024];
signals:
public slots:
    void logJudge();            //登录验证
    void getLogJudge(int flag); //获取返回登录验证结果
};

#endif // LOGWIDGET_H
