// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "DebugDlg.h"
#include "HeatBoxDlg.h"
#include "comm.h"
#include "common.h"
#include "DrawLed.h"
#include "RecordViewDlg.h"
#include "CopyRight.h"
#include "InputGlassTempDlg.h"
#include "InputDlg.h"
#include "InputAutoAdjustTempDlg.h"
#include "PM24_Printer.h"
#include "ProgramDlg.h"

#include "afxinet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int count = 0;


/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog


void GetPcbModeText(int mode,char *buf)
{
	char *s = "无定义";
	switch(mode)
	{
	case PCB_SUB_MODE_STOP:
		s = "停止";
		break;
	case PCB_SUB_MODE_DIRECT_HEAT:
		s = "全速加热";
		break;
	case PCB_SUB_MODE_NORMAL_PID:
		s = "恒温";
		break;
	case PCB_SUB_MODE_INCREASE_PID:
		s = "线性加热";
		break;
	case PCB_SUB_MODE_DIRECT_COOL:
		s = "全速降温";
		break;
	case PCB_SUB_MODE_PID_COOL:
		s = "恒冷";
		break;
	case PCB_SUB_MODE_STEP_PID_COOL:
		s = "线性降温";
		break;
	case PCB_SUB_MODE_AT:
		s = "自整定";
		break;
	case PCB_SUB_MODE_COOL_AT:
		s = "冷";
		break;
	};

	memcpy(buf,s,strlen(s));
};



CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
	, m_bCheckCoolRelay(FALSE)
	, G_CommRecString(_T(""))
	, G_CommSendString(_T(""))
	, m_bDeltaFrequnce(FALSE)
	, m_bJiajuTesting(FALSE)
	, m_LedType(0)
	, m_CalText(_T(""))
	, m_bCheck_192_Interface_PcbType(FALSE)
	, m_bCheckBeep(FALSE)
	, m_bCheckFan(FALSE)
{
	//{{AFX_DATA_INIT(CDebugDlg)
	m_bHighVolt = FALSE;
	m_bCheckRelay = FALSE;
	m_bCheckHighVolt = FALSE;
	m_bReset = FALSE;
	m_CalMsg = _T("");
	//}}AFX_DATA_INIT

	m_PidBeginTime = -1;

	m_SendCmdCount		=	0;
	m_ReceiveCmdCount	=	0;
	
	m_TimerCount		=	0;
	m_StartPid			=   -1;

	m_CurTemp[0] = 0.0f;
	m_CurTemp[1] = 0.0f;
	m_CurTemp[2] = 0.0f;
	m_CurTemp[3] = 0.0f;

	m_JiaJuTestedPass[0] = false;
	m_JiaJuTestedPass[1] = false;
}


void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugDlg)
	DDX_Check(pDX, IDC_CHECK_RELAY, m_bCheckRelay);
	DDX_Check(pDX, IDC_CHECK_HIGH_VOLT, m_bCheckHighVolt);
	DDX_Check(pDX, IDC_CHECK_RESET, m_bReset);
	DDX_Text(pDX, IDC_CAL_MSG, m_CalMsg);
	//DDX_Text(pDX, IDC_EDIT_SEND_CMD_COUNT,		m_SendCmdCount);
	//DDX_Text(pDX, IDC_EDIT_RECEIVE_CMD_COUNT,	m_ReceiveCmdCount);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_COOL_RELAY, m_bCheckCoolRelay);
	DDX_Text(pDX, IDC_EDIT_COMM_REC, G_CommRecString);
	DDX_Text(pDX, IDC_EDIT_COMM_SEND, G_CommSendString);
	DDX_Check(pDX, IDC_CHECK_DELTA_FREQUNCE, m_bDeltaFrequnce);
	DDX_Check(pDX, IDC_CHECK_JIAJU_TEST, m_bJiajuTesting);
	DDX_Control(pDX, IDC_COMBO_INTERFACE_BOARD_TYPE, m_ComboInterfaceBoardType);
	DDX_Text(pDX, IDC_EDIT1, m_CalText);
	DDX_Control(pDX, IDC_COMBO_TM7705_OSC, m_ComboTM7705_OSC);
	DDX_Check(pDX, IDC_CHECK_192_INTERFACE_PCB_TYPE, m_bCheck_192_Interface_PcbType);
	DDX_Check(pDX, IDC_CHECK_BEEP, m_bCheckBeep);
	DDX_Check(pDX, IDC_CHECK_FAN, m_bCheckFan);
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	//{{AFX_MSG_MAP(CDebugDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton_STOP_OUT)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton_ReadAdjustLow)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton_ReadAdjustHigh)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton_Cal_Adjust)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton_PID)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonSetPID)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton_SetAdjustLow)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton_SetAdjustHigh)
	ON_BN_CLICKED(IDC_BUTTON_BOARD_ADJUST, OnButtonBoardAdjust)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDBLCLK()

	ON_BN_CLICKED(IDC_BUTTON_SYNCHRONIZATION_BEGIN, OnButtonSynchronizationBegin)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_READ_SYNCHRONIZATION, OnButtonReadSynchronization)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_VIEW, OnButtonRecordView)
	ON_BN_CLICKED(IDC_BUTTON_COPYRIGHT, OnButtonCopyright)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DOOR, OnButtonOpenDoor)
	ON_BN_CLICKED(IDC_BUTTON_HIGH_VOLT, OnButtonHighVolt)
	ON_BN_CLICKED(IDC_RADIO_LED_STATUS, OnRadioLedStatus)
	ON_BN_CLICKED(IDC_RADIO_LED_ALL_ON, OnRadioLedAllOn)
	ON_BN_CLICKED(IDC_RADIO_LED_ALL_OFF, OnRadioLedAllOff)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST_WIZARD, OnButtonAdjustWizard)
	ON_BN_CLICKED(IDC_BUTTON_SET_ADJUST_ALL, OnButtonSetAdjustAll)
	ON_BN_CLICKED(IDC_BUTTON_READ_ADJUST_ALL, OnButtonReadAdjustAll)
	ON_BN_CLICKED(IDC_CHECK_RESET, OnCheckReset)
	ON_BN_CLICKED(IDC_BUTTON_CAL_TEMP, OnButtonCalTemp)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_RELAY, &CDebugDlg::OnBnClickedCheckRelay)
	ON_BN_CLICKED(IDC_CHECK_HIGH_VOLT, &CDebugDlg::OnBnClickedCheckHighVolt)
	ON_EN_SETFOCUS(IDC_EDIT_PARA4, &CDebugDlg::OnEnSetfocusEditPara4)
	ON_CONTROL_RANGE(EN_SETFOCUS,IDC_EDIT_PARA5,IDC_EDIT_PARA_PID_Nr,OnEnSetfocusEditPara)

	ON_EN_SETFOCUS(IDC_EDIT_SET_ADJUST_ALL, &CDebugDlg::OnEnSetfocusEditSetAdjustAll)
	ON_BN_CLICKED(IDC_CHECK_COOL_RELAY, &CDebugDlg::OnBnClickedCheckCoolRelay)
	ON_BN_CLICKED(IDC_BUTTON14, &CDebugDlg::OnBnClickedButton14)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SET_ADDR, &CDebugDlg::OnBnClickedButtonSetAddr)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE, &CDebugDlg::OnBnClickedButtonUpdateFirmware)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER_TEST,&CDebugDlg::OnBnClickedButtonPrinterTest)
	ON_BN_CLICKED(IDC_BUTTON_AT, &CDebugDlg::OnBnClickedButtonAt)
	ON_BN_CLICKED(IDC_CHECK_DELTA_FREQUNCE, &CDebugDlg::OnBnClickedCheckDeltaFrequnce)
	ON_EN_SETFOCUS(IDC_EDIT_SET_PT100_COUNT, &CDebugDlg::OnEnSetfocusEditSetPt100Count)
	ON_BN_CLICKED(IDC_BUTTON_SET_ADJUST_ALL2, &CDebugDlg::OnBnClickedButtonSetAPT100Count)

	ON_BN_CLICKED(IDC_CHECK_JIAJU_TEST, &CDebugDlg::OnBnClickedCheckJiajuTest)
	ON_CBN_SELENDOK(IDC_COMBO_INTERFACE_BOARD_TYPE, &CDebugDlg::OnCbnSelendokComboInterfaceBoardType)
	ON_BN_CLICKED(IDC_BUTTON_READ_BIMETALLIC_STRIP, &CDebugDlg::OnBnClickedButtonReadBimetallicStrip)
	ON_EN_CHANGE(IDC_EDIT_PARA9, &CDebugDlg::OnEnChangeEditPara9)
	ON_CBN_SELCHANGE(IDC_COMBO_TM7705_OSC, &CDebugDlg::OnCbnSelchangeComboTm7705Osc)
	ON_CBN_SELENDOK(IDC_COMBO_TM7705_OSC, &CDebugDlg::OnCbnSelendokComboTm7705Osc)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_BOOTLOADER, &CDebugDlg::OnBnClickedButtonUpdateBootloader)
	ON_BN_CLICKED(IDC_CHECK_192_INTERFACE_PCB_TYPE, &CDebugDlg::OnBnClickedCheck192InterfacePcbType)
	ON_BN_CLICKED(IDC_CHECK_BEEP, &CDebugDlg::OnBnClickedCheckBeep)
	ON_BN_CLICKED(IDC_CHECK_FAN, &CDebugDlg::OnBnClickedCheckFan)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SOUND, &CDebugDlg::OnBnClickedButtonClearSound)
	ON_EN_SETFOCUS(IDC_EDIT_PARA13, &CDebugDlg::OnEnSetfocusEditPara13)
	ON_EN_SETFOCUS(IDC_EDIT_PARA14, &CDebugDlg::OnEnSetfocusEditPara14)
	ON_BN_CLICKED(IDC_BUTTON_CPU_ID, &CDebugDlg::OnBnClickedButtonCpuId)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE_BOARD_TYPE, &CDebugDlg::OnCbnSelchangeComboInterfaceBoardType)
	ON_BN_CLICKED(IDC_BUTTON_NTC_RELAY_LED, &CDebugDlg::OnBnClickedButtonNtcRelayLed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg message handlers

void CDebugDlg::OnButton_STOP_OUT() 
{
	// TODO: Add your control notification handler code here
#ifdef WT_504
		float temp = 0;
		char para[4]={0,0,0,0};
		para[0] = SP_ADDR / 256;
		para[1] = SP_ADDR % 256;
		para[2] = (temp *10) / 256;
		para[3] = ((int)(temp *10)) % 256;
		
		SetCmdString( m_BoxNr + 1,CMD_WRITE, para);
		m_SendCmdArray.Add(cmdStr);
#else
		G_RS485_Comm.SendStopHeatCmd(-50.0f,m_BoxNr);

#endif
}

void CDebugDlg::OnButton_ReadAdjustLow() 
{
	// TODO: Add your control notification handler code here
#ifndef WT_504
	G_RS485_Comm.ReadAdjustLow( m_BoxNr);

#endif
}

void CDebugDlg::OnButton_ReadAdjustHigh() 
{
	// TODO: Add your control notification handler code here

	
	G_RS485_Comm.ReadAdjustHigh( m_BoxNr);


}



void CDebugDlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
	CWnd *w;
	w = this->GetDlgItem(IDC_EDIT_PARA33);
	w->SetWindowText(_T(""));
	w = this->GetDlgItem(IDC_EDIT_PARA34);
	w->SetWindowText(_T(""));
	w = this->GetDlgItem(IDC_EDIT_PARA35);
	w->SetWindowText(_T(""));
	w = this->GetDlgItem(IDC_EDIT_PARA36);
	w->SetWindowText(_T(""));
	
}

