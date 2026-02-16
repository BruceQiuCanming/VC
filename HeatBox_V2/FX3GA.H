#ifndef _FX3GA_H_
#define  _FX3GA_H_
#include "stdAfx.h"


#define  FX3GA_READ		'0'
#define  FX3GA_WRITE	'1'
#define  FX3GA_ON		'7'
#define  FX3GA_OFF		'8'

#define  FX3GA_STX		0x02
#define  FX3GA_ETX		0x03
#define  FX3GA_ENQ		0x05
#define  FX3GA_ACK		0x06
#define  FX3GA_NAK		0x15

#pragma pack(push)
#pragma pack(1)	

typedef struct{
	unsigned char   STX;
	unsigned char   RS485_Addr;
	unsigned char   cmd[2];
	unsigned char   DataAddr[4];
	unsigned char   DataLen[2];
	unsigned char   ETX;
	unsigned char   CheckSum[2];
}FX3GA_CMD_READ;

typedef struct{
	unsigned char   STX;			//0x02
	unsigned char   RS485_Addr;
	unsigned char   cmd[2];			//"E104"
	unsigned char   DataAddr[4];	// 0DE
	unsigned char   DataLen[2];		// 02
	unsigned char   Data[4];		//	00C8 = 200
	unsigned char   ETX;			//
	unsigned char   CheckSum[2];
}FX3GA_CMD_WRITE;

typedef struct{
    unsigned char   STX;
	unsigned char   RS485_Addr;
    unsigned char   cmd[2];
    unsigned char   DataAddr[4];
    unsigned char   DataLen[2];
    unsigned char   ETX;
    unsigned char   CheckSum[2];
}FX3GA_CMD_READ_M;

//02 45 37 31 37 34 32 03 34 44 06
typedef struct{
    unsigned char   STX;
	unsigned char   RS485_Addr;
    unsigned char   cmd;
    unsigned char   DataAddr[4];
    unsigned char   ETX;
    unsigned char   CheckSum[2];
}FX3GA_CMD_SET_M;

#pragma pack(pop)

extern unsigned int FX3GA_2Word2int(char * buf);
extern unsigned char FX3GA_Hex2Char(unsigned char Hex[2]);
extern int FX3GA_Get_M_Addr(unsigned char DataAddr[2]);
extern void FX3GA_CheckSum(unsigned char * DataBuf,int len,unsigned char* CheckBuf);
extern FX3GA_CMD_READ FX3GA_Read(unsigned int DataAddr,int DataSize);
extern FX3GA_CMD_WRITE FX3GA_Write(unsigned int DataAddr,int Data);
extern FX3GA_CMD_READ_M FX3GA_Read_M(unsigned int DataAddr,int DataSize);
extern FX3GA_CMD_SET_M FX3GA_Set_M528_M543_On(unsigned int DataAddr);
extern FX3GA_CMD_SET_M FX3GA_Set_M528_M543_Off(unsigned int DataAddr);
extern FX3GA_CMD_SET_M FX3GA_Set_M128_M255_On(unsigned int DataAddr);
extern FX3GA_CMD_SET_M FX3GA_Set_M128_M255_Off(unsigned int DataAddr);
extern int FX3GA_Get_DATA_Addr(unsigned char DataAddr[3]);


extern FX3GA_CMD_SET_M FX3GA_Set_STOP(void);
extern FX3GA_CMD_SET_M FX3GA_Set_RUN(void);
#endif