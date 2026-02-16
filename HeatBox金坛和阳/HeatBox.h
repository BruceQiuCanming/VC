// HeatBox.h : main header file for the HEATBOX application
//

#if !defined(AFX_HEATBOX_H__C1EEF5A1_3BA6_458A_B100_F420A0E71A0A__INCLUDED_)
#define AFX_HEATBOX_H__C1EEF5A1_3BA6_458A_B100_F420A0E71A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CHeatBoxApp:
// See HeatBox.cpp for the implementation of this class
//
#define DIRECT_HEAT_DELTA	(4.0f)
#define PID_HEAT_DELTA		(1.0f)
#define STEP_COOL_DELTA		(5.0f)

//#define HIGH_HIGH_LEVEL_DELTA		(3.0f)

#define OVER_HEAT_DELTA		(0.9f)


//#define _HANJIE_192

extern int G_iHighHighLevelDelta;
extern float   G_SwitchTempAdjust[6][500][2];
extern  int G_iMaxLedNr;
extern	int	G_iLedLines;
extern	int	G_iLedNrPerLine;
extern CString theAppDirectory;
extern CString G_sCompany;
extern int		G_CurMaxHeatBox;
#define BK_COLOR	RGB(0, 160, 128)
#define WHITE_COLOR	RGB(255, 255, 255)
extern CBrush	G_bkBrush;
extern CBrush	G_WhiteBrush;
extern CTime STD_TIME;

#define INVALID_TEMP	-999.9f




#define MAX_HEAT_BOX		5
extern unsigned short UNICODE_FILE_HEAR;
extern unsigned short CSV_COMMMA;
typedef enum 
{
	OMRON_PV	= 0x2000,
	OMRON_SP	= 0x2601,
	OMRON_PID_T = 0x2708,
	OMRON_PID_P = 0x2A00,
	OMRON_PID_I = 0x2A01,
	OMRON_PID_D = 0x2A02,


}OMRON_CMD_ADDR;


typedef enum 
{
	NOT_CAL				=	0,
	
	CAL_CLEAR_ADJUST,

	CAL_SET_NORMAL_PID_LOW,

	CAL_SET_NORMAL_PID_HIGH,

}CAL_STEP;

typedef enum 
{
	CLOSE_TEMP_TYPE_RANGE					=	0,
	
	CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP			=	1,

	CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE=	2,

}CLOSE_TEMP_TYPE;

extern CString strMsgCaption;

typedef struct
{
	bool IsUsed;				//产品是否插入
	bool IsClosing;				//现在是否闭合
	bool IsOpenned;				//是否曾经打开过
	bool IsClosed;				//是否曾经复位过
	bool IsOpenFlash;			//是否动作时闪动
	bool IsCloseFlash;			//是否复位时闪动
	bool IsOpenFlashTested;		//是否已经测试了动闪，就是对这个产品升温开门了吗
	bool IsCloseFlashTested;	//是否已经测试了复闪，就是对这个产品关门了吗
	bool IsMoreThan1M;			//for PTC
	float	OpenTemp;			//动作温度
	float	CloseTemp;			//复位温度
	float	OpenFlashTemp;		//动作温度
	float	CloseFlashTemp;		//复位温度
	long	OpenTime;
	long	CloseTime;
}SWITCH_TEST_RESULT;

typedef struct
{
	int ID;
	SWITCH_TEST_RESULT result;
}SWITCH_TEST_RESULT_EX;

typedef struct
{
	int		switch_nr;
	int		level;
	SWITCH_TEST_RESULT test_result;
}SWITCH_TEST_RESULT_AND_LEVEL;

typedef struct
{
	int		ParaType;
	double MainLevel;
	double MainLevelSpanMinus;
	double MainLevelSpanPlus;
	double HighLevel;
	double HighLevelSpanMinus;
	double HighLevelSpanPlus;
	double LowLevel;
	double LowLevelSpanMinus;
	double LowLevelSpanPlus;
	
	double CloseLevel;
	double CloseLevelSpanPlus;
	double CloseLevelSpanMinus;

	double CloseHighLevel;
	double CloseHighLevelSpanPlus;
	double CloseHighLevelSpanMinus;

	double CloseLowLevel;
	double CloseLowLevelSpanPlus;
	double CloseLowLevelSpanMinus;

	unsigned int OpenDoorResetLevel;
	unsigned int TakeoutTime;

	//================
	double CloseLevelDeviationPlus;
	double CloseLevelDeviationMinus;

	double CloseHighLevelDeviationPlus;
	double CloseHighLevelDeviationMinus;

	double CloseLowLevelDeviationPlus;
	double CloseLowLevelDeviationMinus;

}SWITCH_CONFIG_PARA;

