#include "q_headers.h"
#include "q_i2c.h"
#include "q_struct.h"


unsigned char i2cDbgBuffer[MAX_DBG_BUFF_SIZE];
unsigned char i2cBuffer[16];
unsigned int i2c_2fd	=0;
unsigned int devAddr	=0b00101001;

const unsigned int	openI2CBus(const char *devName,const int devAddr)
{
	static char fn[]="openI2CBus()";
	static int fd= 0;
	
	errno=0;
	fd =-1;

	if((fd	= open(devName,O_RDWR)) <= 0){
		snprintf(i2cDbgBuffer,sizeof(i2cDbgBuffer),"I2CDev : %s Open Failed",devName);
		dbgLog(FATAL,fn,__LINE__,i2cDbgBuffer);
		return -1;
	}
	else{
		snprintf(i2cDbgBuffer,sizeof(i2cDbgBuffer),"I2CDev : %s Open Success",devName);
		dbgLog(INFORM,fn,__LINE__,"Opening I2C-[ OK ] ");
	}
	errno = 0;
	if((ioctl(fd,I2C_SLAVE,devAddr)) < 0) {
		snprintf(i2cDbgBuffer,sizeof(i2cDbgBuffer),"I2CDev;%s Dev:%d Access Failed",devName,devAddr);
		dbgLog(FATAL,fn,__LINE__,i2cDbgBuffer);
		return -1;
	}
	else{
		snprintf(i2cDbgBuffer,sizeof(i2cDbgBuffer),"I2CDev:%s Dev:%d Access Success",devName,devAddr);
		dbgLog(INFORM,fn,__LINE__,i2cDbgBuffer);
	}
	return fd;
}
	
int readFromI2CBus(const unsigned int fd,unsigned char *buffer,const unsigned int len)
{
	static char fn[]="readFromI2CBus()";
	int ret= 0;

	errno = 0;
	if((ret = read(fd,buffer,len)) <= 0){
		sprintf(i2cDbgBuffer,"Read failed. Err:%d:%s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,i2cDbgBuffer);	
	}
	else{
		sprintf(i2cDbgBuffer,"Read Success. read:%d bytes",ret);
		dbgLog(REPORT,fn,__LINE__,i2cDbgBuffer);	
	}
	return ret;
}

int writeToI2CBus(const unsigned int fd,const unsigned char *buffer,const unsigned int len)
{
	static char fn[]="readFromI2CBus()";
	int ret= 0;
	errno = 0;
	if((ret = write(fd,buffer,len)) <= 0){
		sprintf(i2cDbgBuffer,"write failed. Err:%d:%s",errno,strerror(errno));
		dbgLog(REPORT,fn,__LINE__,i2cDbgBuffer);	
	}
	else{
		sprintf(i2cDbgBuffer,"Write Success. wrote:%d bytes",ret);
		dbgLog(REPORT,fn,__LINE__,i2cDbgBuffer);	
	}
	return ret;
}


