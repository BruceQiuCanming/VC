#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_


#define INVALID_TEMP	-999.9f
#define INVALID_OPEN_TEMP	9999.9f
#define INVALID_CLOSE_TEMP	-9999.9f

	#define MAX_LED_NR			128
	#define LED_NR_PER_LINE		8
	#define LED_LINES			8

typedef enum 
{
	NOT_CAL				=	0,
	
	CAL_CLEAR_ADJUST,

	CAL_SET_NORMAL_PID_LOW,

	CAL_SET_NORMAL_PID_HIGH,

}CAL_STEP;



extern CString strMsgCaption;

typedef struct
{
	bool IsUsed;		//产品是否插入
	bool IsClosing;	    //现在是否闭合
	bool IsOpenned;		//是否曾经打开过
	bool IsClosed;		//是否曾经复位过
	bool IsOpenFlash;	//是否动作时闪动
	bool IsCloseFlash;	//是否复位时闪动
	bool IsOpenFlashTested;   //是否已经测试了动闪，就是对这个产品升温开门了吗
	bool IsCloseFlashTested;  //是否已经测试了复闪，就是对这个产品关门了吗
	float OpenTemp;		//动作温度
	float CloseTemp;	//复位温度
//	float OpenFlashTemp;	//动作温度
//	float CloseFlashTemp;	//复位温度
}SWITCH_TEST_RESULT;

typedef struct
{
	int		switch_nr;
	int		level;
	SWITCH_TEST_RESULT test_result;
}SWITCH_TEST_RESULT_AND_LEVEL;



typedef struct
{
	char type;
	unsigned int para[4];
}AUTO_TEST_PARA;



#define COMM_MSG	WM_USER+1000








#define SWITCH_STATUS_ADDR	'A'

typedef enum
{
	CMD_NULL			= 0,
	
	CMD_READ_STATUS				= 0x10,
	CMD_READ_OPEN_FLASH			= 0x11,
	CMD_READ_CLOSE_FLASH		= 0x12,
	CMD_START_OPEN_FLASH_TEST	= 0x13,
	CMD_START_CLOSE_FLASH_TEST	= 0x14,
	CMD_END_FLASH_TEST			= 0x15,
	CMD_SET_FLASH_TEST_SPEED	= 0x16,
  
	CMD_COOL_FULL_OUT			= 0x20,
	CMD_COOL_PID_OUT			= 0x21,
	CMD_COOL_STEP_PID_OUT		= 0x22,

	CMD_STOP_OUT		= '0',
	CMD_FULL_OUT		= '1',
	CMD_NORMAL_PID_OUT	= '2',
	CMD_STEP_PID_OUT	= '3',
	CMD_READ_TEMP		= '4',
	CMD_READ_PID_T		= '5',
	CMD_SET_PID_T		= '6',
	CMD_SET_ADJUST_LOW	= '7',
	CMD_SET_ADJUST_HIGH	= '8',	
	CMD_READ_ADJUST_LOW = '9',
	CMD_READ_ADJUST_HIGH= 0x3A, // ':',

//	CMD_READ_FLASH		= 0x3B,	//';',
	CMD_SET_SWITCH_NORMAL_MODE	= 0x3C,
	CMD_AUTO_TEST				= 0x3D,
	CMD_SET_HIGH_VOLT			= 0x3E,
	CMD_SET_RELAY				= 0x3F,
	
	CMD_READ_AUTHOR				= 'A',
	CMD_READ_BUILDDATE			= 'B',
	CMD_READ_PRODUCEDATE		= 'C',
	CMD_SAVE_ADJUST_BOARD		= 'D',
	CMD_READ_ADJUST_BOARD_LOW	= 'E',
	CMD_READ_ADJUST_BOARD_HIGH	= 'F',
	CMD_READ_SN					= 'G',
	CMD_SET_DISPLAY_TYPE		= 'H',
	CMD_SET_LOGO_FONT_NR		=	'I',
	CMD_SET_LOGO_FONT_0			=	'J',
	CMD_SET_LOGO_FONT_1			=	'K',
	CMD_SET_LOGO_FONT_2			=	'L',
	CMD_SET_LOGO_FONT_3			=	'M',
	CMD_SET_LOGO_FONT_4			=	'N',
	CMD_SET_LOGO_FONT_5			=	'O',
	CMD_SET_LOGO_FONT_6			=	'P',
	CMD_SET_LOGO_FONT_7			=	'Q',
	CMD_SET_LOGO_FONT_8			=	'R',
	CMD_SET_LOGO_FONT_9			=	'S',
	
	CMD_SET_ADJUST_ALL			= 0x7C,
	CMD_READ_ADJUST_ALL			= 0x7D,
	CMD_CPLD_RESET				= 0x7E,
	CMD_WRITE_STEP_PID_COUNT	= 0x80,
	CMD_READ_STEP_PID_COUNT		= 0x81,

	CMD_AIR_VALVE				= 0x84,
	CMD_FAN						= 0x85,
	CMD_PID_COOL				= 0x86,
	CMD_AT						= 0x87,
	CMD_COOL_AT					= 0x88,
	CMD_PERCENT_HEAT_WITH_AIR	= 0x89,

	CMD_PROGRAM_START			= 0xF1,
	CMD_PROGRAM_DATA			= 0xF2,
	CMD_PROGRAM_END				= 0xF3,

}COMMAND_ID;



