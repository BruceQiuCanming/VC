// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "DebugDlg.h"
#include "HeatBoxDlg.h"
#include "comm.h"
#include "RecordViewDlg.h"
#include "CopyRight.h"
//#include "TempAdjRst.h"
#include "InputGlassTempDlg.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int count = 0;


/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog


CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
	, m_bCheckCoolRelay(FALSE)
	, m_CommRecString(_T(""))
	, m_CommSendString(_T(""))
	, m_bCheckFan(FALSE)
{
	m_CalStep = NOT_CAL;
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
	DDX_Text(pDX, IDC_EDIT_COMM_REC, m_CommRecString);
	DDX_Text(pDX, IDC_EDIT_COMM_SEND, m_CommSendString);
	DDX_Check(pDX, IDC_CHECK_COOL_RELAY2, m_bCheckFan);
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	//{{AFX_MSG_MAP(CDebugDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton_STOP_OUT)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton_ReadAdjustLow)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton_ReadAdjustHigh)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton_Cal_Adjust)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton_PID)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton_SetAdjustLow)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton_SetAdjustHigh)
	ON_BN_CLICKED(IDC_BUTTON_BOARD_ADJUST, OnButtonBoardAdjust)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON_SYNCHRONIZATION_BEGIN, OnButtonSynchronizationBegin)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_READ_SYNCHRONIZATION, OnButtonReadSynchronization)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_VIEW, OnButtonRecordView)
	ON_BN_CLICKED(IDC_BUTTON_COPYRIGHT, OnButtonCopyright)
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
	ON_CONTROL_RANGE(EN_SETFOCUS,IDC_EDIT_PARA5,IDC_EDIT_PARA60,OnEnSetfocusEditPara)

	ON_EN_SETFOCUS(IDC_EDIT_SET_ADJUST_ALL, &CDebugDlg::OnEnSetfocusEditSetAdjustAll)
	ON_BN_CLICKED(IDC_CHECK_COOL_RELAY, &CDebugDlg::OnBnClickedCheckCoolRelay)
	ON_BN_CLICKED(IDC_BUTTON14, &CDebugDlg::OnBnClickedButton14)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_COOL_RELAY2, &CDebugDlg::OnBnClickedCheckCoolRelay2)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE, &CDebugDlg::OnBnClickedButtonUpdateFirmware)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE2, &CDebugDlg::OnBnClickedButtonUpdateFirmware2)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE3, &CDebugDlg::OnBnClickedButtonUpdateFirmware3)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE4, &CDebugDlg::OnBnClickedButtonUpdateFirmware4)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE5, &CDebugDlg::OnBnClickedButtonUpdateFirmware5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg message handlers

void CDebugDlg::OnButton_STOP_OUT() 
{
	// TODO: Add your control notification handler code here
#ifdef WT_504
		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		float temp = 0;
		char para[4]={0,0,0,0};
		para[0] = SP_ADDR / 256;
		para[1] = SP_ADDR % 256;
		para[2] = (temp *10) / 256;
		para[3] = ((int)(temp *10)) % 256;
		
		maindlg->SetCmdString( m_BoxNr + 1,CMD_WRITE, para);
		maindlg->m_SendCmdArray.Add(cmdStr);
#else
		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		float temp = 0;
		char para[8]={0,0,0,0,0,0,0,0};
		
		SHORT_BUF short_buf;
		short_buf.v = (short)(temp *10);
		para[0] = short_buf.buf[0];
		para[1] = short_buf.buf[1];
		
		maindlg->SetCmdString(m_BoxNr + '1',CMD_STOP_OUT, para);

#endif
}

void CDebugDlg::OnButton_ReadAdjustLow() 
{
	// TODO: Add your control notification handler code here
#ifndef WT_504
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	char para[100]={0,0,0,0,0,0,0,0};

	maindlg->SetCmdString( m_BoxNr + '1',CMD_READ_ADJUST_LOW, para);

#endif
}

void CDebugDlg::OnButton_ReadAdjustHigh() 
{
	// TODO: Add your control notification handler code here
#ifndef WT_504
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	char para[100]={0,0,0,0,0,0,0,0};
	
	maindlg->SetCmdString( m_BoxNr + '1',CMD_READ_ADJUST_HIGH, para);

#endif
}

