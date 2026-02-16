// SwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "SwitchDlg.h"
#include "comm.h"
#include "common.h"
#include "DrawLed.h"
#include "InputGlassTempDlg.h"

#include "shlwapi.h"
#include "RecordViewDlg.h"
#include <stdio.h>
#include "TakeDlg.h"
#include "HeatBoxDlg.h"
#include "CopyRight.h"
#include "DlgInfomation.h"
#include "SorterPlcDlg.h"
#include "HttpTools.h"

#pragma  comment(lib,"shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CNtcTempRecordArray	G_NtcTempRecordArray[MAX_HEAT_BOX];

   


/////////////////////////////////////////////////////////////////////////////
// CSwitchDlg dialog
 int SortSwitchGroup(int id)
 {
	 int ret = -1;
	 switch(id)
	 {
	 case 0: 
			ret = 0;
			break;
	 default:
		 ret = 16 - id;
		 break;
	 }
	 return ret;
 }

CSwitchDlg::CSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSwitchDlg::IDD, pParent)
{
	memset(m_CurTemp,0,sizeof(m_CurTemp));
	m_CurTemp[0]				= 0.0f;
	m_CurTemp[1]				= 0.0f;
	m_CurTemp[2]				= 0.0f;
	m_CurTemp[3]				= 0.0f;
	//m_SwitchNormalOpen			= 0;
	m_WorkMode.m_SubMode		= SUB_MODE_STOP;
//	m_WorkMode.m_WorkStatus.RemainTime		= 0;
	m_IsTaking					= false;
	m_iTestMode					= TEST_MODE_NORMAL;
//	m_OpenDoor_Temp				= 0;
//	m_OpenDoor_StartTime		= m_OpenDoor_StartTime.GetCurrentTime();
//	m_OpenDoorIng				=	FALSE;
	//{{AFX_DATA_INIT(CSwitchDlg)
	m_TestMsg = _T("");
	m_PidTemp = 0.0f;

	m_TestTimeMsg = _T("");
	//}}AFX_DATA_INIT

	memset(&m_TempPara,0,sizeof(m_TempPara));



}


void CSwitchDlg::DoDataExchange(CDataExchange* pDX)
{

//	DDX_Check(pDX, IDC_CHECK_AUTO_PROCESS, m_AutoTestFlag);
	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSwitchDlg)
	DDX_Text(pDX, IDC_STATIC_TEST_MSG, m_TestMsg);
	//DDX_Radio(pDX, IDC_RADIO_CLOSE, m_SwitchNormalOpen);
	
	DDX_Text(pDX, IDC_STATIC_TIME_MSG, m_TestTimeMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSwitchDlg, CDialog)
	//{{AFX_MSG_MAP(CSwitchDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_AUTO_PROCESS, OnCheckStartTest)
	ON_BN_CLICKED(IDC_BUTTON_TAKEOUT, OnButtonTakeout)
	ON_BN_CLICKED(IDC_RADIO_CLOSE, OnRadioClose)
	ON_BN_CLICKED(IDC_RADIO_OPEN, OnRadioOpen)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwitchDlg message handlers
	//CBitmap所需资源
void DrawLED(CWnd *Wnd,CRect LedRect,int BoxNr);

void CSwitchDlg::DrawLED(void)
{




	CWnd *data_area = GetDlgItem(IDC_STATIC_DATA_AREA);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);

	CRect LedRect;
	LedRect.left	= wndRect.left;
	LedRect.top		= wndRect.top + data_area_Rect.bottom;
	LedRect.right	= wndRect.right;
	LedRect.bottom	= wndRect.bottom; 
	::DrawLED(this,LedRect,m_BoxNr,0);

	
}

void CSwitchDlg::DrawLED_128_OLD_DISPLAY(void)
{

	CWnd *data_area = GetDlgItem(IDC_STATIC_DATA_AREA);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);

	CRect LedRect;
	LedRect.left	= wndRect.left;
	LedRect.top		= wndRect.top + data_area_Rect.bottom;
	LedRect.right	= wndRect.right;
	LedRect.bottom	= wndRect.bottom; 
	::DrawLED_128_OLD_DISPLAY(this,LedRect,m_BoxNr);
}

void CSwitchDlg::DrawBiMetalLED(void)
{
	CWnd *data_area = GetDlgItem(IDC_STATIC_DATA_AREA);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);

	CRect LedRect;
	LedRect.left	= wndRect.left;
	LedRect.top		= wndRect.top + data_area_Rect.bottom;
	LedRect.right	= wndRect.right;
	LedRect.bottom	= wndRect.bottom;
//	m_HeatCoolOrder		= CheckHeatOrCoolMode(m_SwitchConfigPara.para, &m_TempRange,::G_NormalConfigPara[m_BoxNr].BoxType);
	DrawLED_BiMetal(this,m_BoxNr,LedRect,&m_SwitchConfigPara.para,m_LED_Rect,G_NormalConfigPara[m_BoxNr].BoxType);

}

BOOL CSwitchDlg::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg); 
}

BOOL CSwitchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString s;	

	if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_24 )
		{
		::G_iMaxLedNr[m_BoxNr]		= 40;
		::G_iLedLines[m_BoxNr]		= 4;
		::G_iLedNrPerLine[m_BoxNr]	= 6;
	}
	else if(G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_40)
	{
		::G_iMaxLedNr[m_BoxNr]		= 40;
		::G_iLedLines[m_BoxNr]		= 4;
		::G_iLedNrPerLine[m_BoxNr]	= 10;
	}

//	CTime cur = CTime::GetCurrentTime();
//	long sec = GetSpanSeconds(cur);
//	CTime  c = GetRecordTime(sec);
//	AfxMessageBox(c.Format(_T("%Y/%m/%d %H:%M:%S")));

	// TODO: Add extra initialization here
	this->SetTimer(NULL,1000,NULL); 
//	memset(&G_PID,0,sizeof(G_PID));

#ifdef MAX_LED_NR_256
	CWnd *w ;
	w = GetDlgItem(IDC_COMBO_FLASH_GAP);
	w->ShowWindow(SW_HIDE);
	w = GetDlgItem(IDC_STATIC_FLASH);
	w->ShowWindow(SW_HIDE);
#endif
	//恢复生产记录
	CFile f;
	CString file_name;
	file_name.Format(_T("record%d.bin"),m_BoxNr);
	

	
	if(f.Open(theAppDirectory+file_name, CFile::modeReadWrite))
	{
		f.SeekToBegin();
		RECORD record;
		DWORD i = f.Read(&record,sizeof(record));
		//if(f.Read(&G_SwitchTestResult[m_BoxNr],sizeof(G_SwitchTestResult[m_BoxNr])) != sizeof(G_SwitchTestResult[m_BoxNr]))
		if( i != sizeof(record))
		{
			memset(&G_SwitchTestResult[m_BoxNr],0,sizeof(G_SwitchTestResult[m_BoxNr]));
			m_WorkMode.m_BeginTestTime = CTime::GetCurrentTime(); 
			m_WorkMode.m_EndTestTime   = CTime::GetCurrentTime(); 
		}
		else
		{
			m_WorkMode.m_BeginTestTime	= STD_TIME + CTimeSpan(0,0,record.StartTime,0); 
			m_WorkMode.m_EndTestTime	= STD_TIME + CTimeSpan(0,0,record.EndTime  ,0); 
			memcpy(&G_SwitchTestResult[m_BoxNr],record.TestResult ,sizeof(G_SwitchTestResult[m_BoxNr]));
		}
		f.Close(); 
	}

