
#ifndef _COMM_H_
#define _COMM_H_

#include "stdafx.h"
#include "modbus.h"



#define MAX_HEAT_BOX	1
#define INVALID_TEMP	999.9f
#define COMM_MSG		WM_USER+1000

 

#define SWITCH_STATUS_ADDR	'A'

#define NEW_192_INTERFACE_PCB	0
#define OLD_192_INTERFACE_PCB	1

extern unsigned char STM32_ID[][12];
extern unsigned char STM32_CopyRight[8];
extern int			G_ProgramStarted;



//#include "stdAfx.h"
#pragma pack(push)
#pragma pack(1)	




typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char Addr_H;
	unsigned char Addr_L;
}MODBUS_RS485_HEADER;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char DataLen;
}MODBUS_RS485_ANSWER_HEADER;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char data[2];
	unsigned short CRC;
}MODBUS_RS485_CMD_DATA_LEN_1;
typedef struct
{
	MODBUS_RS485_ANSWER_HEADER head;

	unsigned char data[2*16];
	unsigned short CRC;
}MODBUS_RS485_READ_ANSWER_WORD_16;
typedef struct
{
	MODBUS_RS485_ANSWER_HEADER head;
	unsigned char data[2 * 3];
	unsigned short CRC;
}MODBUS_RS485_READ_ANSWER_WORD_3;
typedef struct
{
	MODBUS_RS485_ANSWER_HEADER head;

	unsigned char data[2];
	unsigned short CRC;
}MODBUS_RS485_WRITE_ANSWER_WORD_1;

typedef struct
{
	MODBUS_RS485_ANSWER_HEADER head;

	unsigned char data[4];
	unsigned short CRC;
}MODBUS_RS485_ANSWER_WORD_2;


typedef struct
{
	MODBUS_RS485_ANSWER_HEADER head;

	char para[64*sizeof(short)+2]; //+2 为了不和 MODBUS_RS485_CMD_PROGRAM 冲突
	unsigned short crc;
}MODBUS_RS485_NTC_AD_VAL_ANSWER;

typedef struct
{
	short  Temp[20];	//Temp
	short  Sv;
	char TestMode;
	char OutputStatus;
	signed char HeatPercent;
	unsigned char PcbType;//CoolPercent;
	unsigned short  iError;

}MODBUS_RS485_20TEMP_5SSR_ANSWER_PARA;

typedef struct
{
	short	Sv;
	char	UsedPT100[5];
	char    Tel[4];	//7,8,9,10 == '8598'
	char    dummy2[5];

}MODBUS_RS485_20TEMP_5SSR_CMD_PARA;

typedef struct
{
	MODBUS_RS485_ANSWER_HEADER head;
	MODBUS_RS485_20TEMP_5SSR_ANSWER_PARA para;
	unsigned short crc;
}MODBUS_RS485_20TEMP_5SSR_ANSWER;


typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[128/8];
	unsigned short crc;
}MODBUS_RS485_CMD_128;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[32];
	unsigned short crc;
}MODBUS_RS485_CMD_32PT100;

typedef struct
{
	unsigned short  Temp[32];	//Temp
	unsigned short  Sv;
	unsigned	TestMode:8;
	unsigned	OUTPUT_STATUS_DOOR_ON_BIT:1;
	unsigned	OUTPUT_STATUS_AIR_ON_BIT:1;
	unsigned	OUTPUT_STATUS_AIR_VALVE_ON_BIT:	1;
	unsigned	OUTPUT_STATUS_BEEP_BIT:1;
	unsigned	OUTPUT_STATUS_HIGH_VOLT_BIT:1;
	unsigned	AutoTune_recs_id:3;
	unsigned	HeatPercent:8;
	unsigned	TempDecimalPlaces:8;
	
}MODBUS_RS485_32TEMP_ANSWER_PARA;

typedef struct
{
	MODBUS_RS485_HEADER head;
	MODBUS_RS485_32TEMP_ANSWER_PARA para;
	unsigned short crc;
}MODBUS_RS485_32TEMP_ANSWER;

