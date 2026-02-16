#ifndef MODBUS_H
#define MODBUS_H


#pragma pack(push)
#pragma pack(1)

typedef enum
{
    MODBUS_CMD_READ  =				0x03,
    MODBUS_CMD_READ4 =				0x04,
    MODBUS_CMD_WRITE =				0x06,
}COMMAND_ID_MODBUS;

typedef enum
{
    DEVICE_ID_RUNBANG,
    DEVICE_ID_ANS_POWER,
    DEVICE_ID_ZS_K_RS485_I,
    DEVICE_ID_FX3GA,
    DEVICE_ID_PLC_DELTA,
    DEVICE_ID_HUIKONG_DIO,
    DEVICE_ID_OMRON,
    DEVICE_ID_JY_DAMA0303,
	DEVICE_ID_HUILONG_PCB,

}DEVICE_ID;

typedef struct
{
        unsigned char RS485_Addr;
        unsigned char cmd;
        unsigned char DataAddr_H;
        unsigned char DataAddr_L;

}MODBUS_RS485_CMD_HEAD;



typedef struct
{
        unsigned char RS485_Addr;

        unsigned char cmd;

        unsigned char DataAddr_H;
        unsigned char DataAddr_L;
        unsigned char DataLen_H;
        unsigned char DataLen_L;
        unsigned short crc;

}MODBUS_RS485_CMD_READ;
typedef struct
{

        unsigned char RS485_Addr;

        unsigned char cmd;

        unsigned char DataAddr_H;
        unsigned char DataAddr_L;
        unsigned char Data_H;
        unsigned char Data_L;
        unsigned short crc;

}MODBUS_RS485_CMD_SET_WORD_1;

typedef struct
{
    unsigned char RS485_Addr;
    unsigned char cmd;
    unsigned char DataAddr_H;
    unsigned char DataAddr_L;
    unsigned char Data_H;
    unsigned char Data_L;
    unsigned short crc;
}MODBUS_RS485_CMD_SET_ANSWER_WORD_1;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[2];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_1;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[4];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_2;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[8];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_4;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[12];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_6;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[16];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_8;

typedef struct
{
    unsigned char Addr;
    unsigned char cmd;
    unsigned char DataLen;
    unsigned char Data[24];
    unsigned short crc;
}MODBUS_RS485_CMD_READ_ANSWER_WORD_12;

typedef struct
{
    DEVICE_ID     device_ID;
    unsigned int  data_size;
    unsigned char RS485_Addr;
    unsigned char cmd;
    unsigned char DataAddr_H;
    unsigned char DataAddr_L;
    unsigned char para[200];

}MODBUS_RS485_CMD_ALL;

#pragma pack(pop)

extern unsigned char crc16_hi[256];
extern unsigned char crc16_lo[256];
extern unsigned short CRC16_MODBUS(unsigned char *source, int  n ); //, unsigned char ReturnData[2])


#endif // MODBUS_H