void CDebugDlg::OnButton17() 
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<4;i++)
	{
		CEdit *edit1,*edit2;
		CString s;
		edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA17+i);
		edit2=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA45+i);
		edit1->GetWindowText(s);
		edit2->SetWindowText(s);
	}
}

void CDebugDlg::OnButton_Cal_Adjust() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	int i;
	double v0[4],v_low[4],v_high[4],v200[4];
	float glass_low[4],glass_high[4];
	float glass_low_avr,glass_high_avr;
//	CalAdjustTemp();

//	i = CalAdjust(0,0);
//	i = CalAdjust(20000,0);
//	i = CalAdjust(6000,0);
//	i = CalAdjust(14000,0);
	//记录一个低温点，一个高温点，根据线性方程求0,200度的误差
	//低温点一般取常温，高温点最好在150度，要分别恒温 

	CEdit *edit1;
	CString s;
	for(i=0;i<4;i++)
	{
		edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA45+i);
		edit1->GetWindowText(s);
		s.Replace(_T("_"),_T("-")); 
		v_low[i]=_tcstod(s,NULL);

		edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA49+i);
		edit1->GetWindowText(s);
		s.Replace(_T("_"),_T("-")); 
		v_high[i]=_tcstod(s,NULL);

		edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA53+i);
		edit1->GetWindowText(s);
		s.Replace(_T("_"),_T("-")); 
		glass_low[i]=_tcstod(s,NULL);

		edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA57+i);
		edit1->GetWindowText(s);
		s.Replace(_T("_"),_T("-")); 
		glass_high[i]=_tcstod(s,NULL);
	}
	for(int i = 0; i < 4; i++)
	{
		::G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_InputGlassLow[m_BoxNr][i]	=  glass_low[i];
		::G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_InputGlassHigh[m_BoxNr][i]	=  glass_high[i];

		::G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_MeterLow[m_BoxNr][i]			=	v_low[i];
		::G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_MeterHigh[m_BoxNr][i]		=	v_high[i];

	}
	::G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.CalAdjustTemp(this,m_BoxNr);

	//glass_low_avr=(glass_low[0]+glass_low[1]+glass_low[2]+glass_low[3])/4;
	//glass_high_avr=(glass_high[0]+glass_high[1]+glass_high[2]+glass_high[3])/4;

	return;

	for(i=0;i<4;i++)
	{
		// y = kx + delta
		double delta_low = (glass_low_avr-v_low[i]);
		double delta_high = (glass_high_avr-v_high[i]);
		
		if((v_high[i]-v_low[i]) != 0)
		{
			double k = (glass_high_avr-glass_low_avr) /  (v_high[i]-v_low[i]);
			
			double delta = glass_low_avr - k * v_low[i];
			
			v0[i] = delta;
			
			v200[i] =  (200.0*k + delta) - 200;
			
		//	v0[i]=(glass_low_avr-v_low[i])-((glass_high_avr-v_high[i])-(glass_low_avr-v_low[i]))/2;

		//	v200[i]=(glass_high_avr-v_high[i])+((glass_high_avr-v_high[i])-(glass_low_avr-v_low[i]))/2;

			//v200[i]=v150[i]+(v150[i]-v50[i])/2;
			edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA29+i);
			s.Format(_T("%5.1f"),v0[i]); 
			edit1->SetWindowText(s);
			edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA33+i);
			s.Format(_T("%5.1f"),v200[i]); 
			edit1->SetWindowText(s);
		}
	}
}

void CDebugDlg::OnButton18() 
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<4;i++)
	{
		CEdit *edit1,*edit2;
		CString s;
		edit1=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA17+i);
		edit2=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA49+i);
		edit1->GetWindowText(s);
		edit2->SetWindowText(s);
	}
}

void CDebugDlg::OnButton2() 
{
	
		CEdit *edit;
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA5);

		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		double temp = _tcstod(buf,NULL);
		
		
		
		G_RS485_Comm.SendDirectHeatCmd(temp,m_BoxNr);

}

void CDebugDlg::OnButton_PID() 
{
	// TODO: Add your control notification handler code here
		CEdit *edit;
		if(::G_NormalConfigPara[m_BoxNr].BoxType == ::BOX_TEMP2_10IN_12OUT)
		{
			TCHAR buf[10];

			edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA9);
			edit->GetWindowText(buf,10); 
			double temp1 = _tcstod(buf,NULL);

			edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA10);
			edit->GetWindowText(buf,10); 
			double temp2 = _tcstod(buf,NULL);

		
			
			G_RS485_Comm.SendNormalPidCmd(temp1, temp2,m_BoxNr);
			
		}
		else
		{
			edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA9);
			TCHAR buf[10];
			edit->GetWindowText(buf,10); 
			double temp = _tcstod(buf,NULL);

		
			if(temp > ::G_MainDlg->m_SwitchDlg[m_BoxNr].m_Meter.m_Pv )
			{
				G_RS485_Comm.SendNormalPidCmd(temp, m_BoxNr);
			}
			else
			{
				G_RS485_Comm.SendPidCoolCmd(temp, m_BoxNr,true);
			}

		}

		m_StartPid = 0;
}

void CDebugDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
		
		CEdit *edit;
		
		TCHAR buf[10];
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA13);
		edit->GetWindowText(buf,10); 
		double temp = _tcstod(buf,NULL);
		
		
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA14);
		edit->GetWindowText(buf,10); 
		int monutes = _tcstod(buf,NULL);
		
		if(monutes > 0)
		{
			G_RS485_Comm.SendStepPIDCmd(temp,monutes,m_BoxNr);
		}

}

void CDebugDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	OnButtonAll(4);
}

void CDebugDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	
	G_RS485_Comm.ReadPidT( m_BoxNr );
}

void CDebugDlg::OnButtonSetPID() 
{
	// TODO: Add your control notification handler code here


	char para[100];
	memset(para,0,sizeof(para));
	CEdit *edit;
	TCHAR buf[10];
	short v[8];
	
	memset(&v,0,sizeof(v));

	for(int i = 0; i < 4; i++)
	{
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA25+i);
		
		edit->GetWindowText(buf,10); 
		v[i] = _ttoi(buf);
		((short*)para)[i] = v[i];
		if(v[i] <= 0)
		{
			AfxMessageBox(_T("PID  值不能为0"));
			return;
		}
	}
	
	
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA_COOL_P_W);
	edit->GetWindowText(buf,10); 
	v[4] = _ttoi(buf);
	if(v[4] <= 0)
	{
		AfxMessageBox(_T("PID  值不能为0"));
		return;
	}

	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA_COOL_I_W);
	edit->GetWindowText(buf,10); 
	v[5] = _ttoi(buf);
	if(v[5] <= 0)
	{
		AfxMessageBox(_T("PID  值不能为0"));
		return;
	}

	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA_COOL_D_W);
	edit->GetWindowText(buf,10); 
	v[6] = _ttoi(buf);
	if(v[6] <= 0)
	{
		AfxMessageBox(_T("PID  值不能为0"));
		return;
	}
	if(::G_NormalConfigPara[0].BoxType == BOX_TEMP2_10IN_12OUT)
	{
		TCHAR buf[20];
		CEdit * edit = (CEdit * ) this->GetDlgItem(IDC_EDIT_PARA_PID_Nr) ;
		edit->GetWindowText(buf,10); 
		int Nr = _tcstol(buf,NULL,10);
		G_RS485_Comm.SetPID_T(v, m_BoxNr,Nr);
		G_RS485_Comm.ReadPidT(m_BoxNr,Nr);
	}
	else
	{
		G_RS485_Comm.SetPID_T(v, m_BoxNr);
		G_RS485_Comm.ReadPidT(m_BoxNr);
	}

}

void CDebugDlg::OnButton_SetAdjustLow() 
{
	// TODO: Add your control notification handler code here

	
	CEdit *edit;
	float val[32];
	for(int i = 0; i < 4; i++)
	{
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA29+i);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		val[i] = _tcstod(buf,NULL);
		::G_NormalConfigPara[m_BoxNr].AdjustLow[i] = val[i];
		::SaveNormalConfigPara();
	}
	
	if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_SENSATA_32PT100)
	{
		for(int i = 4; i < 32; i++)
		{
			val[i] = 0;
		}
	}

	G_RS485_Comm.SetAdjustLow(val, m_BoxNr);



	OnButton_ReadAdjustLow();
	
}

void CDebugDlg::OnButton_SetAdjustHigh() 
{
	// TODO: Add your control notification handler code here


	CEdit *edit;
	float val[32] = {	0.10, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
						0.00, 0.22, 0.00, 0.20, 0.40, 0.53, 0.49, 0.41,
						0.09, 0.12,-0.08,-0.01, 0.12, 0.12, 0.09, 0.03,
						0.11,-0.02,-0.04,-0.09,-0.34,-0.34,-0.63,-0.46};

	for(int i = 0; i < 4; i++)
	{
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA33+i);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		val[i] = _tcstod(buf,NULL);
		::G_NormalConfigPara[m_BoxNr].AdjustHigh[i] = val[i];
		::SaveNormalConfigPara();
	}


	if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_SENSATA_32PT100)
	{
		for(int i = 4; i < 32; i++)
		{
		//	val[i] = 0;
		}
	}

	::G_RS485_Comm.SetAdjustHigh(val, m_BoxNr);


	OnButton_ReadAdjustHigh();

}

void CDebugDlg::OnButtonAll(int index) 
{

}



void CDebugDlg::OnButtonBoardAdjust() 
{
	// TODO: Add your control notification handler code here
	

}

void CDebugDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if((nChar == 'A' || nChar == 'a') && (nRepCnt > 50))
	{
		CWnd *w = GetDlgItem( IDC_BUTTON_BOARD_ADJUST);
		w->ShowWindow(SW_SHOW);
	}
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CDebugDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDebugDlg::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::WinHelp(dwData, nCmd);
}

void CDebugDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnRButtonDblClk(nFlags, point);
}



void CDebugDlg::OnButtonSynchronizationBegin() 
{

}


void CDebugDlg::OnButtonReset() 
{

}

void CDebugDlg::OnButtonReadSynchronization() 
{

}

void CDebugDlg::OnButtonRecordView() 
{
	// TODO: Add your control notification handler code here
	CRecordViewDlg dlg;
	dlg.m_BoxNr  = m_BoxNr;
	dlg.DoModal(); 
}

void CDebugDlg::OnButtonCopyright() 
{


}

void CDebugDlg::OnButtonOpenDoor() 
{
#ifdef _HEATBOX_RESET_
	return;
#endif
	// TODO: Add your control notification handler code here
	

	UpdateData(true);


	if(m_bCheckRelay)
	{
		if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_TEMP2_10IN_12OUT)
		{
			static int door_id = 0;
			door_id++;
			if(door_id > 13)
			{
				door_id = 0;
			}
			G_RS485_Comm.SendOpenDoorCmd(door_id,m_BoxNr);  
		}
		else
		{
			G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr);  
		}
	}
	else
	{
		G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr);
	}
	
	
	
}

void CDebugDlg::OnButtonHighVolt() 
{
}

void CDebugDlg::OnRadioLedStatus() 
{

	// TODO: Add your control notification handler code here
	
	if(G_iCmdAscii[m_BoxNr] != 1)
	{
		
		G_RS485_Comm.SetDisplayType(0,m_BoxNr);
		
	}
}

void CDebugDlg::OnRadioLedAllOn() 
{

	// TODO: Add your control notification handler code here
	
	
	if(G_iCmdAscii[m_BoxNr] != 1)
	{
		G_RS485_Comm.SetDisplayType(1,m_BoxNr);

	}

}

void CDebugDlg::OnRadioLedAllOff() 
{
	// TODO: Add your control notification handler code here
	
	if(G_iCmdAscii[m_BoxNr] != 1)
	{
		G_RS485_Comm.SetDisplayType(2,m_BoxNr);

	}

}


void CDebugDlg::OnButtonAdjustWizard() 
{
}

void CDebugDlg::ShowPCBType(void)
{

	m_ComboInterfaceBoardType.SetCurSel(G_NormalConfigPara[m_BoxNr].InterfaceBoardType); 
	
}
BOOL CDebugDlg::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg); 


}
BOOL CDebugDlg::OnInitDialog() 
{



	CDialog::OnInitDialog();
	m_OnTiming = false;
	SetTimer(1,1000,NULL);
	for(int i = 0; i < 4; i++)
	{
	
		m_AdjustLow[i]	= 0.0f;
		m_AdjustHigh[i]	= 0.0f;
	}
	
	m_bSetFocusOnPara4 = true;
	CButton* b = (CButton*) GetDlgItem(IDC_RADIO_LED_STATUS);
	b->SetCheck(1);

	for(int i = 0; i < sizeof(G_InterfacePcbTypeName)/sizeof(G_InterfacePcbTypeName[0]); i++)
	{
		this->m_ComboInterfaceBoardType.AddString(G_InterfacePcbTypeName[i].name);
	}
	
	
	ShowPCBType();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDebugDlg::OnButtonSetAdjustAll() 
{
	// TODO: Add your control notification handler code here
	
	TCHAR buf[100];
	CWnd *w;
	w = this->GetDlgItem(IDC_EDIT_SET_ADJUST_ALL);
	w->GetWindowText(buf,100);
	float val  = _tcstod(buf,NULL);
	
	G_RS485_Comm.SetAdjustAll(val,m_BoxNr);


	OnButtonReadAdjustAll();
	
/*
	float val[8] = {1,2,3,4,5,6,7,8};

	G_RS485_Comm.SetAdjustAll(val,m_BoxNr);


	OnButtonReadAdjustAll();
*/

}

void CDebugDlg::OnButtonReadAdjustAll() 
{

	// TODO: Add your control notification handler code here
	
	//if(G_iCmdAscii[m_BoxNr] != 1)
	{
	
		G_RS485_Comm.ReadAdjustAll(m_BoxNr);
	}
}

void CDebugDlg::OnCheckReset() 
{

	// TODO: Add your control notification handler code here
	this->UpdateData(true);
	

	if(G_iCmdAscii[m_BoxNr] != 1)
	{
	
		if(this->m_bReset)
		{
			G_RS485_Comm.SetDisplayType(1,m_BoxNr);
		}
		else
		{
			G_RS485_Comm.SetDisplayType(0,m_BoxNr);
		}


	}
}

void CDebugDlg::OnButtonCalTemp() 
{
	// TODO: Add your control notification handler code here
	//this->InputGlassLowTemp(); 
	
	::G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.EnterCalMode(G_MainDlg,m_BoxNr); 

}

void CDebugDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent != 1)
		return;

	CDialog::OnTimer(nIDEvent);

}





bool CDebugDlg::IsAdjustAllZero()
{
	CWnd *w;
	CString text;
	w	=	GetDlgItem(IDC_EDIT_READ_ADJUST_ALL);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	return true;
}

/*
bool CDebugDlg::IsAllPIDZero()
{
	CWnd *w;
	CString text;
	w	=	GetDlgItem(IDC_EDIT_PARA21);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	return true;
}
*/