typedef struct
{
	MODBUS_RS485_HEADER head;
	signed char para[32];
	unsigned short crc;
}MODBUS_RS485_32TEMP_CMD_ADJUST;

typedef struct
{

   short    Temp_Sv;
   short    Temp_Sv_2;
   char     Temp_Control_Mode;
   char     Temp_Control_Mode_2;
   unsigned TestMode:	2;
   unsigned TestMode_2:	2;
   unsigned WorkMode:	4;
   unsigned WorkMode_2:	4;
   unsigned IsClosing:	1;
   unsigned IsClosing_2:1;
   unsigned Beep:		1;


}LoadLifeTestBench_WORK_PARA;


typedef struct
{
	MODBUS_RS485_HEADER head;
	short   Pv[8];
	short	Sv[8];
	signed char	HeatPercent[8];
	unsigned char TestMode[8];
	unsigned short Error;
	unsigned char SSR_Status;
	unsigned short crc;
}MODBUS_RS485_8TEMP_8SSR_ANSWER;


//14:59:15.93收←◆:010204000020D9
//14:59:21.93收←◆:01020860001085
//14:59:24.93收←◆:010310320001B9
//14:59:27.93收←◆:01030E7A00056F
//14:59:30.109收←◆:010310320001B9

//读 X
//3A 30 31 30 32 30 34 30 30 30 30 32 30 44 39 0D 0A
//3A 30 31 30 32 30 34 30 30 30 30 32 30 30 30 44 39 0D 0A 

//读D(0x1032)的值
//3A 30 31 30 33 31 30 33 32 30 30 30 31 42 39 0D 0A
//Answer: 3A 30 31 30 33 30 32 30 30 30 30 46 41 0D 0A

//读C(0x0E7A)的值,5个计数
//3A 30 31 30 33 30 45 37 41 30 30 30 35 36 46 0D 0A 
//Answer 3A, 30 31, 30 33, 30 41 30 46 42 36 30 30 30 30 30 37 38 38 30 30 30 30 30 32 39 44 46 46 0D 0A


//读M(0x0860)的值
//       3A 30 31 30 32 30 38 36 30 30 30 31 30 38 35 0D 0A
//Answer 3A 30 31 30 32 30 32 30 30 30 30 46 42 0D 0A

// 自动标志 0x0866,赋值 0xFF00,手动
// 复位标志 0x0867,赋值 0xFF00
// 启动标志 0x0867,赋值 0x0000
// 停止标志 0x0865,赋值 0xFF00

//'强制地址 0x0867
//:01,05,0867,FF00,8C


typedef enum
{
	X00	=	0,
	X01,
	X02,
	X03,
	X04,
	X05,
	X06,
	X07,

	X10,
	X11,
	X12,
	X13,
	X14,
	X15,
	X16,
	X17,

	X20,
	X21,
	X22,
	X23,
	X24,
	X25,
	X26,
	X27,
	

}X_INPUT;









#define	X_Has_Switch							X00
#define	X_Low_Level_Input_Cylinder				X06

#define X_Low_Level_Test_Cylinder				X10
#define X_Low_Level_Test_Switch					X11

#define X_Low_Level_Output_Cylinder				X12		
#define X_Low_Level_Fail_Cylinder				X14


#define	X_High_Level_Input_Cylinder				X16

#define X_High_Level_Test_Cylinder				X21
#define X_High_Level_Test_Switch				X22

#define X_High_Level_Output_Cylinder			X23
#define X_High_Level_Selected_Pass_Cylinder		X25
#define X_High_Level_Selected_Fail_Cylinder		X26
#define X_High_Volt_Pass						X27

typedef enum
{
	Y00	=	0,
	Y01,
	Y02,
	Y03,
	Y04,
	Y05,
	Y06,
	Y07,

	Y10,
	Y11,
	Y12,
	Y13,
	Y14,
	Y15,
	Y16,
	Y17,

	Y20,
	Y21,
	Y22,
	Y23,
	Y24,
	Y25,
	Y26,
	Y27,
	

}Y_OUTPUT;