void CDebugDlg::OnButton12() 
{
	// TODO: Add your control notification handler code here
	CWnd *w;
	w = this->GetDlgItem(IDC_EDIT_PARA29);
	w->SetWindowText(_T(""));
	w = this->GetDlgItem(IDC_EDIT_PARA30);
	w->SetWindowText(_T(""));
	w = this->GetDlgItem(IDC_EDIT_PARA31);
	w->SetWindowText(_T(""));
	w = this->GetDlgItem(IDC_EDIT_PARA32);
	w->SetWindowText(_T(""));
		
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
	double glass_low[4],glass_high[4];
	double glass_low_avr,glass_high_avr;
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
	glass_low_avr=(glass_low[0]+glass_low[1]+glass_low[2]+glass_low[3])/4;
	glass_high_avr=(glass_high[0]+glass_high[1]+glass_high[2]+glass_high[3])/4;

	for(i=0;i<4;i++)
	{
		// y = kx + delta
		double delta_low = (glass_low_avr-v_low[i]);
		double delta_high = (glass_high_avr-v_high[i]);
		
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

unsigned short CDebugDlg::PcShort2OmronShort(unsigned short v)
{
	SHORT_BUF pc;
	SHORT_BUF omron;

	pc.v = v;
	omron.buf[0] = pc.buf[1];
	omron.buf[1] = pc.buf[0];

	return  omron.v; 
}
unsigned short CDebugDlg::OmronShort2PcShort(unsigned short v)
{
	
	SHORT_BUF pc;
	SHORT_BUF omron;
	omron.v = v;
	
	pc.buf[0] = omron.buf[1];
	pc.buf[1] = omron.buf[0];

	return  pc.v; 

}

void CDebugDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here

		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		CEdit *edit;
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA5);

		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		double temp = _tcstod(buf,NULL);
		
		if(temp >= 999)
		{
			maindlg->XModem_Start(this->m_BoxNr); 
			return;
		}

		char para[100];
		memset(para,0,sizeof(para));

		*((short * )&para[0]) = PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = PcShort2OmronShort((short)(temp *10));
		
		maindlg->SetCmdString_OMRON( m_BoxNr + 1, CMD_OMRON_WRITE, para);

}

void CDebugDlg::OnButton_PID() 
{
	// TODO: Add your control notification handler code here
		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		CEdit *edit;
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA9);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		double temp = _tcstod(buf,NULL);

		char para[100];
		memset(para,0,sizeof(para));

		SHORT_BUF short_buf;
		short_buf.v = (short)(temp *10);
		
		*((short * )&para[0]) = PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = PcShort2OmronShort((short)(temp *10));

	
		maindlg->SetCmdString( m_BoxNr + 1,CMD_OMRON_WRITE, para);


		m_StartPid = 0;
}

void CDebugDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		CEdit *edit;
		
		TCHAR buf[10];
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA13);
		edit->GetWindowText(buf,10); 
		double temp = _tcstod(buf,NULL);
		char para[100]={0,0,0,0,0,0,0,0};

		SHORT_BUF short_buf;
		short_buf.v = (short)(temp *10);
		para[0] = short_buf.buf[0];
		para[1] = short_buf.buf[1];
		
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA14);
		edit->GetWindowText(buf,10); 
		temp = _tcstod(buf,NULL);
		
		short_buf.v = (short)temp;
		para[2] = short_buf.buf[0];
		para[3] = short_buf.buf[1];

		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA61);
		edit->GetWindowText(buf,10); 
		temp = _tcstod(buf,NULL);
		
		short_buf.v = (short)temp;
		para[4] = short_buf.buf[0];
		para[5] = short_buf.buf[1];

		CString cmdStr;
		maindlg->SetCmdString( m_BoxNr + '1',CMD_STEP_PID_OUT, para);



}

void CDebugDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	OnButtonAll(4);
}

void CDebugDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		
	char para[100];
	
	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_P);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	maindlg->SetCmdString_OMRON( m_BoxNr + 1 ,CMD_OMRON_READ, para);

	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_I);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	maindlg->SetCmdString_OMRON( m_BoxNr + 1 ,CMD_OMRON_READ, para);

	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_D);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	maindlg->SetCmdString_OMRON( m_BoxNr + 1 ,CMD_OMRON_READ, para);

	memset(para,0,sizeof(para));

	*((short * )&para[0]) = PcShort2OmronShort(OMRON_PID_T);

	*((short * )&para[2]) = PcShort2OmronShort((short)(1));

	maindlg->SetCmdString_OMRON( m_BoxNr + 1 ,CMD_OMRON_READ, para);


}

void CDebugDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
#ifndef WT_504
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	char para[100]={0,0,0,0,0,0,0,0};
	CEdit *edit;
	for(int i = 0; i < 4; i++)
	{
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA25+i);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		short pid_t = _ttoi(buf);
		((short*)para)[i] = pid_t;
		if(pid_t <= 0)
		{
			AfxMessageBox(_T("PID  值不能为0"));
			return;
		}
	}
	
	maindlg->SetCmdString( m_BoxNr + '1',CMD_SET_PID_T, para);


#endif
}

void CDebugDlg::OnButton_SetAdjustLow() 
{
	// TODO: Add your control notification handler code here
#ifndef WT_504
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	char para[100]={0,0,0,0,0,0,0,0};
	CEdit *edit;
	for(int i = 0; i < 4; i++)
	{
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA29+i);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		short pid_t = (short)(_tcstod(buf,NULL) *10.0f);
		((short*)para)[i] = pid_t;
	}
	
	maindlg->SetCmdString( m_BoxNr + '1',CMD_SET_ADJUST_LOW, para);


