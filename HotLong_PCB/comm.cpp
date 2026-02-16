// wd_stepDlg.cpp : implementation file
//

#include "stdafx.h"

#include "comm.h"

#include "HotLong_PCBDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CWd_stepDlg dialog

//	ON_MESSAGE(COMM_MSG, OnComm)

DELTA_PLC_SET_CMD	G_Delta_PLC_SetCmd;
DELTA_PLC_READ_CMD	G_Delta_PLC_ReadCmd;


extern CString theAppDirectory;

CWnd * G_pAboutDlg = NULL;

int		G_iLangID = 0;

bool G_bCMD_192_READ_STATUS = false;

#ifdef _ASCII_
#ifdef _DEBUG
	CString strMsgCaption = "热保护器测试系统     ";
#endif

#endif




unsigned char Hex2Char(unsigned char Hex)
{
	unsigned char data = 0xFF;
	if(Hex >= '0' && Hex <= '9')
	{
		data = (Hex - '0');
	}
	else if(Hex >= 'A' && Hex <= 'F')
	{
		data = (Hex - 'A' + 10 );
	}
	else if(Hex >= 'a' && Hex <= 'f')
	{
		data = (Hex - 'a' + 10 );
	}
	return data;
}

BOOL FreeMyData(LPCTSTR ResName, LPCTSTR ResType,  int Type,unsigned char *HexBuf,int *HexBufLen) 
{ 
	BOOL bRet = TRUE; 
	HINSTANCE hInst = AfxGetInstanceHandle();
	//先判断我们指定的资源是否存在 
	HRSRC hResInfo = FindResource(hInst,ResName,ResType);
	if(hResInfo == NULL) 
		return FALSE;
	HANDLE hFile = NULL; 
	DWORD dwWritten = 0;
	//开始调入指定的资源到内存 
	HGLOBAL hResData = LoadResource(hInst,hResInfo); 
	LPVOID pvResData = LockResource(hResData); 
	DWORD dwResSize = SizeofResource(hInst,hResInfo);

	*HexBufLen = dwResSize;
	memcpy(HexBuf,pvResData,dwResSize);
	
	//再创建目标文件并保存数据至文件 
	//if(Type == 1)//系统加隐藏属性 如果指定类型为1,那么加上系统和隐藏属性,防止普通用户对文件进行破坏 
	//{ 
	//	hFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
	//	NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM, NULL); 
	//	WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL); 
	//} 
	//else 
	//{ 
	//	hFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
	//	NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL); 
	//	if(WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL)	== TRUE) 
	//	{
	//	//	AfxMessageBox(FileName);
	//	}
	//}

	//if(hFile == INVALID_HANDLE_VALUE)//创建文件失败 
	//{ 
	//	FreeResource(hResData); 
	//	return FALSE; 
	//}
	////释放有关内存资源 
	//CloseHandle(hFile); 

	FreeResource(hResData);
	return TRUE; 
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

	//if(c >= '0' && c <= '9')
	//{
	//	c = c-'0';
	//}
	//else if (c >= 'a' && c <= 'f')
	//{
	//	c = c-'a' + 10;
	//}
	//else if (c >= 'A' && c <= 'F')
	//{
	//	c = c-'A' + 10;
	//}
	ret = (c - '0') * 16;
	
	c = buf[1];
	//if(c >= '0' && c <= '9')
	//{
	//	c = c-'0';
	//}
	//else if (c >= 'a' && c <= 'f')
	//{
	//	c = c-'a' + 10;
	//}
	//else if (c >= 'A' && c <= 'F')
	//{
	//	c = c-'A' + 10;
	//}
	c = c - '0';
	ret += c;
	
	return ret;
}


void WriteLogFile(char* msg, int BoxNr, bool NoTimeLimit)
{

}
/////////////////////////////////////////////////////////////////////////////

bool    G_IsJustCmdAscii;


