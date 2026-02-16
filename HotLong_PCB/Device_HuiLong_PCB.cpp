#include "stdafx.h"
#include "Device_HuiLong_PCB.h"



CDevice_HuiLong_PCB::CDevice_HuiLong_PCB()
{
	m_Comm = this;
}

CDevice_HuiLong_PCB::~CDevice_HuiLong_PCB()
{

}

void CDevice_HuiLong_PCB::DealOnComm(unsigned char *Data,int DataLen)
{
	
	if (m_LastSendCmd.cmd == 0x06)
	{
		RemoveFirstWaitCmd();
		return;
	}

	if (m_LastSendCmd.cmd == 0x03)
	{
		if (DataLen == sizeof(MODBUS_RS485_READ_ANSWER_WORD_3))
		{
			MODBUS_RS485_READ_ANSWER_WORD_3* Read_Answer_Word_3 = (MODBUS_RS485_READ_ANSWER_WORD_3*)Data;
			CString s;
			unsigned char* para = (unsigned char*)Read_Answer_Word_3->data;
			unsigned short v;
			v = para[0] * 256 + para[1];
			m_Pv = v;

			v = para[4] * 256 + para[5];
			m_Sv = v;

	
		}

		if (DataLen == sizeof(MODBUS_RS485_READ_ANSWER_WORD_16))
		{
			MODBUS_RS485_READ_ANSWER_WORD_16* Read_Answer_Word_16 = (MODBUS_RS485_READ_ANSWER_WORD_16*)Data;
			CString s;
			unsigned char* para = (unsigned char*)Read_Answer_Word_16->data;
			unsigned short v;
			v = para[0] * 256 + para[1];
			m_Pv = v;

			v = para[2*2] * 256 + para[2*2+1];
			m_Sv = v;

			v = para[2*16] * 256 + para[2*16+15];
			m_Pp = v;
	
		}
	}

}
void CDevice_HuiLong_PCB::SetBaudrate(CComm *comm)
{
	if(comm->m_hComm != 0)
	{
		comm->SetBaudrate(comm->m_hComm,9600,8,0,0);
	}
}
void CDevice_HuiLong_PCB::SetSleepMode(bool Sleep)
{
	MODBUS_RS485_CMD_DATA_LEN_1 cmd;

	cmd.head.RS485_Addr = 0xFE;
	cmd.head.cmd		= 6;
	cmd.head.Addr_H		= 0;
	cmd.head.Addr_L		= 6;
	cmd.data[0]			= Sleep / 256;
	cmd.data[1]			= Sleep % 256;
	
	cmd.CRC = CComm::CRC16_MODBUS((unsigned char*)&cmd, sizeof(cmd) - 2);


	MODBUS_RS485_CMD_ALL all;
	all.device_ID = DEVICE_ID_HUILONG_PCB;
	all.data_size = sizeof(cmd);
	memcpy(&all.RS485_Addr, &cmd, sizeof(cmd));

	AppendWaitCmd(all);
}
void CDevice_HuiLong_PCB::ReadData(void)
{
	MODBUS_RS485_CMD_DATA_LEN_1 cmd;

	cmd.head.RS485_Addr = 0xFE;
	cmd.head.cmd		= 3;
	cmd.head.Addr_H		= 0;
	cmd.head.Addr_L		= 0;
	cmd.data[0]			= 16 / 256;
	cmd.data[1]			= 16 % 256;
	
	cmd.CRC = CComm::CRC16_MODBUS((unsigned char*)&cmd, sizeof(cmd) - 2);


	m_Comm->WriteComm( &cmd, sizeof(cmd));

	MODBUS_RS485_CMD_ALL all;
	all.device_ID = DEVICE_ID_HUILONG_PCB;
	all.data_size = sizeof(cmd);
	memcpy(&all.RS485_Addr, &cmd, sizeof(cmd));

	m_Comm->m_LastSendCmd = all;

}
void CDevice_HuiLong_PCB::SetSv(int Sv)
{

	MODBUS_RS485_CMD_DATA_LEN_1 cmd;

	cmd.head.RS485_Addr = 0xFE;
	cmd.head.cmd		= 6;
	cmd.head.Addr_H		= 0;
	cmd.head.Addr_L		= 2;
	cmd.data[0]			= Sv / 256;
	cmd.data[1]			= Sv % 256;
	
	cmd.CRC = CComm::CRC16_MODBUS((unsigned char*)&cmd, sizeof(cmd) - 2);


	MODBUS_RS485_CMD_ALL all;
	all.device_ID = DEVICE_ID_HUILONG_PCB;
	all.data_size = sizeof(cmd);
	memcpy(&all.RS485_Addr, &cmd, sizeof(cmd));

	AppendWaitCmd(all);
	

}