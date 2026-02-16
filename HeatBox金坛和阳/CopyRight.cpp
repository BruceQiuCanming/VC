// CopyRight.cpp: implementation of the CCopyRight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HeatBox.h"
#include "CopyRight.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "comm.h"

CCopyRight G_CopyRight;

extern CHeatBoxApp theApp;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TCHAR default_ProductDate[]=_T("20111010");
TCHAR default_BuildDate[]  =_T("20111010");
TCHAR default_Author[]={3,2,0,1,0,2,1,9,6,9,1,0,3,0,2,8,3,4};
CCopyRight::CCopyRight()
{
	int i;
	for(i = 0; i < MAX_HEAT_BOX; i++)
	{

		memcpy(&m_ProductDate[i][0],default_ProductDate,sizeof(default_ProductDate));
		memcpy(&m_BuildDate[i][0]  ,default_BuildDate,  sizeof(default_BuildDate));
		memcpy(&m_Author[i][0]     ,default_Author,     sizeof(default_Author)); ;
		m_SN[i] = 1000+i;
	}

	first_time = 0;
	last_time = 0;
}

CCopyRight::~CCopyRight()
{

}

void CCopyRight::ReadSN(int BoxNr,CStringArray *SendCmdArray)
{
	
/*

	CString cmdStr;
	
	if(G_iMaxLedNr == 500)
	{

		MODBUS_RS485_CMD_500 cmd;
	#ifndef _ASCII_
		cmd.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
	#else
		cmd.RS485_Addr = '1' + BoxNr;
	#endif
		cmd.cmd = CMD_READ_SN;
		cmd.para_len = sizeof(cmd.para);
		for( int i = 0; i < sizeof(cmd.para); i++)
		{
			cmd.para[i] = 0x00;  
		}
		cmd.crc[0] = CRC_START_VAL_0;
		cmd.crc[1] = CRC_START_VAL_1;
		CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc,(unsigned char*)cmd.crc); 	

		SetCmdString(&cmdStr, cmd.RS485_Addr,(COMMAND_ID)cmd.cmd, (char*)cmd.para);
		SendCmdArray->Add(cmdStr);
	}
	else
	{
		MODBUS_RS485_CMD_128 cmd;
	#ifndef _ASCII_
		cmd.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
	#else
		cmd.RS485_Addr = '1' + BoxNr;
	#endif
		cmd.cmd = CMD_READ_SN;
		cmd.para_len = sizeof(cmd.para);
		for( int i = 0; i < sizeof(cmd.para); i++)
		{
			cmd.para[i] = 0x00;  
		}
		cmd.crc[0] = CRC_START_VAL_0;
		cmd.crc[1] = CRC_START_VAL_1;
		CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc,(unsigned char*)cmd.crc); 	

		SetCmdString(&cmdStr, cmd.RS485_Addr,(COMMAND_ID)cmd.cmd, (char*)cmd.para);
		SendCmdArray->Add(cmdStr);
	}
*/
}

void CCopyRight::SetCmdString(CString *cmdstr,int equip, COMMAND_ID cmdId, char para[4])
{

		int i;
		if(G_iMaxLedNr == 500)
		{
			MODBUS_RS485_CMD_500 cmd;

			cmd.RS485_Addr = equip;
			cmd.cmd  = cmdId;
			
			for(i = 0; i < sizeof(cmd.para); i++)
			{
				((char*)(cmd.para))[i] = para[i];
			}
			
			CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2,(unsigned char*)cmd.crc);
			
			/*if(cmd.RS485_Addr >= 'A')
			{
				char c = cmd.crc[0];
				cmd.crc[0] = cmd.crc[1];
				cmd.crc[1] = c;
			}*/
			char *p =(char*)&cmd;
			CString str;
			for(i = 0; i < sizeof(cmd); i++)
			{
				CString s;
				s.Format(_T("%04d,"), p[i]);
				str += s;
			}
			*cmdstr = str;
		}
		else
		{

			MODBUS_RS485_CMD_128 cmd;

			cmd.RS485_Addr = equip;
			cmd.cmd  = cmdId;
			cmd.para_len = sizeof(cmd.para); 
			for(i = 0; i < sizeof(cmd.para); i++)
			{
				cmd.para[i] = para[i];
			}
			

			CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2,(unsigned char*)cmd.crc);
			
			/*if(cmd.RS485_Addr >= 'A')
			{
				char c = cmd.crc[0];
				cmd.crc[0] = cmd.crc[1];
				cmd.crc[1] = c;
			}*/
			char *p =(char*)&cmd;
			CString str;
			for(i = 0; i < sizeof(cmd); i++)
			{
				CString s;
				s.Format(_T("%04d,"), p[i]);
				str += s;
			}
			*cmdstr = str;
		}

}

