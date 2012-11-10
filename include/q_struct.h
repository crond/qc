/*
# Header File Macro Definition
* The header file macro used to identify which project and which file
* File name is upto 'H' letter
* And the project name is upto 'P' letter
*/

#ifndef _STRUCT__H_QUADROCOPTER_P__
 #define __STRUCT_H_QUADROCOPTER_P_

/*
	MSG Format if EAM-BB are communicating, 
			1. 	UART/zigbee	:	START	MSGTYPE:DATA 	END
			2.  TCP/IP		:	QC_EAM_MSG structure	
*/

/*
	Message Type 0x01 to 0x09 are reserved for Admin Controls only.
	having ACK/NACK is good. But, need more MACROS. So, ACK is considered as sending back the same msg
	For, e.g, To turn ON the camara, QC_CAMARA_TURN_ON will get send  to BB. The same QC_CAMARA_TURN_ON will
	send back by the BB to EAM as ACk
*/

typedef struct{
		unsigned char msgType;
		unsigned char reserved1;
		unsigned char reserved2;
		unsigned char reserved3;
		unsigned char msg[64];
}QC_EAM_MSG;

#define QC_LISTN_PORT			10000
#define DBG_LOG_PORT			8000

//Macros using in EAM-BB UART Communication
#define START					0x55
#define END						0xAA

//Macros using in EAM-BB TCP/IP Communication
#define CTRL_MSG				0x55
#define	DBG_MSG					0xAA


#define RESP_TO					5   //Resp timeout in sec
#define QC_HC_MSG				0x01
#define	QC_BATT_LEV_REQ			0x02


#define QC_CAMARA_MSG			0x10
#define QC_CAMARA_TURN_ON		0x11
#define QC_CAMARA_TURN_OFF		0x12
#define QC_CAMARA_TURN_R		0x13
#define QC_CAMARA_TURN_L		0x14	

#define QC_DIR_MSG				0x20
#define QC_DIR_UP				0x21
#define QC_DIR_DOWN				0x22
#define QC_DIR_RHT				0x23
#define QC_DIR_LFT				0x24
#define QC_DIR_FWD				0x25
#define QC_DIR_REV				0x26
#define QC_DIR_SBY				0x27 //stand-by

//QC should be controlled by the QC_DIR_MSG only. This QC_FAN_MSG is to add more control over the propeller motors.Apply to all 4 at a time.
#define QC_FAN_MSG				0x30
#define QC_FAN_SPEED_LOW		0x31
#define QC_FAN_SPEED_MID		0x32
#define QC_FAN_SPEED_HIGH		0x33

//Control bytes, to control each motors individually
#define QC_MOTOR_A_OFF			0x60
#define QC_MOTOR_A_LOW			0x61
#define QC_MOTOR_A_MID			0x62
#define QC_MOTOR_A_HIGH			0x63
#define QC_MOTOR_B_OFF			0x64
#define QC_MOTOR_B_LOW			0x65
#define QC_MOTOR_B_MID			0x66
#define QC_MOTOR_B_HIGH			0x67
#define QC_MOTOR_C_OFF			0x68
#define QC_MOTOR_C_LOW			0x69
#define QC_MOTOR_C_MID			0x6A
#define QC_MOTOR_C_HIGH			0x6B
#define QC_MOTOR_D_OFF			0x6C
#define QC_MOTOR_D_LOW			0x6D
#define QC_MOTOR_D_MID			0x6E
#define QC_MOTOR_D_HIGH			0x6F

#define QC_OPER_MSG				0x90
#define QC_TAKE_OFF				0x91
#define QC_LAND					0x92

#define	QC_MODE_MSG				0x80
#define QC_MODE_MANUAL			0x81
#define QC_MODE_AUTMS			0x82 //autonoums mode

#endif
