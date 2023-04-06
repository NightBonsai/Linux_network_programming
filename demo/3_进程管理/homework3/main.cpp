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
using namespace std;

//文件拆分
void copy(string path,string name);

int main()
{
	copy("/root/projects/homework3/","swordsman.png");
	return 0;
}

void copy(string path,string name)
{
	//创建文件拆分子进程
	int pid = fork();
	int status = 0, count = 0, res = 0;

	//子进程
	if (0 == pid)
	{
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
			char buf[1000],str[20];
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

		//结束子进程
		exit(count);
	}
	//父进程
	else if (0 < pid)
	{
		//阻塞等待
		pid_t waitID = waitpid(pid, &status, NULL);
		cout << "waitID = " << waitID << endl;
		cout << "子进程退出码 = " << WEXITSTATUS(status) << endl;

		//文件合并
		int i = 0;
		for (i = 0; i < WEXITSTATUS(status); i++)
		{
			char str[20];
			char readPath[200];
			memset(str, 0, sizeof(str));
			memset(readPath, 0, sizeof(readPath));

			//拼接读取路径
			sprintf(str, "%d", i);
			strcat(readPath, path.c_str());
			strcat(readPath, str);
			strcat(readPath, ".temp");
			cout << "父进程读取路径：" << readPath << endl;

			//打开文件
			int rfd = open(readPath, O_RDONLY, 0777);
			if (rfd < 0)
			{
				perror("打开分卷失败");
			}
			else
			{
				//存储读取的文件内容
				char buf[1000];
				memset(buf, 0, sizeof(buf));
				res = 0;

				//若未读到文件末尾
				while (0 < (res = read(rfd, buf, sizeof(buf))))
				{
					//拼接保存路径
					char writePath[200];
					memset(writePath, 0, sizeof(writePath));
					strcat(writePath, path.c_str());
					strcat(writePath, "副本");
					strcat(writePath, name.c_str());
					cout << "父进程保存路径：" << writePath << endl;

					//打开保存路径（文件后添加）
					int wfd = open(writePath, O_CREAT | O_WRONLY | O_APPEND, 0777);
					if (wfd < 0)
					{
						perror("父进程打开保存路径失败");
					}
					else
					{
						//写入文件
						write(wfd, buf, res);

						cout << "父进程写入完成" << endl;

						//关闭写入文件描述符
						close(wfd);
					}
				}
				//关闭读出文件描述符
				close(rfd);
			}
		}
	}
}