#ifdef _DEBUG
		memset(&G_SwitchTestResult[m_BoxNr],0,sizeof(G_SwitchTestResult[m_BoxNr]));
		for(int i =0 ; i < 100; i++)
		{
			G_SwitchTestResult[m_BoxNr][i].IsUsed		= true;
			G_SwitchTestResult[m_BoxNr][i].IsClosing	= (i % 2);
			G_SwitchTestResult[m_BoxNr][i].IsOpenned	= true;
			G_SwitchTestResult[m_BoxNr][i].OpenTemp		= 98+(i/5.0f);
			G_SwitchTestResult[m_BoxNr][i].IsClosed		= true;
			G_SwitchTestResult[m_BoxNr][i].CloseTemp	= 60+(i/5.0f);;
			G_SwitchTestResult[m_BoxNr][i].OpenTime     = GetSpanSeconds(CTime::GetCurrentTime());
			G_SwitchTestResult[m_BoxNr][i].CloseTime    = GetSpanSeconds(CTime::GetCurrentTime());

		}
		for(int i =100 ; i < 120; i++)
		{
			G_SwitchTestResult[m_BoxNr][i].IsUsed		= true;
			G_SwitchTestResult[m_BoxNr][i].IsClosing	= (i % 2);
			G_SwitchTestResult[m_BoxNr][i].IsOpenned	= true;
			G_SwitchTestResult[m_BoxNr][i].OpenTemp		= 98+(i/5.0f);
			G_SwitchTestResult[m_BoxNr][i].IsClosed		= true;
			G_SwitchTestResult[m_BoxNr][i].CloseTemp	= 60+(i/5.0f);;
			G_SwitchTestResult[m_BoxNr][i].OpenTime     = GetSpanSeconds(CTime::GetCurrentTime());
			G_SwitchTestResult[m_BoxNr][i].CloseTime    = GetSpanSeconds(CTime::GetCurrentTime());

		}
		for(int i =120 ; i < 128; i++)
		{
			G_SwitchTestResult[m_BoxNr][i].IsUsed		= false;
			G_SwitchTestResult[m_BoxNr][i].IsClosing	= (i % 2);
			G_SwitchTestResult[m_BoxNr][i].IsOpenned	= true;
			G_SwitchTestResult[m_BoxNr][i].OpenTemp		= 98+(i/5.0f);
			G_SwitchTestResult[m_BoxNr][i].IsClosed		= false;
			G_SwitchTestResult[m_BoxNr][i].CloseTemp	= 60+(i/5.0f);;
			G_SwitchTestResult[m_BoxNr][i].OpenTime     = GetSpanSeconds(CTime::GetCurrentTime());
			G_SwitchTestResult[m_BoxNr][i].CloseTime    = GetSpanSeconds(CTime::GetCurrentTime());

		}

		G_SwitchTestResult[m_BoxNr][11].IsOpenFlash = true;
		G_SwitchTestResult[m_BoxNr][10].IsClosed = true;

#endif

	



	CTimeSpan span(0,0,1,0);
	m_MeterAnswerTime = CTime::GetCurrentTime();
	m_MeterAnswerTime -= span;
	m_LedAnswerTime[0]   = CTime::GetCurrentTime();

	m_LedAnswerTime[0]  -= span;

	
	
	CButton *b = (CButton*)GetDlgItem(IDC_BUTTON_TAKEOUT); 
	if(b != NULL)
	{
		if(b->GetBitmap() == NULL)
		{
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TAKE_BOARD)));
		}
	}

	m_iTestMode = G_NormalConfigPara[m_BoxNr].TestMode;

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSwitchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if(G_NormalConfigPara[m_BoxNr].SwitchType == 0)
	{
		CButton *button =(CButton *) this->GetDlgItem(IDC_RADIO_CLOSE);
		button->SetCheck(true);
		button =(CButton *) this->GetDlgItem(IDC_RADIO_OPEN);
		button->SetCheck(false);
	}
	else
	{
		CButton *button =(CButton *) this->GetDlgItem(IDC_RADIO_CLOSE);
		button->SetCheck(false);
		button =(CButton *) this->GetDlgItem(IDC_RADIO_OPEN);
		button->SetCheck(true);
	}
}

void CSwitchDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting



	// TODO: Add your message handler code here
	if(G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_24 || G_NormalConfigPara[m_BoxNr].BoxType == BOX_DISC_40)
	{
		this->DrawBiMetalLED(); 
	}
	else if((G_NormalConfigPara[m_BoxNr].BoxType == HEAT_BOX_TYPE_128_OLD_DISPLAY) && (::G_iMaxLedNr[m_BoxNr] == 128) )
	{
		DrawLED_128_OLD_DISPLAY();
	}
	else
	{
	//	DrawLED();
	}

	CRect rect;
	this->GetClientRect(&rect);
	rect.left += 5;
	rect.right -= 2;
	rect.bottom = 85;

	if(G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[0] != 0 
		|| G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[1] != 0 
		|| G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[2] != 0 
		|| G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[3] != 0 
		|| G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[4] != 0 
		|| G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[5] != 0 
		|| G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[6] != 0 
		|| G_Meter[m_BoxNr].m_8TEMP_8SSR_Answer.Pv[7] != 0 )
	{
	}
	else
	{

		m_Meter.Init(this->GetSafeHwnd() ,this->GetWindowDC(),rect,m_TempPara);

		
		m_Meter.DrawMeter(m_BoxNr); 
	}
 
	// Do not call CDialog::OnPaint() for painting messages
}

void CSwitchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
}

BOOL CSwitchDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

