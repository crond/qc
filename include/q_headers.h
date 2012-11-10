
/*
# Header File Macro Definition
* The header file macro used to identify which project and which file
* File name is upto 'H' letter
* And the project name is upto 'P' letter
*/

#ifndef __HEADERS_H_QUADROCOPTER_P__
 #define __HEADERS_H_QUADROCOPTER_P_

#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/ioctl.h>
#include<termios.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>


#define INFORM	0
#define REPORT	1
#define FATAL	2
#define MAX_DBG_BUFF_SIZE	256
#define MAX_EAM_BUFF_SIZE	8
#define ARM_ARCH			1
int		 	dbgLog(char type,char *fn,int line,char *str);
void 		doProperClose(void);
void 		sigHandler(int sig);
int 		installSignalHdlr(void);
int 		acceptEAMConnection(const int);
int 		initSock(const unsigned int listnPort);
int 		init(void);
#endif