#endif

	OnButton_ReadAdjustLow();
}

void CDebugDlg::OnButton_SetAdjustHigh() 
{
	// TODO: Add your control notification handler code here
#ifndef WT_504
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	char para[100]={0,0,0,0,0,0,0,0};
	CEdit *edit;
	for(int i = 0; i < 4; i++)
	{
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA33+i);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		short pid_t = (short)(_tcstod(buf,NULL) *10.0f);
		((short*)para)[i] = pid_t;
	}
	
	maindlg->SetCmdString( m_BoxNr + '1',CMD_SET_ADJUST_HIGH, para);


#endif

	OnButton_ReadAdjustHigh();
}

void CDebugDlg::OnButtonAll(int index) 
{

}



void CDebugDlg::OnButtonBoardAdjust() 
{
	


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

void CDebugDlg::OnButton20() 
{
	

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
	// TODO: Add your control notification handler code here
//	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());

//	G_CopyRight.ReadSN(this->m_BoxNr,&maindlg->m_SendCmdArray[m_BoxNr]);  
//	G_CopyRight.ReadAuthor(this->m_BoxNr,&maindlg->m_SendCmdArray[m_BoxNr]);   
//	G_CopyRight.ReadProductDate(this->m_BoxNr,&maindlg->m_SendCmdArray[m_BoxNr]);
//	G_CopyRight.ReadBuildDate(this->m_BoxNr,&maindlg->m_SendCmdArray[m_BoxNr]);

}


void CDebugDlg::OnButtonHighVolt() 
{
}

void CDebugDlg::OnRadioLedStatus() 
{

	// TODO: Add your control notification handler code here
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	char	para[16];



	{
		memset(para,0,sizeof(para));
		para[0] = 0;  
		
		maindlg->SetCmdString( SWITCH_STATUS_ADDR + m_BoxNr,CMD_SET_DISPLAY_TYPE, para);

	}
}

void CDebugDlg::OnRadioLedAllOn() 
{

	// TODO: Add your control notification handler code here
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	char	para[16];

	//if(G_IsCmdAscii[m_BoxNr] == false)
	{
		memset(para,0,sizeof(para));
		para[0] = 1;  
		
		maindlg->SetCmdString(SWITCH_STATUS_ADDR + m_BoxNr,CMD_SET_DISPLAY_TYPE, para);

	}

}

void CDebugDlg::OnRadioLedAllOff() 
{
	// TODO: Add your control notification handler code here
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());

	char	para[16];


	//if(G_IsCmdAscii[m_BoxNr] == false)
	{
		memset(para,0,sizeof(para));
		para[0] = 2;  
		
		maindlg->SetCmdString(SWITCH_STATUS_ADDR + m_BoxNr,CMD_SET_DISPLAY_TYPE, para);


	}

}


void CDebugDlg::OnButtonAdjustWizard() 
{
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
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDebugDlg::OnButtonSetAdjustAll() 
{
	
}

void CDebugDlg::OnButtonReadAdjustAll() 
{

	// TODO: Add your control notification handler code here
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());

	char	para[16];



	//if(G_IsCmdAscii[m_BoxNr] == false)
	{
		memset(para,0,sizeof(para));
		

		maindlg->SetCmdString( '1' + m_BoxNr,CMD_READ_ADJUST_ALL, para);


	}
}

void CDebugDlg::OnCheckReset() 
{

	// TODO: Add your control notification handler code here
	this->UpdateData(true);
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	char	para[16];



	//if(G_IsCmdAscii[m_BoxNr] == false)
	{
		memset(para,0,sizeof(para));
		para[0] = 0;  

		
		if(this->m_bReset)
		{
			para[0] =  '1';
		}
		else
		{
			para[0] =  '0';
		}

		maindlg->SetCmdString( SWITCH_STATUS_ADDR + m_BoxNr,CMD_SET_DISPLAY_TYPE, para);


	}
}

