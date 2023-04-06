#include "logwidget.h"
#include "ui_logwidget.h"

LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWidget)
{
    ui->setupUi(this);

    //线程初始化
    this->sc = SocketConnect::getInstance();
    this->rt = ReadThread::getInstance();
    this->wt = WriteThread::getInstance();

    this->sc->creatConnect();                       //连接服务器
    this->rt->setSocketfd(this->sc->getSocketfd());
    this->wt->setSocketfd(this->sc->getSocketfd());

    //数据成员初始化
    memset(this->buf,0,sizeof(buf));

    //信号与槽事件绑定
    connect(this->ui->logButton,SIGNAL(clicked()),this,SLOT(logJudge()));
    connect(this->rt,SIGNAL(send2LogWin(int)),this,SLOT(getLogJudge(int)));
}

LogWidget::~LogWidget()
{
    delete ui;
}

void LogWidget::getLogJudge(int flag)
{
    if(LOG_SUCCESS == flag)
    {
        QMessageBox::information(nullptr,"登录提示","登录成功");

        //登录成功把当前用户ID传给聊天界面
        this->chatWin = new ChatWidget();
        this->chatWin->setUserName(this->user.name);

        this->hide();
        this->chatWin->show();
    }
    else
    {
        QMessageBox::information(nullptr,"登录提示","登录失败");
    }
}

void LogWidget::logJudge()
{
    //登录验证
    //获取表单内容
    QString account = ui->accountEdit->text();
    QString password = ui->passwordEdit->text();

    //表单验证
    if(account.length()>0 && password.length()>0)
    {
        const char *name = account.toStdString().c_str();
        const char *pass = password.toStdString().c_str();

        //设置协议头&协议体
        this->user.name = atoi(name);
        memcpy(this->user.pass,pass,sizeof(user.pass));
        this->head.businessType = LOG_B;
        this->head.businessLength = sizeof(this->user);

        qDebug()<<"user.name = "<<user.name;
        qDebug()<<"user.pass = "<<user.pass;
        qDebug()<<"head.businessLength = "<<head.businessLength;

        //组装协议包
        memset(buf,0,sizeof(buf));
        memcpy(buf,&(head),sizeof(head));
        memcpy(buf+sizeof(head),&(user),sizeof(user));

        //开启线程
        this->rt->start();
        this->wt->setData(&(buf),sizeof(HEAD)+sizeof(USER));    //设置写出协议包
        this->wt->start();
    }
}
