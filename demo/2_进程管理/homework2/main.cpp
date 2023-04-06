#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main()
{
	//初始化数据
	DIR *dir = NULL;
	struct dirent* pdir = NULL;
	pid_t pid = 0;

	//设置权限掩码为0000
	umask(0);
	char buf[1024] = { 0 };
	int res = 0, wres = 0;

	//打开指定目录文件夹
	if (NULL == (dir = opendir("/root/projects/")))
	{
		perror("opendir error");
	}
	else
	{
		//遍历文件夹下所有子文件和子文件夹
		while (NULL != (pdir = readdir(dir)))
		{
			//若为子文件
			if (8 == pdir->d_type)
			{
				//开辟新子进程进行文件拷贝	***************************没搞太懂，这个执行顺序*****************************
				pid = fork();
				
				//进行文件拷贝
				char openPath[] = "/root/projects/";
				char copyPath[] = "/root/projects/COPY/";
				int rfd = open(strcat(openPath, pdir->d_name), O_RDONLY, 0777);
				int wfd = open(strcat(copyPath, pdir->d_name), O_CREAT | O_WRONLY, 0777);
				cout << "rfd = " << rfd << " wfd = " << wfd << endl;
				if (rfd < 0 || wfd < 0)
				{
					perror("open error");
				}
				else
				{
					//若未读到文件尾
					while (0 < (res = read(rfd, buf, sizeof(buf))))
					{
						//写入文件
						wres = write(wfd, buf, res);
						cout << "wres  = " << wres << endl;
					}

					cout << "finish" << endl;

					//关闭文件描述符
					close(rfd);
					close(wfd);
				}
			}
		}
	}
	
	return 0;
}