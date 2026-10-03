#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#define HIGH_VOLT
#define _TM1638_
//#define _RESET_

#define HEATBOX128_V1_0
//#define HEATBOX4X32_V1_0



//#define AUTHOR_ADDRESS			0x04
#define OSCCAL_ADDRESS			0x04
#define SN_ADDRESS				0x08


#define MODBUS
#define RS485
#define ADJUST 
#define PARA_PASSWORD	1357
#define AT_PASSWORD		2468
#define PRODUCT_DATE_PASSWORD		179

//Mode
enum MAIN_MODE
{
	NORMAL_MODE,
	PASSWORD_MODE,
#ifdef RS485	
	SCALE_NR_MODE,
#endif	
#ifdef ADJUST 	
	ADJUST_LOW_MODE	,
	ADJUST_HIGH_MODE,
	ADJUST2_LOW_MODE,
	ADJUST2_HIGH_MODE,
	ADJUST3_LOW_MODE,
	ADJUST3_HIGH_MODE,
	ADJUST4_LOW_MODE,
	ADJUST4_HIGH_MODE,
#endif
	ALARM_HIGH_MODE,
	SV_MODE,
	PID_P_MODE,
	PID_I_MODE,
	PID_D_MODE,
	PID_T_MODE,
	PID_EXCEED,
	PID_AUTO_PASSWORD_MODE,
	PID_AUTO_MODE,
	PID_AUTO_TIMEOUT_MODE,
	MAX_MODE
};

enum CONTROL_MODE
{
	CONTROL_DIRECT_HEAT,
	CONTROL_NORMAL_PID,
	CONTROL_INCREASE_PID,
	CONTROL_STOP,
	CONTROL_DIRECT_COOL,
	CONTROL_COOL_STEP_PID,
	CONTROL_COOL_PID,
};

#define EEPROM_START_ADDRESS	0x10

typedef struct
{
	char dummy;
	unsigned int StepPidCount;
	signed int	AdjustAll;
	signed int  AdjustLow[4];//-20~+20
	
	signed int  AdjustHigh[4];//-20~+20
#ifdef THREE_POSTION
	signed int AlarmLow;//0~+300
#endif	
//	signed int AlarmMid;//0~+300
	signed int AlarmHigh;//0~300
	long Sv;//0~300	
	signed int Pid_P;
	signed int Pid_I;
	signed int Pid_D;
	signed int Pid_T;
	char   	   Pid_Exceed;  
	char	   Lock;
	char       RS485_id;	
	char	   dummy2;
	int       iChkSum;		
}stEEPROM;



//#define KEY_OK	4




////////////////////////////
//PID.C


  ////////////////
  //DEVICE   
void StartAC(void); 
void timer0_init(void); 
void init_devices(void);
void DisSelectLed(void);

char AnlysizeRs485Data(void);                                                                                                                                               
void DealSend(void);
  int GetTempEntry(int ad_value);
  int FindTemp(int ad_value);
  void RunAD(void);
  void RunAdForTest(void);
  void BTAOut(int bta);
  void ReadRx(void);
  extern int newBTA;
  void adc_init(char chanel);
//  #define MAX_BTA_STEPS 200 
  //void Iout(int temp); 
  
//enum {NO_ERR,TOO_LOW,TOO_HIGH};
//enum AC_CHANEL{NORMAL_AC,AD0_AC,AD1_AC,AD2_AC};
//extern int  int_temp;//测量温度的整数值
//extern int cur_temp;	//温度的整数值+一位小数
//extern char cChanel;  

  ////////////////
  //DISLAY

void spi_init(void);

//////////////////
//main
void long_delay(void);
//void delay(void);
void WritePara(void);
void WritePara_1(void);
void WritePara_2(void);
void WritePara_3(void);
void WritePara_4(void);
void WritePara_5(void);
void ReadPara(void);
extern char display_flag;

void NewPID(void);
int CheckSum(char* source,unsigned char len);

typedef enum
{
	CMD_NULL			= 0,
	
	CMD_SET_RELAY_RESET	= 0x15,
	CMD_COOL_FULL_OUT	= 0x20,
	CMD_COOL_PID_OUT	= 0x21,
	CMD_COOL_STEP_PID_OUT	= 0x22,
	
	
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

	CMD_READ_FLASH		= 0x3B,	//';',
	CMD_SET_SWITCH_NORMAL_MODE = 0x3C,
	CMD_AUTO_TEST_START	= 0x3D,
	CMD_SET_HIGH_VOLT	= 0x3E,
	CMD_SET_RELAY		= 0x3F,
	
	
	CMD_READ_AUTHOR		= 'A',
	CMD_READ_BUILDDATE	= 'B',
	CMD_READ_PRODUCEDATE= 'C',
	CMD_SAVE_ADJUST_BOARD	= 'D',
	CMD_READ_ADJUST_BOARD_LOW	= 'E',
	CMD_READ_ADJUST_BOARD_HIGH	= 'F',
	CMD_READ_SN					= 'G',
	CMD_DISPLAY_TYPE			= 'H',
				
	CMD_SET_ADJUST_ALL			= 0x7C,
	CMD_READ_ADJUST_ALL			= 0x7D,
	CMD_SET_UNLOCK				= 0x7E,
	CMD_SET_LOCK				= 0x7F,
	
	CMD_WRITE_STEP_PID_COUNT	= 0x80,
	CMD_READ_STEP_PID_COUNT		= 0x81,

}COMMAND_ID;


#define MAX_LED_NR	128

typedef struct
{
	char RS485_Addr;
	char cmd;
	char para_len;
	char para[(MAX_LED_NR+7)/8];
	char crc[2];
}MODBUS_RS485_CMD;

typedef struct
{
	char RS485_Addr;
	char cmd;
	char para_len;
	char para[(MAX_LED_NR+7) / 8 * 2];
	char crc[2];
	char dummy[2];
}MODBUS_RS485_ANSWER;

typedef struct
{
	char TestMode;
	char TestSpeed;
	char HeatPercent;
	char CoolPercent;
	char dummy[14];
}MODBUS_RS485_READ_TEMP_CMD;

typedef struct
{
	long CurTemp;
	long CurSv;
	char HeatPercent;
	char CoolPercent;
	char dummy[6];
	char SwitchStatus[16];
}MODBUS_RS485_READ_TEMP_ANSWER;
#endif