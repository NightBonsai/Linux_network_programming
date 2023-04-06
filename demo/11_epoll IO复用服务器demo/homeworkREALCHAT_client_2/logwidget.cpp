#include "logwidget.h"
#include "ui_logwidget.h"
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>	//sockaddr_in包
#include <arpa/inet.h>	//sockaddr_in包
#include <unistd.h>
#include <string.h>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include "protocol.h"	//自定义协议

LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWidget)
{
    ui->setupUi(this);

    //信号与槽事件绑定
    connect(ui->logButton,SIGNAL(clicked()),this,SLOT(logJudge()));
}

LogWidget::~LogWidget()
{
    delete ui;
}

void LogWidget::initNet()
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
            addr.sin_family = AF_INET;							//网络连接协议族 IPv4
            addr.sin_addr.s_addr = inet_addr("192.168.9.128");	//客户端：查找服务器IP		inet_ addr()能够把一个用数字和点表示IP地址的字符串转换成一个无符号长整型
            addr.sin_port = htons(10006);						//客户端：查找服务器端口	主机字节顺序转换为网络字节顺序(对无符号短型进行操作2bytes)

            len = sizeof(addr);
        }
}

void LogWidget::toChatWin()
{
    //客户端连接服务器
    //**************************************函数名冲突了，不清楚如何解决*****************************************
    /*if (-1 == connect(socketfd, (struct sockaddr*)&addr, len))
    {
        perror("connect error");
    }
    else
    {
        qDebug("客户端连接服务器成功");
        QMessageBox::information(this,"登录提示","登录成功");
        this->chatWin = new ChatWidget();
        this->hide();
        this->chatWin->show();
    }*/
}

void LogWidget::logJudge()
{
    //获取表单信息
    QString accountName = ui->accountEdit->text().trimmed();    //trimmed() 剔除字符串左右两边空格
    QString password = ui->passwordEdit->text().trimmed();

    //表单验证
    if(accountName.isEmpty() || password.isEmpty())
    {
        //弹窗提示
        QMessageBox::information(this,"登录提示","表单不能为空");
        return;
    }
    else if(accountName.count()<4 || accountName.count()>6 )
    {
        QMessageBox::information(this,"登录提示","帐号长度(4-6个字符)");
        return;
    }
    else if(password.count()<6 || password.count()>10)
    {
        QMessageBox::information(this,"登录提示","密码长度不合法(6-10个字符)");
        return;
    }

    //帐号密码验证

    //登录成功，跳转界面
    this->toChatWin();
}
