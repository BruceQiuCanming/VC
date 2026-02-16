#ifndef Device_HuiLong_PCB_H
#define Device_HuiLong_PCB_H

#include "modbus.h"
#include "comm.h"
#include "waitcmd.h"
#include "CMyEdit.h"

class CDevice_HuiLong_PCB : public CComm,public CWaitCmd
{
public :


	CDevice_HuiLong_PCB();
    ~CDevice_HuiLong_PCB();

	void DealOnComm(unsigned char *Data,int DataLen);
    void SetBaudrate(CComm *comm);
	void SetSv(int Sv);
	void ReadData(void);
	void SetSleepMode(bool Sleep);
	int					m_Pv;
	int					m_Sv;
	int					m_Pp;
	int					m_BoxNr;
	CComm				*m_Comm;
	COMM_THREAD_PARA	m_CommPara;
	CString				m_BarCode;
	int					m_Device_HuiKong_DIO_Y;
	CTime				m_BoxBeginTime;
	CString				m_EditMsg;
	int					m_SubMode;
	CTime				m_SubModeBeginTime;
	CButton				m_CheckPCB_Power;
	CMyEdit				m_Edit_Result;

};

#endif