typedef struct
{
	char type;
	unsigned int para[4];
}AUTO_TEST_PARA;



#define COMM_MSG				WM_USER+1000
#define START_CHAR				0x02
#define END_CHAR				0x03
#define RS485_PARA_LEN			(16)
#define SWITCH_STATUS_DATA_LEN	(32)
//
#define SWITCH_FLASH_DATA_LEN	(32+32)
typedef struct
{
	char start;
	char des_id;
	char source_id;
	char command;
}ASCII_RS485_HEADER;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char chk[2];
	char end;
}ASCII_RS485_READ_COMMAND;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char val[RS485_PARA_LEN];
	char chk[2];
	char end;
}ASCII_RS485_COMMAND;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char val[RS485_PARA_LEN+SWITCH_STATUS_DATA_LEN];
	char chk[2];
	char end;
}ASCII_RS485_COMMAND_READ_TEMP;

typedef struct 
{
	ASCII_RS485_HEADER header;
	char val[SWITCH_FLASH_DATA_LEN];
	char chk[2];
	char end;
}ASCII_RS485_COMMAND_READ_FLASH;

#define SWITCH_STATUS_ADDR	'A'

typedef enum 
{
	SUB_MODE_READY			=	0,
	SUB_MODE_DIRECT_HEAT	=	1,
	SUB_MODE_NORMAL_PID		=	2,
	SUB_MODE_STEP_PID		=	3,
	SUB_MODE_DIRECT_COOL	=	4,
	SUB_MODE_STEP_COOL		=	5,
	SUB_MODE_HEAT_AGAIN		=	6,

	SUB_COOL_MODE_READY			=	10,
	SUB_COOL_MODE_DIRECT_COOL	=	11,
	SUB_COOL_MODE_PID_COOL		=	12,
	SUB_COOL_MODE_STEP_COOL		=	13,
	SUB_COOL_MODE_DIRECT_HEAT	=	14,
	SUB_COOL_MODE_PID_HEAT		=	15,
	SUB_COOL_MODE_STEP_HEAT		=	16,

}AUTO_TEST_STEP;

typedef enum 
{
	HEAT_BOX_TYPE_NORMAL		=	0,
	HEAT_BOX_TYPE_PTC			=	1,
	HEAT_BOX_TYPE_PTC_NO_TEST	=	2,
	HEAT_BOX_TYPE_HANJIE		=	3,
}HEAT_BOX_TYPE;
typedef enum 
{
	HEAT_COOL_UNKNOW		=	0,
	HEAT_FIRST_COOL_SECOND	=	1,
	COOL_FIRST_HEAT_SECOND	=	2

}HEAT_COOL_ORDER;

typedef struct
{
	double open_temp_min;
	double open_temp_max;
	double close_temp_min;
	double close_temp_max;
}CONTROL_TEMP_RANGE;

typedef struct
{
	unsigned char cmd;
	TCHAR    text[100];
}COMMAND_TEXT;

//#ifdef _ASCII_
typedef enum
{
	CMD_NULL					= 0,


	CMD_OMRON_READ				= 0x03,
	CMD_OMRON_WRITE				= 0x06,

	CMD_STOP_OUT				= '0',
	CMD_FULL_OUT				= '1',
	CMD_NORMAL_PID_OUT			= '2',
	CMD_STEP_PID_OUT			= '3',
	CMD_READ_TEMP				= '4',
	CMD_READ_PID_T				= '5',
	CMD_SET_PID_T				= '6',
	CMD_SET_ADJUST_LOW			= '7',
	CMD_SET_ADJUST_HIGH			= '8',	
	CMD_READ_ADJUST_LOW			= '9',
	CMD_READ_ADJUST_HIGH		= 0x3A, // ':',

	CMD_READ_FLASH				= 0x3B,	//';',
	CMD_SET_SWITCH_NORMAL_MODE	= 0x3C,
//	CMD_AUTO_TEST				= 0x3D,
//	CMD_SET_HIGH_VOLT			= 0x3E,
//	CMD_SET_RELAY				= 0x3F,

//	CMD_READ_AUTHOR				= 'A',
//	CMD_READ_BUILDDATE			= 'B',
//	CMD_READ_PRODUCEDATE		= 'C',
//	CMD_SAVE_ADJUST_BOARD		= 'D',
//	CMD_READ_ADJUST_BOARD_LOW	= 'E',
//	CMD_READ_ADJUST_BOARD_HIGH	= 'F',
//	CMD_READ_SN					= 'G',
//	CMD_SET_ADJUST_ALL			= 'H',

	CMD_CPLD_RESET				=	0x7E,

	CMD_READ_STATUS				=	0x10,
	CMD_SET_AIR_VALVE			=   0x11,
	CMD_SET_AIR_CONTROL			=   0x12,
	CMD_SET_HEAT				=   0x13,
	CMD_SET_FAN					=   0x14,
	CMD_SET_SSR_5				=   0x15,

	CMD_START_OPEN_FLASH_TEST	=  0x20,
	CMD_START_CLOSE_FLASH_TEST  =  0x21,
	CMD_END_FLASH_TEST			=  0x22,

	CMD_SET_DISPLAY_TYPE		= 'H',
	CMD_READ_ADJUST_ALL			= 0x7D,
	CMD_WRITE_STEP_PID_COUNT	= 0x80,
	CMD_READ_STEP_PID_COUNT		= 0x81,
	CMD_SET_COOL_RELAY			= 0x82,
	CMD_STEP_DOWN_PID_OUT       = 0x83,

	CMD_PROGRAM_START			= 0xF1,
	CMD_PROGRAM_DATA			= 0xF2,
	CMD_PROGRAM_END				= 0xF3,

}COMMAND_ID;


