
#ifndef _COMM_H_
#define _COMM_H_

#include "stdAfx.h"

typedef struct
{
	HWND	m_hWnd;
	void	*comm;
	UINT	MessageID;
	CWinThread *ReadThread;

}COMM_THREAD_PARA;

extern	COMM_THREAD_PARA G_CommPara;
unsigned char CalCheckSum(char *buf,int len);
unsigned char Buf2Char(unsigned char * buf);
bool IsValidTemp(float temp);
void WriteLogFile(char *msg, int BoxNr, bool NoTimeLimit = true);
//void CRC16(unsigned char *source, int  n,  unsigned char ReturnData[2]);
//bool CheckCRC16(unsigned char *source, int  n, unsigned char source_crc[2]);
/////////////////////////////////////////////////////////////////////////////
class CComm
{
private:
	HANDLE hComm;
	OVERLAPPED ov;
public:
	bool bReadComm;
	CComm();
	int OpenComm(char comm,COMM_THREAD_PARA *para);
	int CloseComm(void);
	int ReadComm(char *buf,int len); 
	int WriteComm(char *buf,int len);
	bool IsOpened(void);
	bool	bXModem;
    virtual ~CComm();

	static bool CComm::CheckCRC16(unsigned char *source, int  n, unsigned char source_crc[2]);
	static void CComm::CRC16(unsigned char *source, int  n, unsigned char ReturnData[2]);
	static void CComm::CRC16_AFF2(unsigned char *source, int  n, unsigned char ReturnData[2]);
	static unsigned short CComm::XMODEM_CRC(char *ptr, int count);
};

#define COMM_MSG	WM_USER+1000
#define CMD_STX		 0x02
#define CMD_ETX		 0x03

#define CRC_START_VAL_0		(unsigned char)0xFF
#define CRC_START_VAL_1		(unsigned char)0xFF



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WD_STEPDLG_H__8830311D_0F83_4E8F_8DE8_CC092EAFA428__INCLUDED_)
