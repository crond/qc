#include "q_headers.h"
#include "q_i2c.h"
#include "q_struct.h"

unsigned char sockDbgBuffer[MAX_DBG_BUFF_SIZE];
int sockWaitFd=0,eamSockFd=0;
struct sockaddr_in	acceptAddr;

extern unsigned char G_eamConnectStatus; 
int initSock(const unsigned int listnPort)
{
	static char fn[]="initSock()";
	int on=1;

	errno  = 0;
	if((sockWaitFd = socket(AF_INET,SOCK_STREAM,0)) <= 0)
	{
		snprintf(sockDbgBuffer,sizeof(sockDbgBuffer),"socket failed. err:%d %s",errno,strerror(errno));
		dbgLog(INFORM,fn,__LINE__,sockDbgBuffer);
		return -1;
	}
		
	errno	=0;
	if((setsockopt(sockWaitFd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))) < 0)
	{
		snprintf(sockDbgBuffer,sizeof(sockDbgBuffer),"setsockopt failed. err:%d %s",errno,strerror(errno));
		dbgLog(INFORM,fn,__LINE__,sockDbgBuffer);
		return -1;
	}
	memset((void *)&acceptAddr,0,sizeof(acceptAddr));
	acceptAddr.sin_port	= htons(listnPort);
	acceptAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	acceptAddr.sin_family = AF_INET;

	errno = 0;
	if((bind(sockWaitFd,(struct sockaddr *)&acceptAddr,sizeof(acceptAddr))) < 0)
	{
		snprintf(sockDbgBuffer,sizeof(sockDbgBuffer),"Bind failed. err:%d %s",errno,strerror(errno));
		dbgLog(INFORM,fn,__LINE__,sockDbgBuffer);
		return -1;
	}
	
	errno = 0;
	if(listen(sockWaitFd,5) < 0)
	{
		snprintf(sockDbgBuffer,sizeof(sockDbgBuffer),"listen failed. err:%d %s",errno,strerror(errno));
		dbgLog(INFORM,fn,__LINE__,sockDbgBuffer);
		return -1;
	}

	return 0;
}
int acceptEAMConnection(const int sockFd)
{
	static char fn[]="acceptEAMConnection()";
	char ipaddr[16];
	socklen_t namelength;
	static int tmpSock=0;
	struct sockaddr_in recvAddr;
	tmpSock = 0;
	namelength = sizeof(struct sockaddr_in);
	errno = 0;	
	if((tmpSock=accept(sockFd,(struct sockaddr *)&recvAddr,&namelength)) <= 0)
	{
		snprintf(sockDbgBuffer,sizeof(sockDbgBuffer),"accept failed. err:%d %s",errno,strerror(errno));
		dbgLog(INFORM,fn,__LINE__,sockDbgBuffer);
		G_eamConnectStatus = 0;
		return -1;
	}
	memset(ipaddr,0,sizeof(ipaddr));
	strcpy(ipaddr,inet_ntoa(recvAddr.sin_addr));
	snprintf(sockDbgBuffer,sizeof(sockDbgBuffer),"accepted conn from %s. fd;%d",ipaddr,tmpSock);
	dbgLog(INFORM,fn,__LINE__,sockDbgBuffer);
	G_eamConnectStatus = 1;
	return tmpSock;
}
int recvSockMsg(const int fd,char *buffer,const int len)
{
	static char fn[]="recvSockMsg()";
	int nbytes=0;
	
	if(len <= 0)
		return len;

	errno = 0;
	nbytes = recv(fd,buffer,len,0);
	if(nbytes <= 0)
	{
		snprintf(sockDbgBuffer,sizeof(sockDbgBuffer),"recv failed.fd:%d  err:%d %s",fd,errno,strerror(errno));
		dbgLog(INFORM,fn,__LINE__,sockDbgBuffer);
		return -1;
	}	
	return nbytes;
}