UINT ReadCommThreadProc_Delta_PLC(LPVOID pParam)
{
	COMM_THREAD_PARA *para = ((COMM_THREAD_PARA*)pParam);
	CComm *com = (CComm *)para->comm ;
	CByteArray     ReadArray;
	
	ReadArray.RemoveAll();

	static DWORD	tickcount = 0;
	static bool flashed = true;

		int l=0;
		char rec;

		while(com->bReadComm)
		{
			
			if(G_pAboutDlg != 0)
			{
				if(G_pAboutDlg->GetSafeHwnd() != NULL && G_pAboutDlg->IsWindowVisible())
				{
					if((GetTickCount() - tickcount > 20 ) && ( flashed == false))
					{
						flashed = true;
						TCHAR buf[1024];
						wsprintf(buf,_T("%s"),_T("\r\n"));
						::SendMessage(G_pAboutDlg->m_hWnd,COMM_MSG,(WPARAM)buf,wcslen(buf)*sizeof(TCHAR));
					}
				}
			}
			
			l=com->ReadComm(&rec,1);
			
			if(l <= 0)
			{

			}
			else if(l==1)
			{
			
				ReadArray.Add(rec); 
		
#ifdef WINCE
				if(::G_NormalConfigPara[0].BoxType == BOX_DELTA_PLC_8PT100_8SSR
					|| G_NormalConfigPara[0].BoxType == BOX_8PT100_8SSR)
				{
					ReadDeltaPLC(para,&ReadArray);
				}

#endif
					
			}
		}

		return 0;
}



UINT ReadCommThreadProc_PCB(LPVOID pParam)
{
	COMM_THREAD_PARA* comPara = ((COMM_THREAD_PARA*)pParam);
	CComm* com = (CComm*)comPara->comm;
	CByteArray     ReadArray;

	ReadArray.RemoveAll();
	static DWORD	tickcount = 0;
	static bool flashed = true;

	int l = 0;
	char rec;

	while (com->bReadComm)
	{

		l = com->ReadComm(&rec, 1);

		if (l <= 0)
		{

		}
		else if (l == 1)
		{

			ReadArray.Add(rec);

			if (ReadArray.GetSize() >= sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1))
			{
				MODBUS_RS485_WRITE_ANSWER_WORD_1* answer = (MODBUS_RS485_WRITE_ANSWER_WORD_1*)&ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1)];
				unsigned short crc = CComm::CRC16_MODBUS((unsigned char*)answer, sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1) - 2);
				if (answer->CRC == crc)
				{
					::SendMessage(comPara->m_hWnd, comPara->MessageID, (WPARAM)answer, sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1));
					ReadArray.RemoveAll();
				}
			}

			

				if (ReadArray.GetSize() >= sizeof(MODBUS_RS485_READ_ANSWER_WORD_16))
				{
					MODBUS_RS485_READ_ANSWER_WORD_16* answer = (MODBUS_RS485_READ_ANSWER_WORD_16*)&ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_READ_ANSWER_WORD_16)];
					unsigned short crc = CComm::CRC16_MODBUS((unsigned char*)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_16) - 2);
					if (answer->CRC == crc)
					{
						::SendMessage(comPara->m_hWnd, comPara->MessageID, (WPARAM)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_16));
						ReadArray.RemoveAll();
					}
				}
				if (ReadArray.GetSize() >= sizeof(MODBUS_RS485_READ_ANSWER_WORD_3))
				{
					MODBUS_RS485_READ_ANSWER_WORD_3* answer = (MODBUS_RS485_READ_ANSWER_WORD_3*)&ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_READ_ANSWER_WORD_3)];
					unsigned short crc = CComm::CRC16_MODBUS((unsigned char*)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_3) - 2);
					if (answer->CRC == crc)
					{
						::SendMessage(comPara->m_hWnd, comPara->MessageID, (WPARAM)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_3));
						ReadArray.RemoveAll();
					}
				}

		}
	}

	return 0;
}

