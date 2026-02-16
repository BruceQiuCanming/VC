// wd_stepDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "shlwapi.h"

#pragma comment(lib,"shlwapi.lib")

/////////////////////////////////////////////////////////////////////////////
// CWd_stepDlg dialog

//	ON_MESSAGE(COMM_MSG, OnComm)
#include "comm.h"





#ifdef _ASCII_
#ifdef _DEBUG
	CString strMsgCaption = "热保护器测试系统     ";
#endif

#endif


COMM_THREAD_PARA G_CommPara;

bool IsValidTemp(float temp)
{
	if(abs((int)(temp - (float)INVALID_TEMP)) < 10.0f)
	{
		return false;
	}
	return true;
}

unsigned char CalCheckSum(char *buf,int len)
{
	unsigned char chksum = 0;
	
	for(int i=0; i<len; i++)
			chksum+=buf[i];

	return chksum;

}

unsigned char Buf2Char(unsigned char * buf)
{
	//高位在前，低位在后
	unsigned char ret = 0;
	unsigned char c = buf[0];

	/*if(c >= '0' && c <= '9')
	{
		c = c-'0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		c = c-'a' + 10;
	}
	else if (c >= 'A' && c <= 'F')
	{
		c = c-'A' + 10;
	}*/
	ret = (c - '0') * 16;
	
	c = buf[1];
	/*if(c >= '0' && c <= '9')
	{
		c = c-'0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		c = c-'a' + 10;
	}
	else if (c >= 'A' && c <= 'F')
	{
		c = c-'A' + 10;
	}*/
	c = c - '0';
	ret += c;
	
	return ret;
}


