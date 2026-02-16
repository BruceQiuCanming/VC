// ChanliangTongjiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "ChanliangTongjiDlg.h"
#include "ChanliangRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChanliangTongjiDlg dialog


CChanliangTongjiDlg::CChanliangTongjiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChanliangTongjiDlg::IDD, pParent)
{
	m_BoxNr = 0;
	//{{AFX_DATA_INIT(CChanliangTongjiDlg)
	m_iCloseFlashLevel = 0;
	m_iCloseHighLevel = 0;
	m_iCloseLowLevel = 0;
	m_iHighLevel = 0;
	m_iLowLevel = 0;
	m_iMainLevel = 0;
	m_iNotUsedLevel = 0;
	m_iOpenDoorResetLevel = 0;
	m_iOpenFlashLevel = 0;
	m_iOverHighLevel = 0;
	m_iOverLowLevel = 0;
	m_iTotal = 0;
	//}}AFX_DATA_INIT

	m_BeginTime = CTime(2000,1,1,0,0,0);

	m_EndTime	= CTime(2000,1,1,23,59,59);
	

}


void CChanliangTongjiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChanliangTongjiDlg)
	DDX_Control(pDX, IDC_MONTHCALENDAR2, m_EndDay);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_BeginDay);
	DDX_Text(pDX, IDC_EDIT_CLOSE_FLASH_LEVEL, m_iCloseFlashLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL, m_iCloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL, m_iCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL, m_iHighLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL, m_iLowLevel);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL, m_iMainLevel);
	DDX_Text(pDX, IDC_EDIT_NO_ACTION_LEVEL, m_iNotUsedLevel);
	DDX_Text(pDX, IDC_EDIT_OPEN_DOOR_RESET, m_iOpenDoorResetLevel);
	DDX_Text(pDX, IDC_EDIT_OPEN_FLASH_LEVEL, m_iOpenFlashLevel);
	DDX_Text(pDX, IDC_EDIT_OVER_HIGH_LEVEL, m_iOverHighLevel);
	DDX_Text(pDX, IDC_EDIT_OVER_LOW_LEVEL, m_iOverLowLevel);
	DDX_Text(pDX, IDC_EDIT_TOTAL, m_iTotal);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_BeginTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_TIME, m_EndTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChanliangTongjiDlg, CDialog)
	//{{AFX_MSG_MAP(CChanliangTongjiDlg)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_NOTIFY(MCN_GETDAYSTATE, IDC_MONTHCALENDAR1, OnGetdaystateMonthcalendar1)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CSV, OnButtonSaveCsv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChanliangTongjiDlg message handlers
extern TEST_RESULT_LEVEL CheckTestLevel(SWITCH_CONFIG_PARA SwitchConfigPara,
			SWITCH_TEST_RESULT SwitchTestResult,HEAT_BOX_TYPE HeatBoxType);

