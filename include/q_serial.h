

/*
# Header File Macro Definition
* The header file macro used to identify which project and which file
* File name is upto 'H' letter
* And the project name is upto 'P' letter
*/

#ifndef __SERIAL_H_QUADROCOPTER_P__
 #define __SERIAL_H_QUADROCOPTER_P_



#define HFC	0
#define DB	CS8
#define SB	0
#define BASE_PORT_STR	"/dev/ttyO"
#define BB_MC_UART_NUM	1

int openPort(int);
int readFromUart(char *buffer,const int len,const unsigned int fd);
int writeToUart(const char *buffer,const int len,const unsigned int fd);

#endif