UINT ReadCommThreadProc_PLC(LPVOID pParam)
{
	COMM_THREAD_PARA* comPara = ((COMM_THREAD_PARA*)pParam);
	CComm* com = (CComm*)comPara->comm;
	static CByteArray     ReadArray;

	ReadArray.RemoveAll();

	static DWORD	tickcount = 0;
	static bool flashed = true;

	int l = 0;
	char rec;

	while (com->bReadComm)
	{

		if(com->m_hComm == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		l = com->ReadComm(&rec, 1);

		if (l <= 0)
		{

		}
		else if (l == 1)
		{

	
			    ReadArray.Add(rec);

				if (ReadArray.GetSize() >= sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1))
				{
					MODBUS_RS485_WRITE_ANSWER_WORD_1* answer = (MODBUS_RS485_WRITE_ANSWER_WORD_1*)&ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1)];
					unsigned short crc = CComm::CRC16_MODBUS((unsigned char*)answer, sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1) - 2);
					if (answer->CRC == crc)
					{
						::SendMessage(comPara->m_hWnd, comPara->MessageID, (WPARAM)answer, sizeof(MODBUS_RS485_WRITE_ANSWER_WORD_1));
						ReadArray.RemoveAll();
					}
				}

			

				if (ReadArray.GetSize() >= sizeof(MODBUS_RS485_READ_ANSWER_WORD_16))
				{
					MODBUS_RS485_READ_ANSWER_WORD_16* answer = (MODBUS_RS485_READ_ANSWER_WORD_16*)&ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_READ_ANSWER_WORD_16)];
					unsigned short crc = CComm::CRC16_MODBUS((unsigned char*)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_16) - 2);
					
					if (answer->head.cmd == 0x03
						&& answer->head.DataLen == sizeof(answer->data)
						&& answer->CRC == crc)
					{
						::SendMessage(comPara->m_hWnd, comPara->MessageID, (WPARAM)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_16));
						ReadArray.RemoveAll();
					}
				}
				
				if (ReadArray.GetSize() >= sizeof(MODBUS_RS485_READ_ANSWER_WORD_3))
				{
					MODBUS_RS485_READ_ANSWER_WORD_3* answer = (MODBUS_RS485_READ_ANSWER_WORD_3*)&ReadArray.GetData()[ReadArray.GetSize() - sizeof(MODBUS_RS485_READ_ANSWER_WORD_3)];
					unsigned short crc = CComm::CRC16_MODBUS((unsigned char*)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_3) - 2);
					if (answer->CRC == crc)
					{
						::SendMessage(comPara->m_hWnd, comPara->MessageID, (WPARAM)answer, sizeof(MODBUS_RS485_READ_ANSWER_WORD_3));
						ReadArray.RemoveAll();
					}
				}
				
			


		}
	}

	return 0;

}

CComm::CComm(void)
{
	m_hComm	= INVALID_HANDLE_VALUE;
	m_bXModem	=	false;
#ifdef _HeatBox
	for(int i = 0; i < ::G_CurMaxHeatBox; i++)
	{
		m_BeepStartTime[i] = CTime::GetCurrentTime();
	}
#endif

	m_LastRecTime = CTime::GetCurrentTime();
	m_OpenCount = 0;
}
CComm::~CComm(void)
{

}

int CComm::SetBaudrate(HANDLE hComm,DWORD BaudRate,BYTE ByteSize,BYTE StopBits ,BYTE Parity)
{
	DWORD err;
	

	if(hComm == INVALID_HANDLE_VALUE)
	{
		
		return -1;
	}

	if(::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR) == false)
	{

		err = GetLastError();
		if(err == ERROR_ACCESS_DENIED)
		{
			this->CloseComm(); 
			return -1;
		}
	}


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(hComm,&timeout);

	DCB m_dcb;
	
	if(!GetCommState(hComm,&m_dcb))
	{
		return -3;
	}
	else
	{
		//memset(&m_dcb,0,sizeof(m_dcb));
		//m_dcb.DCBlength =sizeof(m_dcb); 
		m_dcb.BaudRate	=	BaudRate; 
		m_dcb.ByteSize	=	ByteSize;
		m_dcb.StopBits	=	StopBits;   //BYTE StopBits;        // 0,1,2 = 1, 1.5, 2            
		m_dcb.fBinary	=	true ; 
		if(Parity == 0)
		{
			m_dcb.fParity	=	false;
		}
		else
		{
			m_dcb.fParity	=	true;
		}
		m_dcb.Parity    =	Parity;
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
	
	
	return 0;
}



