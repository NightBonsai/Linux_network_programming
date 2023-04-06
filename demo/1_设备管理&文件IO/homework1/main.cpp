#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
	//设置权限掩码为0000
	umask(0);
	char buf[1024] = { 0 };
	int res = 0, wres = 0;

	//打开文件
	int rfd = open("/root/projects/OMO2207-Phase II Summary.mp4", O_RDONLY, 0777);
	int wfd = open("/root/projects/copy.mp4", O_CREAT | O_WRONLY, 0777);
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

	return 0;
}