void CDebugDlg::DealCmdAnswer(int cmd_addr, int data_len,unsigned char * para, unsigned char * flash,bool IsCmdAscii)
{


	CString s;
	short short_a;
	int i;
	this->UpdateData();

	MODBUS_RS485_CMD_32PT100   *modbus_cmd_32PT100						=	(MODBUS_RS485_CMD_32PT100*) para;

	MODBUS_RS485_8TEMP_8SSR_ANSWER			*Answer_8TEMP_8SSR			=	(MODBUS_RS485_8TEMP_8SSR_ANSWER*)para;
	MODBUS_RS485_CMD_PARA							*Para_Answer		=	(MODBUS_RS485_CMD_PARA*) para;
	MODBUS_RS485_NTC_AD_VAL_ANSWER					*NTC_Answer			=	(MODBUS_RS485_NTC_AD_VAL_ANSWER*) para;
	ASCII_RS485_COMMAND_READ_TEMP *ascii_cmd							=(ASCII_RS485_COMMAND_READ_TEMP *) para;

	MODBUS_RS485_CMD_128    *modbus_cmd_128								=(MODBUS_RS485_CMD_128*) para;
	MODBUS_RS485_STATUS_FLASH_ANSWER_128 * modbus_status_cmd_128		= (MODBUS_RS485_STATUS_FLASH_ANSWER_128*) para;

	MODBUS_RS485_CMD_192    *modbus_cmd_192								=(MODBUS_RS485_CMD_192*) para;
	MODBUS_RS485_STATUS_FLASH_ANSWER_192 * modbus_status_cmd_192		= (MODBUS_RS485_STATUS_FLASH_ANSWER_192*) para;

	
	unsigned char cmd;

	if(IsCmdAscii == true)
	{
		cmd = ascii_cmd->header.command;
	}
	else
	{
		cmd = modbus_cmd_128->head.cmd;
	}

	switch(cmd)
	{
//#ifndef _ASCII_	
	case CMD_READ_DISC_STATUS:
		DrawLED();
		break;
	case CMD_READ_STATUS:
		if(::G_NormalConfigPara[this->m_BoxNr].BoxType == BOX_NTC)
		{
			for(int i = 0; i < 64; i++)
			{
				this->m_CurNTC[i] = *((float*)&NTC_Answer->para[i * sizeof(float)]);
			}
		}
		if(IsCmdAscii == false)
		{
			DrawLED();
		}
		if(::G_NormalConfigPara[this->m_BoxNr].BoxType == HEAT_BOX_TYPE_HANJIE)
		{
			if(::G_SpeechArray.GetCount() < 1)
			{
				if(G_iMaxLedNr[this->m_BoxNr] == 192)
				{
							
					{
						for(int i=0; i <G_iMaxLedNr[m_BoxNr]; i++)
						{
							if(::G_SwitchTestResult[m_BoxNr][i].IsClosing )
							{
								CString nr;
								
								if(((i + 1) % G_iLedNrPerLine[m_BoxNr]) == 0)
								{
									nr.Format(_T("%dB"),(i + 1) / G_iLedNrPerLine[m_BoxNr] );
								}
								else if(((i + 1) % G_iLedNrPerLine[m_BoxNr]) < 10)
								{
									nr.Format(_T("%d%d"),(i+1)/G_iLedNrPerLine[m_BoxNr],(i+1)%G_iLedNrPerLine[m_BoxNr]);
								}
								else if(((i + 1) % G_iLedNrPerLine[m_BoxNr]) == 10)
								{
									nr.Format(_T("%d0"),(i+1)/G_iLedNrPerLine[m_BoxNr]+1);
								}
								else if(((i + 1) % G_iLedNrPerLine[m_BoxNr]) == 11)
								{
									nr.Format(_T("%dA"),(i + 1) / G_iLedNrPerLine[m_BoxNr] + 1);
								}
								
								G_SpeechArray.Add(nr);
							}
								
						}
					}

				}
				
				if(G_iMaxLedNr[m_BoxNr] == 128)
				{
							
					{
						for(int i=0; i <G_iMaxLedNr[m_BoxNr]; i++)
						{
							if(::G_SwitchTestResult[m_BoxNr][i].IsClosing )
							{
								CString nr;
								
								nr.Format(_T("%d"),i + 1 );
								
								G_SpeechArray.Add(nr);
							}
								
						}
					}

				}
				
			}
		}
		break;
//#endif	

	case CMD_READ_TEMP:
			if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_TEMP2_10IN_12OUT)
			{
					MODBUS_RS485_TEMP_ANSWER_PARA_TEMP2_10IN_12OUT answer_para;
					memcpy(&answer_para,modbus_cmd_128->para,sizeof(answer_para));
					for(int i = 0; i < 2; i++)
					{
						CWnd *w= GetDlgItem(IDC_EDIT_PARA17 + i);
						m_CurTemp[i] = answer_para.Temp[i] / 100.0f;
						double v = m_CurTemp[i];
						s.Format(_T("%5.2f"),v);
						w->SetWindowTextW(s); 
					}

					double sv1 = answer_para.Sv[0] / 10.0f;
					double sv2 = answer_para.Sv[1] / 10.0f;
				
					CWnd *w; 
					w= GetDlgItem(IDC_EDIT_PARA6);
					s.Format(_T("%5.2f,%5.2f,%d%%,%d%%"),sv1,sv2,answer_para.HeatPercent[0],answer_para.HeatPercent[1]);
					w->SetWindowTextW(s); 

					break;
			}
			else if(data_len == sizeof(MODBUS_RS485_8TEMP_8SSR_ANSWER))
		{
			m_PidTemp = 0;
			for(i=0;i<4;i++)
			{
				
				CWnd *w= GetDlgItem(IDC_EDIT_PARA17 + i);
				s.Format(_T("%5.1f"),Answer_8TEMP_8SSR->Pv[i] /100.0f);
				w->SetWindowTextW(s); 
				m_CurTemp[i] = Answer_8TEMP_8SSR->Pv[i] /100.0f;
				m_PidTemp	+= Answer_8TEMP_8SSR->Pv[i] /100.0f;
			}

			CWnd *w= GetDlgItem(IDC_EDIT_PARA3);
			s.Format(_T("%5.2f"),Answer_8TEMP_8SSR->Pv[0] / 100.0f);
			w->SetWindowTextW(s); 
			w= GetDlgItem(IDC_EDIT_PARA4);
			s.Format(_T("%5.2f"),Answer_8TEMP_8SSR->Sv[0] / 10.0f);
			w->SetWindowTextW(s); 
			
			break;
		}
		DrawLED();
		if(IsCmdAscii == true)
		{

			m_PidTemp = 0;
			for(i=0;i<4;i++)
			{
				if(ascii_cmd->val[i*4 + 0] == '_')
				{
					short_a =  (ascii_cmd->val[i*4 + 1] - '0')*100
							  +(ascii_cmd->val[i*4 + 2] - '0')*10
							  +(ascii_cmd->val[i*4 + 3] - '0')*1;
					short_a = 0 - short_a;
				}
				else
				{
						short_a =  (ascii_cmd->val[i*4 + 0] - '0')*1000
							  +(ascii_cmd->val[i*4 + 1] - '0')*100
							  +(ascii_cmd->val[i*4 + 2] - '0')*10
							  +(ascii_cmd->val[i*4 + 3] - '0')*1;
				}
				CWnd *w= GetDlgItem(IDC_EDIT_PARA17 + i);
				s.Format(_T("%5.1f"),short_a /10.0f);
				w->SetWindowTextW(s); 
				m_CurTemp[i] = short_a /10.0f;
				m_PidTemp += short_a /10.0f;
			}
			m_PidTemp /= 4.0f;
			
			TEMP_TIME tt;
			tt.temp = m_PidTemp;
			tt.TimeTick = ::GetTickCount(); 
			if(m_PidTempArray.GetCount() > 1000)
			{
				m_PidTempArray.RemoveAt(0);
			}
			m_PidTempArray.Add(tt); 

		}
		else
		{//MODBUS
			MODBUS_RS485_TEMP_ANSWER_PARA *temp_para = (MODBUS_RS485_TEMP_ANSWER_PARA *) modbus_cmd_128->para;
			m_PidTemp = 0;

			for(i=0;i<4;i++)
			{
				
				if(temp_para->TempDecimalPlaces == 1)
				{
					m_CurTemp[i] = temp_para->Temp[i] / 10.0f; 
				}
				else
				{
					m_CurTemp[i] = temp_para->Temp[i] / 100.0f; 
				}
				
				if(::G_iCmdAscii[m_BoxNr] == 2)//OIL
				{
					m_CurTemp[1]	=	m_CurTemp[0];
					m_CurTemp[2]	=	m_CurTemp[0];
					m_CurTemp[3]	=	m_CurTemp[0];

				}

				CWnd *w= GetDlgItem(IDC_EDIT_PARA17 + i);
				double v = m_CurTemp[i];
				s.Format(_T("%5.2f"),v);
				w->SetWindowTextW(s); 
				m_PidTemp += v;
			}
			{
				m_PidTemp /= 4.0f;

				TEMP_TIME tt;
				tt.temp = m_PidTemp;
				tt.TimeTick = ::GetTickCount(); 
				if(m_PidTempArray.GetCount() > 1000)
				{
					m_PidTempArray.RemoveAt(0);
				}
				m_PidTempArray.Add(tt); 


				CWnd *w= GetDlgItem(IDC_EDIT_PARA3);
				s.Format(_T("%5.2f"),m_PidTemp);
				w->SetWindowTextW(s); 
			}

			{
				/*CWnd *w= GetDlgItem(IDC_EDIT_PARA6);
				s.Format(_T("热%d%%冷%d%%错%d%s "),temp_para->HeatPercent,temp_para->CoolPercent,temp_para->iError,GetPcbModeText(temp_para->TestMode));
				if(temp_para->OutputStatus & (1 << OUTPU_STATUS_AIR_OFF_BIT))
				{
					s += _T("空调 关");
				}
				else
				{
					s += _T("空调 开");
				}
				w->SetWindowTextW(s);
				*/
				static DWORD t = ::GetTickCount();
				char buf[100],buf2[100];
				memset(buf,0,sizeof(buf));
				memset(buf2,0,sizeof(buf2));
				if(GetTickCount() > t + 5000)
				{//在线拔板的时候也会报错
					t = GetTickCount();
					GetPcbModeText(temp_para->TestMode,buf);
					sprintf(buf2,"热%d%%错%d%s ",temp_para->HeatPercent,temp_para->iError,buf);
					WriteLogFile(buf2,m_BoxNr + 1);
				}
			}

			{
				/*short_a = 0;
				char *buf = (char*)&short_a; 
				if(G_iMaxLedNr[m_BoxNr] == 192)
				{
					buf[0]	=	(char) modbus_cmd_192->para[8];
					buf[1]  =	(char) modbus_cmd_192->para[9];
				}
				else
				{
					buf[0]	=	(char) modbus_cmd_128->para[8];
					buf[1]  =	(char) modbus_cmd_128->para[9];
				}*/
				//double v = short_a /10.0f;
				double v = temp_para->Sv / 10.0f;
				
				CWnd *w= GetDlgItem(IDC_EDIT_PARA4);
				s.Format(_T("%5.2f"),v);
				w->SetWindowTextW(s); 
				w= GetDlgItem(IDC_EDIT_PARA6);
				s.Format(_T("%d"),temp_para->HeatPercent);
				w->SetWindowTextW(s); 
			}
		}
		break;
	case CMD_SET_ADJUST_LOW:
		m_CalMsg = (_T("设置0度修正值成功!"));
		break;

	case CMD_SET_ADJUST_HIGH:
		m_CalMsg = (_T("设置200度修正值成功!"));
		break;

	case CMD_READ_ADJUST_LOW:
		if(::G_NormalConfigPara[0].BoxType == BOX_SENSATA_32PT100)
		{
				for(i=0; i<32; i++)
				{
					this->m_ReadAdjustLow[i]  = ((signed char )modbus_cmd_32PT100->para[i]) /10.0f;
					::G_NormalConfigPara[m_BoxNr].AdjustLow[i] = m_ReadAdjustLow[i];
				}

				
				::SaveNormalConfigPara();
				

				for(i = 0; i < 4; i++)
				{
					CWnd *w= GetDlgItem(IDC_EDIT_PARA29 + i);
					s.Format(_T("%.1f"),m_ReadAdjustLow[i]);
					m_AdjustLow[i] = m_ReadAdjustLow[i];
			
					w->SetWindowTextW(s);
				}
		}
		else if(data_len == sizeof(MODBUS_RS485_CMD_PARA))
		{
			
					m_ReadAdjustLow[0]  =	Para_Answer->EEPROM_Answer.AdjustLow[0];
					CWnd *w= GetDlgItem(IDC_EDIT_PARA29);
					s.Format(_T("%.1f"),m_ReadAdjustLow[0]);
					m_AdjustLow[0] = m_ReadAdjustLow[0];
					w->SetWindowTextW(s);

					m_ReadAdjustLow[1]  =	Para_Answer->EEPROM_Answer.AdjustLow[1];
					w= GetDlgItem(IDC_EDIT_PARA30);
					s.Format(_T("%.1f"),m_ReadAdjustLow[1]);
					m_AdjustLow[1] = m_ReadAdjustLow[1];
					w->SetWindowTextW(s);

					m_ReadAdjustLow[2]  =	Para_Answer->EEPROM_Answer.AdjustLow[2];
					w= GetDlgItem(IDC_EDIT_PARA31);
					s.Format(_T("%.1f"),m_ReadAdjustLow[2]);
					m_AdjustLow[2] = m_ReadAdjustLow[2];
					w->SetWindowTextW(s);

					m_ReadAdjustLow[3]  =	Para_Answer->EEPROM_Answer.AdjustLow[3];
					w= GetDlgItem(IDC_EDIT_PARA32);
					s.Format(_T("%.1f"),m_ReadAdjustLow[3]);
					m_AdjustLow[3] = m_ReadAdjustLow[3];
					w->SetWindowTextW(s);
		}
		else
		{
			if(IsCmdAscii == false)
			{
				for(i=0;i<4;i++)
				{
					short_a = 0;
					char *buf = (char*)&short_a;

					if(G_iMaxLedNr[m_BoxNr] == 192)
					{
						buf[0]	=	(char) modbus_cmd_192->para[i*2];
						buf[1]  =	(char) modbus_cmd_192->para[i*2+1];
					}
					else
					{
						buf[0]	=	(char) modbus_cmd_128->para[i*2];
						buf[1]  =	(char) modbus_cmd_128->para[i*2+1];
					}

					this->m_ReadAdjustLow[i]  =	short_a /10.0f;
				}

				if(m_ReadAdjustLow[0] == 0.0f
					&& m_ReadAdjustLow[1] == 0.0f
					&& m_ReadAdjustLow[2] == 0.0f
					&& m_ReadAdjustLow[3] == 0.0f)
				{
					m_ReadAdjustLow[0] = ::G_NormalConfigPara[m_BoxNr].AdjustLow[0];
					m_ReadAdjustLow[1] = ::G_NormalConfigPara[m_BoxNr].AdjustLow[1];
					m_ReadAdjustLow[2] = ::G_NormalConfigPara[m_BoxNr].AdjustLow[2];
					m_ReadAdjustLow[3] = ::G_NormalConfigPara[m_BoxNr].AdjustLow[3];
					
					::G_RS485_Comm.SetAdjustLow(m_ReadAdjustLow,m_BoxNr);
				}
				else
				{
					 ::G_NormalConfigPara[m_BoxNr].AdjustLow[0] = m_ReadAdjustLow[0];
					::G_NormalConfigPara[m_BoxNr].AdjustLow[1] = m_ReadAdjustLow[1];
					::G_NormalConfigPara[m_BoxNr].AdjustLow[2] = m_ReadAdjustLow[2];
					::G_NormalConfigPara[m_BoxNr].AdjustLow[3] = m_ReadAdjustLow[3];
					::SaveNormalConfigPara();
				}
				for(i = 0; i < 4; i++)
				{
					CWnd *w= GetDlgItem(IDC_EDIT_PARA29 + i);
					s.Format(_T("%.1f"),m_ReadAdjustLow[i]);
					m_AdjustLow[i] = m_ReadAdjustLow[i];
			
					w->SetWindowTextW(s);
				}
			}
			else 
			{ //ASCII
				for(i=0;i<4;i++)
				{
					short_a = (ascii_cmd->val[i*4 + 1] - '0')*100
								+(ascii_cmd->val[i*4 + 2] - '0')*10
								+(ascii_cmd->val[i*4 + 3] - '0')*1;
					if(ascii_cmd->val[i*4] == '_')
					{
						short_a = 0-short_a;
					}
					para[i] = short_a ;
					CWnd *w= GetDlgItem(IDC_EDIT_PARA29 + i);
					s.Format(_T("%5.1f"),short_a /10.0f);
					m_AdjustLow[i] = short_a /10.0f;
					w->SetWindowTextW(s); 
				}
			}
		}
		m_CalMsg = (_T("读取0度修正值成功!"));
		break;

	case CMD_READ_ADJUST_HIGH:
		if(::G_NormalConfigPara[0].BoxType == BOX_SENSATA_32PT100)
		{
				for(i=0; i<32; i++)
				{
					this->m_ReadAdjustHigh[i]  = ((signed char )modbus_cmd_32PT100->para[i]) /10.0f;
					::G_NormalConfigPara[m_BoxNr].AdjustHigh[i] = m_ReadAdjustHigh[i];
				}

				
				::SaveNormalConfigPara();
				

				for(i = 0; i < 4; i++)
				{
					CWnd *w= GetDlgItem(IDC_EDIT_PARA33 + i);
					s.Format(_T("%.1f"),m_ReadAdjustHigh[i]);
					m_AdjustHigh[i] = m_ReadAdjustHigh[i];
			
					w->SetWindowTextW(s);
				}
		}
		else if(data_len == sizeof(MODBUS_RS485_CMD_PARA))
		{
			
					m_ReadAdjustHigh[0]  =	Para_Answer->EEPROM_Answer.AdjustHigh[0];
					CWnd *w= GetDlgItem(IDC_EDIT_PARA33);
					s.Format(_T("%.1f"),m_ReadAdjustHigh[0]);
					m_AdjustHigh[0] = m_ReadAdjustHigh[0];
					w->SetWindowTextW(s);

					m_ReadAdjustHigh[1]  =	Para_Answer->EEPROM_Answer.AdjustHigh[1];
					w= GetDlgItem(IDC_EDIT_PARA34);
					s.Format(_T("%.1f"),m_ReadAdjustHigh[1]);
					m_AdjustHigh[1] = m_ReadAdjustHigh[1];
					w->SetWindowTextW(s);

					m_ReadAdjustHigh[2]  =	Para_Answer->EEPROM_Answer.AdjustHigh[2];
					w= GetDlgItem(IDC_EDIT_PARA35);
					s.Format(_T("%.1f"),m_ReadAdjustHigh[2]);
					m_AdjustHigh[2] = m_ReadAdjustHigh[2];
					w->SetWindowTextW(s);

					m_ReadAdjustHigh[3]  =	Para_Answer->EEPROM_Answer.AdjustHigh[3];
					w= GetDlgItem(IDC_EDIT_PARA36);
					s.Format(_T("%.1f"),m_ReadAdjustHigh[3]);
					m_AdjustHigh[3] = m_ReadAdjustHigh[3];
					w->SetWindowTextW(s);
		}
		else
		{
			if(IsCmdAscii == false)
			{
				for(i=0;i<4;i++)
				{
					short_a = 0;
					char *buf = (char*)&short_a; 
					if(G_iMaxLedNr[m_BoxNr] == 192)
					{
						buf[0]	=	(char) modbus_cmd_192->para[i*2];
						buf[1]  =	(char) modbus_cmd_192->para[i*2+1];
					}
					else
					{
						buf[0]	=	(char) modbus_cmd_128->para[i*2];
						buf[1]  =	(char) modbus_cmd_128->para[i*2+1];
					}
					this->m_ReadAdjustHigh[i]  =	short_a /10.0f;
				}
				if(m_ReadAdjustHigh[0] == 0.0f
					&& m_ReadAdjustHigh[1] == 0.0f
					&& m_ReadAdjustHigh[2] == 0.0f
					&& m_ReadAdjustHigh[3] == 0.0f)
				{
					m_ReadAdjustHigh[0] = ::G_NormalConfigPara[m_BoxNr].AdjustHigh[0];
					m_ReadAdjustHigh[1] = ::G_NormalConfigPara[m_BoxNr].AdjustHigh[1];
					m_ReadAdjustHigh[2] = ::G_NormalConfigPara[m_BoxNr].AdjustHigh[2];
					m_ReadAdjustHigh[3] = ::G_NormalConfigPara[m_BoxNr].AdjustHigh[3];
					
					::G_RS485_Comm.SetAdjustHigh(m_ReadAdjustHigh,m_BoxNr);
				}
				else
				{
					 ::G_NormalConfigPara[m_BoxNr].AdjustHigh[0] = m_ReadAdjustHigh[0];
					::G_NormalConfigPara[m_BoxNr].AdjustHigh[1] = m_ReadAdjustHigh[1];
					::G_NormalConfigPara[m_BoxNr].AdjustHigh[2] = m_ReadAdjustHigh[2];
					::G_NormalConfigPara[m_BoxNr].AdjustHigh[3] = m_ReadAdjustHigh[3];
					::SaveNormalConfigPara();
				}
				for(i = 0; i < 4; i++)
				{
					CWnd *w= GetDlgItem(IDC_EDIT_PARA33 + i);
					s.Format(_T("%.1f"),m_ReadAdjustHigh[i]);
					m_AdjustHigh[i] = m_ReadAdjustHigh[i];
					w->SetWindowTextW(s); 
				}
			}
			else
			{//MODBUS
				for(i=0;i<4;i++)
				{
					short_a = (ascii_cmd->val[i*4 + 1] - '0')*100
								+(ascii_cmd->val[i*4 + 2] - '0')*10
								+(ascii_cmd->val[i*4 + 3] - '0')*1;
					if(ascii_cmd->val[i*4] == '_')
					{
						short_a = 0-short_a;
					}
					para[i] = short_a ;
					CWnd *w= GetDlgItem(IDC_EDIT_PARA33 + i);
					s.Format(_T("%5.1f"),short_a /10.0f);
					m_AdjustHigh[i] = short_a /10.0f;
					w->SetWindowTextW(s); 
				}
			}
		}
		m_CalMsg = ( _T("读取200度修正值成功!"));
		break;
	case CMD_NORMAL_PID_OUT:
		m_CalMsg = (_T("设置恒温成功!"));
		break;
	case CMD_READ_PID_T:
		
		if(data_len == sizeof(MODBUS_RS485_CMD_PARA))
		{
			G_PID[m_BoxNr].P = Para_Answer->EEPROM_Answer.PID.Pid_P;
			CWnd *w= GetDlgItem(IDC_EDIT_PARA25);
			s.Format(_T("%d"),G_PID[m_BoxNr].P );
			w->SetWindowTextW(s); 

			G_PID[m_BoxNr].I = Para_Answer->EEPROM_Answer.PID.Pid_I;
			w= GetDlgItem(IDC_EDIT_PARA26);
			s.Format(_T("%d"),G_PID[m_BoxNr].I );
			w->SetWindowTextW(s); 

			G_PID[m_BoxNr].D = Para_Answer->EEPROM_Answer.PID.Pid_D;
			w= GetDlgItem(IDC_EDIT_PARA27);
			s.Format(_T("%d"),G_PID[m_BoxNr].D );
			w->SetWindowTextW(s); 

			G_PID[m_BoxNr].T = Para_Answer->EEPROM_Answer.PID.Pid_T;
			w= GetDlgItem(IDC_EDIT_PARA28);
			s.Format(_T("%d"),G_PID[m_BoxNr].T );
			w->SetWindowTextW(s); 

			G_PID[m_BoxNr].COOL_P = Para_Answer->EEPROM_Answer.PID.Pid_P_Cool;
			w= GetDlgItem(IDC_EDIT_PARA_COOL_P_W);
			s.Format(_T("%d"),G_PID[m_BoxNr].COOL_P );
			w->SetWindowTextW(s); 

			G_PID[m_BoxNr].COOL_I = Para_Answer->EEPROM_Answer.PID.Pid_I_Cool;
			w= GetDlgItem(IDC_EDIT_PARA_COOL_I_W);
			s.Format(_T("%d"),G_PID[m_BoxNr].COOL_I );
			w->SetWindowTextW(s); 

			G_PID[m_BoxNr].COOL_D = Para_Answer->EEPROM_Answer.PID.Pid_D_Cool;
			w= GetDlgItem(IDC_EDIT_PARA_COOL_D_W);
			s.Format(_T("%d"),G_PID[m_BoxNr].COOL_D );
			w->SetWindowTextW(s); 

			

		}
		else
		{
			if(IsCmdAscii == false)
			{
				CWnd *w;
				
				
				
				if(G_iMaxLedNr[m_BoxNr] == 192)
				{
					memcpy(&G_PID[m_BoxNr],modbus_cmd_192->para,sizeof(G_PID[m_BoxNr]));
				}
				else
				{
					memcpy(&G_PID[m_BoxNr],modbus_cmd_128->para,sizeof(G_PID[m_BoxNr]));
				}
				/*if(G_PID[m_BoxNr].P == 0 || G_PID[m_BoxNr].I == 0 || G_PID[m_BoxNr].D == 0)
				{
					G_PID[m_BoxNr] = G_NormalConfigPara[m_BoxNr].pid_t;
					::G_RS485_Comm.SetPID_T((short *)&G_PID[m_BoxNr],m_BoxNr);
				}
				else*/
				{
					G_NormalConfigPara[m_BoxNr].pid_t = G_PID[m_BoxNr];
					::SaveNormalConfigPara();
				}
				
						w= GetDlgItem(IDC_EDIT_PARA25 + 0);
						s.Format(_T("%d"),G_PID[m_BoxNr].P );
						w->SetWindowTextW(s); 
						
						w= GetDlgItem(IDC_EDIT_PARA25 + 1);
						s.Format(_T("%d"),G_PID[m_BoxNr].I );
						w->SetWindowTextW(s); 
						
						w= GetDlgItem(IDC_EDIT_PARA25 + 2);
						s.Format(_T("%d"),G_PID[m_BoxNr].D );
						w->SetWindowTextW(s); 
						
						w= GetDlgItem(IDC_EDIT_PARA25 + 3);
						s.Format(_T("%d"),G_PID[m_BoxNr].T );
						w->SetWindowTextW(s); 
						
						w= GetDlgItem(IDC_EDIT_PARA_COOL_P_W);
						s.Format(_T("%d"),G_PID[m_BoxNr].COOL_P );
						w->SetWindowTextW(s); 
						
						w= GetDlgItem(IDC_EDIT_PARA_COOL_I_W);
						s.Format(_T("%d"),G_PID[m_BoxNr].COOL_I );
						w->SetWindowTextW(s); 
					
						w= GetDlgItem(IDC_EDIT_PARA_COOL_D_W);
						s.Format(_T("%d"),G_PID[m_BoxNr].COOL_D );
						w->SetWindowTextW(s); 
				
			}
			else
			{//ASCII
				for(i=0;i<4;i++)
				{
					short_a = (ascii_cmd->val[i*4 + 0] - '0')*1000
								+(ascii_cmd->val[i*4 + 1] - '0')*100
								+(ascii_cmd->val[i*4 + 2] - '0')*10
								+(ascii_cmd->val[i*4 + 3] - '0')*1;
					switch(i)
					{
					case 0:
					G_PID[m_BoxNr].P = short_a;
					break;
					case 1:
					G_PID[m_BoxNr].I = short_a;
					break;
					case 2:
					G_PID[m_BoxNr].D = short_a;
					break;
					case 3:
					G_PID[m_BoxNr].T = short_a;
					break;
					}
					CWnd *w= GetDlgItem(IDC_EDIT_PARA25 + i);
					s.Format(_T("%d"),short_a);
					w->SetWindowTextW(s); 
				}
			}
		}
		break;
	case CMD_READ_ADJUST_ALL:
		if(data_len == sizeof(MODBUS_RS485_CMD_PARA))
		{

			CWnd * w = GetDlgItem(IDC_EDIT_READ_ADJUST_ALL);
			s.Format(_T("%5.1f"),Para_Answer->EEPROM_Answer.AdjustAll); 
			w->SetWindowTextW(s); 
		}
		else
		{
			if(IsCmdAscii == false)
			{
					{
						short_a = 0;
						char *buf = (char*)&short_a; 
						if(G_iMaxLedNr[m_BoxNr] == 192)
						{
							buf[0]	=	(char) modbus_cmd_192->para[0];
							buf[1]  =	(char) modbus_cmd_192->para[1];
						}
						else
						{
							buf[0]	=	(char) modbus_cmd_128->para[0];
							buf[1]  =	(char) modbus_cmd_128->para[1];
						}
					
						CWnd * w = GetDlgItem(IDC_EDIT_READ_ADJUST_ALL);
						s.Format(_T("%5.1f"),short_a/10.0f); 
						w->SetWindowTextW(s); 
					}
			}
		}
		break;
		
	}
	this->UpdateData(false); 
}