void CChanliangTongjiDlg::OnButtonView() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(); 
	long start_minutes,end_minutes;

	m_iCloseFlashLevel = 0;
	m_iCloseHighLevel = 0;
	m_iCloseLowLevel = 0;
	m_iHighLevel = 0;
	m_iLowLevel = 0;
	m_iMainLevel = 0;
	m_iNotUsedLevel = 0;
	m_iOpenDoorResetLevel = 0;
	m_iOpenFlashLevel = 0;
	m_iOverHighLevel = 0;
	m_iOverLowLevel = 0;
	m_iTotal = 0;

	CString s;
	SYSTEMTIME sys;
	m_BeginDay.GetCurSel(&sys);
	
	CTimeSpan span;
	CTime start_time(sys.wYear,sys.wMonth,sys.wDay,m_BeginTime.GetHour(),m_BeginTime.GetMinute(),0);
	span = start_time - STD_TIME;
	start_minutes = span.GetTotalMinutes();

	m_EndDay.GetCurSel(&sys);
	CTime end_time(sys.wYear,sys.wMonth,sys.wDay,m_EndTime.GetHour(),m_EndTime.GetMinute(),0);
	span = end_time - STD_TIME;
	end_minutes = span.GetTotalMinutes();

	
	CFile RecFile;
	RECORD	record;
	CString fName,ym_str;
	CFileFind ff;
	
	
	fName.Format(_T("log\\RecFile%04d_%d.bin"),sys.wYear,m_BoxNr);    
	
	if(RecFile.Open(theAppDirectory+fName,CFile::modeRead))
	{
		RecFile.SeekToBegin();
		while(1)
		{
			if(RecFile.Read(&record,sizeof(record)) == sizeof(record)) 
			{
				if(record.size == sizeof(record))
				{
					long total_minutes;
					total_minutes = record.BeginStart_From201001010000Minutes;
					if(total_minutes >= start_minutes && total_minutes <= end_minutes)
					{
						for(int i = 0; i < G_iMaxLedNr; i++)
						{
							switch(CheckTestLevel(record.ConfigPara,record.TestResult[i],(HEAT_BOX_TYPE)record.SwitchType))
							{
							case MAIN_LEVEL:
								m_iMainLevel++;
								break;
							case HIGH_LEVEL:
								m_iHighLevel++;
								break;
							case LOW_LEVEL:
								m_iLowLevel ++;
								break;
							
							case OVER_HIGH_LEVEL:
								m_iOverHighLevel ++;
								break;
							case OVER_LOW_LEVEL:
								m_iOverLowLevel ++;
								break;
							case CLOSE_HIGH_LEVEL:
								m_iCloseHighLevel ++;
								break;
							case CLOSE_LOW_LEVEL:
								m_iCloseLowLevel ++;
								break;
							
							case OPEN_FLASH_LEVEL:
								m_iOpenFlashLevel ++;
								break;
							
							case CLOSE_FLASH_LEVEL:
								m_iCloseFlashLevel++;
								break;

							case OPEN_DOOR_RESET_LEVEL:
								m_iOpenDoorResetLevel ++;
								break;

							case NOT_USED:
								//m_iNotUsedLevel ++;
								break;
							
							case NO_LEVEL:
							case HIGH_HIGH_LEVEL:
							case NO_PTC_LEVEL:
							case HIGH_LEVEL_CLOSE_HIGH_LEVEL:
							case HIGH_LEVEL_CLOSE_LOW_LEVEL:
							case LOW_LEVEL_CLOSE_HIGH_LEVEL:
							case LOW_LEVEL_CLOSE_LOW_LEVEL:
							default:
								m_iNotUsedLevel ++;
								break;
							}
						}
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
			
		}
		RecFile.Close();
	}	

	m_iTotal = 
	m_iCloseFlashLevel +
	m_iCloseHighLevel +
	m_iCloseLowLevel +
	m_iHighLevel +
	m_iLowLevel +
	m_iMainLevel +
	m_iNotUsedLevel +
	m_iOpenDoorResetLevel +
	m_iOpenFlashLevel +
	m_iOverHighLevel +
	m_iOverLowLevel;
	
	this->UpdateData(false);

}

void CChanliangTongjiDlg::OnGetdaystateMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(); 
	*pResult = 0;
}

void CChanliangTongjiDlg::FindRecord()
{
	CString s;
	SYSTEMTIME sys;
	this->m_BeginDay.GetCurSel(&sys);
	s.Format(_T("%04d%02d%02d"),sys.wYear,sys.wMonth,sys.wDay);  
	
}

void CChanliangTongjiDlg::OnButtonSaveCsv() 
{

	// TODO: Add your control notification handler code here
	static TCHAR szFilter[] = _T("Excel Files (*.csv)|*.csv|");
	CString  file_name;
	
	file_name = _T("c:\\¼ìÑéÍ³¼Æ.csv");
	CFileDialog dlg(false,_T("*.csv"),file_name,NULL,szFilter);



	this->UpdateData(); 
	
	m_iCloseFlashLevel = 0;
	m_iCloseHighLevel = 0;
	m_iCloseLowLevel = 0;
	m_iHighLevel = 0;
	m_iLowLevel = 0;
	m_iMainLevel = 0;
	m_iNotUsedLevel = 0;
	m_iOpenDoorResetLevel = 0;
	m_iOpenFlashLevel = 0;
	m_iOverHighLevel = 0;
	m_iOverLowLevel = 0;
	m_iTotal = 0;


	CString s;
	SYSTEMTIME sys;
	this->m_BeginDay.GetCurSel(&sys);
	s.Format(_T("%04d%02d%02d"),sys.wYear,sys.wMonth,sys.wDay);  
	
	


	this->UpdateData(false);
	
}
