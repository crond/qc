#include "q_headers.h"
#include "q_i2c.h"
#include "q_serial.h"
#include "q_struct.h"

unsigned char mainDbgBuffer[MAX_DBG_BUFF_SIZE];
unsigned char dbgBuffer[MAX_DBG_BUFF_SIZE];

extern unsigned int i2c_2fd;
extern unsigned int devAddr;
extern unsigned int mcUartFd;

int dbgLog(char type,char *fn,int line,char *str)
{
	bzero(dbgBuffer,sizeof(dbgBuffer));
	snprintf(dbgBuffer,sizeof(dbgBuffer),"%s:%s:%d:%s\n",__FILE__,fn,line,str);
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
	
	errno=0;
	if( (signal(SIGKILL,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGKILL Reg Failed. Err:%d %s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	
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
	if( (signal(SIGSTOP,sigHandler)) == SIG_ERR){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"SIGSTOP Reg Failed. Err:%d %s",errno,strerror(errno));
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
	
}
void sigHandler(int sig)
{
	static int secCnt=0,minCnt=0;

    if( (sig ==4) ||(sig == 2) || (sig==9) || (sig==11) )
    {
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

	if((i2c_2fd=openI2CBus(I2C_CHA_2,devAddr)) <= 0){
		dbgLog(FATAL,fn,__LINE__," I2C Init Failed.");
		return -1;
	}
	if((mcUartFd=openSerPort(BB_MC_UART_NUM)) <= 0){
		dbgLog(FATAL,fn,__LINE__,"Serial Port Init Failed");
		return -1;
	}
	return 0;
}
int main(int argc,char *argv[])
{
	if((init()) < 0)
		return 0;

	return 0;
}