HBRUSH CDebugDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
		switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		hbr = G_bkBrush;
		break;

	case CTLCOLOR_BTN:	//    Button control
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		break;
	case CTLCOLOR_EDIT:	//    Edit control
		//hbr = m_bkBrush;
		 
		break;
	case CTLCOLOR_LISTBOX:	//    List-box control
		break;
	case CTLCOLOR_MSGBOX:	//    Message box
		break;
	case CTLCOLOR_SCROLLBAR:	//    Scroll-bar control
		break;
	case CTLCOLOR_STATIC:	//    Static control
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		pDC->SetTextColor(RGB(255, 255, 60));
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDebugDlg::OnBnClickedCheckRelay()
{
	// TODO: 在此添加控件通知处理程序代码
#ifdef _HEATBOX_RESET_
	return;
#endif

	//OnButton_STOP_OUT();


	UpdateData(true);

	if(m_bCheckRelay)
	{
		//G_RS485_Comm.Send_NTC24_TH2512_Cmd(true,m_BoxNr);
		if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_TEMP2_10IN_12OUT)
		{
			static int door_id = 0;
			door_id++;
			if(door_id > 13)
			{
				door_id = 0;
			}
			G_RS485_Comm.SendOpenDoorCmd(door_id,m_BoxNr);  
		}
		else
		{
			G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr);  
		}
	}
	else
	{
		//G_RS485_Comm.Send_NTC24_TH2512_Cmd(false,m_BoxNr);
		G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr);
	}
	
	
}

