#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>

using namespace std;

//学生结构体
typedef struct student
{
	int num;
	char name[20];
}STU;

//消息体
typedef struct msgBuf
{
	long mtype;			//消息类型
	char mtext[2];		//消息数据
}MSGBUF;

int main()
{
	STU stu1 = { 1,"何昊松" };

	//创建共享内存
	int shmID = shmget(key_t(1001), 100, IPC_CREAT | 0777);
	void* shmAddr = NULL;	//接收共享内存地址
	if (0 > shmID)
	{
		perror("shmget error");
	}
	else
	{
		while (true)
		{
			//写入数据
			cout << "写入学号：" << endl;
			cin >> stu1.num;
			cout << "写入姓名：" << endl;
			cin >> stu1.name;

			//连接共享内存
			shmAddr = shmat(shmID, NULL, 0);

			//将数据拷贝进共享内存中
			memcpy(shmAddr, &stu1, sizeof(STU));

			cout << "数据写入成功" << endl;

			//断开共享内存连接
			shmdt(shmAddr);

			//发送写入成功消息至消息队列
			int msgID = msgget(key_t(1002), IPC_CREAT | 0777);
			if (0 > msgID)
			{
				perror("msgget error");
			}
			else
			{
				MSGBUF msg;
				msg.mtype = 1;

				//发送消息
				msgsnd(msgID, &msg, sizeof(MSGBUF), 0);
				cout << "发送消息" << endl;
			}
		}
	}

	return 0;
}