bool CCopyRight::IsCopyRightCheckTime()
{
	CTime cur;
	cur =cur.GetCurrentTime();
	
	if((cur.GetMinute() == 5) && (cur.GetSecond() == 0))  
	{
		return true;
	}
	
	return false;
}

void CCopyRight::ReadAuthor(int BoxNr, CStringArray *SendCmdArray)
{
	/*
	CString cmdStr;
	
	if(G_iMaxLedNr == 500)
	{
		MODBUS_RS485_CMD_500 cmd;
		
	#ifndef _ASCII_
		cmd.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
	#else
		cmd.RS485_Addr = '1' + BoxNr;
	#endif
		cmd.cmd = CMD_READ_AUTHOR;
		cmd.para_len = sizeof(cmd.para);
		for( int i = 0; i < sizeof(cmd.para ); i++)
		{
			cmd.para[i] = 0;  
		}
		cmd.crc[0] = CRC_START_VAL_0;
		cmd.crc[1] = CRC_START_VAL_1;
		CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc,(unsigned char*)cmd.crc); 	

		SetCmdString(&cmdStr, cmd.RS485_Addr,(COMMAND_ID)cmd.cmd, (char*)cmd.para);
		SendCmdArray->Add(cmdStr);
	}
	else
	{
			MODBUS_RS485_CMD_128 cmd;
	
		#ifndef _ASCII_
			cmd.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
		#else
			cmd.RS485_Addr = '1' + BoxNr;
		#endif
			cmd.cmd = CMD_READ_AUTHOR;
			cmd.para_len = sizeof(cmd.para);
			for( int i = 0; i < sizeof(cmd.para ); i++)
			{
				cmd.para[i] = 0;  
			}
			cmd.crc[0] = CRC_START_VAL_0;
			cmd.crc[1] = CRC_START_VAL_1;
			CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc,(unsigned char*)cmd.crc); 	

			SetCmdString(&cmdStr, cmd.RS485_Addr,(COMMAND_ID)cmd.cmd, (char*)cmd.para);
			SendCmdArray->Add(cmdStr);

	}
	*/
}

void CCopyRight::ReadProductDate(int BoxNr, CStringArray *SendCmdArray)
{
	/*
	MODBUS_RS485_CMD_128 cmd_128;
	MODBUS_RS485_CMD_500 cmd_192;

	CString cmdStr;
	if(G_iMaxLedNr == 500)
	{
		#ifndef _ASCII_
		cmd_192.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
	#else
		cmd_192.RS485_Addr = '1' + BoxNr;
	#endif
		cmd_192.cmd = CMD_READ_PRODUCEDATE;
		cmd_192.para_len = sizeof(cmd_192.para);
		for( int i = 0; i < sizeof(cmd_192.para); i++)
		{
			cmd_192.para[i] = 0;  
		}
		cmd_192.crc[0] = CRC_START_VAL_0;
		cmd_192.crc[1] = CRC_START_VAL_1;
		CRC16((unsigned char*)&cmd_192,sizeof(cmd_192) - 2,(unsigned char*)cmd_192.crc,(unsigned char*)cmd_192.crc); 	

		SetCmdString(&cmdStr, cmd_192.RS485_Addr,(COMMAND_ID)cmd_192.cmd, (char*)cmd_192.para);
		SendCmdArray->Add(cmdStr);
	}
	else
	{
		#ifndef _ASCII_
		cmd_128.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
	#else
		cmd_128.RS485_Addr = '1' + BoxNr;
	#endif
		cmd_128.cmd = CMD_READ_PRODUCEDATE;
		cmd_128.para_len = sizeof(cmd_128.para);
		for( int i = 0; i < sizeof(cmd_128.para); i++)
		{
			cmd_128.para[i] = 0;  
		}
		cmd_128.crc[0] = CRC_START_VAL_0;
		cmd_128.crc[1] = CRC_START_VAL_1;
		CRC16((unsigned char*)&cmd_128,sizeof(cmd_128) - 2,(unsigned char*)cmd_128.crc,(unsigned char*)cmd_128.crc); 	

		SetCmdString(&cmdStr, cmd_128.RS485_Addr,(COMMAND_ID)cmd_128.cmd, (char*)cmd_128.para);
		SendCmdArray->Add(cmdStr);

		
	}
*/
}

