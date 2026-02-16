// ChangLiangTongJi.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "ChanLiangTongJiDlg.h"
#include "SwitchDlg.h"
#include "common.h"

//d:\work\VC\HeatBox_V2\ChanLiangTongJiDlg.cpp
// CChangLiangTongJi 对话框

IMPLEMENT_DYNAMIC(CChanLiangTongJiDlg, CDialog)

CChanLiangTongJiDlg::CChanLiangTongJiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChanLiangTongJiDlg::IDD, pParent)
{
	m_BoxNr = 0;
}

CChanLiangTongJiDlg::~CChanLiangTongJiDlg()
{
}

void CChanLiangTongJiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_StartTime);
	DDX_Control(pDX, IDC_MONTHCALENDAR2, m_EndTime);
}


BEGIN_MESSAGE_MAP(CChanLiangTongJiDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &CChanLiangTongJiDlg::OnBnClickedButtonView)
END_MESSAGE_MAP()


// CChangLiangTongJi 消息处理程序
extern void TraverseFolder(LPCTSTR lpPath,LPCTSTR file_ext,CStringArray *fileStr);

void CChanLiangTongJiDlg::OnBnClickedButtonView()
{
	// TODO: 在此添加控件通知处理程序代码



	int m_iCloseFlashLevel = 0;
	int m_iCloseHighLevel = 0;
	int m_iCloseLowLevel = 0;
	int m_iHighLevel = 0;
	int m_iLowLevel = 0;
	int m_iMainLevel = 0;
	int m_iNotUsedLevel = 0;
	int m_iOpenDoorResetLevel = 0;
	int m_iOpenFlashLevel = 0;
	int m_iOverHighLevel = 0;
	int m_iOverLowLevel = 0;
	int m_iTotal = 0;

	CString s;

	
	CTimeSpan span(1,0,0,0);
	this->UpdateData();
	
	CFile RecFile;
	RECORD	record;
	CString fName,ym_str;
	CFileFind ff;
	



	CStringArray sa;
	CString sDir;

	CTime start,end;
	m_StartTime.GetCurSel(start) ;
	m_EndTime.GetCurSel(end);
	while(start <= end)
	{
	sDir = GetDateDirection(m_BoxNr,start);
	start +=span;

	sDir += _T("\\");

	TraverseFolder(sDir,_T("*.record"),&sa);

	while(sa.GetCount() > 0)
	{
		CString s;
		s =  sa.GetAt(0);

		if(RecFile.Open(sDir+s,CFile::modeRead))
		{
			CTime m_FileTime;
			
			CFileStatus fs;
			if(CFile::GetStatus(sDir+s,fs))
			{
				m_FileTime =  fs.m_ctime;
			}

		//	if(m_FileTime.GetYear() == cur.GetYear() &&  m_FileTime.GetMonth() == cur.GetMonth() && m_FileTime.GetDay() == cur.GetDay())
			{
			
				if(RecFile.Read(&record,sizeof(record)) <= sizeof(record)) 
				{
					for(int i = 0; i < ::G_iMaxLedNr[m_BoxNr]; i++)
					{
						
						switch(CheckTestLevel(record.ConfigPara,record.TestResult[i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
							{
							case MAIN_LEVEL:
								m_iMainLevel++;
								break;
							case TEST_RESULT_HIGH_LEVEL:
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
							//case HIGH_LEVEL_CLOSE_HIGH_LEVEL:
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
			RecFile.Close(); 
			sa.RemoveAt(0);
		}
	}
	}

	m_iTotal = 
	m_iCloseFlashLevel +
	m_iCloseHighLevel +
	m_iCloseLowLevel +
	m_iHighLevel +
	m_iLowLevel +
	m_iMainLevel +
	//m_iNotUsedLevel +
	m_iOpenDoorResetLevel +
	m_iOpenFlashLevel +
	m_iOverHighLevel +
	m_iOverLowLevel;

	s.Format(_T("主规格:%8d\r\n高规格:%8d\r\n低规格:%8d\r\n高温品:%8d\r\n低温品:%8d\r\n复位大:%8d\r\n复位小:%8d\r\n动作闪:%8d\r\n复位闪:%8d\r\n总产量:%8d\r\n"),
				m_iMainLevel,
				m_iHighLevel,
				m_iLowLevel,
				m_iOverHighLevel,
				m_iOverLowLevel,
				m_iCloseLowLevel,
				m_iCloseHighLevel,
				m_iOpenFlashLevel,
				m_iCloseFlashLevel,
				m_iTotal);

	AfxMessageBox(s);

}
