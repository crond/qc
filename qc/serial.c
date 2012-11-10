#include "q_headers.h"
#include "q_struct.h"
#include "q_serial.h"

int mcUartFd=0;
static tcflag_t BAUD=B9600;
unsigned char serDbgBuffer[MAX_DBG_BUFF_SIZE];

int openSerPort(int portNum)
{
	char buffer[32];
	static char fn[]="openSerPort()";
	int	 ret=0;
	static int 	serialFd=0;
	struct termios setTermios;

	serialFd=0;
	memset(buffer,0,sizeof(buffer));
	snprintf(buffer,sizeof(buffer),"%s%d",BASE_PORT_STR,portNum);
	snprintf(serDbgBuffer,sizeof(serDbgBuffer),"Opening Port:%s\n",buffer);
	dbgLog(INFORM,fn,__LINE__,serDbgBuffer);

	errno = 0;
	serialFd=open(buffer,O_RDWR);
	if(serialFd<= 0)
	{
		snprintf(serDbgBuffer,sizeof(serDbgBuffer),"Port:%s open error. Errno:%d Str;%s\n",buffer,errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,serDbgBuffer);	
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
		snprintf(serDbgBuffer,sizeof(serDbgBuffer),"Error while setting attr for Port:%s%d. err:%d errstr:%s",BASE_PORT_STR,portNum,errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,serDbgBuffer);	
		return -1;
	}
	return serialFd;
}
int readFromUart(char *buffer,const int len,const unsigned int fd)
{
	static char fn[]="readFromUart()";
	static int nread=0;
	
	if(len <= 0)
		return len;

	nread = 0;
	errno = 0;
	nread = read(fd,buffer,len);
	if(nread <= 0)
	{
		snprintf(serDbgBuffer,sizeof(serDbgBuffer),"Reading from Serport Fd:%d failed.Err:%d %s",fd,errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,serDbgBuffer);
	}
	else
	{
		snprintf(serDbgBuffer,sizeof(serDbgBuffer),"Reading from Serport Fd:%d OK.read:%d",fd,nread);
		dbgLog(REPORT,fn,__LINE__,serDbgBuffer);
	
	}
	return nread;
}		

int writeToUart(const char *buffer,const int len,const unsigned int fd)
{
	static char fn[]="writeToUart()";
	static int nwrote=0;
	
	if(len <= 0)
		return len;

	nwrote = 0;
	errno = 0;
	nwrote = write(fd,buffer,len);
	if(nwrote <= 0)
	{
		snprintf(serDbgBuffer,sizeof(serDbgBuffer),"Writing to Serport Fd:%d failed.Err:%d %s",fd,errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,serDbgBuffer);
	}
	else
	{
		snprintf(serDbgBuffer,sizeof(serDbgBuffer),"Writing to Serport Fd:%d OK.recvd byte to write:%d wrote:%d",fd,len,nwrote);
		dbgLog(REPORT,fn,__LINE__,serDbgBuffer);
	
	}
	return nwrote;
}		