#ifndef _WORK_MODE_H_
void CSwitchDlg::AutoTestStep_DirectHeat(void)
{
//	this->m_WorkMode.AutoTestStep_DirectHeat(this,m_BoxNr,this->m_PidTemp,this->m_SwitchConfigPara.para);

//	return;

	//如果现在的温度已经达到目标温度，就进入下一步

	if((m_PidTemp + 0.3f )>=  GetControlGoalTemp(SUB_MODE_DIRECT_HEAT))
	{
		m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_PID;
			
		::G_RS485_Comm.SendNormalPidCmd( GetControlGoalTemp(SUB_MODE_PID),m_BoxNr);

		m_CurSubModeBeginTime = m_CurSubModeBeginTime.GetCurrentTime(); 
	}
	else
	{
//		m_WorkMode.m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_DIRECT_HEAT) * (short)10 ; 
//		m_WorkMode.m_WorkStatus.RemainTime = 0;
		this->m_TestMsg.Format(_T("全速升温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_DIRECT_HEAT));
		this->UpdateData(false);
	}
}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AuotTestStep_NormalPID_2(void)
{
	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;


	cur_t	=	CTime::GetCurrentTime(); 
	span	=	cur_t - m_CurSubModeBeginTime;
	
	//如果时间已到，就进入下一步
			
	if( span.GetTotalMinutes() >= G_NormalConfigPara[m_BoxNr].PidTime)
	{
		
		m_OpenDoorIng	= false;

		m_OpenDoor_Temp	=  GetControlGoalTemp(SUB_MODE_PID) + ::G_NormalConfigPara[m_BoxNr].OpenDoorReset_HeatDegree; 

		m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_STEP_PID_2;

		m_CurSubModeBeginTime = m_CurSubModeBeginTime.GetCurrentTime(); 
		
		
		int test_minutes = (10.0f)  * 10.0f /  G_NormalConfigPara[m_BoxNr].HeatSpeed;
			//(GetControlGoalTemp(SUB_MODE_STEP_PID) - GetControlGoalTemp(SUB_MODE_PID))* 10 / m_iStepPidSpeed;
		//SendControlCmd(CMD_STEP_PID_OUT,m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f/*GetControlGoalTemp(SUB_MODE_STEP_PID)*/,test_minutes);
		
		G_RS485_Comm.SendStepPIDCmd(m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f,test_minutes,m_BoxNr);

		
	}
	else
	{
//	    m_WorkMode.m_WorkStatus.GoalTemp = /*GetControlGoalTemp(SUB_MODE_PID)*/ (m_TempRange.open_temp_max + G_iHighHighLevelDelta) * (short)10; 
		int RemainTime = (G_NormalConfigPara[m_BoxNr].PidTime - span.GetTotalMinutes())  * 60 - span.GetSeconds(); 

		this->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_PID) ,(int)(RemainTime/60),(int)(RemainTime%60));
		this->UpdateData(false);
	}
}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AuotTestStep_NormalPID(void)
{


	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;


	cur_t	=	CTime::GetCurrentTime(); 
	span	=	cur_t - m_CurSubModeBeginTime;
	
	if(IsAllOpened())
	{
		this->ExitAutoTest();
	}
	//如果时间已到，就进入下一步
	else if(span.GetTotalMinutes() >= G_NormalConfigPara[m_BoxNr].PidTime)
	{
		if(m_iTestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
		{
			this->ExitAutoTest();
		}
		else
		{
			m_OpenDoorIng	= false;

			m_OpenDoor_Temp	= GetControlGoalTemp(SUB_MODE_PID) + ::G_NormalConfigPara[m_BoxNr].OpenDoorReset_HeatDegree; 

			m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_STEP_PID;

			m_CurSubModeBeginTime = m_CurSubModeBeginTime.GetCurrentTime(); 
			 
			int test_minutes = (GetControlGoalTemp(SUB_MODE_STEP_PID) - GetControlGoalTemp(SUB_MODE_PID))* 10 / G_NormalConfigPara[m_BoxNr].HeatSpeed;
			::G_RS485_Comm.SendStepPIDCmd(  GetControlGoalTemp(SUB_MODE_STEP_PID),test_minutes,m_BoxNr);
		}
		
	}
	else
	{
		if(m_iTestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
		{
			if((m_PidTemp + 0.2f) >= GetControlGoalTemp(SUB_MODE_PID))
			{//开始计时
				if(m_CurSubModeBeginTime == 0)
				{
					m_CurSubModeBeginTime = CTime::GetCurrentTime();
				}
			}
		}

		int RemainTime = (G_NormalConfigPara[m_BoxNr].PidTime - span.GetTotalMinutes())  * 60 - span.GetSeconds(); 

		this->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_PID) ,(int)(RemainTime/60),(int)(RemainTime%60));
		this->UpdateData(false);
	}

	
}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AutoTestStep_StepPID_2(void)
{
	//如果现在的温度已经达到目标温度，就进入下一步
	// 或者时间已到


	if((m_PidTemp  >=  (m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f))
		|| IsAllOpened())
	{
		if(IsAllOpened())
		{
		//	::WriteLogFile("AutoTestStep_StepPID  IsAllOpened",this->m_BoxNr + 1);
		}

		if(m_TempRange.close_temp_min == m_TempRange.close_temp_max)
		{//不用测复位，直接结束测试
			::WriteLogFile("AutoTestStep_StepPID  close_temp_min == close_temp_max",this->m_BoxNr + 1);
			this->ExitAutoTest(); 
		}
		else
		{

			m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_DIRECT_COOL;
	
			G_RS485_Comm.SendStopHeatCmd(m_BoxNr);
			G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); 
			this->m_TestMsg.Format(_T("降温到 %5.1f度 "),m_SwitchConfigPara.para.CloseLowLevel - m_SwitchConfigPara.para.CloseLowLevelSpanMinus);
			this->UpdateData(false);

			
			if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
			{
				G_RS485_Comm.SendEPM1270Reset(true,m_BoxNr);
			//	SendEPM1270Reset(true);
			}

		}

	}
	else
	{
		
		CTime cur = cur.GetCurrentTime();
		CTimeSpan span = cur - m_CurSubModeBeginTime; 
		

		if(m_iTestMode == TEST_MODE_OPEN_DOOR_RESET || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			OpenDoor_For_OpenFlashTest();
		}
		else
		{
			
			
			this->m_TestMsg.Format(_T("线性升温到 %5.1f度 %02d:%02d"),m_TempRange.open_temp_max + G_iHighHighLevelDelta + 10.0f,span.GetTotalMinutes(),span.GetSeconds());
			this->UpdateData(false);
		}
		
	}

}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AutoTestStep_TopPID(void)
{



	CTimeSpan span =  CTime::GetCurrentTime() - m_TopPIDStartTime;

	if(span.GetTotalMinutes() >= ::G_NormalConfigPara[m_BoxNr].TopPidMinutes) 
	{
				m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_DIRECT_COOL;
		
				G_RS485_Comm.SendStopHeatCmd(m_BoxNr);

				if(::G_iMaxLedNr[m_BoxNr] == 192)
				{//开门就是开空调
					if(::G_NormalConfigPara[m_BoxNr].AirControlEquipment == 0)
					{
						G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); 
					}
					else
					{
						if((this->m_PidTemp < ::G_NormalConfigPara[m_BoxNr].AirControlEquipment) && (::G_NormalConfigPara[m_BoxNr].AirControlEquipment >  0))
						{
							G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); 
						}
					}
				}
				else
				{
					G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); 
				}
				this->m_TestMsg.Format(_T("降温到 %5.1f度 "),m_SwitchConfigPara.para.CloseLowLevel - m_SwitchConfigPara.para.CloseLowLevelSpanMinus);
				this->UpdateData(false);

				
				if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
				{
					G_RS485_Comm.SendEPM1270Reset(true,m_BoxNr);
				}
	}
	else
	{

		this->m_TestMsg.Format(_T("顶部恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_TOP_PID),span.GetTotalMinutes(),span.GetSeconds());
		this->UpdateData(false);
	
	}



}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AutoTestStep_StepPID(void)
{
	//如果现在的温度已经达到目标温度，就进入下一步
	// 或者时间已到

	if((m_PidTemp  >=  m_TempRange.open_temp_max + G_iHighHighLevelDelta)
		|| IsAllOpened())
	{
		m_CurSubModeBeginTime = CTime::GetCurrentTime();

		if(IsAllOpened())
		{
		//	::WriteLogFile("AutoTestStep_StepPID  IsAllOpened",this->m_BoxNr + 1);
		}

		if(m_TempRange.close_temp_min == m_TempRange.close_temp_max)
		{//不用测复位，直接结束测试
		//	AfxMessageBox(_T("1"));
			::WriteLogFile("AutoTestStep_StepPID  close_temp_min == close_temp_max",this->m_BoxNr + 1);
			this->ExitAutoTest(); 
		}
		else
		{
#ifdef _CHENG_HONG_
			m_NormalPidBeginTime_2 = CTime::GetCurrentTime();
			m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_NORMAL_PID_2;
			SendControlCmd(CMD_NORMAL_PID_OUT,m_TempRange.open_temp_max + G_iHighHighLevelDelta,0);
			G_RS485_Comm.SendOpenDoorCmd(false); 
			this->m_TestMsg.Format(_T("恒温在 %5.1f度 "),m_TempRange.open_temp_max + G_iHighHighLevelDelta);
			this->UpdateData(false);

			
		

#else
		
			if(::G_NormalConfigPara[m_BoxNr].TestMode == TEST_MODE_TOP_TEMP_PID)
			{
				m_TopPIDStartTime	= CTime::GetCurrentTime();
				m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_STEP_TOP_PID;
				G_RS485_Comm.SendNormalPidCmd(GetControlGoalTemp(SUB_MODE_STEP_TOP_PID),m_BoxNr);
			}
			else
			{
				m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_DIRECT_COOL;
		
				G_RS485_Comm.SendStopHeatCmd(m_BoxNr);

				if(::G_iMaxLedNr[m_BoxNr] == 192)
				{//开门就是开空调
					if(::G_NormalConfigPara[m_BoxNr].AirControlEquipment  == 0)
					{
						G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); 
					}
					else
					{
						if((m_PidTemp < G_NormalConfigPara[m_BoxNr].AirControlEquipment) && (G_NormalConfigPara[m_BoxNr].AirControlEquipment > 0))
						{
							G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); 
						}
					}
				}
				else
				{
					G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); 
				}
				this->m_TestMsg.Format(_T("降温到 %5.1f度 "),m_SwitchConfigPara.para.CloseLowLevel - m_SwitchConfigPara.para.CloseLowLevelSpanMinus);
				this->UpdateData(false);

				
				if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
				{
					G_RS485_Comm.SendEPM1270Reset(true,m_BoxNr);
				}
			}
#endif
			
		}

	}
	else
	{
		
		CTime cur = cur.GetCurrentTime();
		CTimeSpan span = cur - m_CurSubModeBeginTime; 
		

		if(m_iTestMode == TEST_MODE_OPEN_DOOR_RESET || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			OpenDoor_For_OpenFlashTest();
		}
		else
		{
			
			
			this->m_TestMsg.Format(_T("线性升温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_PID),span.GetTotalMinutes(),span.GetSeconds());
			this->UpdateData(false);
		}
		
	}


}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AutoTestStep_DircetCool(void)
{


	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;


	if(m_iTestMode == TEST_MODE_NORMAL)
	{//烘箱带空调，在一定温度以下自动开空调
		int cool = G_NormalConfigPara[m_BoxNr].AirControlEquipment;
		if(cool != 0)
		{
			if((m_PidTemp < cool) && (m_TempPara.OutputStatus && (1 << OUTPU_STATUS_AIR_OFF_BIT)))
			{
				G_RS485_Comm.SendAirControlCmd(true,false,m_BoxNr);
			}
		}
	}
	
	if(::G_NormalConfigPara[m_BoxNr].CoolSpeed > 0)
	{//降温也要控制
		if((m_PidTemp < m_TempRange.close_temp_max + G_NormalConfigPara[m_BoxNr].StepCoolDelta)
			|| IsAllClosed())
		{
			if(G_iMaxLedNr[m_BoxNr] != 192) 
			{
				G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr); 
			}
			int minutes = (m_PidTemp - m_TempRange.close_temp_min) / (::G_NormalConfigPara[m_BoxNr].CoolSpeed / 10.0f);
			G_RS485_Comm.SendStepPidCoolCmd(m_TempRange.close_temp_min,minutes,m_BoxNr);
			m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_STEP_COOL;

			this->m_TestMsg.Format(_T("匀速降温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_STEP_COOL));
			this->UpdateData(false);
			m_CurSubModeBeginTime = CTime::GetCurrentTime();
		}
		else
		{//每分钟1度
			if(m_PidTemp <= m_LastClosePIDTemp)
			{
				span = CTime::GetCurrentTime() - m_ClosePIDStartTime;
				if(span.GetTotalMinutes() >= G_NormalConfigPara[m_BoxNr].PidTime)
				{
					m_ClosePIDStartTime = m_ClosePIDStartTime.GetCurrentTime();
					m_LastClosePIDTemp = m_LastClosePIDTemp - 1;
					G_RS485_Comm.SendNormalPidCmd(m_LastClosePIDTemp,m_BoxNr);
					m_TestMsg.Format(_T("恒温在%5.1f度1分钟"), m_LastClosePIDTemp);
				}
			}
			else
			{
				span = CTime::GetCurrentTime() - m_CurSubModeBeginTime;
				m_TestMsg.Format(_T("全速降温到%5.1f度 %d:%02d"), m_TempRange.close_temp_max + G_NormalConfigPara[m_BoxNr].StepCoolDelta,span.GetTotalSeconds()/ 60,span.GetTotalSeconds() % 60);
			}
			

		}
	}
	else
	{
		OpenDoor_For_CloseFlashTest();

		cur_t =  CTime::GetCurrentTime(); 
		
		if(m_iTestMode != TEST_MODE_CLOSE_DOOR_ONCE)
		{
			double stop_temp ;
			if(m_StopTestType == 0)
			{//降温到最低复位温度还是一直降温
				stop_temp = m_TempRange.close_temp_min;
			}
			else
			{
				stop_temp = 0;
			}
			if((m_PidTemp < stop_temp ) || (m_TempRange.close_temp_max ==  m_TempRange.close_temp_min)
				|| IsAllClosed())
			{
				
					ExitAutoTest();
				
			}
			else
			{
				if(m_StopTestType == 0)
				{
					this->m_TestMsg.Format(_T("降温到 %5.1f度 "),stop_temp);
				}
				else
				{
					this->m_TestMsg = _T("降温到所有产品都复位 ");
				}
				this->UpdateData(false);
			}
		}
		else //if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
		{
			if((m_PidTemp < m_TempRange.close_temp_min)
				|| IsAllClosed())
			{
				if((GetLowestCloseTemp() < m_TempRange.close_temp_max)
					&& (GetLowestCloseTemp() < 200))
				{
					if(m_iTestMode == TEST_MODE_CLOSE_DOOR_ONCE)
					{
					
						::G_RS485_Comm.SendDirectHeatCmd(GetHighestCloseTemp(),m_BoxNr);
						G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr); 
						m_WorkMode.m_WorkStatus.SubMode = SUB_MODE_HEAT_AGAIN;
					}
					else
					{
						ExitAutoTest();
					}
				}
				else
				{
					ExitAutoTest();
				}
			}
			else
			{
				this->m_TestMsg.Format(_T("降温到 %5.1f度 "),m_TempRange.close_temp_min);
				this->UpdateData(false);
			}
		}
	}


}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AutoTestStep_HeatAgain(void)
{

	if((GetLowestCloseTemp() < m_TempRange.close_temp_max)
						&& (GetLowestCloseTemp() < 200))
	{//此时至少有一个复位的产品	
		this->m_TestMsg.Format(_T("升温到 %5.1f度 "),(float)GetHighestCloseTemp());
		if(m_PidTemp > GetHighestCloseTemp())
		{
			::G_RS485_Comm.SendStopHeatCmd(m_BoxNr);

			ExitAutoTest();
			G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); //WriteLogFile("5",m_BoxNr+1,true);
		}
	}
	else
	{
			G_RS485_Comm.SendStopHeatCmd(m_BoxNr);

			ExitAutoTest();
			G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); //WriteLogFile("6",m_BoxNr+1,true);
	}


}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::AutoTestStep_StepCool(void)
{
	
	//如果现在的温度已经达到目标温度，就进入下一步
	// 或者时间已到

	if((m_PidTemp  <  m_TempRange.close_temp_min)
		|| IsAllClosed())
	{
		this->ExitAutoTest(); 
	}
	else
	{
		int	span_minutes,span_seconds;
		CTime cur = cur.GetCurrentTime();
		CTimeSpan ts = cur - m_CurSubModeBeginTime; 
		span_seconds = ts.GetTotalSeconds();
		span_minutes	 = span_seconds / 60;

		if(m_iTestMode == TEST_MODE_OPEN_DOOR_RESET || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
		{
			OpenDoor_For_OpenFlashTest();

		}
		else
		{
			
			this->m_TestMsg.Format(_T("线性降温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_COOL),span_minutes,span_seconds % 60);
			this->UpdateData(false);
		}
		
	}

}
#endif

void CSwitchDlg::OnTimer(UINT nIDEvent) 
{

	m_WorkMode.Timer(this,m_BoxNr,m_CurTemp,m_SwitchConfigPara.para);   

	CDialog::OnTimer(nIDEvent);
	
}

void CSwitchDlg::SaveCurve(void) 
{
	CString s;
	s.Format(_T("d:\\温度曲线[%d]%04d%02d%02d%02d%02d%02d.bmp"),
		m_BoxNr+1,
				G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_BeginTestTime.GetYear(),
				G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_BeginTestTime.GetMonth(),
				G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_BeginTestTime.GetDay(),
				G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_BeginTestTime.GetHour(),
				G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_BeginTestTime.GetMinute(),
				G_MainDlg->m_SwitchDlg[m_BoxNr].m_WorkMode.m_BeginTestTime.GetSecond());

	G_MainDlg->m_Tab[m_BoxNr].SetCurSel(3); 
	G_MainDlg->SelectTab(m_BoxNr);
	G_MainDlg->m_CurveDlg[m_BoxNr].RedrawWindow(); 
	
//	G_MainDlg->m_CurveDlg[m_BoxNr].OnExportChart(s);  
	
	G_MainDlg->m_Tab[m_BoxNr].SetCurSel(0); 
	G_MainDlg->SelectTab(m_BoxNr);
}

extern CSorterPlcDlg	G_SorterPlcDlg;
void CSwitchDlg::OnCheckStartTest() 
{

	//MyAfxMessageBox(_T("123"),0);

//	::G_SorterPlcDlg.DoModal();

//	return;

//	this->m_WorkMode.SaveTestRecord(0);
//	return;


	m_WorkMode.m_SwitchConfigPara = this->m_SwitchConfigPara.para;
	m_WorkMode.OnClickStartButton( this,m_BoxNr,m_CurTemp,m_SwitchConfigPara.para);


}

void CSwitchDlg::OnButtonTakeout() 
{
	//RECORD record;
	//CLPM260_Printer::PrintResultHead( 0,&record,0,0);

	//this->SaveTestRecord();

	//return;
	// TODO: Add your control notification handler code here

	m_SwitchConfigPara.para_ex.HeatSpeed = ::G_NormalConfigPara[m_BoxNr].HeatSpeed / 10.0f;

	G_TakeoutDlg.m_BoxNr = this->m_BoxNr;
	
//	Dlg.DoModal();

	//初始化数据
	if(m_WorkMode.m_SubMode != SUB_MODE_STOP)
	{
		G_TakeoutDlg.m_SwitchConfigPara = this->m_SwitchConfigPara;
		for(int j = 0; j < G_iMaxLedNr[m_BoxNr]; j++)
		{
			//memcpy(&G_TakeDlgSwitchTestResult[0][j], &G_SwitchTestResult[m_BoxNr][j], sizeof(G_SwitchTestResult[m_BoxNr][j]));
			G_TakeDlgSwitchTestResult[0][j] = G_SwitchTestResult[m_BoxNr][j];
			//memcpy(&G_TakeDlgSwitchTestResult_Bak[0][j], &G_SwitchTestResult[m_BoxNr][j], sizeof(G_SwitchTestResult[m_BoxNr][j]));
			G_TakeDlgSwitchTestResult_Bak[0][j] = G_SwitchTestResult[m_BoxNr][j];
		}
		/*
		for(int j = 0; j < G_iMaxLedNr[m_BoxNr]; j++)
		{
			if((G_TakeDlgSwitchTestResult[0][j].OpenTemp  > (m_SwitchConfigPara.MainLevel - 7.0f))
				&& (G_TakeDlgSwitchTestResult[0][j].OpenTemp  < m_SwitchConfigPara.MainLevel))
			{
				G_TakeDlgSwitchTestResult[0][j].OpenTemp += (m_SwitchConfigPara.MainLevel - G_TakeDlgSwitchTestResult[0][j].OpenTemp) * 2 / 7;
			}
			if((G_TakeDlgSwitchTestResult[0][j].OpenTemp  < (m_SwitchConfigPara.MainLevel + 7.0f))
				&& (G_TakeDlgSwitchTestResult[0][j].OpenTemp  > m_SwitchConfigPara.MainLevel))
			{
				G_TakeDlgSwitchTestResult[0][j].OpenTemp -= ( G_TakeDlgSwitchTestResult[0][j].OpenTemp - m_SwitchConfigPara.MainLevel ) * 2 / 7;
			}
		}
		*/
		G_TakeoutDlg.m_SwitchConfigPara	=	m_SwitchConfigPara;
		G_TakeoutDlg.m_iTakeBoardType	= 0;
		G_TakeoutDlg.m_fOpenTempShow	= 999.99f;
		G_TakeoutDlg.m_FlashGap			= 1;

		G_TakeoutDlg.m_WindowText		= _T("在线查看"); 
		G_TakeoutDlg.m_ParaName			=  m_ParaName;
		G_TakeoutDlg.m_SwitchNormalOpen = G_NormalConfigPara[m_BoxNr].SwitchType;
		G_TakeoutDlg.m_BeginTestTime	= m_WorkMode.m_BeginTestTime;
		G_TakeoutDlg.m_EndTestTime		= m_WorkMode.m_EndTestTime;
		G_TakeoutDlg.m_iTestMode		= m_iTestMode;

		G_TakeoutDlg.m_iDisplayTempType = this->m_SwitchConfigPara.para.CloseTempType;
		G_TakeoutDlg.m_BoxNr			= m_BoxNr;
		G_TakeoutDlg.m_iSelect			= 0;//2;
		G_TakeoutDlg.m_BoxType			= (BOX_TYPE)G_NormalConfigPara[m_BoxNr].BoxType;
		G_TakeoutDlg.m_UpDownID			= 14;
		G_TakeoutDlg.SetAllSelected();

		CWnd * w;
		CString s;
/*
		w = G_TakeoutDlg.GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), G_TakeoutDlg.m_iFailHighLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

		w = G_TakeoutDlg.GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), G_TakeoutDlg.m_iFailOtherLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

		

		w = G_TakeoutDlg.GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), G_TakeoutDlg.m_iFailLowLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);
*/
		m_IsTaking	=	false;
		G_TakeoutDlg.DoModal();
	}
	else
	{
		G_TakeoutDlg.m_SwitchConfigPara = this->m_SwitchConfigPara;
		for(int j = 0; j < G_iMaxLedNr[m_BoxNr]; j++)
		{
			memcpy(&G_TakeDlgSwitchTestResult[0][j], &G_SwitchTestResult[m_BoxNr][j], sizeof(G_SwitchTestResult[m_BoxNr][j]));
			memcpy(&G_TakeDlgSwitchTestResult_Bak[0][j], &G_SwitchTestResult[m_BoxNr][j], sizeof(G_SwitchTestResult[m_BoxNr][j]));
		}
		/*
		for(int j = 0; j < G_iMaxLedNr[m_BoxNr]; j++)
		{
			if((G_TakeDlgSwitchTestResult[0][j].OpenTemp  > (m_SwitchConfigPara.MainLevel - 7.0f))
				&& (G_TakeDlgSwitchTestResult[0][j].OpenTemp  < m_SwitchConfigPara.MainLevel))
			{
				G_TakeDlgSwitchTestResult[0][j].OpenTemp += (m_SwitchConfigPara.MainLevel - G_TakeDlgSwitchTestResult[0][j].OpenTemp) * 2 / 7;
			}
			if((G_TakeDlgSwitchTestResult[0][j].OpenTemp  < (m_SwitchConfigPara.MainLevel + 7.0f))
				&& (G_TakeDlgSwitchTestResult[0][j].OpenTemp  > m_SwitchConfigPara.MainLevel))
			{
				G_TakeDlgSwitchTestResult[0][j].OpenTemp -= (G_TakeDlgSwitchTestResult[0][j].OpenTemp - m_SwitchConfigPara.MainLevel) * 2 / 7;
			}
		}*/

		
		G_TakeoutDlg.m_iTakeBoardType	= 0;

		G_TakeoutDlg.m_WindowText		= _T("在线拔板"); 
		G_TakeoutDlg.m_ParaName			=  m_ParaName;
		G_TakeoutDlg.m_SwitchNormalOpen = G_NormalConfigPara[m_BoxNr].SwitchType;
		G_TakeoutDlg.m_BeginTestTime	= m_WorkMode.m_BeginTestTime;
		G_TakeoutDlg.m_EndTestTime		= m_WorkMode.m_EndTestTime;
		G_TakeoutDlg.m_iTestMode		= m_iTestMode;
		G_TakeoutDlg.m_BoxNr			= m_BoxNr;
		G_TakeoutDlg.m_iSelect			= 0;
		G_TakeoutDlg.ClearAllSelected(); 
		G_TakeoutDlg.m_iDisplayTempType = this->m_SwitchConfigPara.para.CloseTempType;
		G_TakeoutDlg.m_SwitchNormalOpen = G_NormalConfigPara[m_BoxNr].SwitchType; 
		G_TakeoutDlg.m_FlashGap			= 1;
		G_TakeoutDlg.m_BoxType			= (BOX_TYPE)G_NormalConfigPara[m_BoxNr].BoxType;
		G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr);
		G_TakeoutDlg.m_UpDownID			= 14;
		 
		m_IsTaking	=	true;
		G_TakeoutDlg.DoModal();

	
		m_IsTaking	=	false;
	}
	
	

}

SYSTEMTIME CloseTime,FlashTime;

unsigned char OIL_BIT[64] =
{ 
	 0, 2, 4, 6, 8,10,12,14,
	 1, 3, 5, 7, 9,11,13,15,
	16,18,20,22,24,26,28,30,
	17,19,21,23,25,27,29,31,
	32,34,36,38,40,42,44,46,
	33,35,37,39,41,43,45,47,
	48,50,52,54,56,58,60,62,
	49,51,53,55,57,59,61,63
};
unsigned char OIL_BIT_2[64] =
{
	10, 9,12,11,14,13,16,15, 2, 1,
	 4, 3, 6, 5, 8, 7,26,25,28,27,
	30,29,32,31,18,17,20,19,22,21,
	24,23,42,41,44,43,46,45,48,47,
	34,33,36,35,38,37,40,39,58,57,
	60,59,62,61,64,63,50,49,52,51
};

//用于处理发送测试命令后，判断仪表是否已经执行
void CSwitchDlg::DealCmdAnswer(unsigned char *para, unsigned char *flash,bool IsCmdAscii,int data_len)
{

	this->m_WorkMode.DealCmdAnswer(this,m_BoxNr,para,flash,IsCmdAscii,data_len);

}


void CSwitchDlg::OnRadioClose() 
{
	// TODO: Add your control notification handler code here
	
	G_NormalConfigPara[m_BoxNr].SwitchType = 0;
	::SaveNormalConfigPara(); 
	ReadNormalConfigPara(); 
	this->UpdateData(false); 

}

void CSwitchDlg::OnRadioOpen() 
{

	
	::G_NormalConfigPara[m_BoxNr].SwitchType =  1;
	::SaveNormalConfigPara(); 
	ReadNormalConfigPara(); 
	this->UpdateData(false); 

}



LRESULT CSwitchDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}




extern int AdjustID(int k);
extern TCHAR level_str[18][10];


void CSwitchDlg::SaveExcel(void)
{



	int j;
	TCHAR str[1000];
	TCHAR str1[1000];
	CString  file_name;
	CTime cur = cur.GetCurrentTime();


	
	CString s ;
	s.Format(_T("生产记录%04d%02d%02d%02d%02d%02d"),
		m_WorkMode.m_BeginTestTime.GetYear(), 
		m_WorkMode.m_BeginTestTime.GetMonth(), 
		m_WorkMode.m_BeginTestTime.GetDay(), 
		m_WorkMode.m_BeginTestTime.GetHour(), 
		m_WorkMode.m_BeginTestTime.GetMinute(), 
		m_WorkMode.m_BeginTestTime.GetSecond()); 
	
	CString sDir;
	
	sDir = _T("d:\\测试记录");
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	sDir.Format(_T("d:\\测试记录\\烘箱%d"),m_BoxNr + 1);

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}
	
	sDir.Format(_T("d:\\测试记录\\烘箱%d\\%04d年%02d月"),m_BoxNr + 1,cur.GetYear(), cur.GetMonth());

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	CFile RecFile;

	{
		file_name.Format(_T("%02d日 %02d时 %02d分 %02d秒.csv"), cur.GetDay(), cur.GetHour(), cur.GetMinute(), cur.GetSecond());     
	
		file_name = sDir + _T("\\") + file_name;
		if(RecFile.Open(file_name,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{
			RecFile.Write(&UNICODE_FILE_HEAR,2);

			_stprintf_s(str,_T("测试参数%c\r\n"),CSV_COMMMA);
			RecFile.Write(str,wcslen(str) * sizeof(TCHAR));

			
			_stprintf_s(str,_T("主规格\r\n"));
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));

			_stprintf_s(str,_T("动作温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.para.MainLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.para.MainLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.para.MainLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			_stprintf_s(str,_T("复位温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.para.CloseLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.para.CloseLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.para.CloseLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));


			_stprintf_s(str,_T("高规格\r\n"));
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));

			_stprintf_s(str,_T("动作温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.para.HighLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.para.HighLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.para.HighLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			_stprintf_s(str,_T("复位温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.para.CloseHighLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.para.CloseHighLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.para.CloseHighLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));


			_stprintf_s(str,_T("低规格\r\n"));
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));

			_stprintf_s(str,_T("动作温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.para.LowLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.para.LowLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.para.LowLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			_stprintf_s(str,_T("复位温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.para.CloseLowLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.para.CloseLowLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.para.CloseLowLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			//sprintf_s(str,"序号,动作温度,复位温度,规格,动作时间,复位时间\r\n");


			_stprintf_s(str,_T("序号%c动作温度%c复位温度%c规格\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));
				
			for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
			{
				if(G_SwitchTestResult[m_BoxNr][j].IsUsed)
				{
					if(G_iMaxLedNr[m_BoxNr] == 192)
					{
						if((j % 12) < 9)
						{
							_stprintf_s(str,_T("%d%d%c"),j/12,(j%12)+1 ,CSV_COMMMA);				
						}
						else if((j % 12) == 9)
						{
							_stprintf_s(str,_T("%d0%c"),j/12 + 1,CSV_COMMMA);				
						}
						else if((j % 12) == 10)
						{
							_stprintf_s(str,_T("%dA%c"),j/12 + 1,CSV_COMMMA);				
						}
						else if((j % 12) == 11)
						{
							_stprintf_s(str,_T("%dB%c"),j/12 +1,CSV_COMMMA);				
						}
					}
					else
					{
						_stprintf_s(str,_T("%d%c"),j + 1,CSV_COMMMA);				
					}
					//CTime t  = GetRecordTime(G_SwitchTestResult[m_BoxNr][j].OpenTime); 
					//CTime t1 = GetRecordTime(G_SwitchTestResult[m_BoxNr][j].CloseTime); 
					//sprintf_s(str1,"%5.1f,%5.1f,%s,%04d/%02d/%02d %02d:%02d:%02d,%04d/%02d/%02d %02d:%02d:%02d\r\n",G_SwitchTestResult[m_BoxNr][j].OpenTemp,G_SwitchTestResult[m_BoxNr][j].CloseTemp,level_str_4_csv[CheckTestLevel(m_SwitchConfigPara,G_SwitchTestResult[m_BoxNr][j],HEAT_BOX_TYPE_NORMAL)],
					//	t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
					//	t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());  
					CONTROL_TEMP_RANGE	TempRange;
	
					HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(m_BoxNr,m_SwitchConfigPara.para , &TempRange,G_NormalConfigPara[m_BoxNr].BoxType);

					TEST_RESULT_LEVEL level = CheckTestLevel(m_SwitchConfigPara.para,G_SwitchTestResult[m_BoxNr][j],HeatCool,TempRange, G_SwitchTestResult[m_BoxNr][j],::G_NormalConfigPara[m_BoxNr].BoxType);
					_stprintf_s(str1,_T("%5.1f%c%5.1f%c%s \r\n"),G_SwitchTestResult[m_BoxNr][j].OpenTemp,CSV_COMMMA,G_SwitchTestResult[m_BoxNr][j].CloseTemp,CSV_COMMMA,GetLevelName(level));
						
					wcscat_s(str,str1);
					RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));
				}
				else
				{
					continue;

				}
			}

			RecFile.Close();
		}
	}
	
	
}



void CSwitchDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//初始化数据

	CDialog::OnLButtonDblClk(nFlags, point);


	G_TakeoutDlg.m_SwitchConfigPara = this->m_SwitchConfigPara;
	for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		memcpy(&G_TakeDlgSwitchTestResult[0][i], &G_SwitchTestResult[m_BoxNr][i], sizeof(G_SwitchTestResult[m_BoxNr][i]));
		memcpy(&G_TakeDlgSwitchTestResult_Bak[0][i], &G_SwitchTestResult[m_BoxNr][i], sizeof(G_SwitchTestResult[m_BoxNr][i]));
	}

	G_TakeoutDlg.m_iTakeBoardType = 0;
	
	G_TakeoutDlg.m_FlashGap		= 1;

	G_TakeoutDlg.m_WindowText		= _T("在线查看"); 
	G_TakeoutDlg.m_ParaName			=  m_ParaName;
	G_TakeoutDlg.m_SwitchNormalOpen = G_NormalConfigPara[m_BoxNr].SwitchType;
	G_TakeoutDlg.m_BeginTestTime	= m_WorkMode.m_BeginTestTime;
	G_TakeoutDlg.m_EndTestTime		= m_WorkMode.m_EndTestTime;
	G_TakeoutDlg.m_iTestMode		= m_iTestMode;

	G_TakeoutDlg.m_iDisplayTempType = this->m_SwitchConfigPara.para.CloseTempType;
	G_TakeoutDlg.m_BoxNr = m_BoxNr;
	G_TakeoutDlg.m_iSelect = 0;//2;
	G_TakeoutDlg.m_BoxType	= (BOX_TYPE)G_NormalConfigPara[m_BoxNr].BoxType;
	G_TakeoutDlg.SetAllSelected();
	
	G_TakeoutDlg.DoModal();

}