void CDebugDlg::OnBnClickedCheckHighVolt()
{

	UpdateData(true);

	G_RS485_Comm.SendSetHighVolt(m_BoxNr,m_bCheckHighVolt);

	return;

	

}



/*
bool CDebugDlg::CheckJiaJuPass(unsigned char ID)
{

	

	if(G_iMaxLedNr[m_BoxNr] == 128)
	{
		if(ID < 16)
		{
			for(int i = 0; i < 64; i++)
			{
				if((i % 16) == ID)
				{
					if(G_SwitchTestResult[m_BoxNr][i].IsClosing == false)
					{
						if(i < 16)
						{
							if(G_SwitchTestResult[m_BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+32].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+48].IsClosing == true)
							{
								
								
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									
									::G_SpeechArray.Add (s);
								}
							}
						}
						else if(i < 32)
						{
							if(G_SwitchTestResult[m_BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+32].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						else if(i < 48)
						{
							if(G_SwitchTestResult[m_BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						else
						{// < 64
							if(G_SwitchTestResult[m_BoxNr][i-48].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i-16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						return false;
					}
				}
				else
				{
					if(G_SwitchTestResult[m_BoxNr][i].IsClosing == true)
					{//常亮
						//if(G_SpeechArray.GetSize() < 10)
						//{
						//	CString s;
						//	s.Format(_T("%d常亮"),i+1); 
						//	::G_SpeechArray.Add (s);
						//}
						return false;
					}
				}
			}
		}
		else
		{
			for(int i = 64; i < 128; i++)
			{
				if((i % 16) == (ID - 16))
				{
					if(G_SwitchTestResult[m_BoxNr][i].IsClosing == false)
					{
						if(i < 80)
						{
							if(G_SwitchTestResult[m_BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+32].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+48].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						else if(i < 96)
						{
							if(G_SwitchTestResult[m_BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+32].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						else if(i < 112)
						{
							if(G_SwitchTestResult[m_BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i-16].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i+16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						else
						{// < 64
							if(G_SwitchTestResult[m_BoxNr][i-48].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i-32].IsClosing == true
								&& G_SwitchTestResult[m_BoxNr][i-16].IsClosing == true)
							{
								G_ErrJiaJu.Add(i); 
								CString s;
								s.Format(_T("%d 错误"),i+1); 
								if(G_SpeechArray.GetSize() < 5)
								{
									::G_SpeechArray.Add (s);
								}
							}
						}
						return false;
					}
				}
				else
				{
					if(G_SwitchTestResult[m_BoxNr][i].IsClosing == true)
					{
						//if(G_SpeechArray.GetSize() < 10)
						//{
						//	CString s;
						//	s.Format(_T("%d常亮"),i+1); 
						//	::G_SpeechArray.Add (s);
						//}
						return false;
					}
				}
			}
		}
		m_JiaJuTested[ID] = 1;

		if(m_JiaJuTestedPass[0] == false)
		{
			bool pass = true;
			for(int i = 0; i < 16; i++)
			{
				if(m_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (m_JiaJuTestedPass[1] == false))
			{
				m_JiaJuTestedPass[0] = true;
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("1到64夹具测试成功!!!"));
				}
			}
		}
		if(m_JiaJuTestedPass[1] == false) 
		{
			bool pass = true;
			for(int i = 16; i < 32; i++)
			{
				if(m_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (m_JiaJuTestedPass[0] == false))
			{
				m_JiaJuTestedPass[1] = true;
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("65到128夹具测试成功!!!"));
				}
			}
		}

		for(int i = 0; i < 32; i++)
		{
			if(m_JiaJuTested[i] != 1)
			{
				return false;
			}
		}
#ifndef WINCE
		if(G_SpeechArray.GetSize() < 10)
		{
			::G_SpeechArray.Add (_T("1到128夹具测试成功!!!"));
		}
#endif
		if(G_NormalConfigPara[m_BoxNr].bSpeech == false)
		{
			AfxMessageBox(_T("1到128夹具测试成功!!!"));
		}
		m_bJiajuTesting = false;
		this->UpdateData(false);
	}
	else
	{
		

			if(G_SwitchTestResult[m_BoxNr][group[ID]].IsClosing == false)
			{
				return false;
			}
			if(G_SwitchTestResult[m_BoxNr][group[ID]+8].IsClosing == false)
			{
				return false;
			}
			if(G_SwitchTestResult[m_BoxNr][group[ID]+96].IsClosing == false)
			{
				return false;
			}
			if(G_SwitchTestResult[m_BoxNr][group[ID]+104].IsClosing == false)
			{
				return false;
			}
		
		int all_closed = 0;
		for(int i = 0; i <G_iMaxLedNr[m_BoxNr][m_BoxNr]; i++)
		{
			if(G_SwitchTestResult[m_BoxNr][i].IsClosing == true)
			{
				all_closed++;
			}
		}
		if(all_closed > 4)
		{
			CString s;
			s.Format(_T("%d, %d, %d, %d错误"),group[ID],group[ID]+8,group[ID]+96,group[ID]+104); 
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add (s);
			}
			return false;
		}
		
		m_JiaJuTested[ID] = 1;

		if(m_JiaJuTestedPass[0] == false)
		{
			bool pass = true;
			for(int i = 0; i < 16; i++)
			{
				if(m_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (m_JiaJuTestedPass[1] == false || m_JiaJuTestedPass[2] == false))
			{
				m_JiaJuTestedPass[0] = true;
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("1到64夹具测试成功!!!"));
				}
			}
		}
		if(m_JiaJuTestedPass[1] == false) 
		{
			bool pass = true;
			for(int i = 16; i < 32; i++)
			{
				if(m_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (m_JiaJuTestedPass[0] == false ||m_JiaJuTestedPass[2] == false) )
			{
				m_JiaJuTestedPass[1] = true;
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("65到128夹具测试成功!!!"));
				}
			}
		}

		if(m_JiaJuTestedPass[2] == false) 
		{
			bool pass = true;
			for(int i = 32; i < 48; i++)
			{
				if(m_JiaJuTested[i] != 1)
				{
					pass = false;
					break;
				}
			}
			if(pass && (m_JiaJuTestedPass[0] == false || m_JiaJuTestedPass[1] == false))
			{
				m_JiaJuTestedPass[2] = true;
				if(G_SpeechArray.GetSize() < 10)
				{
					::G_SpeechArray.Add (_T("129到196夹具测试成功!!!"));
				}
			}
		}
		if(::G_iMaxLedNr[m_BoxNr] == 128)
		{
			for(int i = 0; i < 32; i++)
			{
				if(m_JiaJuTested[i] != 1)
				{
					return false;
				}
			}
#ifndef WINCE
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add (_T("1到128夹具测试成功!!!"));
			}
#endif
			if(G_NormalConfigPara[m_BoxNr].bSpeech == false)
			{
				AfxMessageBox(_T("1到128夹具测试成功!!!"));
			}
			m_bJiajuTesting = false;
		}
		else
		{
			for(int i = 0; i < 48; i++)
			{
				if(m_JiaJuTested[i] != 1)
				{
					return false;
				}
			}
#ifndef WINCE
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add (_T("1到192夹具测试成功!!!"));
			}
#endif
			if(G_NormalConfigPara[m_BoxNr].bSpeech == false)
			{
				AfxMessageBox(_T("1到192夹具测试成功!!!"));
			}
			m_bJiajuTesting = false;
		}
		this->UpdateData(false);
	}
	return true;
}
*/

