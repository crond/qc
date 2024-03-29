#include "q_headers.h"
#include "q_i2c.h"


#define SNP(x,y)  snprintf(x,sizeof(x),"%s:%s:%d:%s",__FILE__,fn,__LINE__,y);dbgLog(x)
#define SNEP(x,y) snprintf(x,sizeof(x),"%s:%s:%d:%s. err:%d:%s",__FILE__,fn,__LINE__,y,errno,strerror(errno))

unsigned char mainDbgBuffer[MAX_DBG_BUFF_SIZE];
unsigned char dbgBuffer[MAX_DBG_BUFF_SIZE];
unsigned char i2cBuffer[16];
unsigned int i2c_2fd	=0;
unsigned int devAddr	=0b00101001;

int dbgLog(char type,char *fn,int line,char *str)
{
	bzero(dbgBuffer,sizeof(dbgBuffer));
	snprintf(dbgBuffer,sizeof(dbgBuffer),"%s:%s:%d:%s\n",__FILE__,fn,line,str);
	printf("%s\n",dbgBuffer);
	return 0;
}
void doProperClose(void)
{
	close(i2c_2fd);
	printf("Finished Proper Closing. Exitting...\n");
}
int main(int argc,char *argv[])
{
	static char fn[]="main()";

	atexit(&doProperClose);
	printf("I2C Test Pgm \n");

	if((i2c_2fd=openI2CBus(I2C_CHA_2,devAddr)) <= 0) 	
		return 0;
	if(readFromI2CBus(i2c_2fd,i2cBuffer,4) <= 0) 	
		return 0;
	if(writeToI2CBus(i2c_2fd,i2cBuffer,4) <= 0) 	
		return 0;
}
const unsigned int	openI2CBus(const char *devName,const int devAddr)
{
	static char fn[]="openI2CBus()";
	static int fd= 0;
	
	errno=0;
	fd =-1;

	if((fd	= open(devName,O_RDWR)) <= 0){
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"I2CDev : %s Open Failed",devName);
		dbgLog(FATAL,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	else{
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"I2CDev : %s Open Success",devName);
		dbgLog(INFORM,fn,__LINE__,"Opening I2C-[ OK ] ");
	}
	errno = 0;
	if((ioctl(fd,I2C_SLAVE,devAddr)) < 0) {
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"I2CDev;%s Dev:%d Access Failed",devName,devAddr);
		dbgLog(FATAL,fn,__LINE__,mainDbgBuffer);
		return -1;
	}
	else{
		snprintf(mainDbgBuffer,sizeof(mainDbgBuffer),"I2CDev:%s Dev:%d Access Success",devName,devAddr);
		dbgLog(INFORM,fn,__LINE__,mainDbgBuffer);
	}
	return fd;
}
	
int readFromI2CBus(const unsigned int fd,unsigned char *buffer,const unsigned int len)
{
	static char fn[]="readFromI2CBus()";
	int ret= 0;

	errno = 0;
	if((ret = read(fd,buffer,len)) <= 0){
		sprintf(mainDbgBuffer,"Read failed. Err:%d:%s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);	
	}
	else{
		sprintf(mainDbgBuffer,"Read Success. read:%d bytes",ret);
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);	
	}
	return ret;
}

int writeToI2CBus(const unsigned int fd,const unsigned char *buffer,const unsigned int len)
{
	static char fn[]="readFromI2CBus()";
	int ret= 0;
	errno = 0;
	if((ret = write(fd,buffer,len)) <= 0){
		sprintf(mainDbgBuffer,"write failed. Err:%d:%s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);	
	}
	else{
		sprintf(mainDbgBuffer,"Write Success. wrote:%d bytes",ret);
		dbgLog(REPORT,fn,__LINE__,mainDbgBuffer);	
	}
	return ret;
}
