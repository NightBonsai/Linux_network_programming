#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main()
{
	//����Ȩ������Ϊ0000
	umask(0);
	char buf[1024] = { 0 };
	int res = 0, wres = 0;

	//���ļ�
	int rfd = open("/root/projects/OMO2207-Phase II Summary.mp4", O_RDONLY, 0777);
	int wfd = open("/root/projects/copy.mp4", O_CREAT | O_WRONLY, 0777);
	cout << "rfd = " << rfd << " wfd = " << wfd << endl;
	if (rfd < 0 || wfd < 0)
	{
		perror("open error");
	}
	else
	{
		//��δ�����ļ�β
		while (0 < (res = read(rfd, buf, sizeof(buf))))
		{
			//д���ļ�
			wres = write(wfd, buf, res);
			cout << "wres  = " << wres << endl;
		}

		cout << "finish" << endl;

		//�ر��ļ�������
		close(rfd);
		close(wfd);
	}

	return 0;
}