int CComm::OpenComm(CString commName,COMM_THREAD_PARA *para)
{
	TCHAR sPort[20];
	DWORD err;
	//wsprintf(sPort,_T("\\\\.\\COM%d"),comm);


	wsprintf(sPort,_T("\\\\.\\%s"),commName);



	
	m_hComm = CreateFile(sPort, 
			GENERIC_READ | GENERIC_WRITE, 
			0,//FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,                    // exclusive access
			NULL,                 // no security attrs
			OPEN_EXISTING,            
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O   
			NULL );

	err = GetLastError();

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		if(m_OpenCount == 0)
		{
			CString s;
			if(G_iLangID == 0)
			{
				s.Format(_T("串口名 %s 异常！"),commName);
			}
			else
			{
				s.Format(_T("COMM Port %s is used or not available!"),commName);
			}

			

			AfxMessageBox(s,NULL,MB_ICONERROR);
		}
		m_OpenCount ++;
		return -1;
	}
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(m_hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(m_hComm,&timeout);

	DCB m_dcb;
	
	if(!GetCommState(m_hComm,&m_dcb))
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
				m_dcb.fParity	=	true;//false;
				m_dcb.Parity    =	NOPARITY;
			}

    if(!SetCommState(m_hComm,&m_dcb))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
			return -4;
		}
	if(!SetupComm(m_hComm,1024*30,1024))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -2;
		}
	
	//::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

	ov.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	bReadComm=true;
	
	
	
	return 0;
}
int CComm::OpenComm(CString commName,COMM_THREAD_PARA *para,DCB dcb)
{
	TCHAR sPort[20];
	DWORD err;
	//wsprintf(sPort,_T("\\\\.\\COM%d"),comm);

#ifdef WINCE
	wsprintf(sPort,_T("%s"),commName);
#else
	wsprintf(sPort,_T("\\\\.\\%s"),commName);
#endif


	
	m_hComm = CreateFile(sPort, 
			GENERIC_READ | GENERIC_WRITE, 
			0,//FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,                    // exclusive access
			NULL,                 // no security attrs
			OPEN_EXISTING,            
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O   
			NULL );

	err = GetLastError();

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		if(m_OpenCount == 0)
		{
			CString s;
			if(G_iLangID == 0)
			{
				s.Format(_T("串口 %s 异常！"),commName);
			}
			else
			{
				s.Format(_T("COMM Port %s is used or not available!"),commName);
			}

			

			AfxMessageBox(s,NULL,MB_ICONERROR);
		}
		m_OpenCount ++;
		return -1;
	}
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);


	COMMTIMEOUTS timeout;
	::GetCommTimeouts(m_hComm,&timeout);
	
	timeout.ReadIntervalTimeout	= 10; // 读间隔超时。 接收时，两字符间最大的时延。
	timeout.ReadTotalTimeoutMultiplier = 10; // 读时间系数。 读取每字节的超时。
	timeout.ReadTotalTimeoutConstant = 10000; // 读时间常量。 读串口数据的固定超时。
    

	::SetCommTimeouts(m_hComm,&timeout);

	DCB m_dcb;
	
		if(!GetCommState(m_hComm,&m_dcb))
			{
				AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -3;
			}
			else
			{
				//memset(&m_dcb,0,sizeof(m_dcb));
				//m_dcb.DCBlength =sizeof(m_dcb); 
				
				m_dcb.BaudRate	=	dcb.BaudRate;	//9600
				m_dcb.ByteSize	=	dcb.ByteSize;	//8
				m_dcb.StopBits	=	dcb.StopBits;	//0; 
				m_dcb.fBinary	=	dcb.fBinary;	//true ; 
				m_dcb.fParity	=	dcb.fParity;	//false;
				m_dcb.Parity    =	dcb.Parity;		//NOPARITY;
			}

		if(!SetCommState(m_hComm,&m_dcb))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
			return -4;
		}
		if(!SetupComm(m_hComm,1024*30,1024))
		{
			AfxMessageBox(_T("串口异常！"),NULL,MB_ICONERROR);
				return -2;
		}
	
	//::PurgeComm(hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

	ov.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	bReadComm=true;
	
	
	
	return 0;
}

bool CComm::IsOpened(void)
{
	if(m_hComm == INVALID_HANDLE_VALUE)
		return false;
	else
		return true;
}
int CComm::CloseComm(void)
{
	bReadComm = 0;
	
	Sleep(1000);
	::CloseHandle(m_hComm); 
	m_hComm = INVALID_HANDLE_VALUE;
	

	return 0;
}

