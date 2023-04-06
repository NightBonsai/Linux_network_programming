#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
using namespace std;

void slot(int x, siginfo_t* info, void* vo);

int main()
{
	pid_t pid = 0;

	//设置处理函数
	struct sigaction func;
	func.sa_flags = SA_SIGINFO;	//信号带参标志位
	func.sa_sigaction = slot;	//带参信号处理函数

	//绑定信号和处理函数
	sigaction(SIGUSR1, &func, NULL);

	//生成进程B
	pid = fork();
	if (0 < pid)
	{
		//进程A
		sleep(3);

		//设置携带参数1000
		union sigval value;
		value.sival_int = 1000;

		cout << "进程A发送信号" << endl;

		//发送带参信号给进程B
		sigqueue(pid, SIGUSR1, value);

		while (true)
		{
			cout << "进程A—" << getpid() << endl;
			sleep(1);
		}
	}
	else if (0 == pid)
	{
		//进程B
		pid_t pid = 0;

		sigaction(SIGRTMIN, &func, NULL);

		//生成进程C
		pid = fork();
		if (0 < pid)
		{
			//进程B
			sleep(3);

			//**************************************不会咋累加发送带参信号********************************
		}
		else if (0 == pid)
		{
			//进程C

		}
	}

	return 0;
}
void slot(int x, siginfo_t* info, void* vo)
{
	cout << "处理函数被调用" << endl;

	//数据累加
	info->si_int++;
}