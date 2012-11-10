/*
# Header File Macro Definition
* The header file macro used to identify which project and which file
* File name is upto 'H' letter
* And the project name is upto 'P' letter
*/

#ifndef __I2C_H_QUADROCOPTER_P__
 #define __I2C_H_QUADROCOPTER_P_

#include <linux/i2c-dev.h>
#define I2C_CHA_1	"/dev/i2c-1"
#define I2C_CHA_2	"/dev/i2c-2"
#define I2C_CHA_3	"/dev/i2c-3"

#define MAX_DBG_BUFF_SIZE	256

const unsigned int	openI2CBus(const char *devName,const int devAddr);

int		 	dbgLog(char type,char *fn,int line,char *str);
void 		doProperClose(void);
int 		readFromI2CBus(const unsigned int fd,unsigned char *buffer,const unsigned int len);
int 		writeToI2CBus(const unsigned int fd,const unsigned char *buffer,const unsigned int len);
#endif
