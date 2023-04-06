#ifndef PROTOCOL_H
#define PROTOCOL_H

//协议头
typedef struct head
{
    int businessType;	//业务类型
    int businessLength;	//协议体长
}HEAD;

//协议体—登录
typedef struct user
{
    int name;			//账号
    char pass[20];		//密码
}USER;

//协议体—聊天
typedef struct chatmsg
{
    int sendID;			//聊天发送者ID
    int recvID;			//聊天接收者ID
    char context[150];	//聊天内容
}CHATMSG;

//协议体—返回
typedef struct backmsg
{
    int flag;			//业务结果
    char message[20];	//业务反馈内容
}BACKMSG;

//宏定义
#define LOG_B 0
#define CHAT_B 1
#define LOG_SUCCESS 2

#endif // PROTOCOL_H
