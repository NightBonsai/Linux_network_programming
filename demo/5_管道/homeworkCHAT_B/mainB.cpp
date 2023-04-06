#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main()
{
	umask(0);
	char buf[150] = { 0 };
	char readBuf[150] = { 0 };

	//创建命名文件
	//如果目录内找不到文件
	if (-1 == access("/root/projects/B2A.fifo", F_OK))
	{
		//如果创建文件失败
		if (-1 == mkfifo("/root/projects/B2A.fifo", 0777))
		{
			perror("创建命名文件失败");
		}
	}
	else
	{
		cout << "命名文件已存在" << endl;
	}

	pid_t pid = fork();
	if (0 < pid)
	{
		//父进程：用于写出
		int wfd = open("/root/projects/B2A.fifo", O_WRONLY, 0777);

		while (true)
		{
			cin >> buf;
			int wres = write(wfd, buf, sizeof(buf));
			cout << wres << endl;
			bzero(buf, sizeof(buf));
		}
	}
	else if (0 == pid)
	{
		//子进程：用于读入
		int rfd = open("/root/projects/A2B.fifo", O_RDONLY, 0777);

		while (true)
		{
			int rres = read(rfd, readBuf, sizeof(readBuf));
			cout << "A：" << readBuf << " " << rres << endl;
			bzero(readBuf, sizeof(readBuf));
		}
	}

	return 0;
}