void CDebugDlg::OnButtonCalTemp() 
{
	// TODO: Add your control notification handler code here
	//this->InputGlassLowTemp(); 

	//InputGlassLowTemp();
	//return;
	

	CHeatBoxDlg* main = (CHeatBoxDlg*)this->GetParent()->GetParent();
	char para[100]={0,0,0,0,0,0,0,0};
	

	// TODO: 在此添加控件通知处理程序代码
//	CWnd *w = GetDlgItem(IDC_EDIT_PARA21);
//	w->SetFocus();
	
	if(m_CalStep != NOT_CAL)
	{
		if(AfxMessageBox(_T("重新开始校温吗？"),MB_OKCANCEL) == IDOK)
		{
			m_CalStep = NOT_CAL;
		}
		else
		{
			return;
		}

	}
	CInputDlg dlg;


	switch(m_CalStep)
	{
	case NOT_CAL:
		
		dlg.m_Title = _T("请输入校温密码");
		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_Input == _T("1357"))
			{
				m_CalStep = CAL_CLEAR_ADJUST;
				m_CalMsg = (_T("请在夹具帽子上插入4根水银温度计(能标记40--160度)"));
				
				memset(para,0,sizeof(para));
				main->SetCmdString( m_BoxNr+'1',CMD_SET_ADJUST_LOW, para);

				main->SetCmdString( m_BoxNr+'1',CMD_READ_ADJUST_LOW, para);

			
				memset(para,0,sizeof(para));
				main->SetCmdString( m_BoxNr+'1',CMD_SET_ADJUST_HIGH, para);

				main->SetCmdString( m_BoxNr+'1',CMD_READ_ADJUST_HIGH, para);

				SHORT_BUF short_buf;
				short_buf.v = (short)(60 *10);
				para[1] = short_buf.buf[1];
				para[0] = short_buf.buf[0];
				//para[1] = (char)((60 *10) / 256);
				//para[0] = ((int)(60 *10)) % 256;
				main->SetCmdString( m_BoxNr+'1',CMD_NORMAL_PID_OUT, para);


				m_CalMsg = (_T("恒温在 60度"));

				m_CalStep = CAL_SET_NORMAL_PID_LOW;

				this->UpdateData(false);
		
			}
			else
			{
				AfxMessageBox(_T("校温密码错误"));
			}
		}
		break;

		default:
			break;

	}
	
}

void CDebugDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    

	if(nIDEvent != 1)
		return;



	if(this->m_OnTiming)
	{
		CDialog::OnTimer(nIDEvent);
		return;
	}
	this->UpdateData();

	m_OnTiming = true;


	if(m_CalStep == CAL_SET_NORMAL_PID_LOW)
	{
		if(m_PidTemp >= (60.0 - 0.1))
		{
			m_PidBeginTime ++; 
			if(m_PidBeginTime > 90)
			{
				this->m_MeterLow[0] = this->m_CurTemp[0];  
				this->m_MeterLow[1] = this->m_CurTemp[1];
				this->m_MeterLow[2] = this->m_CurTemp[2];
				this->m_MeterLow[3] = this->m_CurTemp[3];
				m_PidBeginTime = -1;
				this->UpdateData(false);
				this->KillTimer(1);
				InputGlassLowTemp();
				SetTimer(1,1000,NULL);
			}
		}
	}
	else if(m_CalStep == CAL_SET_NORMAL_PID_HIGH)
	{
		if(m_PidTemp >= (140.0 - 0.1))
		{
			m_PidBeginTime ++; 
			if(m_PidBeginTime > 90)
			{
				this->m_MeterHigh[0] = this->m_CurTemp[0];  
				this->m_MeterHigh[1] = this->m_CurTemp[1];
				this->m_MeterHigh[2] = this->m_CurTemp[2];
				this->m_MeterHigh[3] = this->m_CurTemp[3];
				m_PidBeginTime = -1;
				this->UpdateData(false);
				this->KillTimer(1);
				InputGlassHighTemp();
				SetTimer(1,1000,NULL);
			}
		}
	}

	m_OnTiming = false;

	this->UpdateData(false);
	CDialog::OnTimer(nIDEvent);

}

bool CDebugDlg::IsCalLowAllZero()
{
	CWnd *w;
	CString text;
	w	=	GetDlgItem(IDC_EDIT_PARA37);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	w	=	GetDlgItem(IDC_EDIT_PARA38);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	w	=	GetDlgItem(IDC_EDIT_PARA39);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	w	=	GetDlgItem(IDC_EDIT_PARA40);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;

	return true;
}

