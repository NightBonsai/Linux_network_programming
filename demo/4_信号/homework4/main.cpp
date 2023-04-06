#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
using namespace std;

//文件拆分
void splite(string path, string name);
//文件合并
void combineSIGNAL(int i);
void combine(string path);

int main()
{
	pid_t pid = 0;
	int status = 0;

	//设置信号
	signal(SIGUSR1, combineSIGNAL);

	pid = fork();
	if (0 < pid)
	{
		//父进程
		//等待子进程结束
		//阻塞等待
		pid_t waitID = waitpid(pid, &status, NULL);
		cout << "waitID = " << waitID << endl;
		cout << "子进程退出码 = " << WEXITSTATUS(status) << endl;
		
		//发送信号
		kill(getpid(), SIGUSR1);
	}
	else if (0 == pid)
	{
		//子进程
		//进行文件拆分
		splite("/root/projects/homework4/", "swordsman.png");

		exit(0);
	}
	return 0;
}

void splite(string path, string name)
{
	int status = 0, count = 0, res = 0;

	//设置权限掩码
	umask(0);
	char readPath[200];
	memset(readPath, 0, sizeof(readPath));

	//拼接读取路径
	strcat(readPath, path.c_str());
	strcat(readPath, name.c_str());
	cout << "子进程读取路径：" << readPath << endl;

	//打开文件
	int rfd = open(readPath, O_RDONLY, 0777);
	if (rfd < 0)
	{
		perror("打开图片失败");
	}
	else
	{
		//存储读取的文件内容
		char buf[1000], str[20];
		memset(buf, 0, sizeof(buf));
		memset(str, 0, sizeof(str));

		//若未读到文件末尾
		while (0 < (res = read(rfd, buf, sizeof(buf))))
		{
			//拼接保存路径
			sprintf(str, "%d", count);
			char writePath[200];
			memset(writePath, 0, sizeof(writePath));
			strcat(writePath, path.c_str());
			strcat(writePath, str);
			strcat(writePath, ".temp");
			count++;
			cout << "拆分文件保存路径：" << writePath << endl;

			//打开保存路径
			int wfd = open(writePath, O_CREAT | O_WRONLY, 0777);
			if (wfd < 0)
			{
				perror("打开拆分文件保存路径失败");
			}
			else
			{
				//写入文件
				write(wfd, buf, res);

				cout << "拆分文件写入完成" << endl;

				//关闭写入文件描述符
				close(wfd);
			}
		}
		//关闭读出文件描述符
		close(rfd);
	}
}

void combineSIGNAL(int i)
{
	combine("/root/projects/homework4/");
}

void combine(string path)
{
	cout << "文件合并" << endl;
}