enum WORK_MODE
{
	WORK_MODE_IDLE,		//
	WORK_MODE_READY,
	WORK_MODE_STEP_PID,
	WORK_MODE_COOL
};

#pragma pack(push)
#pragma pack(1)	

typedef struct
{
	char RS485_Addr;
	char cmd;
	char para_len;
	char para[(MAX_LED_NR + 7)/8];
	unsigned char crc[2];
}MODBUS_RS485_CMD;

typedef struct
{
	char RS485_Addr;
	char cmd;
	char para_len;

	char para[(MAX_LED_NR + 7)/8 * 2];
	unsigned char crc[2];
}MODBUS_RS485_CMD_ANSWER;

typedef struct
{
	float		CurTemp;
	float		CurSv;
	char		HeatPercent;
	char		CoolPercent;
	char		TestMode;
	unsigned	AirControlIsOn	: 1;
	unsigned	ValveIsOn 		: 1;
	unsigned	dummy_bits		: 30;
	char		dummy[1];
	char		SwitchStatus[16];
}MODBUS_RS485_READ_TEMP_ANSWER;

enum CONTROL_MODE
{

	SUB_MODE_STOP					=	0,	
	SUB_MODE_DIRECT_HEAT			=	1,
	SUB_MODE_NORMAL_PID				=	2,
	SUB_MODE_INCREASE_PID			=	3,
	SUB_MODE_DIRECT_COOL			=	4,
	SUB_MODE_PID_COOL				=	5,
	SUB_MODE_STEP_PID_COOL			=	6,
	SUB_MODE_AT						=	7,
	SUB_MODE_COOL_AT				=	8,
	SUB_MODE_OUTSIDE_AIR_HEAT		=	9,
	SUB_MODE_PERCENT_HEAT_WITH_AIR	=	10,

};
typedef struct
{
	char TestMode;
	char TestSpeed;
	char HeatPercent;
	char CoolPercent;
	char dummy[12];
}WORK_STATUS;
typedef struct
{
	unsigned short VoltAmp[8];
}VOLT_AMP_AD;


typedef struct
{
	unsigned char RS485_Addr;
	unsigned char cmd;
	unsigned char para_len;
	char para[129];
	unsigned char crc[2];
}MODBUS_RS485_CMD_PROGRAM;

#pragma pack(pop)

typedef CArray<MODBUS_RS485_CMD_PROGRAM,MODBUS_RS485_CMD_PROGRAM>CCommCmdProgram;
typedef CArray<MODBUS_RS485_CMD,MODBUS_RS485_CMD>CModbus_RS485_Cmd_Array;

#define HEX_TYPE_DataRecord						0
#define HEX_TYPE_EndOfFileRecord				1
#define HEX_TYPE_ExtendedSegmentAddressRecord	2
#define HEX_TYPE_StartSegmentAddressRecord		3
#define HEX_TYPE_ExtendedLinearAddressRecord	4
#define HEX_TYPE_StartLinearAddressRecord		5


typedef struct
{
	unsigned char header;
	unsigned char dataLen[2];
	unsigned char dataAddr[4];
	unsigned char dataType[2];
	unsigned char data[100];
}HEX_LINE;

#endif

