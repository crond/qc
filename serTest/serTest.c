#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

int main()
{
	int fd0=0;
	int ret=0;
	errno = 0;
	fd0=open("/dev/ttyO0",O_RDWR);
	if(fd0<= 0)
	{
		printf("ttyO1 Error. Errno:%d Str;%s\n",errno,strerror(errno));
		return -1;	
	}
	printf("fd : %d \n",fd0);
	while(1) {
		errno = 0;	
		if( (ret = write(fd0,"HiS0",4)) <= 0)
		{
			printf("ttyO1 Write Error.ret;%d Errno:%d Str;%s\n",ret,errno,strerror(errno));
			break;
		}		
		else
			printf("iwrote ;%d \ni",ret);			
		sleep(3);
	}
	printf("\n Closing \n");
	if(fd0 != 0)
		close(fd0);
	return 0;
}		
