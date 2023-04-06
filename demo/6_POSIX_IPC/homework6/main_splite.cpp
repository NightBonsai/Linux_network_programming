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

//文件拆分
void splite(string path, string name);

int main()
{
	pid_t pid = fork();
	if (0 < pid)
	{
		//父进程
		while (true) { }

	}
	else if (0 == pid)
	{
		//子进程
		splite("/root/projects/homework6/", "swordsman.png");
	}

	return 0;
}

void splite(string path, string name)
{
	//拆分文件
	//设置权限掩码
	umask(0);
	int res = 0,count = 0;
	char readPath[50] = { 0 };

	//拼接读取路径
	strcat(readPath, path.c_str());
	strcat(readPath, name.c_str());
	cout << "子进程读取路径：" << readPath << endl;

	//打开文件
	int rfd = open(readPath, O_RDONLY, 0777);
	if (rfd < 0)
	{
		perror("打开图片失败");
		return;
	}
	else
	{
		//存储读取的文件内容
		char buf[1000] = { 0 };
		char str[20] = { 0 };

		//若未读到文件末尾
		while (0 < (res = read(rfd, buf, sizeof(buf))))
		{
			//拼接保存路径
			sprintf(str, "%d", count);
			char writePath[200] = { 0 };
			strcat(writePath, path.c_str());
			strcat(writePath, str);
			strcat(writePath, ".temp");
			count++;
			cout << "子进程保存路径：" << writePath << endl;

			//打开保存路径
			int wfd = open(writePath, O_CREAT | O_WRONLY, 0777);
			if (wfd < 0)
			{
				perror("子进程打开保存路径失败");
			}
			else
			{
				//写入文件
				write(wfd, buf, res);

				cout << "子进程写入完成" << endl;

				//关闭写入文件描述符
				close(wfd);
			}
		}
		//关闭读出文件描述符
		close(rfd);
	}

	//发送消息至消息队列
	//创建or访问消息队列
	int msgID = msgget(key_t(1001), IPC_CREAT | 0777);
	if (0 > msgID)
	{
		perror("msgget error");
	}
	else
	{
		MSGBUF buf;
		buf.mtype = 1;
		char tmp[100] = { 0 };
		sprintf(tmp, "%d ", count);
		strcat(tmp, path.c_str());
		memcpy(buf.mtext, tmp, sizeof(buf.mtext));
		cout << "发送的消息：" << buf.mtext << endl;

		//发送消息
		msgsnd(msgID, &buf, sizeof(buf.mtext), 0);
		cout << "消息发送成功" << endl;
	}
}