//:0105 08C8 FF002B 低温进料电磁阀动作
#define	Y_Low_Level_Input_Cylinder				Y00

//:0105 08CB FF0028 低温检测电磁阀动作
#define Y_Low_Level_Test_Cylinder				Y03

//#define Y_Low_Level_Test_Light					Y04

//:0105 08CC FF0027 低温出料电磁阀动作
#define Y_Low_Level_Output_Cylinder				Y04	

//:0105 08CE FF0025 低温接料电磁阀动作
#define Y_Low_Level_Fail_Cylinder				Y06

//3A 30 31 30 35 30 38 43 46 46 46 30 30 32 34 0D 0A
//:0105 08CF FF0024
#define	Y_High_Level_Input_Cylinder				Y07

//:0105 08D0 FF0023
#define Y_High_Level_Test_Cylinder				Y10

//#define Y_High_Level_Test_Light					Y11

#define Y_High_Level_Output_Cylinder			Y11

#define Y_High_Level_Selected_Cylinder			Y12

#define Y_High_Volt_Pass						Y14

#define Y_Red_Light								Y14
#define Y_Green_Light							Y15
#define Y_Yellow_Light							Y16

#define DELTA_READ_POINT						0x01
#define DELTA_READ_POINT_X						0x02
#define DELTA_READ_POINT_Y						0x02
#define DELTA_READ_M							0x02
#define DELTA_READ_D							0x03
#define DELTA_READ_C							0x03

#define DELTA_SET_POINT							0x05
#define DELTA_SET_POINT_OFF						0x0000
#define DELTA_SET_POINT_ON						0xFF00
#define DELTA_SET_MEMERY						0x06

#define DELTA_X_ADDR							0x0400
#define DELTA_Y_READ_ADDR						0x0500
#define DELTA_Y_ADDR							0x08C8
#define DELTA_M_ADDR							0x0800
#define DELTA_D_ADDR							0x1000
#define DELTA_C_ADDR							0x0E00


typedef struct
{
	unsigned char STX;

// S addr = 0x0000 - - 0x03FF
// X addr = 0x0400 - - 0x04FF
// Y addr = 0x0500 - - 0x05FF
// T addr = 0x0600 - - 0x06FF
// M addr = 0x0700 - - 0xB9FF
// C addr = 0x0E00 - - 0x0EFF
// D addr = 0x1000 - - 0xA70F
	unsigned char Addr[2];
	
	//====
	// 01 读节点状态 S,Y,M，T，C(不可读输入节点状态)
	// 02 读节点状态 S,X，Y,M，T，C(可读输入节点状态)
	// 03 读寄存器的内容值 T，C，D
	// 05 强制单节点状态On.Off	S,Y,M,T,C
	// 06 预设单独寄存器的值 T,C,D
	// 15(0x0FH) 强制多个节点状态On.Off  S,Y,M,T,C
	// 16(0x10H) 预设多个寄存器的值 T,C,D
	// 17（0x11H）报告从站地址
	// 23 (0x17H) PLC LINK 在一个轮询时间内同时执行读写功能
	unsigned char Cmd[2];


}DELTA_PLC_CMD_HEAD;


typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Addr[4];
	

	unsigned char Data_Len[4];

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_READ_CMD;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Addr[4];
	

	unsigned char Data[4];

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_SET_CMD;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[8];

	unsigned char LCR[2];

	unsigned char Tail[2];

}DELTA_PLC_READ_X_ANSWER;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[4];

	unsigned char LCR[2];

	unsigned char Tail[2];

}DELTA_PLC_READ_Y_ANSWER;


typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[4];
	

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_READ_D_ANSWER;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];

	unsigned char Data[4];
	

	unsigned char LCR[2];
	
	unsigned char Tail[2];
}DELTA_PLC_READ_M_ANSWER;

