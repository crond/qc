#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/ioctl.h>
#include<termios.h>

#define HFC	0
#define DB	CS8
#define SB	0
#define BASE_PORT_STR	"/dev/ttyO"

unsigned int portNum=0;
int serialFd=0;
static tcflag_t BAUD=B9600;
int openPort(void);

int main(int argc, char *argv[])
{
	int ret =0;
	if(argc != 2)
	{
		printf("\n Syntax: serPort <PORTNUM> \n");
		return 0;
	}
	portNum = atoi(argv[1]);
	if((portNum < 0) || (portNum > 3))
	{
		printf("\n Syntax: serPort <PORTNUM> \n PORTNUM : 0 1 2 3\n");
		return 0;
	}
	
	if( (openPort()) < 0)
		return -1;

	while(1) {
		if( (ret = write(serialFd,"HiS0",4)) <= 0)
			printf("Port;%d Fd;%d Write Error. Errno:%d Str;%s\n",portNum,serialFd,errno,strerror(errno));
		else
			printf("%d Bytes Sent to port:%d Fd;%d\n",ret,portNum,serialFd);
		sleep(1);
	}
	printf("\n Closing \n");
	if(serialFd != 0)
		close(serialFd);
	return 0;
}

int openPort(void)
{
	int ret=0;
	struct termios setTermios;
	char buffer[32];

	memset(buffer,0,sizeof(buffer));
	snprintf(buffer,sizeof(buffer),"%s%d",BASE_PORT_STR,portNum);
	printf("Opening Port:%s\n",buffer);
	serialFd=open(buffer,O_RDWR);
	if(serialFd<= 0)
	{
		printf("Port:%s open error. Errno:%d Str;%s\n",buffer,errno,strerror(errno));
		return -1;
	}
	setTermios.c_cflag = B9600 | HFC | DB | SB | CLOCAL | CREAD ;	
	setTermios.c_iflag = IGNPAR;  // No parity
	setTermios.c_oflag = 0;    //s/w flow ctrl off
	setTermios.c_lflag = 0; //non-canonical, no echo
	errno = 0;
	ret = tcsetattr(serialFd,TCSANOW,&setTermios);
	if(ret < 0)
	{
		printf("Error while setting attr for Port:%s%d. err:%d errstr:%s",BASE_PORT_STR,portNum,errno,strerror(errno));
		return -1;
	}
	return 0;
}		
