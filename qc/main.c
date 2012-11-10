#include "q_headers.h"
#include "q_i2c.h"
#include "q_serial.h"
#include "q_struct.h"

unsigned char mainDbgBuffer[MAX_DBG_BUFF_SIZE];
unsigned char dbgBuffer[MAX_DBG_BUFF_SIZE];
unsigned char eamBuffer[MAX_EAM_BUFF_SIZE];
unsigned char timeStr[32];

unsigned char G_eamConnectStatus=0; 


extern  int i2cFd;
extern unsigned int devAddr;
extern  int mcUartFd;
extern int sockWaitFd,eamSockFd;

int startQc(void)
{
	static char fn[]="startQc()";
	int ret=0,maxFd=0;
	fd_set rd_msk;	
	struct timeval tv;

	while(1)
	{
		tv.tv_sec	=0;
		tv.tv_usec	=10000;
		
		FD_ZERO(&rd_msk);
		
		FD_SET(i2cFd,&rd_msk);
		if(i2cFd > maxFd)
			maxFd = i2cFd;
		
		FD_SET(mcUartFd,&rd_msk);
		if(mcUartFd> maxFd)
			maxFd =mcUartFd;
		
		FD_SET(sockWaitFd,&rd_msk);
		if(sockWaitFd> maxFd)
			maxFd = sockWaitFd;
		
		if(G_eamConnectStatus == 1){
			FD_SET(eamSockFd,&rd_msk);
			if(eamSockFd> maxFd)
				maxFd = eamSockFd;
		}
		else{
			//Turn OFF the indication LEDs in QC Wings
		}
		maxFd = maxFd+1;
		errno = 0;
		ret = select(maxFd,&rd_msk,NULL,NULL,(struct timeval *)&tv);
		if(ret > 0)
		{
			if(FD_ISSET(eamSockFd,&rd_msk))
			{
				//Read the EAM master Message
				if((recvSockMsg(eamSockFd,eamBuffer,sizeof(eamBuffer))) < 0){	
					dbgLog(REPORT,fn,__LINE__,"EAM Sock Read Failed. Closing connection");
					close(eamSockFd);
					G_eamConnectStatus	= 0;
				}
			}
			if(FD_ISSET(i2cFd,&rd_msk))
			{
				//I2C Operations here
			}
			if(FD_ISSET(mcUartFd,&rd_msk))
			{
				//read the MC msg
			}
			if(FD_ISSET(sockWaitFd,&rd_msk))
			{
				//accept the incoming EAM connection
				eamSockFd = acceptEAMConnection(sockWaitFd);
			}
		}
		else
		{
			if(errno == EINTR){
				continue;
			}
			else
			{
				snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"Select call failed. Err:%d %s",errno,strerror(errno));
				dbgLog(FATAL,fn,__LINE__,mainDbgBuffer);
				break;
			}
		}	
	}
	snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"Select call returnning. i2cfd:%d uartFd;%d sockFd:%d  eamfd:%d",i2cFd,mcUartFd,sockWaitFd,eamSockFd);
	dbgLog(FATAL,fn,__LINE__,mainDbgBuffer);
	
	return 0;
}





int dbgLog(char type,char *fn,int line,char *str)
{
	time_t tmp_t;
	bzero(dbgBuffer,sizeof(dbgBuffer));
	bzero(timeStr,sizeof(timeStr));
	tmp_t=time(NULL);	
	strftime(timeStr,sizeof(timeStr),"%D-%T",localtime(&tmp_t));
	snprintf(dbgBuffer,sizeof(dbgBuffer),"%s:%s:%s:%d:%s\n",timeStr,__FILE__,fn,line,str);
	printf("%s\n",dbgBuffer);
	return 0;
}
int installSignalHdlr(void)
{
    static char fn[]="installSignalHdlr()";
	
	errno=0;
	if( (signal(SIGTERM,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGTERM Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGINT,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGINT Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGHUP,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGHUP Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
#ifndef ARM_ARCH	
	errno=0;
	if( (signal(SIGKILL,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGKILL Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGSTOP,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGSTOP Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
#endif	
	errno=0;
	if( (signal(SIGSEGV,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGSEGV Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGQUIT,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGQUIT Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGABRT,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGABRT Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGPIPE,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGPIPE Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGALRM,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGALRM Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	errno=0;
	if( (signal(SIGCHLD,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGCHLD Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
	
	errno=0;
	if( (signal(SIGUSR1,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGUSR1 Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	return 0;
}
void doProperClose(void)
{
	static char fn[]="doProperClose()";

	if(i2cFd != 0)
		close(i2cFd);
	if(mcUartFd != 0)
		close(mcUartFd);
	if(sockWaitFd != 0)
		close(sockWaitFd);
	if(eamSockFd != 0)
		close(eamSockFd);
	dbgLog(FATAL,fn,__LINE__,"Finished Proper Closing. Exitting..");
	return;	
}
void sigHandler(int sig)
{
	static int secCnt=0,minCnt=0;

    if( (sig ==4) ||(sig == 2) || (sig==9) || (sig==11) )
    {
        doProperClose();
		_exit(1);
    }
    else if( sig == SIGUSR1)
    {
    }
    else if( sig == SIGALRM)
    {
        secCnt++;
        if(secCnt == 60)
        {
            secCnt=0;
            if(minCnt == 60)
                minCnt = 0;
            else
                minCnt++;

        }
        alarm(1);
    }
}

int init(void)
{
	static char fn[]="init()";
	
	errno = 0;
	if( (atexit(&doProperClose)) != 0){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"atexit reg failed. err:%d %s",errno,strerror(errno));
		dbgLog(FATAL,fn,__LINE__,mainDbgBuffer);
		return -1;
	}

	if((installSignalHdlr()) < 0){
		dbgLog(FATAL,fn,__LINE__,"Signal Hdlr Install Failed.");
		return -1;
	}

	if((i2cFd=openI2CBus(I2C_CHA_2,devAddr)) <= 0){
		dbgLog(FATAL,fn,__LINE__," I2C Init Failed.");
		return -1;
	}
	if((mcUartFd=openSerPort(BB_MC_UART_NUM)) <= 0){
		dbgLog(FATAL,fn,__LINE__,"Serial Port Init Failed");
		return -1;
	}
	
	if( (initSock(QC_LISTN_PORT)) < 0){
		dbgLog(FATAL,fn,__LINE__,"Socket Init Failed");
		return -1;
	}
	return 0;
}
int main(int argc,char *argv[])
{
	if((init()) < 0)
		return 0;
	startQc();	
	return 0;
}
