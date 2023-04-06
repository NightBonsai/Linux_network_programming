#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

//消息体
typedef struct msgBuf
{
	long mtype;			//消息类型
	char mtext[100];	//消息数据
}MSGBUF;

//文件拼合
void combine();

int main()
{
	combine();

	while (true) {}

	return 0;
}

void combine()
{
	//创建or访问消息队列
	int msgID = msgget(key_t(1001), IPC_CREAT | 0777);
	if (0 > msgID)
	{
		perror("msgget error");
	}

	MSGBUF resBuf;

	//消息接收
	msgrcv(msgID, &resBuf, sizeof(resBuf.mtext), 1, 0);
	char delim[2] = " ";
	char* number = NULL;
	char* path = NULL;

	//获取数量和保存路径**************************************************不能准确获取*******************************************
	number = strtok(resBuf.mtext, delim);
	path = strstr(resBuf.mtext, "/");

	cout << number << " " << path << " " << resBuf.mtext << endl;
}