void CDebugDlg::DrawLED_JiaJu(void)
{
	
	if(G_CurMaxHeatBox > 1)
	{
		return;
	}

	if(!this->IsWindowVisible()) 
	{
		return;
	}


	CWnd *data_area = GetDlgItem(IDC_EDIT_PARA4);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);
		CRect LedRect;

	LedRect.left	=	wndRect.left + wndRect.Width() / 2; 
	LedRect.right	=	wndRect.right;
	LedRect.top		=	wndRect.top;
	LedRect.bottom  =	wndRect.bottom; 
	 
	::DrawLED_JiaJu(this,LedRect,m_BoxNr);

	m_bJiajuTesting = G_bJiajuTesting; 

}
void CDebugDlg::DrawLED(void)
{

	
	if(G_CurMaxHeatBox > 1)
	{
		return;
	}

	if(m_bJiajuTesting)
	{
		DrawLED_JiaJu();
		return;
	}

	if(!this->IsWindowVisible()) 
	{
		return;
	}



	
	CWnd *data_area = GetDlgItem(IDC_EDIT_PARA4);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);
	
	CRect LedRect;
	LedRect.left	= wndRect.left + wndRect.Width()/3;
	LedRect.right	= wndRect.right;
	LedRect.top		= wndRect.top;
	LedRect.bottom	= wndRect.bottom;

	if(G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_40 || G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_24)
	{
		//G_MainDlg->m_SwitchDlg[m_BoxNr].m_HeatCoolOrder		= CheckHeatOrCoolMode(G_MainDlg->m_SwitchDlg[m_BoxNr].m_SwitchConfigPara.para, &G_MainDlg->m_SwitchDlg[m_BoxNr].m_TempRange,::G_NormalConfigPara[m_BoxNr].BoxType);
		DrawLED_BiMetal(this,this->m_BoxNr,LedRect,&G_MainDlg->m_SwitchDlg[m_BoxNr].m_SwitchConfigPara.para,m_LED_Rect,G_NormalConfigPara[m_BoxNr].BoxType);
	}
	else
	{
		::DrawLED(this, LedRect,m_BoxNr,1);
	}
	return;

}


void CDebugDlg::OnEnSetfocusEditPara4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSetFocusOnPara4 = true;

}

CString EditMsg[] =
{
	_T("请输入需要升温的最高温度值"),	//PARA5
	_T(""),
	_T(""),
	_T(""),

	_T("请输入需要恒温的温度值"),		//para9
	_T("请输入需要恒温的时间(秒),输入值为0时，表示一直恒温"),
	_T(""),
	_T(""),

	_T("请输入需要匀速升温的最高温度值"),	//PARA 13
	_T("请输入需要匀速升温的时间值(秒)"),
	_T(""),
	_T(""),

	_T(""),//PARA 17
	_T(""),
	_T(""),
	_T(""),

	_T(""),//PARA 21
	_T(""),
	_T(""),
	_T(""),

	_T("请输入PID参数的P值"),			//PARA 25
	_T("请输入PID参数的I值"),
	_T("请输入PID参数的D值"),
	_T("请输入PID参数的 T 值"),

	_T("0度 1#传感器的修正值"),		//PARA 29
	_T("0度 2#传感器的修正值"),
	_T("0度 3#传感器的修正值"),
	_T("0度 4#传感器的修正值"),


	_T("200度 1#传感器的修正值"),	//PARA 33
	_T("200度 2#传感器的修正值"),
	_T("200度 3#传感器的修正值"),
	_T("200度 4#传感器的修正值"),

	_T("") ,	//PARA 37
	_T("") ,
	_T("") ,
	_T("") ,

	_T("") ,	//PARA 41
	_T("") ,
	_T("") ,
	_T("") ,

	_T("输入低温时 1# 传感器的仪表温度") , //PARA 45
	_T("输入低温时 2# 传感器的仪表温度") , //PARA 46
	_T("输入低温时 3# 传感器的仪表温度") , //PARA 47
	_T("输入低温时 4# 传感器的仪表温度") , //PARA 48

	_T("输入高温时 1# 传感器的仪表温度") , //PARA 49
	_T("输入高温时 2# 传感器的仪表温度") , //PARA 50
	_T("输入高温时 3# 传感器的仪表温度") , //PARA 51
	_T("输入高温时 4# 传感器的仪表温度") , //PARA 52


	_T("输入低温时 1# 水银温度计的温度") , //PARA 45
	_T("输入低温时 2# 水银温度计的温度") , //PARA 46
	_T("输入低温时 3# 水银温度计的温度") , //PARA 47
	_T("输入低温时 4# 水银温度计的温度") , //PARA 48


	_T("输入高温时 1# 水银温度计的温度") , //PARA 45
	_T("输入高温时 2# 水银温度计的温度") , //PARA 46
	_T("输入高温时 3# 水银温度计的温度") , //PARA 47
	_T("输入高温时 4# 水银温度计的温度") , //PARA 48

	_T("输入自整定温度") ,					//PARA 48

};
void CDebugDlg::OnEnSetfocusEditPara(UINT id)
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bSetFocusOnPara4 == false)
	{
		return;
	}
	m_bSetFocusOnPara4 = false;
	CWnd *w = this->GetDlgItem(IDC_EDIT_PARA4); 
	CString s1,s2,s3,s4,s5,s6,s7;

	
	CInputDlg dlg;
	if(id == IDC_EDIT_PARA_COOL_P_W)
	{
		dlg.m_Title = _T("请输入制冷PID参数的P值");
	}
	else if(id == IDC_EDIT_PARA_COOL_I_W)
	{
		dlg.m_Title = _T("请输入制冷PID参数的I值");
	}
	else if(id == IDC_EDIT_PARA_COOL_D_W)
	{
		dlg.m_Title = _T("请输入制冷PID参数的D值");
	}
	else if(id == IDC_EDIT_PARA_PID_Nr)
	{
		dlg.m_Title = _T("请输入PID的编号");
	}
	else
	{
		dlg.m_Title = EditMsg[id - IDC_EDIT_PARA5] ;
	}
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(id) ;
		w->SetWindowTextW(dlg.m_Input);
		switch(id)
		{
		case IDC_EDIT_PARA13:
		case IDC_EDIT_PARA14:
			OnButton4();
					break;
		case IDC_EDIT_PARA5:
				OnButton2();
				break;
		case IDC_EDIT_PARA9:
				OnButton_PID();
				break;
		case IDC_EDIT_PARA15:
		case IDC_EDIT_PARA16:
				OnBnClickedButton14();
					break;
		case IDC_EDIT_PARA29:
		case IDC_EDIT_PARA30:
		case IDC_EDIT_PARA31:
		case IDC_EDIT_PARA32:
			w = this->GetDlgItem(IDC_EDIT_PARA29) ;
			w->GetWindowTextW(s1); 
			w = this->GetDlgItem(IDC_EDIT_PARA30) ;
			w->GetWindowTextW(s2);
			w = this->GetDlgItem(IDC_EDIT_PARA31) ;
			w->GetWindowTextW(s3);
			w = this->GetDlgItem(IDC_EDIT_PARA32) ;
			w->GetWindowTextW(s4);
			if((s1.GetLength() > 0) && (s2.GetLength() > 0) && (s3.GetLength() > 0) && (s4.GetLength() > 0))
			{
				this->OnButton_SetAdjustLow(); 
			}
			break;
		case IDC_EDIT_PARA33:
		case IDC_EDIT_PARA34:
		case IDC_EDIT_PARA35:
		case IDC_EDIT_PARA36:
			w = this->GetDlgItem(IDC_EDIT_PARA33) ;
			w->GetWindowTextW(s1); 
			w = this->GetDlgItem(IDC_EDIT_PARA34) ;
			w->GetWindowTextW(s2);
			w = this->GetDlgItem(IDC_EDIT_PARA35) ;
			w->GetWindowTextW(s3);
			w = this->GetDlgItem(IDC_EDIT_PARA36) ;
			w->GetWindowTextW(s4);
			if((s1.GetLength() > 0) && (s2.GetLength() > 0) && (s3.GetLength() > 0) && (s4.GetLength() > 0))
			{
				this->OnButton_SetAdjustHigh(); 
			}
			break;
		
		case IDC_EDIT_PARA25:
		case IDC_EDIT_PARA26:
		case IDC_EDIT_PARA27:
		case IDC_EDIT_PARA28:
		case IDC_EDIT_PARA_COOL_P_W:
		case IDC_EDIT_PARA_COOL_I_W:
		case IDC_EDIT_PARA_COOL_D_W:

			w = this->GetDlgItem(IDC_EDIT_PARA25) ;
			w->GetWindowTextW(s1); 
			w = this->GetDlgItem(IDC_EDIT_PARA26) ;
			w->GetWindowTextW(s2);
			w = this->GetDlgItem(IDC_EDIT_PARA27) ;
			w->GetWindowTextW(s3);
			w = this->GetDlgItem(IDC_EDIT_PARA28) ;
			w->GetWindowTextW(s4);
			w = this->GetDlgItem(IDC_EDIT_PARA_COOL_P_W) ;
			w->GetWindowTextW(s5);
			w = this->GetDlgItem(IDC_EDIT_PARA_COOL_I_W) ;
			w->GetWindowTextW(s6);
			w = this->GetDlgItem(IDC_EDIT_PARA_COOL_D_W) ;
			w->GetWindowTextW(s7);
			

			if((s1.GetLength() > 0) && (s2.GetLength() > 0) && (s3.GetLength() > 0) && (s4.GetLength() > 0)
				&& (s5.GetLength() > 0) && (s6.GetLength() > 0) && (s7.GetLength() > 0))
			{
				OnButtonSetPID();
			}	
			
			break;
		case IDC_EDIT_PARA_PID_Nr:
			{
				TCHAR buf[20];
				CEdit * edit = (CEdit * ) this->GetDlgItem(IDC_EDIT_PARA_PID_Nr) ;
				edit->GetWindowText(buf,10); 
				int Nr = _tcstol(buf,NULL,10);
				G_RS485_Comm.ReadPidT(m_BoxNr,Nr);
			}
			break;
		}
	}
	

	w->SetFocus(); 
}