void CCopyRight::ReadBuildDate(int BoxNr, CStringArray *SendCmdArray)
{
	/*
	MODBUS_RS485_CMD_128 cmd_128;
	MODBUS_RS485_CMD_500 cmd_192;

	CString cmdStr;
	if(G_iMaxLedNr == 500)
	{
	#ifndef _ASCII_
		cmd_192.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
	#else
		cmd_192.RS485_Addr = '1' + BoxNr;
	#endif
		cmd_192.cmd = CMD_READ_BUILDDATE;
		cmd_192.para_len = sizeof(cmd_192.para);
		for( int i = 0; i < sizeof(cmd_192.para ); i++)
		{
			cmd_192.para[i] = 0;  
		}
		cmd_192.crc[0] = CRC_START_VAL_0;
		cmd_192.crc[1] = CRC_START_VAL_1;
		CRC16((unsigned char*)&cmd_192,sizeof(cmd_192) - 2,(unsigned char*)cmd_192.crc,(unsigned char*)cmd_192.crc); 	

		SetCmdString(&cmdStr, cmd_192.RS485_Addr,(COMMAND_ID)cmd_192.cmd, (char*)cmd_192.para);
		SendCmdArray->Add(cmdStr);
	}
	else
	{
	#ifndef _ASCII_
		cmd_128.RS485_Addr = SWITCH_STATUS_ADDR + BoxNr;
	#else
		cmd_128.RS485_Addr = '1' + BoxNr;
	#endif
		cmd_128.cmd = CMD_READ_BUILDDATE;
		cmd_128.para_len = sizeof(cmd_128.para);
		for( int i = 0; i < sizeof(cmd_128.para ); i++)
		{
			cmd_128.para[i] = 0;  
		}
		cmd_128.crc[0] = CRC_START_VAL_0;
		cmd_128.crc[1] = CRC_START_VAL_1;
		CRC16((unsigned char*)&cmd_128,sizeof(cmd_128) - 2,(unsigned char*)cmd_128.crc,(unsigned char*)cmd_128.crc); 	

		SetCmdString(&cmdStr, cmd_128.RS485_Addr,(COMMAND_ID)cmd_128.cmd, (char*)cmd_128.para);
		SendCmdArray->Add(cmdStr);
	}
*/
}

bool CCopyRight::IsCopyRightOK()
{

	

	CTime cur = cur.GetCurrentTime();
	
	   
	if(first_time == 0)
	{
		first_time = AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("FIRST_RUN"),0);
		if(first_time == 0)
		{
			first_time = cur.GetYear() * 10000
						 +cur.GetMonth() * 100
						 +cur.GetDay(); 
			theApp.WriteProfileInt(_T("SETTING"),_T("FIRST_RUN"),first_time);
		}
	}


	if(last_time == 0)
	{
		last_time = AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("LAST_RUN"),0);
		if(last_time == 0)
		{
			last_time = cur.GetYear() * 10000
						 +cur.GetMonth() * 100
						 +cur.GetDay(); 
			AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("LAST_RUN"),last_time);
		}
	}
return true;	
	CTime first_test_time(first_time / 10000,(first_time % 10000) / 100,first_time % 100,0,0,0);
	CTime last_test_time(last_time / 10000,(last_time % 10000) / 100,last_time % 100,0,0,0);
	CTimeSpan span;

	span = cur - last_test_time;

	

	if(span.GetDays() >= 0)
	{
		cur_time = cur.GetYear() * 10000
					 +cur.GetMonth() * 100
					 +cur.GetDay();
		if(cur_time != last_time)
		{
			last_time = cur_time;
			AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("LAST_RUN"),last_time);
		}

		int year,month,day;
		year = first_time / 10000;
		month = (first_time % 10000) / 100;
		day   = first_time % 100;
		span = cur - CTime(year,month,day,0,0,0);
		if(span.GetDays() > COPYWRITE_DAYS)
		{
			WriteLogFile("first_time_span.GetDays() > 800",0,true);
			return false;
		}
	}
	else
	{
		WriteLogFile("last_time_span.GetDays() < 0",0,true);
		return false;
	}
	return true;
}

void CCopyRight::SaveSN(int BoxNr, char *para)
{

}

void CCopyRight::SaveAuthor(int BoxNr, char *para)
{

}

void CCopyRight::SaveProductDate(int BoxNr, char *para)
{

}

void CCopyRight::SaveBuildDate(int BoxNr, char *para)
{

}
