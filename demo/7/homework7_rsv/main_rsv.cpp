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
	//创建or访问消息队列
	int msgID = msgget(key_t(1002), IPC_CREAT | 0777);
	if (0 > msgID)
	{
		perror("msgget error");
	}

	MSGBUF resBuf;

	while (true)
	{
		//阻塞函数	消息接收
		int msgResult = msgrcv(msgID, &resBuf, sizeof(MSGBUF), 1, 0);
		if (0 > msgResult)
		{
			perror("msgrcv error");
		}
		else
		{
			STU resStu1 = { 0 };

			//创建共享内存
			int shmID = shmget(key_t(1001), 100, IPC_CREAT | 0777);
			void* shmAddr = NULL;

			//连接共享内存段
			shmAddr = shmat(shmID, NULL, 0);

			//将数据从共享内存中导出
			memcpy(&resStu1, shmAddr, sizeof(STU));
			memset(shmAddr, 0, sizeof(STU));

			cout << "数据导出成功：" << resStu1.num << " " << resStu1.name << endl;
			memset(&resStu1, 0, sizeof(MSGBUF));

			//断开共享内存连接
			shmdt(shmAddr);
		}
	}
	return 0;
}