void CDebugDlg::OnEnSetfocusEditSetAdjustAll()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bSetFocusOnPara4 == false)
	{
		return;
	}
	m_bSetFocusOnPara4 = false;
	CWnd *w = this->GetDlgItem(IDC_EDIT_PARA4); 
	

	
	CInputDlg dlg;
	dlg.m_Title = _T("设置温度总调");
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(IDC_EDIT_SET_ADJUST_ALL) ;
		w->SetWindowTextW(dlg.m_Input);
		OnButtonSetAdjustAll();
	}
	

	w->SetFocus(); 
}

void CDebugDlg::OnBnClickedCheckCoolRelay()
{
	// TODO: 在此添加控件通知处理程序代码
	
	UpdateData(true);

	CString msg;
	if(m_bCheckCoolRelay)
	{
		if(G_RS485_Comm.SendAirControlCmd(true,::G_MainDlg->m_SwitchDlg[m_BoxNr].m_Meter.m_IsAcOn,m_BoxNr,&msg) == -2)
		{
			AfxMessageBox(msg);
		}
	}
	else
	{
		if(G_RS485_Comm.SendAirControlCmd(false,G_MainDlg->m_SwitchDlg[m_BoxNr].m_Meter.m_IsAcOn,m_BoxNr,&msg) == -2)
		{
			AfxMessageBox(msg);
		}
	}
	
}

void CDebugDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
		
		CEdit *edit;
		
		TCHAR buf[10];
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA15);
		edit->GetWindowText(buf,10); 
		double temp = _tcstod(buf,NULL);
		
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA16);
		edit->GetWindowText(buf,10); 
		int minutes = _tcstod(buf,NULL);
		if(minutes > 0)
		{
			G_RS485_Comm.SendStepPidCoolCmd(temp,minutes,m_BoxNr,true);
		}

}

void CDebugDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	UpdateData(false);
}

void CDebugDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	
}

void CDebugDlg::OnBnClickedButtonSetAddr()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString s;
	s.Format( _T("只能设置第 %d 号控制箱的编号\r\n请关闭其他控制箱的电源！！！！！！"),this->m_BoxNr + 1);
	
	if(AfxMessageBox(s,MB_OKCANCEL) != IDOK)
	{
		return;
	}
	m_CmdArray_192[m_BoxNr].RemoveAll();
	m_CmdArray_128[m_BoxNr].RemoveAll();
	G_RS485_Comm.SetAddress(m_BoxNr);
		
	
}
void CDebugDlg::OnBnClickedButtonPrinterTest()
{
	CPM24_Printer::PrintTestPaper();
}
void CDebugDlg::OnBnClickedButtonUpdateFirmware()
{
	// TODO: 在此添加控件通知处理程序代码


	CString s;
	s.Format( _T("只能更新第 %d 号控制箱\r\n请关闭其他控制箱的电源！！！！！！"),this->m_BoxNr + 1);
	if(G_SpeechArray.GetSize() < 10)
	{
		::G_SpeechArray.Add (s);
	}
	CProgramDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
	
		if(MessageBoxW(s,NULL,MB_OKCANCEL) == IDOK)
		{
		//	G_RS485_Comm.XModem_Start(this->m_BoxNr,::G_NormalConfigPara[m_BoxNr].InterfaceBoardType,G_NormalConfigPara[m_BoxNr].TM7705_NoOSC); 

			int BoxNr = dlg.m_BoxNr;
			G_RS485_Comm.XModem_Start(BoxNr,
				dlg.m_InterfaceBoardType,
				dlg.m_TM7705_OSC); 

			G_ProgramBoxNr = BoxNr;
		}
	}
		
}

void CDebugDlg::OnBnClickedButtonAt()
{
	// TODO: 在此添加控件通知处理程序代码

	CInputDlg dlg;

	dlg.m_Title = _T("输入自整定的温度值");
	if(dlg.DoModal() == IDOK)
	{
		double temp = _tcstod(dlg.m_Input,NULL);
		G_RS485_Comm.SendAT(temp,m_BoxNr);
	}
	
}

void CDebugDlg::OnBnClickedCheckDeltaFrequnce()
{
	// TODO: 在此添加控件通知处理程序代码
	
/*
	
	
	DELTA_FREQUNCE_CMD_HEX cmd_hex;

	UpdateData(true);

	cmd_hex.RS485Addr	= 01;
	cmd_hex.cmd			= 0x06;
	cmd_hex.para_addr	= 0x2000;	
	if(m_bDeltaFrequnce)
	{
		cmd_hex.para = 0x0002;
	}
	else
	{
		cmd_hex.para = 0x0001; 
	}
	
	SetCmdString_DeltaFrequence(m_BoxNr,cmd_hex);
	*/
}

void CDebugDlg::OnEnSetfocusEditSetPt100Count()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bSetFocusOnPara4 == false)
	{
		return;
	}
	m_bSetFocusOnPara4 = false;
	CWnd *w = this->GetDlgItem(IDC_EDIT_PARA4); 
	

	
	CInputDlg dlg;
	dlg.m_Title = _T("设置传感器数量");
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(IDC_EDIT_SET_PT100_COUNT) ;
		w->SetWindowTextW(dlg.m_Input);
		OnBnClickedButtonSetAPT100Count();
	}
	

	w->SetFocus(); 
}

void CDebugDlg::OnBnClickedButtonSetAPT100Count()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString s;
	CWnd *w;
	w = this->GetDlgItem(IDC_EDIT_SET_PT100_COUNT);
	w->GetWindowText(s);
	int val = (short)(_tcstod(s,NULL));
	
	G_RS485_Comm.SendSetUsefulSensors(val,m_BoxNr);

	G_RS485_Comm.ReadUsedSensor(m_BoxNr);


}


void CDebugDlg::OnBnClickedCheckJiajuTest()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(true);

	if(G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_24 || G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_24)
	{
		memset(&G_Noise,0,sizeof(G_Noise)); 
		if(::G_NormalConfigPara[0].SoundOutputType == 0)
		{
			G_NormalConfigPara[0].SoundOutputType = 1;
		}
		else
		{
			G_NormalConfigPara[0].SoundOutputType = 0;
		}
	}
	else
	{
		G_ErrJiaJu.RemoveAll();

		G_JiaJuTestedPass[0] = false;
		G_JiaJuTestedPass[1] = false;
		G_JiaJuTestedPass[2] = false;

		memset(G_JiaJuTested,0,sizeof(G_JiaJuTested));
		
		G_bJiajuTesting = true;
	}
}



void CDebugDlg::OnCbnSelendokComboInterfaceBoardType()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->m_ComboInterfaceBoardType.GetCurSel() >= 0)
	{

	
	
		/*if(text.Compare(_T("无接口板")) == 0)
		{
			::G_NormalConfigPara[m_BoxNr].BoxType  =(BOX_TYPE) 0xFF;
		}
		else*/
		{
			::G_NormalConfigPara[m_BoxNr].InterfaceBoardType = (BOX_TYPE)m_ComboInterfaceBoardType.GetCurSel();
		}
		::SaveNormalConfigPara(); 
	
		ReadNormalConfigPara();

		G_RS485_Comm.SendPcbBoardType(m_BoxNr);
	}
}

void CDebugDlg::OnBnClickedButtonReadBimetallicStrip()
{
	// TODO: 在此添加控件通知处理程序代码

	MODBUS_RS485_CMD_128 * cmd =(MODBUS_RS485_CMD_128*)G_RS485_Comm.SendReadBimetalStatus(m_BoxNr,::G_NormalConfigPara[0].SoundLevel_Heat );

	::G_MainDlg->DisplaySendCmd(cmd->head.cmd,(unsigned char *)cmd,sizeof(MODBUS_RS485_CMD_128),this->m_BoxNr);
}

void CDebugDlg::OnEnChangeEditPara9()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CDebugDlg::OnCbnSelchangeComboTm7705Osc()
{
	// TODO: 在此添加控件通知处理程序代码
	


}

void CDebugDlg::OnCbnSelendokComboTm7705Osc()
{
	// TODO: 在此添加控件通知处理程序代码
	
	
		::G_NormalConfigPara[m_BoxNr].TM7705_NoOSC  = (BOX_TYPE)m_ComboTM7705_OSC.GetCurSel();
		
		::SaveNormalConfigPara(); 
	
		ReadNormalConfigPara();
//		G_RS485_Comm.SetConfigPara(G_NormalConfigPara);
		G_RS485_Comm.SendTM7705_OSC(m_BoxNr);
}

void CDebugDlg::OnBnClickedButtonUpdateBootloader()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDebugDlg::OnBnClickedCheck192InterfacePcbType()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(true);

	G_bCMD_192_READ_STATUS = true;

	if(m_bCheck_192_Interface_PcbType)
	{
		G_RS485_Comm.SendSet192InterfacePcbType(OLD_192_INTERFACE_PCB,m_BoxNr);
	}
	else
	{
		G_RS485_Comm.SendSet192InterfacePcbType(NEW_192_INTERFACE_PCB,m_BoxNr);
	}
}

void CDebugDlg::OnBnClickedCheckBeep()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(true);

	CString msg;
	if(m_bCheckBeep)
	{
		G_RS485_Comm.SendBeepCmd(true,m_BoxNr);
	}
	else
	{
		G_RS485_Comm.SendBeepCmd(false,m_BoxNr);
	}
}

void CDebugDlg::OnBnClickedCheckFan()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(true);

	G_RS485_Comm.SendSetFan(m_bCheckFan,m_BoxNr);

	return;

}

void CDebugDlg::OnBnClickedButtonClearSound()
{
	// TODO: 在此添加控件通知处理程序代码
	memset(&G_Noise,0,sizeof(G_Noise));
}

void CDebugDlg::OnStnClickedStaticTest()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void CDebugDlg::OnEnSetfocusEditPara13()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDebugDlg::OnEnSetfocusEditPara14()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDebugDlg::OnBnClickedButtonCpuId()
{
	// TODO: 在此添加控件通知处理程序代码
	//	G_RS485_Comm.SendGetStm32ID(m_BoxNr);


	G_MainDlg->m_LPM260_Printer.PrintTestPaper();
}

void CDebugDlg::OnCbnSelchangeComboInterfaceBoardType()
{
	// TODO: 在此添加控件通知处理程序代码
}




void CDebugDlg::OnBnClickedButtonNtcRelayLed()
{
	// TODO: 在此添加控件通知处理程序代码
	static char id = 0;
	unsigned char pass[3];
	pass[0] = 0;
	pass[1] = 0;
	pass[2] = 0;
	pass[id/8] = (1 << (id % 8));
	
	G_RS485_Comm.Send_NTC_TH2512_Relay(id,0,pass);

	id++;
	if(id > 23)
	{
		id = 0;
	}
}



