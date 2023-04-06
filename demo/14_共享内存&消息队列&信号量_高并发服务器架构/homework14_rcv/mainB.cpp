#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_NUM 100	//共享内存数据块数量

using namespace std;

//信号量结构体
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds* buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short* array;  /* Array for GETALL, SETALL */
	struct seminfo* __buf;  /* Buffer for IPC_INFO
								(Linux-specific) */
};

//消息结构体
typedef struct messageBuf
{
	long mtype;       /* message type, must be > 0 */
	char mtext[2];    /* message data */
}MSGBUF;

//学生结构体
typedef struct student
{
	char stuID[20];
	char stuName[20];
}STU;

/*创建/访问信号量
key_t：信号量key，若不存在则创建，若存在则访问
numSems：信号量数组长度*/
int semCreate(key_t key, int numSems);

void semP(int semID, int semIndex);	//信号量P操作	-1加锁
void semV(int semID, int semIndex);	//信号量V操作	+1解锁

int main()
{
	//数据准备
	int arr[BLOCK_NUM] = { 0 };	//共享内存索引区 0—对应下标数据区空闲 1—对应下标数据区有数据
	STU stu = { 0 };
	int num = 0, index = 0;
	void* shmAddr = NULL;

	//访问共享内存
	int shmID = shmget((key_t)1001, sizeof(arr) + sizeof(STU) * BLOCK_NUM, IPC_CREAT | 0777);
	if (0 > shmID)
	{
		perror("shmget error");
	}
	else
	{
		cout << "访问共享内存成功" << endl;
	}

	//访问消息队列
	int msgID = msgget((key_t)1002, IPC_CREAT | 0777);
	if (0 > msgID)
	{
		perror("msgget error");
	}
	else
	{
		cout << "访问消息队列成功" << endl;
	}

	//访问信号量
	int semID = semCreate((key_t)1003, BLOCK_NUM);	//共享内存数据区个数=信号量个数
	cout << "访问信号量成功" << endl;

	while (true)
	{
		//接收消息
		MSGBUF buf = { 0 };
		if (-1 == msgrcv(msgID, &buf, sizeof(buf.mtext), 1, 0))	//接收消息
		{
			perror("msgrcv error");
		}
		index = atoi(buf.mtext);
		cout << "收到下标 index = " << index << endl;

		//共享内存操作
		shmAddr = shmat(shmID, NULL, 0);	//连接共享内存

		memcpy(&arr[index], shmAddr + sizeof(int) * index, sizeof(int));				//读索引区：首地址 + 一个索引*下标
		if (1 == arr[index])					//若索引区对应数据区内有数据
		{
			semP(semID, index);		//对应信号量加锁

				//从共享内存中读取数据
				memcpy(&stu, shmAddr + sizeof(arr) + sizeof(STU) * index, sizeof(STU));	//读数据区：首地址 + 整个索引区长度 + 数据块*下标
				cout << "stu.stuID = " << stu.stuID << endl;
				cout << "stu.stuName = " << stu.stuName << endl;

				memset(shmAddr + sizeof(arr) + sizeof(STU) * index, 0, sizeof(STU));	//清空读出的数据区数据
				memset(shmAddr + sizeof(int) * index, 0, sizeof(int));					//修改索引区为空闲—0

				//共享内存断开链接
				shmdt(shmAddr);	

			semV(semID, index);		//对应信号量解锁

			//数据初始化
			bzero(&stu, sizeof(STU));
			bzero(&buf, sizeof(MSGBUF));
			bzero(&arr[index], sizeof(int));
			index = 0;

			sleep(4);
		}
	}

	return 0;
}

int semCreate(key_t key, int numSems)
{
	int res = semget(key, numSems, IPC_CREAT | 0777);
	if (0 > res)
	{
		perror("semget error");
	}
	return res;
}

void semP(int semID, int semIndex)
{
	struct sembuf buf = { semIndex,-1,SEM_UNDO };

	int res = semop(semID, &buf, 1);
	if (0 > res)
	{
		perror("semop p error");
	}
}

void semV(int semID, int semIndex)
{
	struct sembuf buf = { semIndex,1,SEM_UNDO };

	int res = semop(semID, &buf, 1);
	if (0 > res)
	{
		perror("semop v error");
	}
}