bool CDebugDlg::IsCalHighAllZero()
{
	CWnd *w;
	CString text;
	w	=	GetDlgItem(IDC_EDIT_PARA41);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	w	=	GetDlgItem(IDC_EDIT_PARA42);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	w	=	GetDlgItem(IDC_EDIT_PARA43);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;
	w	=	GetDlgItem(IDC_EDIT_PARA44);
	w->GetWindowText(text);
	if(text.GetLength() > 0)
		return false;

	return true;
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

void CDebugDlg::DealCmdAnswer(int cmd_addr, char * para, char * flash,bool IsCmdAscii)
{

	CHeatBoxDlg *main = (CHeatBoxDlg *)(AfxGetApp()->m_pMainWnd); 
	CString s;

	int i;
	this->UpdateData();
	
	
	MODBUS_RS485_CMD_128    *modbus_cmd_128 =(MODBUS_RS485_CMD_128*) para;

	MODBUS_RS485_ANSWER_OMRON_READ    *omron_read_answer =(MODBUS_RS485_ANSWER_OMRON_READ*) para;

	MODBUS_RS485_ANSWER_500		* modbus_status_cmd_500 = (MODBUS_RS485_ANSWER_500*) para;

	
	

	switch(cmd_addr)
	{
	case CMD_OMRON_READ:
		{
			short addr = OmronShort2PcShort(*((short*)para));
			short v = OmronShort2PcShort(*((short*)&para[2]));
			CWnd *w;
			switch(addr)
			{
				case OMRON_PID_P:
					w = this->GetDlgItem(IDC_EDIT_PARA21);
					s.Format(_T("%5.1f"),v / 10.0f);
					w->SetWindowTextW(s); 
					break;
				case OMRON_PID_I:
					w = this->GetDlgItem(IDC_EDIT_PARA22);
					s.Format(_T("%d"),v);
					w->SetWindowTextW(s); 
					break;
				case OMRON_PID_D:
					w = this->GetDlgItem(IDC_EDIT_PARA23);
					s.Format(_T("%d"),v);
					w->SetWindowTextW(s); 
					break;
				case OMRON_PID_T:
					w = this->GetDlgItem(IDC_EDIT_PARA24);
					s.Format(_T("%d"),v );
					w->SetWindowTextW(s); 
					break;
				case OMRON_PV:
				for(i = 0; i < 4; i++)
				{
					SHORT_BUF sb;
					sb.buf[0] =  omron_read_answer->para[1];
					sb.buf[1] =  omron_read_answer->para[0];

					m_CurTemp[i] = sb.v  / 10.0f;
					
					w = this->GetDlgItem(IDC_EDIT_PARA17 + i); 
					s.Format(_T("%5.1f"),m_CurTemp[i]); 
					w->SetWindowTextW(s); 	

				}
				break;
			}
		}
		break;
	case CMD_READ_STATUS:
		if(IsCmdAscii == false)
		{
			DrawLED();
		}
		
		break;
//#endif	

	
	case CMD_SET_ADJUST_LOW:
		m_CalMsg = (_T("设置0度修正值成功!"));
		break;

	case CMD_SET_ADJUST_HIGH:
		m_CalMsg = (_T("设置200度修正值成功!"));
		break;

	case CMD_READ_ADJUST_LOW:
	
		break;

	case CMD_READ_ADJUST_HIGH:
	
		break;
	case CMD_NORMAL_PID_OUT:
		m_CalMsg = (_T("设置恒温成功!"));
		break;
	case CMD_READ_PID_T:
	
		
	
		break;
	case CMD_READ_ADJUST_ALL:
		
		break;
		
	}
	this->UpdateData(false); 
}

void CDebugDlg::InputGlassLowTemp(void)
{
	CHeatBoxDlg *main = (CHeatBoxDlg *)(AfxGetApp()->m_pMainWnd); 
	char para[16];
	memset(para,0,sizeof(para));

	OnButton17();
	

	CInputGlassTempDlg dlg;
	
	CString s;
	s.Format(_T("%d#烘箱: "),m_BoxNr+1);


		dlg.m_Title = s;
		dlg.m_Title += _T("请输入4根水银温度计的度数");
		m_CalMsg = (dlg.m_Title);
		dlg.m_Temp1 = "";
		dlg.m_Temp2 = "";
		dlg.m_Temp3 = "";
		dlg.m_Temp4 = "";
		
		if(dlg.DoModal() == IDOK)
		{
			m_InputGlassLow[0] = _tcstod(dlg.m_Temp1,NULL);
			CWnd * w = GetDlgItem(IDC_EDIT_PARA53);
			w->SetWindowText(dlg.m_Temp1);

			m_InputGlassLow[1] = _tcstod(dlg.m_Temp2,NULL);
			w = GetDlgItem(IDC_EDIT_PARA54);
			w->SetWindowText(dlg.m_Temp2);
			
			m_InputGlassLow[2] = _tcstod(dlg.m_Temp3,NULL);
			w = GetDlgItem(IDC_EDIT_PARA55);
			w->SetWindowText(dlg.m_Temp3);
			
			m_InputGlassLow[3] = _tcstod(dlg.m_Temp4,NULL);
			w = GetDlgItem(IDC_EDIT_PARA56);
			w->SetWindowText(dlg.m_Temp4);

		}


		para[1] = (char)((140 *10) / 256);
		para[0] = ((int)(140 *10)) % 256;

		main->SetCmdString( m_BoxNr+'1',CMD_NORMAL_PID_OUT, para);

		
		m_CalStep = CAL_SET_NORMAL_PID_HIGH;

}

void CDebugDlg::InputGlassHighTemp(void)
{
	CHeatBoxDlg *main = (CHeatBoxDlg *)(AfxGetApp()->m_pMainWnd); 
	char para[16];
	memset(para,0,sizeof(para));
	short short_a;

	OnButton18();//读高度仪表温度
	
	CInputGlassTempDlg	dlg;

	CString s;
	s.Format(_T("%d#烘箱:"),m_BoxNr+1);

		dlg.m_Title = s;
		dlg.m_Title += _T("请输入4根水银温度计的度数");
		m_CalMsg = (dlg.m_Title);
		dlg.m_Temp1 = "";
		dlg.m_Temp2 = "";
		dlg.m_Temp3 = "";
		dlg.m_Temp4 = "";
		if(dlg.DoModal() == IDOK)
		{
			m_InputGlassHigh[0] = _tcstod(dlg.m_Temp1,NULL);
			CWnd * w = GetDlgItem(IDC_EDIT_PARA57);
			w->SetWindowText(dlg.m_Temp1);
			
			m_InputGlassHigh[1] = _tcstod(dlg.m_Temp2,NULL);
			w = GetDlgItem(IDC_EDIT_PARA58);
			w->SetWindowText(dlg.m_Temp2);

			m_InputGlassHigh[2] = _tcstod(dlg.m_Temp3,NULL);
			w = GetDlgItem(IDC_EDIT_PARA59);
			w->SetWindowText(dlg.m_Temp3);

			m_InputGlassHigh[3] = _tcstod(dlg.m_Temp4,NULL);
			w = GetDlgItem(IDC_EDIT_PARA60);
			w->SetWindowText(dlg.m_Temp4);

			m_CalStep = NOT_CAL;

			CalAdjustTemp();

			para[1] = (char)((0 *10) / 256);
			para[0] = ((int)(0 *10)) % 256;
			main->SetCmdString( m_BoxNr+'1',CMD_NORMAL_PID_OUT, para);


			memset(para,0,sizeof(para));
			
			short_a = (short)(m_WriteAdjustLow[0] * 10.0f);
			((short*)para)[0] = short_a;

			short_a = (short)(m_WriteAdjustLow[1] * 10.0f);
			((short*)para)[1] = short_a;

			short_a = (short)(m_WriteAdjustLow[2] * 10.0f);
			((short*)para)[2] = short_a;

			short_a = (short)(m_WriteAdjustLow[3] * 10.0f);
			((short*)para)[3] = short_a;

			main->SetCmdString( m_BoxNr + '1',CMD_SET_ADJUST_LOW, para);



			memset(para,0,sizeof(para));
			short_a = (short)(m_WriteAdjustHigh[0] * 10.0f);
			((short*)para)[0] = short_a;

			short_a = (short)(m_WriteAdjustHigh[1] * 10.0f);
			((short*)para)[1] = short_a;

			short_a = (short)(m_WriteAdjustHigh[2] * 10.0f);
			((short*)para)[2] = short_a;

			short_a = (short)(m_WriteAdjustHigh[3] * 10.0f);
			((short*)para)[3] = short_a;


			main->SetCmdString( m_BoxNr + '1',CMD_SET_ADJUST_HIGH, para);

		
			main->SetCmdString( m_BoxNr + '1',CMD_READ_ADJUST_LOW, para);

			main->SetCmdString( m_BoxNr + '1',CMD_READ_ADJUST_HIGH, para);


			main->SetCmdString( m_BoxNr + '1',CMD_STOP_OUT, para);


			m_CalMsg = (_T("校温成功!"));
		}

}

void CDebugDlg::CalAdjustTemp(void)
{
	int i;
	double glass_low_avr,glass_high_avr;
	CString s;
	CWnd *w;
	//记录一个低温点，一个高温点，根据线性方程求0,200度的误差
	//低温点一般取常温，高温点最好在150度，要分别恒温 
	/*s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_InputGlassLow[0] ,m_InputGlassLow[1] ,m_InputGlassLow[2] ,m_InputGlassLow[3]);
	AfxMessageBox(s);

	s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_InputGlassHigh[0] ,m_InputGlassHigh[1] ,m_InputGlassHigh[2] ,m_InputGlassHigh[3]);
	AfxMessageBox(s);

		s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_MeterLow[0] ,m_MeterLow[1] ,m_MeterLow[2] ,m_MeterLow[3]);
	AfxMessageBox(s);

		s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_MeterHigh[0] ,m_MeterHigh[1] ,m_MeterHigh[2] ,m_MeterHigh[3]);
	AfxMessageBox(s);
*/
	glass_low_avr	=(m_InputGlassLow[0] +m_InputGlassLow[1] +m_InputGlassLow[2] +m_InputGlassLow[3] )/4;
	glass_high_avr	=(m_InputGlassHigh[0]+m_InputGlassHigh[1]+m_InputGlassHigh[2]+m_InputGlassHigh[3])/4;

	/*
	m_MeterLow[0]  = 60;
	m_MeterHigh[0] = 140;
	glass_low_avr = 58;
	glass_high_avr = 130;
*/
	for(i=0;i<4;i++)
	{
		// y = kx + delta
		double delta_low	= (glass_low_avr - m_MeterLow[i]); 
		double delta_high	= (glass_high_avr- m_MeterHigh[i]);
		
		double k = (glass_high_avr-glass_low_avr)/(m_MeterHigh[i]-m_MeterLow[i]);
		
		double delta = glass_low_avr - k * m_MeterLow[i];
		

		m_WriteAdjustLow[i] = ((int)(delta * 10.0f + 0.5))/ 10.0f ;
		s.Format(_T("%5.1f"),m_WriteAdjustLow[i]); 	
		w =GetDlgItem(IDC_EDIT_PARA29 + i);
		w->SetWindowTextW(s); 

		m_WriteAdjustHigh[i] = ((int)((200.0*k + delta) * 10.0f + 0.5))/10.0f - 200.0f;
		s.Format(_T("%5.1f"),m_WriteAdjustHigh[i]); 	
		CWnd *w =GetDlgItem(IDC_EDIT_PARA33 + i);
		w->SetWindowTextW(s); 
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

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	UpdateData(true);

	if(m_bCheckRelay)
	{
		maindlg->SetCmdAirValve(true);
		WriteLogFile(("CMD_SET_AIR_VALVE W1"),1);
	}
	else
	{
		maindlg->SetCmdAirValve(false);
		WriteLogFile(("CMD_SET_AIR_VALVE W0"),1);
	}
	


}

void CDebugDlg::OnBnClickedCheckHighVolt()
{
	// TODO: 在此添加控件通知处理程序代码
	
	// TODO: Add your control notification handler code here
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());


	UpdateData(true);

	{
	
		if(m_bCheckHighVolt)
		{
			maindlg->SetCmdAirControl(true); 
			
		}
		else
		{
			maindlg->SetCmdAirControl(false); 
		}
		

	}

}

void CDebugDlg::DrawLED(void)
{
//#define TEMP_STRING_HEIGHT	160	//非图标区


	
	int i,j,k;
	CFile log_file;
	
	if(G_CurMaxHeatBox > 1)
	{
		return;
	}

	if(!this->IsWindowVisible()) 
	{
		return;
	}


	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=this->GetWindowDC();
		

	if(dc == NULL)
	{
		m_CalMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		return;
	}
	
	CWnd *data_area = GetDlgItem(IDC_EDIT_PARA4);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);
	nWidth	= wndRect.right / 2;
	nHeight = wndRect.bottom -data_area_Rect.bottom;//wndRect.top -TEMP_STRING_HEIGHT;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		m_CalMsg.Format(_T("!MemDC.CreateCompatibleDC(dc)%s %d\r\n"),__FILE__,__LINE__);
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			m_CalMsg.Format(_T("!bitmap.DeleteObject()%s %d\r\n"),__FILE__,__LINE__);
			this->UpdateData(false);
			if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
			{
				log_file.SeekToEnd();
				log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
				log_file.Close();
			}
			ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth * MAX_HEAT_BOX,nHeight))	//建立与显示设备兼容的位图
	{
		m_CalMsg.Format(_T("!bitmap.CreateCompatibleBitmap%s %d\r\n"),__FILE__,__LINE__);
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth - PEN_WEIGHT *2) / G_iLedNrPerLine - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / G_iLedLines - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}

	int Ellipse_H_Space = nHeight / G_iLedLines - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	for(k=0;k<G_CurMaxHeatBox;k++)
	{
		MemDC.RoundRect(PEN_WEIGHT + nWidth * k,PEN_WEIGHT,nWidth-PEN_WEIGHT*3 + nWidth * k + 4,nHeight-PEN_WEIGHT*5 ,10,10);
	}

	MemDC.SelectObject(&graybrush);				//选择画刷
	j = 0 ;