#ifndef _WORK_MODE_H_
double CSwitchDlg::GetLowestCloseTemp()
{
	int i;
	double temp = 10000;

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i ++)
	{
		if(G_SwitchTestResult[m_BoxNr][i].IsClosed && G_SwitchTestResult[m_BoxNr][i].IsUsed)
		{
			if((G_SwitchTestResult[m_BoxNr][i].CloseTemp < temp) && (G_SwitchTestResult[m_BoxNr][i].CloseTemp>0))
			{
				temp = G_SwitchTestResult[m_BoxNr][i].CloseTemp;
			}
		}
	}
	return temp;
}
#endif

#ifndef _WORK_MODE_H_
int CSwitchDlg::GetHighestCloseTemp()
{
	int i;
	int temp = 0;

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i ++)
	{
		if(G_SwitchTestResult[m_BoxNr][i].IsClosed && G_SwitchTestResult[m_BoxNr][i].IsUsed)
		{
			if(G_SwitchTestResult[m_BoxNr][i].CloseTemp > temp)
			{
				temp = G_SwitchTestResult[m_BoxNr][i].CloseTemp;
			}
		}
	}

	if(temp > m_SwitchConfigPara.para.CloseHighLevel + m_SwitchConfigPara.para.CloseHighLevelSpanPlus)
	{
		temp = m_SwitchConfigPara.para.CloseHighLevel + m_SwitchConfigPara.para.CloseHighLevelSpanPlus;
	}
	temp += 5;

	return temp;
}
#endif