void WriteLogFile(char* msg, int BoxNr, bool NoTimeLimit)
{
	int i = true;

	CFile logFile;
	CTime cur;
	cur = cur.GetCurrentTime();
	CString path = ::theAppDirectory +  _T("log");

	if (!::PathIsDirectory(path))
	{
		::CreateDirectory(path, NULL);
	}
	if(NoTimeLimit || (((cur.GetMinute() % 20) == 10) && (cur.GetSecond() <= 1) ))  
	{
		CString fName;
		fName.Format(path + _T("\\logFile%04d%02d_%d.txt"),cur.GetYear(),cur.GetMonth(),BoxNr);    
		if(logFile.m_hFile == CFile::hFileNull)
		{
			i = 1;
			if(logFile.Open(fName,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite))
			{
//				CFileStatus status;
//				logFile.GetStatus(status);
				logFile.SeekToEnd();
				char s[200];
				
				//status.m_ctime = 12345678;

				//s.Format("%ld %04d-%02d-%0d %02d:%02d:%02d ",status.m_ctime, status.m_ctime.GetYear(),status.m_ctime.GetMonth(),status.m_ctime.GetDay(),status.m_ctime.GetHour(),status.m_ctime.GetMinute(),status.m_ctime.GetSecond()); 

				sprintf_s(s,"%0d %02d%02d%02d ",cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond()); 
				logFile.Write(s,strlen(s)); 
				logFile.Write(msg,strlen(msg)); 
				logFile.Write("\r\n",2); 
				logFile.Flush();
				logFile.Close();
			}
		}
		else
		{
			try
			{
				logFile.SeekToEnd();
				char s[200];
				sprintf_s(s,"%0d %02d%02d%02d ",cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond()); 
				logFile.Write(s,strlen(s)); 
				logFile.Write(msg,strlen(msg)); 
				logFile.Write("\r\n",2); 
				logFile.Flush();
				logFile.Close();
			}
			catch(CFileException e)
			{
				
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////


UINT ReadCommThreadProc(LPVOID pParam)
{
	COMM_THREAD_PARA *para = ((COMM_THREAD_PARA*)pParam);
	CComm *com = (CComm *)para->comm ;
	CByteArray     ReadArray;
	
		int l=0;
		char rec;

		while(com->bReadComm)
		{
			
			l=com->ReadComm(&rec,1);
			if(l < 0)
			{
			}
			else if((l==1))
			{
				
					ReadArray.Add(rec); 
					
					//if(ReadArray.GetSize() >= 7) 
					{
						//int i;
						
						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_128))
						{
							MODBUS_RS485_CMD_128    *modbus_cmd_128 =(MODBUS_RS485_CMD_128*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_128)]);
							unsigned char crc[2];
							CComm::CRC16_AFF2((unsigned char*)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_128) -2, crc);

							if((crc[0] == (unsigned char)modbus_cmd_128->crc[0])
									&& (crc[1] == (unsigned char)modbus_cmd_128->crc[1])
									&& ((modbus_cmd_128->RS485_Addr >= '1' && modbus_cmd_128->RS485_Addr<= '9')
										|| (modbus_cmd_128->RS485_Addr >= 'A' && modbus_cmd_128->RS485_Addr<= 'Z'))
									&& (modbus_cmd_128->para_len = sizeof(modbus_cmd_128->para) ))
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_128));
									}

									ReadArray.RemoveAll();
									
								}
							else
							{
						//		AfxMessageBox(_T("1"));
							}
						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_CMD_PROGRAM))
						{
							MODBUS_RS485_CMD_PROGRAM    *modbus_cmd_128 =(MODBUS_RS485_CMD_PROGRAM*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_CMD_PROGRAM)]);
							unsigned char crc[2];
							CComm::CRC16_AFF2((unsigned char*)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_PROGRAM) -2, crc);

							if((crc[0] ==(unsigned char) modbus_cmd_128->crc[0])
									&& (crc[1] == (unsigned char)modbus_cmd_128->crc[1])
									&& (modbus_cmd_128->RS485_Addr >= '1' && modbus_cmd_128->RS485_Addr<= '9')
									&& (modbus_cmd_128->para_len = sizeof(modbus_cmd_128->para)))
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)modbus_cmd_128,sizeof(MODBUS_RS485_CMD_PROGRAM));
									}

									ReadArray.RemoveAll();
									
								}
						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_ANSWER_OMRON_READ))
						{
							MODBUS_RS485_ANSWER_OMRON_READ    *answer_omron =(MODBUS_RS485_ANSWER_OMRON_READ*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_ANSWER_OMRON_READ)]);
							unsigned char crc[2];
							CComm::CRC16((unsigned char*)answer_omron,sizeof(MODBUS_RS485_ANSWER_OMRON_READ) - 2, crc);

							if((crc[0] == (unsigned char)answer_omron->crc[0])
									&& (crc[1] == (unsigned char)answer_omron->crc[1])
									&& (answer_omron->para_len == sizeof(answer_omron->para)) )
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_omron,sizeof(MODBUS_RS485_ANSWER_OMRON_READ));
									}

									ReadArray.RemoveAll();
									
								}
						}

						

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE))
						{
							MODBUS_RS485_ANSWER_OMRON_WRITE    *answer_500 =(MODBUS_RS485_ANSWER_OMRON_WRITE*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE)]);
							unsigned char crc[2];
							CComm::CRC16((unsigned char*)answer_500,sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE) - 2, crc);

							if((crc[0] == (unsigned char)answer_500->crc[0])
									&& (crc[1] == (unsigned char)answer_500->crc[1])
									&& (answer_500->RS485_Addr == 1)
									&& (answer_500->cmd == CMD_OMRON_WRITE))
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_500,sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE));
									}

									ReadArray.RemoveAll();
									
								}
						}

						if(ReadArray.GetSize() >= sizeof(MODBUS_RS485_ANSWER_500))
						{
							MODBUS_RS485_ANSWER_500    *answer_500 =(MODBUS_RS485_ANSWER_500*) &(ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_ANSWER_500)]);
							unsigned char crc[2];
							CComm::CRC16_AFF2((unsigned char*)answer_500,sizeof(MODBUS_RS485_ANSWER_500) - 2, crc);

							if((crc[0] == (unsigned char)answer_500->crc[0])
									&& (crc[1] == (unsigned char)answer_500->crc[1])
									&& (answer_500->RS485_Addr >= 'A' && answer_500->RS485_Addr<= 'E')
									&& (answer_500->para_len = sizeof(answer_500->para)))
								{
										
									if(para->m_hWnd != NULL)
									{
										
										::SendMessage(para->m_hWnd,para->MessageID,(WPARAM)answer_500,sizeof(MODBUS_RS485_ANSWER_500));
									}

									ReadArray.RemoveAll();
									
								}
						}


					}
			}
			
			else
			{
			}
		}
	WriteLogFile("EXIT READ THREAD",0,true);
	return 0;
}