typedef enum
{
	ERROR_LEVEL				= -1,
	NO_LEVEL				= 0,
	MAIN_LEVEL				= 1,
    HIGH_LEVEL				= 2,
    LOW_LEVEL				= 3,
	OVER_HIGH_LEVEL			= 4,
    OVER_LOW_LEVEL			= 5,
	CLOSE_HIGH_LEVEL		= 6,
	CLOSE_LOW_LEVEL			= 7,
	OPEN_FLASH_LEVEL		= 8,
	CLOSE_FLASH_LEVEL		= 9,
	OPEN_DOOR_RESET_LEVEL	=	10,
	HIGH_HIGH_LEVEL			=	11,
	NOT_USED				=	12,
	NO_PTC_LEVEL					= 13,
	HIGH_LEVEL_CLOSE_HIGH_LEVEL		= 14,
	HIGH_LEVEL_CLOSE_LOW_LEVEL		= 15,
	LOW_LEVEL_CLOSE_HIGH_LEVEL		= 16,
	LOW_LEVEL_CLOSE_LOW_LEVEL		= 17,
}TEST_RESULT_LEVEL;

typedef struct
{
	int ID;
	TCHAR str[100];
}COMMAND_STRING;

typedef struct
{
	char  AutoTestFlag;
	char  SubMode;
	short GoalTemp;
	short RemainTime;
	short CurTemp[4];
	char dummy[2];

}WORK_STATUS;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	char para[128/8];

	char crc[2];
}MODBUS_RS485_CMD_128;


typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	char para[4];
	char crc[2];
}MODBUS_RS485_CMD_OMRON;


typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	char para[129];
	char crc[2];
}MODBUS_RS485_CMD_PROGRAM;

typedef struct
{
	unsigned char	RS485_Addr;
	unsigned char	cmd;
	unsigned char	para_len;
	char			para[16];
	char			crc[2];
}MODBUS_RS485_CMD_500;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	char para[2];
	char crc[2];
}MODBUS_RS485_ANSWER_OMRON_READ;

typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	char para[4];
	char crc[2];
}MODBUS_RS485_ANSWER_OMRON_WRITE;


typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
#ifdef _INTERFACE_5X20_
	char para[13+13];
#else
	char para[13+13];
#endif
	char crc[2];
}MODBUS_RS485_ANSWER_500;







typedef struct
{
	int		size;
	long BeginStart_From201001010000Minutes;
	long EndStart_From201001010000Minutes;
	char				SwitchType;
	TCHAR				ParaName[20];
	TCHAR				Memo[100];
	SWITCH_CONFIG_PARA	ConfigPara;
	SWITCH_TEST_RESULT	TestResult[(500 + 7)];
}RECORD;



typedef union
{
	short v;
	char  buf[2];
}SHORT_BUF;

typedef CArray<MODBUS_RS485_CMD_PROGRAM,MODBUS_RS485_CMD_PROGRAM>CCommCmdProgram;
typedef CArray<MODBUS_RS485_CMD_OMRON ,MODBUS_RS485_CMD_OMRON> CCommCmdArray_OMRON;
typedef CArray<MODBUS_RS485_CMD_500 ,MODBUS_RS485_CMD_500> CCommCmdArray_500;


typedef CArray<double ,double> CDoubleArray;

class CHeatBoxApp : public CWinApp
{
public:
	int m_TabActiveIndex[2*MAX_HEAT_BOX];
	CHeatBoxApp();

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatBoxApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHeatBoxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEATBOX_H__C1EEF5A1_3BA6_458A_B100_F420A0E71A0A__INCLUDED_)