#ifndef _WORK_MODE_H_
void CSwitchDlg::OpenDoor_For_OpenFlashTest()
{
#define HEAT_DEGREE	m_OpenDoorResetHeatDegree
#define COOL_DEGREE	m_OpenDoorResetCoolDegree


	if(m_WorkMode.m_SubMode != SUB_MODE_STEP_PID)
	{
		return;
	}

	if(m_OpenDoorIng == false)
	{
		CTime cur = CTime::GetCurrentTime(); 
		CTimeSpan span = cur - m_OpenDoor_StartTime;
		
		if(((m_OpenDoor_Temp <= m_PidTemp) /*&& OpennedAndNotFlashTested()*/)
			|| (m_PidTemp >=  m_TempRange.open_temp_max)
			|| (span.GetTotalMinutes() >= 5)
			)
		{
			m_OpenDoor_StartTime = CTime::GetCurrentTime(); 
			this->m_TestMsg.Format(_T("开关门降温到%5.1f"),m_OpenDoor_Temp - ::G_NormalConfigPara[m_BoxNr].OpenDoorReset_CoolDegree );
			this->UpdateData(false);
			G_RS485_Comm.SendStopHeatCmd(m_BoxNr);
			G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr);
			m_OpenDoorIng = true;
		}
	}
	else
	{
		CTime cur = CTime::GetCurrentTime(); 
		CTimeSpan span = cur - m_OpenDoor_StartTime;
			

		if(((m_OpenDoor_Temp	- ::G_NormalConfigPara[m_BoxNr].OpenDoorReset_CoolDegree  ) >= m_PidTemp) 
			|| (span.GetTotalMinutes() >= 5))
		{
			m_OpenDoor_StartTime = CTime::GetCurrentTime(); 
			
			m_OpenDoorIng = false;

			

			int toptemp = GetControlGoalTemp(SUB_MODE_STEP_PID);
			
			int minutes = (toptemp - m_OpenDoor_Temp)*10 / G_NormalConfigPara[m_BoxNr].HeatSpeed;

			
			char buf[200];
			sprintf_s(buf,sizeof(buf),"minutes = %d",minutes);
			WriteLogFile(buf,m_BoxNr+1,true);

			m_OpenDoor_Temp += ::G_NormalConfigPara[m_BoxNr].OpenDoorReset_HeatDegree;
			
			this->m_TestMsg.Format(_T("开关门升温到%5.1f"),m_OpenDoor_Temp);
			this->UpdateData(false);
			


			
			if(minutes < 1)
			{//有可能升温速率太小，导致温度升不上去
				//minutes = 1;
				G_RS485_Comm.SendDirectHeatCmd(toptemp+1.0,m_BoxNr);
				G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr); //WriteLogFile("17",m_BoxNr+1,true);
			}
			else
			{
				G_RS485_Comm.SendStepPIDCmd(toptemp,minutes,m_BoxNr);
				G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr); //WriteLogFile("18",m_BoxNr+1,true);
			}
			
			


			/*for(int i = 0; i < MAX_LED_NR; i++)
			{
				if(m_SwitchTestResult[i].IsOpenned == true)
				{
					m_SwitchTestResult[i].IsOpenFlashTested = true;
				}
			}*/
		}
	}

}
#endif