//#ifndef _HANJIE_192
	//for(j=0;j<MAX_HEAT_BOX;j++)
	{
		for(i=0; i < G_iMaxLedNr /* MAX_HEAT_BOX*/; i++)
		{//默认的全部是灰色
					rect.left =  nWidth * j + (i % G_iLedNrPerLine)*(Ellipse_W_GAP+Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
					rect.top = 5+(i / G_iLedNrPerLine)*(Ellipse_H + Ellipse_H_Space)+Ellipse_H_Space / 2;
					rect.right = rect.left + Ellipse_W;
					rect.bottom = rect.top + Ellipse_H;
					if(G_SwitchTestResult[i].IsClosing == false)
					{
						MemDC.Ellipse(rect);		//填充背景
					}
		
		}
	}	
//#endif
	MemDC.SelectObject(&redbrush);				//选择画刷


	k = 0;
	{
		for(i=0; i< G_iLedNrPerLine; i++)
		{
			for(j=0; j < G_iLedLines; j++)
			{
				if((j * G_iLedNrPerLine + i) >= G_iMaxLedNr)
				{
				}
				else
				{
//#ifdef _HANJIE_192
//					if(m_SwitchTestResult[m_BoxNr][11 - i + j * G_iLedNrPerLine ].IsClosing)
//#else
					if(G_SwitchTestResult[     i + j * G_iLedNrPerLine ].IsClosing)
//#endif

					{//闭合就是红色
						rect.left =  nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
						rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
						rect.right = rect.left + Ellipse_W;
						rect.bottom = rect.top + Ellipse_H;
						MemDC.Ellipse(rect);		//填充背景
					}
				}
			}
		}
	}


	if(G_iMaxLedNr == 500)
	{
//#ifdef _HANJIE_192
//	CFont font;
//	if(font.CreatePointFont(250,_T("黑体"),&MemDC))
//	{
//		CFont *oldfont = MemDC.SelectObject(&font); 
//#endif				
				
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr; i++)
			{
					CString nr;\
					
					nr.Format(_T("%d"),(i+1));
					
					//nr.Format("%02d%c",((i+1) / G_iLedNrPerLine), nr_low[(i % G_iLedNrPerLine)]);
					CSize size = MemDC.GetTextExtent(nr);
					MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
			}
//#ifdef _HANJIE_192		
//		}

//		MemDC.SelectObject(oldfont); 
//		font.DeleteObject(); 
//#endif
	}
	}
	else
	{
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr; i++)
			{
					CString nr;
					nr.Format(_T("%02d"),i+1);
					CSize size = MemDC.GetTextExtent(nr);
					MemDC.TextOut( 5 + nWidth * k + (i % G_iLedNrPerLine) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
					/*CRect rect;
					rect.left =5+(i % G_iLedNrPerLine) *(Ellipse_W + Ellipse_W_Space);
					rect.top  =5+(i / G_iLedNrPerLine)*((Ellipse_H + Ellipse_H_Space));
					rect.right = rect.left + Ellipse_W;
					rect.bottom= rect.top + Ellipse_H;
					
					MemDC.DrawText(nr,rect,DT_CENTER | DT_VCENTER);
					*/
					
			}
		}	
	}

	dc->BitBlt(nWidth,data_area_Rect.bottom + PEN_WEIGHT * 5 ,nWidth * G_CurMaxHeatBox + 30 ,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
	this->UpdateData(false); 


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	ReleaseDC(dc); 
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
	

	
	CInputDlg dlg;
	dlg.m_Title = EditMsg[id - IDC_EDIT_PARA5] ;
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(id) ;
		w->SetWindowTextW(dlg.m_Input);
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
	}
	

	w->SetFocus(); 
}

