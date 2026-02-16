#include "stdAfx.h"
#include "FX3GA.h"
/*
	地址，4个 char,从高到低
	数据，2个 char,从高到低；如果是4个char的，后面2个是高位，前面2个是低位
*/
unsigned char ASCII_09AF[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

unsigned char Shakehand_CMD_1[1] = {0x05};
unsigned char Shakehand_CMD_2[11] = {0x02,0x30,0x30,0x45,0x30,0x32,0x30,0x32,0x03,0x36,0x43};
unsigned char Shakehand_CMD_3[11] = {0x02,0x30,0x30,0x45,0x43,0x41,0x30,0x32,0x03,0x38,0x45};
unsigned char Shakehand_CMD_4[13] = {0x02,0x45,0x30,0x30,0x30,0x45,0x45,0x38,0x30,0x34,0x03,0x46,0x45};
unsigned char Shakehand_CMD_5[21] = {0x02,0x45,0x31,0x30,0x30,0x45,0x45,0x43,0x30,0x34,0x34,0x32,0x44,0x33,0x36,0x38,0x37,0x43,0x03,0x43,0x46};
unsigned char Shakehand_CMD_6[6] = {0x02,0x41,0x31,0x03,0x37,0x35};

unsigned char FX3GA_Hex2Char(unsigned char Hex[2])
{
    unsigned char c = 0 ;

    if(Hex[0] >= '0' && Hex[0] <= '9')
    {
        c = Hex[0] - '0';
    }
    else if(Hex[0] >= 'A' && Hex[0] <= 'F')
    {
        c = Hex[0] - 'A' + 10;
    }
    c *= 16;


    if(Hex[1] >= '0' && Hex[1] <= '9')
    {
        c += Hex[1] - '0';
    }
    else if(Hex[1] >= 'A' && Hex[1] <= 'F')
    {
        c += Hex[1] - 'A' + 10;
    }

    return c;
}

void FX3GA_CheckSum(unsigned char * DataBuf,int len,unsigned char* CheckBuf)
{
	unsigned int CheckSum = 0;


	for(int i = 0; i < len; i++)
	{
		CheckSum += DataBuf[i];
	}

	CheckSum = CheckSum & 0x000000FF;
	
	
	CheckBuf[1] = ASCII_09AF[ CheckSum & 0x0F];
	CheckSum /= 16;
	CheckBuf[0] = ASCII_09AF[ CheckSum & 0x0F];

}
void FX3GA_SetDataAddr(unsigned int DataAddr,unsigned char *buf)
{
	unsigned int Addr = DataAddr;

	buf[0]		=	'4';

	buf[1] = Addr / 256;
	
	Addr = (Addr % 256);
	buf[2] = Addr / 16;

	Addr = (Addr % 16);
	buf[3] = Addr;

	buf[1] = ASCII_09AF[buf[1]];
	buf[2] = ASCII_09AF[buf[2]];
	buf[3] = ASCII_09AF[buf[3]];

}
void FX3GA_SetDataLen(unsigned int DataLen,unsigned char *buf)
{
	unsigned int len = DataLen;

	buf[0]	= len / 16;
	
	len		= (len %16);
	buf[1]	= len ;

	
	buf[0] = ASCII_09AF[buf[0]];
	buf[1] = ASCII_09AF[buf[1]];
	
}

void FX3GA_SetData(unsigned int Data,unsigned char *buf)
{
	unsigned int data;

	data	= Data % 256;
	buf[0]	= data / 16;
	data	= (data %16);
	buf[1]	= data ;
	buf[0]	= ASCII_09AF[buf[0]];
	buf[1]	= ASCII_09AF[buf[1]];


	data	= Data / 256;
	buf[2]	= data / 16;
	data	= (data %16);
	buf[3]	= data ;
	buf[2]	= ASCII_09AF[buf[2]];
	buf[3]	= ASCII_09AF[buf[3]];

	
}

FX3GA_CMD_READ FX3GA_Read(unsigned int DataAddr,int DataSize)
{
	static FX3GA_CMD_READ cmd;
	cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr	=	'E';
	cmd.cmd[0]		=	FX3GA_READ;//'0';
	cmd.cmd[1]		=	'0';

	
	FX3GA_SetDataAddr(DataAddr,cmd.DataAddr);
	FX3GA_SetDataLen (DataSize,cmd.DataLen);
	cmd.ETX			=	FX3GA_ETX;
	FX3GA_CheckSum(&cmd.cmd[0],sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;

}

FX3GA_CMD_WRITE FX3GA_Write(unsigned int DataAddr,int Data)
{
	static FX3GA_CMD_WRITE cmd;
	cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr		=	'E';
	cmd.cmd[0]		=	FX3GA_WRITE;//'1';
	cmd.cmd[1]		=	'0';
	
	FX3GA_SetDataAddr(DataAddr,cmd.DataAddr);
	FX3GA_SetDataLen (2,cmd.DataLen);
	FX3GA_SetData(Data,cmd.Data);
	cmd.ETX			=	FX3GA_ETX;
	FX3GA_CheckSum(&cmd.cmd[0],sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;

}

void FX3GA_Read_M_Addr(unsigned int DataAddr,unsigned char *buf)
{
    unsigned int Addr = DataAddr / 16;

	buf[0]		=	'8';
    buf[1]		=	'8';
    

	Addr *= 2;

    buf[2] = Addr / 16;

    Addr = (Addr % 16);
    buf[3] = Addr;

    buf[2] = ASCII_09AF[buf[2]];
    buf[3] = ASCII_09AF[buf[3]];

}

void FX3GA_Set_M528_M543_Addr(unsigned int DataAddr,unsigned char *buf)
{
    unsigned int Addr = DataAddr / 16;

	buf[0]	=	'1';

	buf[1] =   ASCII_09AF[DataAddr % 16];

	Addr *= 2;

    buf[2] = Addr / 16;

    Addr = (Addr % 16);
    buf[3] = Addr;

    buf[2] = ASCII_09AF[buf[2]];
    buf[3] = ASCII_09AF[buf[3]];

}

FX3GA_CMD_SET_M FX3GA_Set_M528_M543_On(unsigned int DataAddr)
{
	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_ON;
	FX3GA_Set_M528_M543_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}

FX3GA_CMD_SET_M FX3GA_Set_M528_M543_Off(unsigned int DataAddr)
{
	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_OFF;
	
	FX3GA_Set_M528_M543_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}

void FX3GA_Set_M128_M255_Addr(unsigned int DataAddr,unsigned char *buf)
{
    unsigned int Addr = DataAddr / 16;

	buf[0]	=	ASCII_09AF[Addr];

	buf[1] =   ASCII_09AF[DataAddr % 16];

	
    

    buf[2] = '4';
    buf[3] = '0';

}
FX3GA_CMD_SET_M FX3GA_Set_M128_M255_On(unsigned int DataAddr)
{
	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_ON;
	FX3GA_Set_M128_M255_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}

FX3GA_CMD_SET_M FX3GA_Set_M128_M255_Off(unsigned int DataAddr)
{
	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_OFF;
	
	FX3GA_Set_M128_M255_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}

FX3GA_CMD_READ_M FX3GA_Read_M(unsigned int DataAddr,int DataSize)
{
    static FX3GA_CMD_READ_M cmd;
    cmd.STX			=	FX3GA_STX;
    cmd.RS485_Addr	=	'E'; 
    cmd.cmd[0]		=	FX3GA_READ;//'0';
    cmd.cmd[1]		=	'0';
    FX3GA_Read_M_Addr(DataAddr,cmd.DataAddr);
    FX3GA_SetDataLen (DataSize,cmd.DataLen);
    cmd.ETX			=	FX3GA_ETX;
    FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
    return cmd;

}

int AF09_2_int(char c)
{
    if(c >= '0' && c <= '9')
    {
        return c - '0';
    }

    if(c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return -1;
}

unsigned int FX3GA_2Word2int(char * buf)
{
    unsigned int ret = 0;

    ret = AF09_2_int(buf[1]);
    ret += AF09_2_int(buf[0]) * 16;
    ret += AF09_2_int(buf[3]) * 16 * 16;
    ret += AF09_2_int(buf[2]) * 16 * 16 * 16 ;

    return ret;
}

int FX3GA_Get_M_Addr(unsigned char DataAddr[2])
{
    unsigned int Addr = 0 ;

    if(DataAddr[0] >= '0' && DataAddr[0] <= '9')
    {
        Addr = DataAddr[0] - '0';
    }
    else if(DataAddr[0] >= 'A' && DataAddr[0] <= 'F')
    {
        Addr = DataAddr[0] - 'A' + 10;
    }
    Addr *= 16;


    if(DataAddr[1] >= '0' && DataAddr[1] <= '9')
    {
        Addr += DataAddr[1] - '0';
    }
    else if(DataAddr[1] >= 'A' && DataAddr[1] <= 'F')
    {
        Addr += DataAddr[1] - 'A' + 10;
    }

    return Addr;

}

int FX3GA_Get_DATA_Addr(unsigned char DataAddr[3])
{
    unsigned int Addr = 0 ;

    if(DataAddr[0] >= '0' && DataAddr[0] <= '9')
    {
        Addr = DataAddr[0] - '0';
    }
    else if(DataAddr[0] >= 'A' && DataAddr[0] <= 'F')
    {
        Addr = DataAddr[0] - 'A' + 10;
    }
    Addr *= 16;


    if(DataAddr[1] >= '0' && DataAddr[1] <= '9')
    {
        Addr += DataAddr[1] - '0';
    }
    else if(DataAddr[1] >= 'A' && DataAddr[1] <= 'F')
    {
        Addr += DataAddr[1] - 'A' + 10;
    }
    Addr *= 16;

    if(DataAddr[2] >= '0' && DataAddr[2] <= '9')
    {
        Addr += DataAddr[2] - '0';
    }
    else if(DataAddr[2] >= 'A' && DataAddr[2] <= 'F')
    {
        Addr += DataAddr[2] - 'A' + 10;
    }
    return Addr;

}

//дD111 20.0
//02, 45 31 30, 34, 30 44 45, 30 32, 43 38 30 30, 03 44 33 //0x00C8 = 200 0x0DE = 222 = 2 * D111

FX3GA_CMD_SET_M FX3GA_Set_STOP(void)
{
	//02 45 37 32 44 34 31 03 35 41 06 //停止命令 

	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_ON;
	//FX3GA_Set_M528_M543_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.DataAddr[0]	=	'2';
	cmd.DataAddr[1]	=	'D';
	cmd.DataAddr[2]	=	'4';
	cmd.DataAddr[0]	=	'1';
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}

FX3GA_CMD_SET_M FX3GA_Set_RUN(void)
{
	//02 45 38 32 43 34 31 03 35 41 06 //运行命令

	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_OFF;
	//FX3GA_Set_M528_M543_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.DataAddr[0]	=	'2';
	cmd.DataAddr[1]	=	'D';
	cmd.DataAddr[2]	=	'4';
	cmd.DataAddr[0]	=	'1';
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}

FX3GA_CMD_SET_M FX3GA_Set_MAN(void)
{
	//02 45 38 32 46 34 31 03 35 44 06 //手动命令

	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_OFF;
	//FX3GA_Set_M528_M543_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.DataAddr[0]	=	'2';
	cmd.DataAddr[1]	=	'F';
	cmd.DataAddr[2]	=	'4';
	cmd.DataAddr[0]	=	'1';
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}

FX3GA_CMD_SET_M FX3GA_Set_AUTO(void)
{
	//02 45 37 33 30 34 31 03 34 37 06 //自动命令

	static FX3GA_CMD_SET_M cmd;


    cmd.STX			=	FX3GA_STX;
	cmd.RS485_Addr  =   'E';
	cmd.cmd			=	FX3GA_OFF;
	//FX3GA_Set_M528_M543_Addr(DataAddr,&cmd.DataAddr[0]);
	cmd.DataAddr[0]	=	'2';
	cmd.DataAddr[1]	=	'F';
	cmd.DataAddr[2]	=	'4';
	cmd.DataAddr[0]	=	'1';
	cmd.ETX	=	FX3GA_ETX;

	FX3GA_CheckSum(&cmd.RS485_Addr,sizeof(cmd) - 3,cmd.CheckSum);
	return cmd;
}



