#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

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

/*信号量初始化
semID：哪个信号量
semIndex：下标
value：具体数值*/
void semSetVal(int semID, int semIndex, int value);

void semP(int semID, int semIndex);	//信号量P操作	-1加锁
void semV(int semID, int semIndex);	//信号量V操作	+1解锁

int main()
{
	//数据准备
	int arr[BLOCK_NUM] = { 0 };	//共享内存索引区 0—对应下标数据区空闲 1—对应下标数据区有数据
	STU stu = { 0 };
	int num = 0, index = 0;
	void* shmAddr = NULL;

	//创建共享内存
	int shmID = shmget((key_t)1001, sizeof(arr) + sizeof(STU) * BLOCK_NUM, IPC_CREAT | 0777);
	if (0 > shmID)
	{
		perror("shmget error");
	}
	else
	{
		cout << "共享内存创建完成" << endl;
	}

	//创建消息队列
	int msgID = msgget((key_t)1002, IPC_CREAT | 0777);
	if (0 > msgID)
	{
		perror("msgget error");
	}
	else
	{
		cout << "消息队列创建完成" << endl;
	}

	//创建信号量
	int semID = semCreate((key_t)1003, BLOCK_NUM);	//共享内存数据区个数=信号量个数
	for (int i = 0; i < BLOCK_NUM; i++)
	{
		semSetVal(semID, i, 1);						//信号量初始化
	}
	cout << "信号量创建完成" << endl;
	
	while (true)
	{
		sprintf(stu.stuID, "OMO2207%d", num);
		sprintf(stu.stuName, "张三%d", num);
		cout << "学生数据准备 " << num << endl;
		num++;

		//共享内存操作
		shmAddr = shmat(shmID, NULL, 0);	//连接共享内存

		memcpy(arr, shmAddr, sizeof(arr));	//获取当前共享内存索引区信息
		for (int i = 0; i < BLOCK_NUM; i++)
		{
			if (0 == arr[i])
			{
				index = i;	//若索引区对应数据域空闲，记录当前索引区下标
				break;
			}
		}
		cout << "index = " << index << endl;

		semP(semID, index);		//对应信号量加锁

			//数据写入共享内存
			memcpy(shmAddr + sizeof(arr) + sizeof(STU) * index, &stu, sizeof(STU));		//操作数据区：首地址 + 索引区总长度 + 数据区*下标
			arr[index] = 1;
			memcpy(shmAddr + sizeof(int) * index, &arr[index], sizeof(int));			//操作索引区：首地址 + 一个索引大小*下标

			//共享内存断开链接
			shmdt(shmAddr);
		
		semV(semID, index);		//对应信号量解锁

		//发送消息至消息队列，提示后端服务器读取数据
		MSGBUF buf = { 0 };				
		buf.mtype = 1;
		sprintf(buf.mtext, "%d", index);			//消息初始化

		if (-1 == msgsnd(msgID, &buf, sizeof(buf.mtext), 0))	//消息发送
		{
			perror("msgsnd error");
		}
		cout << "消息发送成功" << endl;

		//数据初始化
		bzero(&stu, sizeof(STU));
		bzero(&buf, sizeof(MSGBUF));
		bzero(&arr[index], sizeof(int));
		index = 0;

		sleep(2);
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

void semSetVal(int semID, int semIndex, int value)
{
	union semun arg;
	arg.val = value;

	int res = semctl(semID, semIndex, SETVAL, arg);
	if (0 > res)
	{
		perror("semctl error");
	}
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