void CDebugDlg::OnBnClickedCheckCoolRelay()
{
	// TODO: 在此添加控件通知处理程序代码
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	char para[16];
	
	UpdateData(true);

	memset(para,0,sizeof(para));

	if(m_bCheckCoolRelay)
	{
		para[0] = 1;  
	}
	else
	{
		para[0] = 0;  
	}
	
	
	maindlg->SetCmdString( 'A' + this->m_BoxNr,CMD_SET_HEAT, para);

}

void CDebugDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		CEdit *edit;
		
		TCHAR buf[10];
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA15);
		edit->GetWindowText(buf,10); 
		double temp = _tcstod(buf,NULL);
		char para[100]={0,0,0,0,0,0,0,0};

		SHORT_BUF short_buf;
		short_buf.v = (short)(temp *10);
		para[0] = short_buf.buf[0];
		para[1] = short_buf.buf[1];
		
		edit=(CEdit*)this->GetDlgItem(IDC_EDIT_PARA16);
		edit->GetWindowText(buf,10); 
		temp = _tcstod(buf,NULL);
		
		short_buf.v = (short)temp;
		para[2] = short_buf.buf[0];
		para[3] = short_buf.buf[1];
		
		maindlg->SetCmdString( m_BoxNr + '1',CMD_STEP_DOWN_PID_OUT, para);


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