CString WriteCommMsg;
#ifdef WINCE
int CComm::WriteComm(void *buf,int len)
{
	ASSERT(buf);
	ASSERT(len);
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
    DWORD wr_len = 0;
	
	if(!IsOpened())
	{
		return -999;
	}

	//DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	BOOL b = WriteFile(m_hComm,buf,len,(LPDWORD)&wr_len,NULL);
	return wr_len;
}
#else
int CComm::WriteComm(void *buf,int len)
{
	
	
    OVERLAPPED ol;	
	DWORD wr_len = 0;
	DWORD sum_num = 0;
	int   writeTimer = 0;
	DWORD err;

	if(!IsOpened())
	{
		return -999;
	}

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	//return 0;

	ASSERT(buf);
	ASSERT(len);

	memset(&ol,0,sizeof(ol));
	ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	

	//DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	::PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
	BOOL b = WriteFile(m_hComm,buf,len,(LPDWORD)&wr_len,&ol);
	int ret	= 0;
	if(b)
	{
			sum_num=wr_len;
			while((int)sum_num<len)
			{
				writeTimer ++;
				BOOL	bResult = GetOverlappedResult(m_hComm, &ol, 
					&wr_len, true); 
				sum_num+=wr_len;
				if(writeTimer > 10000)
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
				BOOL	bResult = GetOverlappedResult(m_hComm, &ol, 
					&wr_len, true); 
				sum_num+=wr_len;
				if(writeTimer > 10000)
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
			if(err == ERROR_ACCESS_DENIED)
			{
				CloseComm();
			}
			
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
		sprintf_s(buf,"WriteComm %d %d %d err=%d %d:%d", m_hComm, ret, sum_num, err, cur.GetMinute(),cur.GetSecond());
		WriteLogFile(buf,0,true);
	}
	return ret;
}
#endif


int CComm::ReadComm(char *buf,int len)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	if(m_hComm == INVALID_HANDLE_VALUE)
		return -9;

	OVERLAPPED ol;
	static char rec[10];
	memset(&ol,0,sizeof(ol));
	ol.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);	
	DWORD num;
	//BOOL b=ReadFile(hComm,buf,1,NULL,&ov);
	BOOL b=ReadFile(m_hComm,rec,len,&num,&ol);
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
			BOOL	bResult = GetOverlappedResult(m_hComm, &ol, 
					&num, true);
			if (bResult && num>=1)  
			{
				memcpy(buf,rec,num);
				CloseHandle(   ol.hEvent   ); 
				return num;
			}
		//	else
		//	{
		//		memcpy(buf,rec,num);
		//		//::CloseHandle(   ol.hEvent   ); 
		//		return num;
		//		//::CloseHandle(   ol.hEvent   ); 
		//		//return -2;
		//	}
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


bool CComm::CheckCRC16(unsigned char *source, int  n, unsigned short source_crc)
{
	unsigned short crc;
	crc = CRC16(source,n);//,crc);
	if(crc == source_crc)
	//	&&(crc[1] == source_crc[1]))
	{
		return true;
	}
	return false;
}

