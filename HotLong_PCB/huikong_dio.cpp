#include "stdafx.h"
#include "modbus.h"
#include    "string.h"
#include "comm.h"
#include "huikong_dio.h"


CDevice_HuiKong_DIO::CDevice_HuiKong_DIO()
{
	m_BoxNr = 1;
}
void CDevice_HuiKong_DIO::Init(int CommNr,int BoxNr)
{
    this->m_BoxNr = BoxNr;

}

CDevice_HuiKong_DIO::~CDevice_HuiKong_DIO(void)
{

}


void CDevice_HuiKong_DIO::Read_X(CComm *comm,int Start_X,int counts)
{
    MODBUS_RS485_CMD_READ read;

    memset(&read,0,sizeof(read));

    read.RS485_Addr = m_BoxNr;
    read.cmd        = MODBUS_CMD_READ4;

    read.DataAddr_H = Start_X / 256;
    read.DataAddr_L = Start_X % 256;

    read.DataLen_H    = counts / 256;
    read.DataLen_L    = counts % 256;


    read.crc  =   CRC16_MODBUS((unsigned char*)&read,sizeof(read) - 2);
    comm->WriteComm(&read,sizeof(read));
    comm->m_LastSendCmd.device_ID = DEVICE_ID_HUIKONG_DIO;
    comm->m_LastSendCmd.data_size      = sizeof(read);
    memcpy(&comm->m_LastSendCmd.RS485_Addr,&read.RS485_Addr,sizeof(read));

}

void CDevice_HuiKong_DIO::SetBaudrate(CComm *comm)
{
	if(comm->m_hComm != 0)
	{
		comm->SetBaudrate(comm->m_hComm,38400,8,0,0);
	}
}
void CDevice_HuiKong_DIO::Read_Y(CComm *comm,int Start_Y, int counts)
{

    MODBUS_RS485_CMD_READ read;

    memset(&read,0,sizeof(read));

    read.RS485_Addr = m_BoxNr;
    read.cmd        = MODBUS_CMD_READ;

    read.DataAddr_H = Start_Y / 256;
    read.DataAddr_L = Start_Y % 256;

    read.DataLen_H    = counts / 256;
    read.DataLen_L    = counts % 256;


    read.crc  =   CRC16_MODBUS((unsigned char*)&read,sizeof(read) - 2);
    comm->WriteComm(&read,sizeof(read));
    comm->m_LastSendCmd.device_ID = DEVICE_ID_HUIKONG_DIO;
    comm->m_LastSendCmd.data_size      = sizeof(read);
    memcpy(&comm->m_LastSendCmd.RS485_Addr,&read.RS485_Addr,sizeof(read));
}


void CDevice_HuiKong_DIO::SetY(int Relay_ID,bool On)
{
    MODBUS_RS485_CMD_SET_WORD_1 set_word_1;

    memset(&set_word_1,0,sizeof(set_word_1));

    set_word_1.RS485_Addr = m_BoxNr;
    set_word_1.cmd        = MODBUS_CMD_WRITE;

    set_word_1.DataAddr_H = Relay_ID / 256;
    set_word_1.DataAddr_L = Relay_ID % 256;
    if(On)
    {
        set_word_1.Data_H     = 0;
        set_word_1.Data_L     = 1;
    }
    else
    {
        set_word_1.Data_H     = 0;
        set_word_1.Data_L     = 0;
    }

    set_word_1.crc  =   CRC16_MODBUS((unsigned char*)&set_word_1,sizeof(set_word_1) - 2);

    SetCmdString(sizeof(MODBUS_RS485_CMD_SET_WORD_1) ,&set_word_1);

}


void CDevice_HuiKong_DIO::SetCmdString(int size, void* para)
{
        MODBUS_RS485_CMD_ALL all;
        all.device_ID       = DEVICE_ID_HUIKONG_DIO;
        all.data_size       = size;
        memcpy(&all.RS485_Addr,para,size);
        this->AppendWaitCmd(all);

}

void CDevice_HuiKong_DIO::DealOnComm(unsigned char *Data,int DataLen)
{
	if (m_LastSendCmd.device_ID != DEVICE_ID_HUIKONG_DIO)
	{
		return;
	}
	m_LastRecTime = CTime::GetCurrentTime();

	if(m_LastSendCmd.cmd == 0x03)
	{
		if (DataLen == sizeof(MODBUS_RS485_READ_ANSWER_WORD_16))
		{
			MODBUS_RS485_READ_ANSWER_WORD_16* Read_Answer_Word_16 = (MODBUS_RS485_READ_ANSWER_WORD_16*)Data;

			unsigned short* para = (unsigned short*)Read_Answer_Word_16->data;
			for(int i = 0; i < 16; i++)
			{
				this->m_Y[i] = para[i];
			}
			
	
		}
	}
	else if(m_LastSendCmd.cmd == 0x06)
	{
		if (DataLen == sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1))
		{
			RemoveFirstWaitCmd();
		}
	}
}