CComm::CComm(void)
{
	hComm	= INVALID_HANDLE_VALUE;
	bXModem	=	false;
}
CComm::~CComm(void)
{

}
int CComm::OpenComm(char comm,COMM_THREAD_PARA *para)
{
	TCHAR sPort[20];
	
	_stprintf_s(sPort,_T("\\\\.\\COM%d"),comm);

	hComm=CreateFile(sPort, 
			GENERIC_READ | GENERIC_WRITE, 
			0,//FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,                    // exclusive access
			NULL,                 // no security attrs
			OPEN_EXISTING,            
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O   
			NULL );

	if(hComm == INVALID_HANDLE_VALUE)
	{
		CString s;
		s.Format(_T("串口 %d 异常！"),comm);
		AfxMessageBox(s,NULL,MB_ICONERROR);
		return -1;
	}
	::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(hComm,&timeout);

	DCB m_dcb;
	
	if(!GetCommState(hComm,&m_dcb))
			{
				AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -3;
			}
			else
			{
				//memset(&m_dcb,0,sizeof(m_dcb));
				//m_dcb.DCBlength =sizeof(m_dcb); 
				m_dcb.BaudRate	=	9600; 
				m_dcb.ByteSize	=	8;
				m_dcb.StopBits	=	0; 
				m_dcb.fBinary	=	true ; 
				m_dcb.fParity	=	false;
				m_dcb.Parity    =	NOPARITY;
			}

    if(!SetCommState(hComm,&m_dcb))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
			return -4;
		}
	if(!SetupComm(hComm,1024*30,1024))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -2;
		}
	
	//::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

	ov.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	bReadComm=true;
	
	if(this->bXModem == false)
	{
		para->ReadThread = AfxBeginThread(ReadCommThreadProc,(LPVOID)(para),THREAD_PRIORITY_NORMAL);
	}
	
	return 0;
}
bool CComm::IsOpened(void)
{
	if(hComm == INVALID_HANDLE_VALUE)
		return false;
	else
		return true;
}
int CComm::CloseComm(void)
{
	bReadComm=0;
	
	Sleep(1000);
	::CloseHandle(hComm); 
	hComm = INVALID_HANDLE_VALUE;
	

	return 0;
}

CString WriteCommMsg;
int CComm::WriteComm(char *buf,int len)
{
	ASSERT(buf);
	ASSERT(len);
	ASSERT(hComm != INVALID_HANDLE_VALUE);
    /*static*/ OVERLAPPED ol;	
	DWORD wr_len = 0;
	DWORD sum_num = 0;
	int   writeTimer = 0;
	DWORD err;

	/*if(!IsOpened())
	{
		return -999;
	}*/

	memset(&ol,0,sizeof(ol));
	ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	

	//DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	BOOL b = WriteFile(hComm,buf,len,(LPDWORD)&wr_len,&ol);
	int ret=0;
	if(b)
	{
			sum_num=wr_len;
			while((int)sum_num<len)
			{
				writeTimer ++;
				BOOL	bResult = GetOverlappedResult(hComm, &ol, 
					&wr_len, true); 
				sum_num+=wr_len;
				if(writeTimer > 1000)
				{
					WriteLogFile("WriteComm writeTimer 1 break",0,false);				
					break;
				}
			}
			if(writeTimer > 1000)
			{
				ret = -8;
			}
			else
			{
				ret = -1;
			}
	}
	else
	{
		err=GetLastError();
		if(err == ERROR_IO_PENDING)
		{
			sum_num=wr_len;
			while((int)sum_num<len)
			{
				writeTimer ++;
				Sleep(1);
				BOOL	bResult = GetOverlappedResult(hComm, &ol, 
					&wr_len, true); 
				sum_num+=wr_len;
				if(writeTimer > 1000)
				{
					WriteLogFile("WriteComm writeTimer 2 break",0,false);				
					break;
				}
			}
			if(writeTimer > 1000)
			{
				ret = -9;
			}
			else
			{
				ret =-2;
			}
		}
		else
		{
//			CloseHandle(   ol.hEvent   ); 
			ret = -3;
		}
//		CloseHandle(   ol.hEvent   ); 
//		ret = -4;
	}
	CloseHandle(   ol.hEvent   ); 
	if( ret != -2)
	{
		CTime cur;
		cur = cur.GetCurrentTime();
		char buf[100];
		sprintf_s(buf,"WriteComm %d %d %d err=%d %d:%d", hComm, ret, sum_num, err, cur.GetMinute(),cur.GetSecond());
		WriteLogFile(buf,0,true);
	}
	return ret;
}
int CComm::ReadComm(char *buf,int len)
{
	ASSERT(hComm != INVALID_HANDLE_VALUE);
	if(hComm == INVALID_HANDLE_VALUE)
		return -9;

	OVERLAPPED ol;
	static char rec[10];
	memset(&ol,0,sizeof(ol));
	ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	BOOL b=ReadFile(hComm,rec,len,&num,&ol);
	if(b)
	{
		if(num<=0)
			return -1;
		else
		{
			memcpy(buf,rec,num);
			CloseHandle(   ol.hEvent   ); 
			return num;
		}
	}
	else
	{
		DWORD err=GetLastError();
		if(err==ERROR_IO_PENDING)
		{
			BOOL	bResult = GetOverlappedResult(hComm, &ol, 
					&num, true);
			if (bResult && num>=1)  
			{
				memcpy(buf,rec,num);
				CloseHandle(   ol.hEvent   ); 
				return num;
			}
		/*	else
			{
				memcpy(buf,rec,num);
				//::CloseHandle(   ol.hEvent   ); 
				return num;
				//::CloseHandle(   ol.hEvent   ); 
				//return -2;
			}*/
		}
		else
		{
			CloseHandle(   ol.hEvent   ); 
			return -3;
		}
		
		CloseHandle(   ol.hEvent   ); 
		return -4;
	}
}