void CDebugDlg::OnBnClickedCheckCoolRelay2()
{
	// TODO: 在此添加控件通知处理程序代码
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	char para[16];

	UpdateData(true);

	memset(para,0,sizeof(para));
	if(m_bCheckFan)
	{
		para[0] = 1;  
	}
	else
	{
		para[0] = 0;  
	}
	
	
	maindlg->SetCmdString( 'A' + this->m_BoxNr,CMD_SET_FAN, para);
	maindlg->SetCmdString( 'A' + this->m_BoxNr,CMD_SET_FAN, para);
	maindlg->SetCmdString( 'A' + this->m_BoxNr,CMD_SET_FAN, para);
}

void CDebugDlg::OnBnClickedButtonUpdateFirmware()
{
	// TODO: 在此添加控件通知处理程序代码
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	maindlg->XModem_Start('A'); 
		

}

void CDebugDlg::OnBnClickedButtonUpdateFirmware2()
{
	// TODO: 在此添加控件通知处理程序代码
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	maindlg->XModem_Start('B'); 
}

void CDebugDlg::OnBnClickedButtonUpdateFirmware3()
{
	// TODO: 在此添加控件通知处理程序代码
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	maindlg->XModem_Start('C'); 
}

void CDebugDlg::OnBnClickedButtonUpdateFirmware4()
{
	// TODO: 在此添加控件通知处理程序代码

	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	maindlg->XModem_Start('D'); 

}

void CDebugDlg::OnBnClickedButtonUpdateFirmware5()
{
	// TODO: 在此添加控件通知处理程序代码

	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	maindlg->XModem_Start('E'); 

}