typedef struct
{
	DELTA_PLC_CMD_HEAD head;

	unsigned char Data_Len[2];


	unsigned char Data[20];

	unsigned char LCR[2];

	unsigned char Tail[2];

}DELTA_PLC_READ_C_ANSWER;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[16];
	unsigned short crc;
}MODBUS_RS485_CMD_ARM_128;

typedef struct
{
	MODBUS_RS485_HEADER head;
	unsigned char para[129];
	unsigned short crc;
}MODBUS_RS485_CMD_PROGRAM;


#pragma pack(pop)







typedef struct
{
	int ID;
	TCHAR str[100];
}COMMAND_STRING;

extern void GetUsefulCommPort(CStringArray *CommStrArray);

extern bool G_bCMD_192_READ_STATUS;
extern bool    G_IsJustCmdAscii;


extern	int								G_iCmdAscii[MAX_HEAT_BOX];

extern unsigned char CalCheckSum(char *buf,int len);

extern unsigned char Buf2Char(unsigned char * buf);

void WriteLogFile(char *msg, int BoxNr, bool NoTimeLimit = true);
unsigned short PcShort2OmronShort(unsigned short v);
unsigned short OmronShort2PcShort(unsigned short v);
LRESULT DealOnComm_RS485(WPARAM wParam,LPARAM lParam,CWnd * ParentWnd);
LRESULT DealOnComm_Delta_PLC(WPARAM wParam,LPARAM lParam,CWnd * ParentWnd);
unsigned int CRC16_Ex ( unsigned char *arr_buff, unsigned int len);


UINT ReadCommThreadProc_PLC(LPVOID pParam);
UINT ReadCommThreadProc_PCB(LPVOID pParam);



typedef struct
{
	HWND	m_hWnd;
	void	*comm;
	UINT	MessageID;
	CWinThread *ReadThread;

}COMM_THREAD_PARA;


//void CRC16(unsigned char *source, int  n,  unsigned char ReturnData[2]);
//bool CheckCRC16(unsigned char *source, int  n, unsigned char source_crc[2]);
/////////////////////////////////////////////////////////////////////////////
class CComm 
{
private:
	
	OVERLAPPED ov;

	
	
public:
	
	
	int	 m_OpenCount;
	bool bReadComm;
	HANDLE m_hComm;
	//NORMAL_CONFIG_PARA	G_NormalConfigPara[BoxNr];

	MODBUS_RS485_CMD_ALL	m_LastSendCmd;
	CTime					m_LastRecTime;
	unsigned char OSCCAL;
	CComm();
	int OpenComm(CString commName,COMM_THREAD_PARA *para,DCB dcb);
	int OpenComm(CString commName,COMM_THREAD_PARA *para);

	int CloseComm(void);
	int ReadComm(char *buf,int len); 
	int WriteComm(void *buf,int len);
	bool IsOpened(void);
	bool	m_bXModem;
    virtual ~CComm();
	CTime	m_BeepStartTime[5];

	
	 bool CheckCRC16(unsigned char *source, int  n, unsigned short source_crc);
	static unsigned short CRC16(unsigned char *source, int  n );//, unsigned char ReturnData[2]);
	static unsigned short CRC16_MODBUS(unsigned char *source, int  n ); //, unsigned char ReturnData[2])
	 unsigned short XMODEM_CRC(char *ptr, int count);
	int SetBaudrate(HANDLE hComm,DWORD BaudRate,BYTE ByteSize,BYTE StopBits ,BYTE Parity);
	int CComm::Hex2Bin(CString HexFile, char* binData, int MaxLen);
	int CComm::Hex2Bin(unsigned char* Hex, int HexBufLen, char* binData, int MaxLen);

	void CComm::Set_HuiKong_DIO_ONOFF(int DO_Nr, bool On);
	static	CString GetCmdString(unsigned char cmd);
};





extern	COMM_THREAD_PARA G_CommPara;

extern int								G_ProgramBoxNr;

extern int								G_iLangID ;


#endif