#ifndef _WORK_MODE_H_
BOOL CSwitchDlg::OpennedAndNotFlashTested()
{
	int i;
	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(G_SwitchTestResult[m_BoxNr][i].IsOpenFlashTested == false
			&& G_SwitchTestResult[m_BoxNr][i].IsOpenned == true)
		{
			return true;
		}
	}
	return false;
}
#endif


HBRUSH CSwitchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		//hbr = G_bkBrush;
		 
		break;
	case CTLCOLOR_LISTBOX:	//    List-box control
		break;
	case CTLCOLOR_MSGBOX:	//    Message box
		hbr = G_bkBrush;
		break;
	case CTLCOLOR_SCROLLBAR:	//    Scroll-bar control
		break;
	case CTLCOLOR_STATIC:	//    Static control
		hbr = G_bkBrush;

		if(pWnd->GetDlgCtrlID() == IDC_STATIC_TEST_MSG)
		{
			pDC->SetBkColor(RGB(255, 255, 255)); 
		}
		else
		{
			pDC->SetBkColor(BK_COLOR); 
			
			pDC->SetTextColor(RGB(255, 255, 60));
		}
		break;
	}

	switch(pWnd->GetDlgCtrlID())
	{
	
	case IDC_EDIT_5:
		pDC->SetBkColor(RGB(255,255,255)); 
		pDC->SetTextColor(RGB(255,0,0));
		break;
	case IDC_CHECK_AUTO_PROCESS:
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		break;

	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

#ifndef _WORK_MODE_H_
void CSwitchDlg::OpenDoor_For_CloseFlashTest(void)
{


	int	i;

			if(m_iTestMode == TEST_MODE_CLOSE_DOOR_REPEAT || m_iTestMode == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
			{
				if(m_WorkMode.m_WorkStatus.SubMode == SUB_MODE_DIRECT_COOL)
				{//cool 

					m_CloseDoorTopTemp = 0;
					for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
					{
						if((G_SwitchTestResult[m_BoxNr][i].IsClosed) && (G_SwitchTestResult[m_BoxNr][i].IsCloseFlashTested == false))
						{
							if(m_CloseDoorTopTemp < (int)(G_SwitchTestResult[m_BoxNr][i].CloseTemp + 6.5))
							{
								m_CloseDoorTopTemp =(int)(G_SwitchTestResult[m_BoxNr][i].CloseTemp + 6.5);
							}
						}
					}
					
					CTimeSpan span = CTime::GetCurrentTime() - m_CloseDoorStartTime;

					if(m_Meter.m_IsDoorOpened == false && (m_CloseDoorTopTemp > 0))
					{//开始升温，只要有一个产品复位
						G_RS485_Comm.SendDirectHeatCmd(m_CloseDoorTopTemp + 1,m_BoxNr);
						m_CloseDoorStartTemp = m_PidTemp;
						G_RS485_Comm.SendOpenDoorCmd(false,m_BoxNr);  //WriteLogFile("11",m_BoxNr+1,true);
						m_TestMsg.Format(_T("关门升温"));
						m_CloseDoorStartTime = m_CloseDoorStartTime.GetCurrentTime();
						UpdateData(false);
					}
					else if((m_Meter.m_IsDoorOpened == true)  && ((m_PidTemp > (int)m_CloseDoorTopTemp) || ((span.GetTotalSeconds() > 120))))
					{//开始降温
						G_RS485_Comm.SendStopHeatCmd(m_BoxNr);
						G_RS485_Comm.SendOpenDoorCmd(true,m_BoxNr); //WriteLogFile("12",m_BoxNr+1,true);
						for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
						{
							if((G_SwitchTestResult[m_BoxNr][i].IsClosed) && (G_SwitchTestResult[m_BoxNr][i].IsCloseFlashTested == false))
							{
								G_SwitchTestResult[m_BoxNr][i].IsCloseFlashTested = true;
							}
						}
						m_TestMsg.Format(_T("开门降温"));
						UpdateData(false);
					}
				}
			}


}
#endif
/*
HEAT_COOL_ORDER	CheckHeatOrCoolMode(SWITCH_CONFIG_PARA	SwitchConfigPara,CONTROL_TEMP_RANGE *temp_range)
{
	HEAT_COOL_ORDER order = HEAT_COOL_UNKNOW;

	if(SwitchConfigPara.MainLevel < -50.0f || SwitchConfigPara.MainLevel > 200.0f)
	{
		temp_range->open_temp_max = INVALID_TEMP; 
		temp_range->open_temp_min = INVALID_TEMP; 
		temp_range->close_temp_max = INVALID_TEMP; 
		temp_range->close_temp_min = INVALID_TEMP; 
		return order;
	}

	if((SwitchConfigPara.LowLevelSpanMinus != 0) ||  (SwitchConfigPara.LowLevelSpanPlus != 0))
	{//测低规格
		if((SwitchConfigPara.HighLevelSpanMinus != 0) ||  (SwitchConfigPara.HighLevelSpanPlus != 0))
		{//测低规格,测高规格
			temp_range->open_temp_min  = SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus;

			temp_range->open_temp_max  = SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus; 
			if((SwitchConfigPara.CloseLevelSpanMinus != 0) || (SwitchConfigPara.CloseLevelSpanPlus != 0))
			{//测复位温度
				temp_range->close_temp_min = SwitchConfigPara.CloseLowLevel - SwitchConfigPara.CloseLowLevelSpanMinus;
				if(temp_range->close_temp_min > (SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus))
				{
					temp_range->close_temp_min = (SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus);
				}
				if(temp_range->close_temp_min > (SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseHighLevelSpanMinus))
				{
					temp_range->close_temp_min = (SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseHighLevelSpanMinus);
				}

				temp_range->close_temp_max = SwitchConfigPara.CloseHighLevel+ SwitchConfigPara.CloseHighLevelSpanPlus;
				if(temp_range->close_temp_max <( SwitchConfigPara.CloseLevel+ SwitchConfigPara.CloseLevelSpanPlus))
				{
					temp_range->close_temp_max = ( SwitchConfigPara.CloseLevel+ SwitchConfigPara.CloseLevelSpanPlus);
				}
				if(temp_range->close_temp_max <( SwitchConfigPara.CloseLowLevel+ SwitchConfigPara.CloseLowLevelSpanPlus))
				{
					temp_range->close_temp_max = ( SwitchConfigPara.CloseLowLevel+ SwitchConfigPara.CloseLowLevelSpanPlus);
				}
			//	AfxMessageBox(_T("测低规格,测主规格,测高规格"));
			}
			else
			{//不测复位温度
				temp_range->close_temp_min = 0;
				temp_range->close_temp_max = 0;
			//	AfxMessageBox(_T("测低规格,测主规格,测高规格，不测复位温度"));
			}
			//AfxMessageBox(_T("1"));
		}
		else
		{//测低规格,测主规格
			temp_range->open_temp_min  = SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus;
			temp_range->open_temp_max  = SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus; 
			if((SwitchConfigPara.CloseLevelSpanMinus != 0) || (SwitchConfigPara.CloseLevelSpanPlus != 0))
			{//测复位温度
				temp_range->close_temp_min = SwitchConfigPara.CloseLowLevel - SwitchConfigPara.CloseLowLevelSpanMinus;
				temp_range->close_temp_max = SwitchConfigPara.CloseLevel+ SwitchConfigPara.CloseLevelSpanPlus;
			//	AfxMessageBox(_T("测低规格,测主规格"));
			}
			else
			{//不测复位温度
				temp_range->close_temp_min = 0;
				temp_range->close_temp_max = 0;
			//	AfxMessageBox(_T("测低规格,测主规格，不测复位温度"));
			}
			//AfxMessageBox(_T("2"));
		}
	}
	else if((SwitchConfigPara.HighLevelSpanMinus != 0) ||  (SwitchConfigPara.HighLevelSpanPlus != 0))
	{//测高规格
		//测高规格,测主规格
		temp_range->open_temp_min  = SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus;
		temp_range->open_temp_max  = SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus; 

		if((SwitchConfigPara.CloseLevelSpanMinus != 0) || (SwitchConfigPara.CloseLevelSpanPlus != 0))
		{//测复位温度
			temp_range->close_temp_min = SwitchConfigPara.CloseLevel		- SwitchConfigPara.CloseLevelSpanMinus;
			temp_range->close_temp_max = SwitchConfigPara.CloseHighLevel	+ SwitchConfigPara.CloseHighLevelSpanPlus;
		//	AfxMessageBox(_T("测主规格,测高规格"));
		}
		else
		{//不测复位温度
			temp_range->close_temp_min = 0;
			temp_range->close_temp_max = 0;
		//	AfxMessageBox(_T("测主规格,测高规格，不测复位温度"));
		}
		//AfxMessageBox(_T("3"));
	}
	else
	{//只测主规格、
		temp_range->open_temp_min  = SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus;
		temp_range->open_temp_max  = SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus; 
		if(SwitchConfigPara.CloseLevelSpanMinus != 0 || SwitchConfigPara.CloseLevelSpanPlus != 0)
		{//测复位温度
			temp_range->close_temp_min = SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus;
			temp_range->close_temp_max = SwitchConfigPara.CloseLevel + SwitchConfigPara.CloseLevelSpanPlus;
		//	AfxMessageBox(_T("测主规格"));
		}
		else
		{//不测复位温度
			temp_range->close_temp_min = 0;
			temp_range->close_temp_max = 0;
		//	AfxMessageBox(_T("测主规格，不测复位温度"));
		}
		//AfxMessageBox(_T("4"));
	}
	
	if(temp_range->close_temp_min != temp_range->close_temp_max )
	{//测复位
		if(temp_range->open_temp_max > temp_range->close_temp_max)
		{//先制热，再制冷
			order = HEAT_FIRST_COOL_SECOND;
		}
		else
		{//先制冷，再制热

			order = COOL_FIRST_HEAT_SECOND;
		}
	}
	else
	{//不测复位、
		order = HEAT_FIRST_COOL_SECOND;
		
	}

	//CString s;
	//s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f,%d"),temp_range->open_temp_min,temp_range->open_temp_max,temp_range->close_temp_min,temp_range->close_temp_max,order);
	//AfxMessageBox(s);

	
	return order;
}
*/

#ifndef _WORK_MODE_H_
double CSwitchDlg::GetControlGoalTemp(SUB_MODE step)
{
	double goal_temp = INVALID_TEMP;
	//CONTROL_TEMP_RANGE	range;
	CheckHeatOrCoolMode(m_SwitchConfigPara.para  , &m_TempRange,G_NormalConfigPara[m_BoxNr].BoxType);

	if(m_HeatCoolOrder == HEAT_FIRST_COOL_SECOND)
	{
		switch(step)
		{
		case SUB_MODE_DIRECT_HEAT:
			goal_temp = m_TempRange.open_temp_min - G_PID[m_BoxNr].P - G_PID[m_BoxNr].P * 0.2f;
			
			break;
		case SUB_MODE_PID:
			goal_temp = m_TempRange.open_temp_min - G_PID[m_BoxNr].P * 0.2f;
			
			break;
		case SUB_MODE_STEP_PID:
			goal_temp = m_TempRange.open_temp_max + G_iHighHighLevelDelta + 2.0f;
			break;
		case SUB_MODE_STEP_TOP_PID:
			goal_temp = m_TempRange.open_temp_max + G_iHighHighLevelDelta + 2.0f;
			break;
		case SUB_MODE_DIRECT_COOL:
			goal_temp = m_TempRange.close_temp_min;
			break;
		case SUB_MODE_STEP_COOL:
			goal_temp = m_TempRange.close_temp_min;
			break;
		case SUB_MODE_HEAT_AGAIN:
			goal_temp = GetLowestCloseTemp();
			break;
		}
	}
	else if(m_HeatCoolOrder == COOL_FIRST_HEAT_SECOND)
	{//制冷
		AfxMessageBox(_T("制冷 没有处理此功能\r\n请选择测试参数"));
	}
	return goal_temp;
}
#endif

#ifndef _WORK_MODE_H_
void CSwitchDlg::PrintTestResult(void) 
{
	// TODO: Add your control notification handler code here
	
	CDC dc;
	int i;
	
	CPrintDialog prnDlg(false);
	TCHAR PrinterName[100];
	if(prnDlg.GetDefaults())
	{
		if(prnDlg.m_pd.hDC )
		{
			DeleteDC(prnDlg.m_pd.hDC);
		}

		if(prnDlg.m_pd.hDevNames)
		{
			LPDEVNAMES pdn = (LPDEVNAMES)GlobalLock(prnDlg.m_pd.hDevNames);  
			memset(PrinterName,0,sizeof(PrinterName));
			_tcsncpy_s(PrinterName,(LPTSTR)pdn + pdn->wDeviceOffset,99);
			m_Printer = PrinterName;
			::GlobalUnlock(pdn);
			::GlobalFree(pdn); 
		}
	}

	
	if(!dc.CreateDC(_T(""),m_Printer,_T(""),NULL))
	{
		AfxMessageBox(_T("请设置打印机"));
		return;
	}	
	
	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	CString strTitle;
	this->GetWindowText(strTitle); 
	CString app_name;
	AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += "[";
	app_name += strTitle;
	app_name += "]";
	docInfo.lpszDocName =   app_name;


	i=dc.StartDoc(&docInfo);  
	i=	dc.StartPage();

	CString BoxNr;


	BoxNr.Format(_T("%d"), m_BoxNr + 3-2);

	//AfxMessageBox("打印1");
	
	CString switchtype;
	if(G_NormalConfigPara[m_BoxNr].SwitchType)
	{
		switchtype = _T("常开");
	}
	else
	{
		switchtype = _T("常闭");
	}
	
	SWITCH_TEST_RESULT_EX SwitchTestResult_Ex[192];

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		SwitchTestResult_Ex[i].ID = i;
		SwitchTestResult_Ex[i].result = G_SwitchTestResult[m_BoxNr][i];
	}

	PrintReport_ByGroup(&dc,app_name, m_BoxNr,
				m_BeginTestTime,m_EndTestTime,
				m_ParaName, m_TestMemo,
				switchtype,
				m_SwitchConfigPara, SwitchTestResult_Ex,
				TRUE,(BOX_TYPE)G_NormalConfigPara[m_BoxNr].BoxType,0);


	i =	dc.EndPage(); 
	
	i =	dc.EndDoc(); 
	
	dc.DeleteDC();

}
#endif

#ifndef _WORK_MODE_H_

bool CSwitchDlg::IsPT100Caled(void) 
{
	
	if(G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustLow[0] == 0.0f
		&& G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustLow[1] == 0.0f
		&& G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustLow[2] == 0.0f
		&& G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustLow[3] == 0.0f
	)
	{
		return false;
	}

	if(G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustHigh[0] == 0.0f
		&& G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustHigh[1] == 0.0f
		&& G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustHigh[2] == 0.0f
		&& G_MainDlg->m_DebugDlg[m_BoxNr].m_AdjustHigh[3] == 0.0f
	)
	{
		return false;
	}
	 
	return true;
}
#endif

void CSwitchDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	
	CDialog::OnClose();
}

