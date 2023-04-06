#include "chatwidget.h"
#include "ui_chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->friendList->setIconSize(QSize(50,50));

    //线程初始化
    this->wt = WriteThread::getInstance();
    this->rt = ReadThread::getInstance();

    //数据成员初始化
    memset(buf,0,sizeof(buf));

    //信号与槽事件绑定
    connect(this->ui->send,SIGNAL(clicked()),this,SLOT(sendChatMsg()));
    connect(this->rt,SIGNAL(send2ChatWin(CHATMSG*)),this,SLOT(showChatMsg(CHATMSG*)));
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::setUserName(int value)
{
    userName = value;
}

void ChatWidget::sendChatMsg()
{
    //获取接收方ID
    const char *rcvID = ui->friendList->currentItem()->text().toStdString().c_str();
    //获取要发送的聊天内容
    QString chatText = ui->chatEdit->text();

    //设置协议头&协议体
    this->chatMsg.recvID = atoi(rcvID);
    this->chatMsg.sendID = this->userName;
    memcpy(this->chatMsg.context,chatText.toStdString().c_str(),sizeof(chatMsg.context));

    this->head.businessType = CHAT_B;
    this->head.businessLength = sizeof(this->chatMsg);

    //组装协议包
    memcpy(this->buf,&(head),sizeof(HEAD));
    memcpy(this->buf+sizeof(HEAD),&(chatMsg),sizeof(CHATMSG));

    this->wt->setData(&(this->buf),sizeof(HEAD)+sizeof(CHATMSG));

    ui->chatEdit->setText("");
}

void ChatWidget::showChatMsg(CHATMSG *msg)
{
    qDebug()<<"发送者:"<<msg->sendID;
    qDebug()<<"接收者:"<<msg->recvID;

    //更新显示的聊天内容
    QString send = QString::number(msg->sendID,10);
    QString mid ="：";
    QString recv = QString(msg->context);
    send = send + mid;
    send = send + recv;

    ui->textEdit->append(send);
    update();
}