//CRC低位字节值表
unsigned char crc16_lo[]=
 {          0x0, 0xC0, 0xC1, 0x1, 0xC3, 0x3, 0x2, 0xC2, 0xC6, 0x6, 
            0x7, 0xC7, 0x5, 0xC5, 0xC4, 0x4, 0xCC, 0xC, 0xD, 0xCD, 
            0xF, 0xCF, 0xCE, 0xE, 0xA, 0xCA, 0xCB, 0xB, 0xC9, 0x9, 
            0x8, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
            0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
            0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
            0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
            0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
            0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
            0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
            0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
            0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
            0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
            0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
            0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
            0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
            0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
            0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
            0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
            0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
            0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
            0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
            0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
            0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
            0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
            0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

//'CRC高位字节值表
unsigned char crc16_hi[]=
{
			0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0,
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 
            0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40,
            0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40,
            0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 
            0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40,
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0,  
			0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 
            0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0,  
			0x80, 0x41, 0x0, 0xC1, 0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 
            0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 0x80, 0x41, 0x0, 0xC1, 
            0x81, 0x40, 0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41,
            0x0, 0xC1, 0x81, 0x40, 0x1, 0xC0, 0x80, 0x41, 0x1, 0xC0, 
            0x80, 0x41, 0x0, 0xC1, 0x81, 0x40
};
bool CComm::CheckCRC16(unsigned char *source, int  n, unsigned char source_crc[2])
{
	unsigned char crc[2];
	CRC16(source,n,crc);
	if((crc[0] == source_crc[0])
		&&(crc[1] == source_crc[1]))
	{
		return true;
	}
	return false;
}

unsigned int CRC16_Ex ( unsigned char *arr_buff, unsigned char len)
{

 unsigned short crc=0xFFFF;
 unsigned char i, j;
 for ( j=0; j < len; j++)
 {

   crc=crc ^*arr_buff++;
   for ( i=0; i<8; i++)
  {

       if( ( crc&0x0001) >0)

       {

           crc=crc>>1;

           crc=crc^ 0xa001;

        }
      else

          crc=crc>>1;
   }
 }
return ( crc);

}
void CComm::CRC16_AFF2(unsigned char *source, int  n, unsigned char ReturnData[2])
{
	 unsigned char CRC16Hi = 0xAF;
    unsigned char CRC16Lo  = 0xF2;
    int i;
    int iIndex;

    for( i = 0; i < n; i++)
	{
        iIndex = CRC16Lo ^ source[i];
        CRC16Lo = CRC16Hi ^ crc16_hi[iIndex]    ;//         '低位处理
        CRC16Hi = crc16_lo[iIndex];				//                         '高位处理
    }

	ReturnData[1] = CRC16Hi;//                                'CRC高位
    ReturnData[0] = CRC16Lo;//                                'CRC低位
}
void CComm::CRC16(unsigned char *source, int  n, unsigned char ReturnData[2])
{
  
#ifndef _OIL_    
	//MODBUS 开始是0xFFFF
    unsigned char CRC16Hi = CRC_START_VAL_1;
    unsigned char CRC16Lo = CRC_START_VAL_0;
    int i;
    int iIndex;

    for( i = 0; i < n; i++)
	{
        iIndex = CRC16Lo ^ source[i];
        CRC16Lo = CRC16Hi ^ crc16_hi[iIndex]    ;//         '低位处理
        CRC16Hi = crc16_lo[iIndex];				//                         '高位处理
    }

	ReturnData[1] = CRC16Hi;//                                'CRC高位
    ReturnData[0] = CRC16Lo;//                                'CRC低位

	//unsigned short ret =CRC16_Ex(source,n);
#else
    int i;
    short iSum = 0;

    for( i = 0; i < n; i++)
	{
        iSum += source[i];
    }

	ReturnData[1] = ((iSum & 0xFF00) >> 8);
    ReturnData[0] = ((iSum & 0x00FF) );
#endif

}


unsigned short CComm::XMODEM_CRC(char *ptr, int count)
{
    unsigned short crc = 0;
    char i;

    while (--count >= 0)
    {
        crc = crc ^ (unsigned short) (*ptr++ << 8);
        i = 8;
        do
        {
            if (crc & 0x8000)
            {
                crc = crc << 1 ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
        while(--i);
    }
    return (crc);
}