unsigned int CRC16_Ex ( unsigned char *arr_buff, unsigned int len)
{

 unsigned short crc=0xFFFF;
 unsigned int i, j;
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

unsigned short CComm::CRC16_MODBUS(unsigned char *source, int  n ) //, unsigned char ReturnData[2])
{
    unsigned char CRC16Hi = 0xFF;
    unsigned char CRC16Lo = 0xFF;
    int i;
    int iIndex;

    for( i = 0; i < n; i++)
	{
        iIndex = CRC16Lo ^ source[i];
        CRC16Lo = CRC16Hi ^ crc16_hi[iIndex]    ;//         '低位处理
        CRC16Hi = crc16_lo[iIndex];				//                         '高位处理
    }

	//ReturnData[1] = CRC16Hi;//                                'CRC高位
    //ReturnData[0] = CRC16Lo;//      
	
	unsigned short crc = (CRC16Hi << 8) + CRC16Lo;

	return crc;
}

unsigned short CComm::CRC16(unsigned char *source, int  n) //, unsigned char ReturnData[2])
{
#ifdef _HeatBox
	if(::G_iMaxLedNr[0] == 300)
	{
		return CRC16_MODBUS(source, n);
	}
#endif

#ifndef _OIL_    
	//MODBUS 开始是0xFFFF
    unsigned char CRC16Hi = 0xFA;//CRC_START_VAL_1;
    unsigned char CRC16Lo = 0xAF;//CRC_START_VAL_0;
    int i;
    int iIndex;

    for( i = 0; i < n; i++)
	{
        iIndex = CRC16Lo ^ source[i];
        CRC16Lo = CRC16Hi ^ crc16_hi[iIndex]    ;//         '低位处理
        CRC16Hi = crc16_lo[iIndex];				//                         '高位处理
    }

	//ReturnData[1] = CRC16Hi;//                                'CRC高位
    //ReturnData[0] = CRC16Lo;//                                'CRC低位
	unsigned short crc = (CRC16Hi << 8) + CRC16Lo;
	return crc;
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


























char bin[1024*1024];





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
// ===========================
//HEX文件以行为单位。每行以字符 ‘:’ (0x3A)开头，以回车--换行符0x0D, 0x0A为结束。
//每行开始和结束之间的所有内容，都是以字符形式表现的。
//8 位数据：数据如果是 0xFE ，那么转换到HEX格式的行里面就是0x3F 0x4E。
//16位数据：如16位地址，先显示高位，后显示底位。
//例如 0xABCD，转换成HEX格式文件后变成 0x3A 0x3B 0x3C 0x3D，显示出来以后就是ABCD。
//数据部分---内容每2个字符看做一个HEX数据。
//
//:020000040800F2    ----->第一行数据： 0x02 0x00 0x00  0x04  0x08  0x00  0xF2
//第一个 0x02 为数据长度。
//紧跟着后面的0x00 0x00 为地址。
//再后面的0x04为数据类型，类型共分一下几类：
//'00' Data Record                    '00'数据记录
//'01' End of File Record           '01'文件记录结束
//'02' Extended Segment Address Record     '02'扩展段地址记录
//'03' Start Segment Address Record        '03'开始段地址记录
//'04' Extended Linear Address Record      '04'扩展线性地址记录
//'05' Start Linear Address Record          '05'启动线性地址记录
//
//然后，接着0x04后面的两个 0x00 0x00就是数据。最后一个0xF2是校验码。
//HEX文件的每一行都是这样的格式：
//            <0x3a>
//            [数据长度1Byte]
//            [数据地址2Byte]
//            [数据类型1Byte]
//            [数据nByte]
//            [校验1Byte]
//            <0x0d>
//            <0x0a>
//校验值：每一行的最后一个值为此行数据的校验和
//
//开头 长度  地址  数据类型   数据   校验值
// :      02   0000    04       0800     F2
//
// 校验和算法：
//    校验和的算法为：计算从0x3A 以后（不包括0x3A）的所有各字节的和模256的余。
// 即各字节二进制算术和，不计超过256的溢出值，然后用0x100减去这个算数累加和，
// 得出得值就是此行得校验和。
//举例：---十六进制运算：02+00+00+04+08+00=0E
//               校验值：0x100 - 0x0E =0xF2
//
//10+00+00+00+30+0B+00+20+CD+01+00+08+E1+2D+00+08+E1+2D+00+08=36D
//        36D--->十进制877--->%256=109==0x6D 
//        校验值：0x100-0x6D=0X93
//
// //stm32f4为32位数据，以0x00 0x00每16位数据作为一帧数据发送       校验位    回车   换行
//:     10  0000    00    300B 0020 CD01 0008 E12D 0008 E12D 0008   93      <0X0D>  <0X0A>
//
//:10000000300B0020CD010008E12D0008E12D000893
//安装上面的数据行格式分析如下:
//            <0x3a>
//            [数据长度1Byte]  10
//            [数据地址2Byte]  00 00
//            [数据类型1Byte]  00
//            [数据nByte]      30 0B 00 20 CD 01 00 08 E1 2D 00 08 E1 2D 00 08
//            [校验1Byte]       C0
//            <0x0d>
//            <0x0a>
//
//每行中的数据并不是一定有的，第二个直接的数据长度为0，那么这行就没有数据。
//由于每行标识数据地址的只有2Byte，所以最大只能到64K，为了可以保存更大数据地址数据，就有了Extended LinearAddress Record。
//如果这行的数据类型是0x04，那么，这行的数据就是随后数据的基地址。
//:020000040800F2
//:10000000300B0020CD010008E12D0008E12D000893
//:10001000E12D0008E12D0008E12D0008000000009E
//第一行，是Extended LinearAddress Record，里面的基地址是0x0004，
//第二行是Data Record，里面的地址值是0x0000。那么数据300B0020CD010008E12D0008E12D0008
//要写入FLASH中的地址为(0x0004 << 16) | 0x0000，也就是写入FLASH的0x40000这个地址。
//同样，第三行的数据的写入地址为0x40010。
//当一个HEX文件的数据超过64k的时候，文件中就会出现多个Extended Linear Address Record。
//
//:00000001FF
//End of File Record 行是每一个HEX文件的最后一行。例如：
//这样的一行数据内容是固定的，数据长度为0，地址为0。
//
//
//HEX文件如下：
//:020000040800F2
//:10000000300B0020CD010008E12D0008E12D000893
//:10001000E12D0008E12D0008E12D0008000000009E
//          1      2       3       4       5       6       7       8       9       10
//: 10 0020 0000 0000 0000 0000 0000 0000 00E1 2D00 08BA    末尾的回车换行省略
//:10003000E12D000800000000E12D0008E12D00087E
//:10004000E12D0008E12D0008E12D0008E12D000858
//:10005000E12D0008E12D0008E12D0008E12D000848
//:10006000E12D0008E12D0008E12D0008E12D000838
//:10004000E12D0008E12D0008E12D0008E12D000858
//:10005000E12D0008E12D0008E12D0008E12D000848
//:10006000E12D0008E12D0008E12D0008E12D000838
//:10007000E12D0008E12D0008E12D0008E12D000828
//    ......
//:107780002C0A29C02C8B7A44040B2A07438813E463
//:107790006A12081A6B04011F1A0C243201690400D2
//:04000005080000ED02
//
//:00000001FF

unsigned char Hex2Char(unsigned char Hex[2])
{
	unsigned char c = 0;
	if(Hex[0] >= '0' && Hex[0] <= '9')
	{
		c = 16 * (Hex[0] - '0');
	}
	else if(Hex[0] >= 'A' && Hex[0] <= 'F')
	{
		c = 16 * (Hex[0] - 'A' + 10);
	}
	else if(Hex[0] >= 'a' && Hex[0] <= 'f')
	{
		c = 16 * (Hex[0] - 'a' + 10);
	}

	if(Hex[1] >= '0' && Hex[1] <= '9')
	{
		c +=  (Hex[1] - '0');
	}
	else if(Hex[1] >= 'A' && Hex[1] <= 'F')
	{
		c +=  (Hex[1] - 'A' + 10);
	}
	else if(Hex[1] >= 'a' && Hex[1] <= 'f')
	{
		c +=  (Hex[1] - 'a' + 10);
	}
	return c;

}

char HexBuf[102400];

int CComm::Hex2Bin(CString HexFile,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	unsigned int BinPos = 0;

	CFile f;
	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
		f.SeekToBegin();
		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
		f.Close(); 

		if(fileLen == 0)
		{
			return 0;
		}
		
		while(HexPos < fileLen)
		{
			HEX_LINE hex_line;
			int i;
			if(HexBuf[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = HexBuf[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);
				for(int j = 0; j < data_len; j++)
				{
					binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

					if(BinPos < (dataAddr + j))
					{
						BinPos = (dataAddr + j);
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	else
	{
		CString s = HexFile;
		s+= _T("\r\n 没有找到此文件"); 
		AfxMessageBox(s);
	}
	return BinPos;
}

int CComm::Hex2Bin(unsigned char *Hex,int HexBufLen ,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	unsigned int BinPos = 0;

//	CFile f;
//	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
//		f.SeekToBegin();
//		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
//		f.Close(); 

		if(HexBufLen <= 0)
		{
			return 0;
		}
		
		while(HexPos < HexBufLen)
		{
			HEX_LINE hex_line;
			int i;
			if(Hex[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = Hex[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);
#ifdef _ARM	
				if(dataAddr >= 0x4000)
#endif
				{
#ifdef _ARM	
					dataAddr -= 0x4000;
#endif
					for(int j = 0; j < data_len; j++)
					{
						binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

						if(BinPos < (dataAddr + j))
						{
							BinPos = (dataAddr + j);
						}
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	
	return BinPos;
}


void CComm::Set_HuiKong_DIO_ONOFF(int DO_Nr,bool On )
{
	unsigned char para[100];

	memset(para, 0, sizeof(para));
	if (On)
	{
		para[0] = 1;
	}
	else
	{
		para[0] = 0;
	}
//	AddCommCmd('1' + DO_Nr, para, DO_Nr);

}









char  s_buf[1000],s1_buf[100];


LRESULT DealOnComm_RS485(WPARAM wParam,LPARAM lParam,CWnd * ParentWnd)
{
	

	return 0;

}

