// RecordViewDlg.cpp : implementation file
//4821 6825

#include "stdafx.h"
#include "HeatBox.h"
#include "RecordViewDlg.h"
#include "ProductTempBar.h"
#include "RecordPrintPriew.h"
#include "ChanliangTongjiDlg.h"
#include "SwitchDlg.h"
#include "TakeDlg.h"
#include <stdio.h>
#include "ChanliangTongjiDlg.h"
#include "FullScreenCurve.h"
#include "LPM260_Printer.h"
#include "common.h"
#include "ListDlg.h"


#include "shlwapi.h"
#include "HttpTools.h"


#pragma comment(lib,"shlwapi.lib")

CString G_PdfPrinterName;
CDC		G_PdfPrinterDC;
int		G_PdfPrintFormat;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRecordViewDlg dialog
/*
CString strParaName[] =
{
		"产品规格",
		"产品类型",
	    "主规格",
		"主规负偏差",
		"主规正偏差",
		"高规格",
		"高规负偏差",
		"高规正偏差",
		"低规格",
		"低规负偏差",
		"低规正偏差",
		"主规复位温度",
		"主规复位负偏差",
		"主规复位正偏差",
		"高规复位温度",
		"高规复位负偏差",
		"高规复位正偏差",
		"低规复位温度",
		"低规复位负偏差",
		"低规复位正偏差",
		"开门复位"
};

*/


















CRecordViewDlg::CRecordViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordViewDlg::IDD, pParent)
{
	m_BoxNr = 0;
	//{{AFX_DATA_INIT(CRecordViewDlg)
	m_Para = _T("");
	//}}AFX_DATA_INIT

}


void CRecordViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordViewDlg)
	DDX_Control(pDX, IDC_LIST_RECORD, m_ListCtrlRecord);

	DDX_Text(pDX, IDC_STATIC_PARA, m_Para);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_SORT_TYPE, m_ComboBoxSortType);
	DDX_Control(pDX, IDC_COMBO_PRINT_TYPE, m_ComboPrintType);
	DDX_Control(pDX, IDC_TREE1, m_DirTreeCtrl);
}


BEGIN_MESSAGE_MAP(CRecordViewDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordViewDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_PRN, OnButtonPrn)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_BAR, OnButtonTempBar)
	ON_BN_CLICKED(IDC_BUTTON_PRN_PREVIEW, OnButtonPrnPreview)
	ON_BN_CLICKED(IDC_BUTTON_TONGJI, OnButtonTongji)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, OnButtonSaveas)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS2, &CRecordViewDlg::OnBnClickedButtonSaveas2)
	ON_BN_CLICKED(IDC_BUTTON_TAKEDLG, &CRecordViewDlg::OnBnClickedButtonTakedlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT_TYPE, &CRecordViewDlg::OnCbnSelchangeComboSortType)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINT_TYPE, &CRecordViewDlg::OnCbnSelchangeComboPrintType)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_TEMP_SHOW, &CRecordViewDlg::OnBnClickedButtonOpenTempShow)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CRecordViewDlg::OnNMClickTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CRecordViewDlg::OnNMDblclkTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CRecordViewDlg::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_CURVE, &CRecordViewDlg::OnBnClickedButtonTempCurve)
	ON_BN_CLICKED(IDC_BUTTON_MES, &CRecordViewDlg::OnBnClickedButtonMes)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordViewDlg message handlers

BOOL CRecordViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	CRect rect;

	
	
	m_ListCtrlRecord.GetClientRect(&rect); 
	m_ListCtrlRecord.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	this->m_ListCtrlRecord.InsertColumn(0,_T("编号"),    LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(1,_T("有产品"),  LVCFMT_LEFT,70);  
	this->m_ListCtrlRecord.InsertColumn(2,_T("动作"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(3,_T("复位"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(4,_T("动闪"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(5,_T("复闪"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(6,_T("动作温度"),LVCFMT_LEFT,80);  
	this->m_ListCtrlRecord.InsertColumn(7,_T("复位温度"),LVCFMT_LEFT,80);  
 	this->m_ListCtrlRecord.InsertColumn(8,_T("产品规格"),LVCFMT_LEFT,80); 
	this->m_ListCtrlRecord.InsertColumn(9,_T("动闪温度"),LVCFMT_LEFT,80); 
	this->m_ListCtrlRecord.InsertColumn(10,_T("复闪温度"),LVCFMT_LEFT,80); 
	this->m_ListCtrlRecord.InsertColumn(11,_T("动作时间"),LVCFMT_LEFT,80);  
	this->m_ListCtrlRecord.InsertColumn(12,_T("复位时间"),LVCFMT_LEFT,80);  
 	
	m_ComboBoxSortType.AddString(_T("按测试序号")); 
	m_ComboBoxSortType.AddString(_T("按温度由高到低"));
	m_ComboBoxSortType.AddString(_T("按温度由低到高"));
	m_ComboBoxSortType.SetCurSel(0);

	memset(m_SwitchTestResult_Ex,0,sizeof(m_SwitchTestResult_Ex));



	// TODO: Add extra initialization here
	
	this->m_ComboPrintType.SetCurSel(AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("PRINT TYPE"),0)); 

	HTREEITEM m_TreeRoot = NULL;//m_DirTreeCtrl.InsertItem(_T("测试记录"),0,0);//插入根节点
	CString sDir;
	sDir.Format(_T("\\log\\box%d\\"),this->m_BoxNr + 1);
	ShowFile(::theAppDirectory + sDir , m_TreeRoot);//以E:\\test为根目录进行遍历

	m_DirTreeCtrl.Expand(m_TreeRoot,TVE_EXPAND);

	this->UpdateData(false); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecordViewDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}


 
extern void TraverseFolder(LPCTSTR lpPath,LPCTSTR file_ext,CStringArray *fileStr);



extern TCHAR level_str[18][10];

void CRecordViewDlg::UpdateRecordDetail(CString begin, CString end)
{
		CFile RecFile;
		CString fName,ym_str;
		CFileFind ff;
		
		CString header;
		CString s;
		CTimeSpan BeginSpan,EndSpan;
		int years,months,days,hours,minutes,seconds;
		
		years = (begin.GetAt(0) - '0')*1000
				+(begin.GetAt(1) - '0')*100
				+(begin.GetAt(2) - '0')*10
				+(begin.GetAt(3) - '0');

		months = (begin.GetAt(5) - '0')*10
				+(begin.GetAt(6) - '0');

		days	= (begin.GetAt(8) - '0')*10
				+(begin.GetAt(9) - '0');

		hours	= (begin.GetAt(11) - '0')*10
				+(begin.GetAt(12) - '0');

		minutes	= (begin.GetAt(14) - '0')*10
				+(begin.GetAt(15) - '0');

		seconds= (begin.GetAt(17) - '0')*10
				+(begin.GetAt(18) - '0');

		s.Format(_T("log\\box%d\\%04d%02d%02d%02d%02d%02d-"),m_BoxNr+1,years,months,days,hours,minutes,seconds); 
		fName = s;
		m_BeginTestTime = CTime(years,months,days,hours,minutes,0);
		
		BeginSpan = m_BeginTestTime - STD_TIME;

		years = (end.GetAt(0) - '0')*1000
				+(end.GetAt(1) - '0')*100
				+(end.GetAt(2) - '0')*10
				+(end.GetAt(3) - '0');

		months = (end.GetAt(5) - '0')*10
				+(end.GetAt(6) - '0');

		days	= (end.GetAt(8) - '0')*10
				+(end.GetAt(9) - '0');

		hours	= (end.GetAt(11) - '0')*10
				+(end.GetAt(12) - '0');

		minutes	= (end.GetAt(14) - '0')*10
				+(end.GetAt(15) - '0');

		seconds= (end.GetAt(17) - '0')*10
				+(end.GetAt(18) - '0');
		s.Format(_T("%04d%02d%02d%02d%02d%02d.record"),years,months,days,hours,minutes,seconds); 
		fName += s;

		m_EndTestTime = CTime(years,months,days,hours,minutes,0);
		EndSpan = m_EndTestTime - STD_TIME;

		header = begin;
		header +=",";
		header +=end;
		
		m_ListCtrlRecord.DeleteAllItems(); 
		fName = theAppDirectory + fName;
		if(RecFile.Open(fName,CFile::modeRead))
		{
			RecFile.SeekToBegin();
			RecFile.Read(&m_SelectedRecord,sizeof(m_SelectedRecord));
			RecFile.Close();
		}
		else
		{
			AfxMessageBox(_T("没有找到文件"));
			return;
		}

		int i = 0,j;

						CString strTemp;
						
						m_SwitchConfigPara.para			= m_SelectedRecord.ConfigPara;
						m_SwitchConfigPara.para_ex		= m_SelectedRecord.ConfigParaEx;
						if(m_SelectedRecord.SwitchType == 0)
						{
							m_TestProductType  = _T("常闭");
						}
						else
						{
							m_TestProductType  = _T("常开");
						}

						m_TestParaName.Format(_T("%s"),m_SelectedRecord.ParaName); 
						
						m_TestMemo.Format(_T("%s"),m_SelectedRecord.Memo);  
					 
						for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
						{	
							//m_SwitchTestResult[j] = record.TestResult[j];
							m_SwitchTestResult_Ex[j].ID = j;
							m_SwitchTestResult_Ex[j].result = m_SelectedRecord.TestResult[j];
						}
						CONTROL_TEMP_RANGE temp_range;

						HEAT_COOL_ORDER HeatOrCool = ::CheckHeatOrCoolMode(m_BoxNr,m_SelectedRecord.ConfigPara,&temp_range,m_SelectedRecord.BoxType); 

						SortRecord();

						for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
						{
							int item_count = m_ListCtrlRecord.GetItemCount(); 
							CString s;
							s.Format(_T("% 3d"),m_SwitchTestResult_Ex[j].ID+1); 
							m_ListCtrlRecord.InsertItem(item_count,s);
							if(m_SwitchTestResult_Ex[j].result.IsUsed)
							{
								s = "Y";
							
								m_ListCtrlRecord.SetItemText(item_count,1,s);	

								if(m_SwitchTestResult_Ex[j].result.IsOpenned)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,2,s);	

								if(m_SwitchTestResult_Ex[j].result.IsClosed )
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,3,s);	 	
								
								if(m_SwitchTestResult_Ex[j].result.IsOpenFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,4,s);

								if(m_SwitchTestResult_Ex[j].result.IsCloseFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,5,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenTemp);
								m_ListCtrlRecord.SetItemText(item_count,6,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseTemp);
								m_ListCtrlRecord.SetItemText(item_count,7,s);
								
								TEST_RESULT_LEVEL level =CheckTestLevel(m_SelectedRecord.ConfigPara,m_SwitchTestResult_Ex[j].result,HeatOrCool,temp_range,m_SwitchTestResult_Ex[j].result,m_SelectedRecord.BoxType);
								m_ListCtrlRecord.SetItemText(item_count,8,level_str[level]);


								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,9,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,10,s);

								CTime t = GetRecordTime(m_SwitchTestResult_Ex[j].result.OpenTime);
								m_ListCtrlRecord.SetItemText(item_count,11,t.Format(_T("%H:%M:%S")));
								
								t = GetRecordTime(m_SwitchTestResult_Ex[j].result.CloseTime);
								m_ListCtrlRecord.SetItemText(item_count,12,t.Format(_T("%H:%M:%S")));
							}
							else
							{
								continue;
								s = "无产品";
								m_ListCtrlRecord.SetItemText(item_count,1,s);
								m_ListCtrlRecord.SetItemText(item_count,2,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,3,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,4,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,5,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,6,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,7,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,8,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,9,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,10,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,11,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,12,_T("-"));
							}
						}

						float open_max_temp, open_min_temp,open_avr_temp,close_max_temp,close_min_temp,close_avr_temp;
						int total_product,total_main_level;

						CString str;
						m_Para = "";
						str.Format(_T("主规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),m_SelectedRecord.ConfigPara.MainLevel,m_SelectedRecord.ConfigPara.MainLevelSpanMinus,m_SelectedRecord.ConfigPara.MainLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("主复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),m_SelectedRecord.ConfigPara.CloseLevel,m_SelectedRecord.ConfigPara.CloseLevelSpanMinus,m_SelectedRecord.ConfigPara.CloseLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("高规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),m_SelectedRecord.ConfigPara.HighLevel,m_SelectedRecord.ConfigPara.HighLevelSpanMinus,m_SelectedRecord.ConfigPara.HighLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("高复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),m_SelectedRecord.ConfigPara.CloseHighLevel,m_SelectedRecord.ConfigPara.CloseHighLevelSpanMinus,m_SelectedRecord.ConfigPara.CloseHighLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("低规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),m_SelectedRecord.ConfigPara.LowLevel,m_SelectedRecord.ConfigPara.LowLevelSpanMinus,m_SelectedRecord.ConfigPara.LowLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("低复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),m_SelectedRecord.ConfigPara.CloseLowLevel,m_SelectedRecord.ConfigPara.CloseLowLevelSpanMinus,m_SelectedRecord.ConfigPara.CloseLowLevelSpanPlus);      
						m_Para += str;
						this->TongJi(&m_SelectedRecord,&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level);
						
						str.Format(_T("产品数:%d 合格品:%d 合格率:%5.1f%%\r\n动作最高温:%5.1f 动作最低温:%5.1f  动作平均温:%5.1f \r\n复位最高温:%5.1f 复位最低温:%5.1f  复位平均温:%5.1f"),total_product,total_main_level,(float)(total_main_level*100.0f/total_product),
							open_max_temp,open_min_temp,(open_avr_temp),close_max_temp,close_min_temp,(close_avr_temp));
						m_Para += str;

						this->UpdateData(false);
						

}

void CRecordViewDlg::UpdateRecordDetail(CString FileName)
{
		CFile RecFile;
		
		CFileFind ff;
	
		CString header;
		CString s;

		
		m_ListCtrlRecord.DeleteAllItems(); 
		s.Format(_T("box%d\\"),m_BoxNr + 1); 
		FileName = theAppDirectory + _T("log\\") + s + FileName;
		if(RecFile.Open(FileName,CFile::modeRead))
		{
			RecFile.SeekToBegin();
			RecFile.Read(&m_SelectedRecord,sizeof(m_SelectedRecord));
			RecFile.Close();
		}
		else
		{
			AfxMessageBox(_T("没有找到文件"));
			return;
		}

		CONTROL_TEMP_RANGE temp_range;

		HEAT_COOL_ORDER HeatOrCool = ::CheckHeatOrCoolMode(m_BoxNr,m_SelectedRecord.ConfigPara,&temp_range,m_SelectedRecord.BoxType); 

		m_BeginTestTime =	GetRecordTime(m_SelectedRecord.StartTime  * 60L);

		m_EndTestTime	=	GetRecordTime(m_SelectedRecord.EndTime    * 60L);

		int i = 0,j;

						CString strTemp;
						
						m_SwitchConfigPara.para		= m_SelectedRecord.ConfigPara;
						m_SwitchConfigPara.para_ex  = m_SelectedRecord.ConfigParaEx;
						

						if(m_SelectedRecord.SwitchType == 0)
						{
							m_TestProductType  = _T("常闭");
						}
						else
						{
							m_TestProductType  = _T("常开");
						}

						m_TestParaName.Format(_T("%s"),m_SelectedRecord.ParaName); 
						
						m_TestMemo.Format(_T("%s"),m_SelectedRecord.Memo);  
					 
						for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
						{	
							//m_SwitchTestResult[j] = m_SelectedRecord.TestResult[j];
							m_SwitchTestResult_Ex[j].ID = j;
							m_SwitchTestResult_Ex[j].result = m_SelectedRecord.TestResult[j];
						}
						
						SortRecord();

						for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
						{
							int item_count = m_ListCtrlRecord.GetItemCount(); 
							CString s;
							s.Format(_T("% 3d"),m_SwitchTestResult_Ex[j].ID+1); 
							m_ListCtrlRecord.InsertItem(item_count,s);
							if(m_SwitchTestResult_Ex[j].result.IsUsed)
							{
								s = "Y";
							
								m_ListCtrlRecord.SetItemText(item_count,1,s);	

								if(m_SwitchTestResult_Ex[j].result.IsOpenned)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,2,s);	

								if(m_SwitchTestResult_Ex[j].result.IsClosed )
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,3,s);	 	
								
								if(m_SwitchTestResult_Ex[j].result.IsOpenFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,4,s);

								if(m_SwitchTestResult_Ex[j].result.IsCloseFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,5,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenTemp);
								m_ListCtrlRecord.SetItemText(item_count,6,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseTemp);
								m_ListCtrlRecord.SetItemText(item_count,7,s);
								
								TEST_RESULT_LEVEL level =CheckTestLevel(m_SelectedRecord.ConfigPara,m_SwitchTestResult_Ex[j].result,HeatOrCool,temp_range,m_SwitchTestResult_Ex[j].result,m_SelectedRecord.BoxType);
								m_ListCtrlRecord.SetItemText(item_count,8,level_str[level]);


								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,9,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,10,s);

								CTime t = GetRecordTime(m_SwitchTestResult_Ex[j].result.OpenTime);
								m_ListCtrlRecord.SetItemText(item_count,11,t.Format(_T("%H:%M:%S")));
								
								t = GetRecordTime(m_SwitchTestResult_Ex[j].result.CloseTime);
								m_ListCtrlRecord.SetItemText(item_count,12,t.Format(_T("%H:%M:%S")));
							}
							else
							{
								continue;
								s = "无产品";
								m_ListCtrlRecord.SetItemText(item_count,1,s);
								m_ListCtrlRecord.SetItemText(item_count,2,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,3,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,4,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,5,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,6,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,7,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,8,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,9,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,10,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,11,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,12,_T("-"));
							}
						}

						float open_max_temp, open_min_temp,open_avr_temp,close_max_temp,close_min_temp,close_avr_temp;
						int total_product,total_main_level;

						CString str;
						m_Para = "";
						str.Format(_T("主规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),m_SelectedRecord.ConfigPara.MainLevel,m_SelectedRecord.ConfigPara.MainLevelSpanMinus,m_SelectedRecord.ConfigPara.MainLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("主复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),m_SelectedRecord.ConfigPara.CloseLevel,m_SelectedRecord.ConfigPara.CloseLevelSpanMinus,m_SelectedRecord.ConfigPara.CloseLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("高规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),m_SelectedRecord.ConfigPara.HighLevel,m_SelectedRecord.ConfigPara.HighLevelSpanMinus,m_SelectedRecord.ConfigPara.HighLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("高复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),m_SelectedRecord.ConfigPara.CloseHighLevel,m_SelectedRecord.ConfigPara.CloseHighLevelSpanMinus,m_SelectedRecord.ConfigPara.CloseHighLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("低规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),m_SelectedRecord.ConfigPara.LowLevel,m_SelectedRecord.ConfigPara.LowLevelSpanMinus,m_SelectedRecord.ConfigPara.LowLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("低复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),m_SelectedRecord.ConfigPara.CloseLowLevel,m_SelectedRecord.ConfigPara.CloseLowLevelSpanMinus,m_SelectedRecord.ConfigPara.CloseLowLevelSpanPlus);      
						m_Para += str;
						this->TongJi(&m_SelectedRecord,&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level);
						
						str.Format(_T("产品数:%d 合格品:%d 合格率:%5.1f%%\r\n动作最高温:%5.1f 动作最低温:%5.1f  动作平均温:%5.1f \r\n复位最高温:%5.1f 复位最低温:%5.1f  复位平均温:%5.1f"),total_product,total_main_level,(float)(total_main_level*100.0f/total_product),
							open_max_temp,open_min_temp,(open_avr_temp),close_max_temp,close_min_temp,(close_avr_temp));
						m_Para += str;

						this->UpdateData(false);
						

}

void SavePDF(CDC *dc,CString Title ,int BoxNr, CString PdfFileName,RECORD *record)
{
	DOCINFO docInfo;
	int i;
	

	

	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	
	
	
	docInfo.lpszDocName =   PdfFileName;

	docInfo.lpszOutput = PdfFileName;


	i=dc->StartDoc(&docInfo);  

	

	SWITCH_CONFIG_PARA_ALL ConfigPara_All;
	ConfigPara_All.para = record->ConfigPara;

	SWITCH_TEST_RESULT_EX TestResult_Ex[192];
	for(int i = 0; i < 192; i++)
	{
		TestResult_Ex[i].ID		=	i;
		TestResult_Ex[i].result = record->TestResult[i];
	}

	CString ProduceType;
	if(record->SwitchType == 0)
	{
		ProduceType = _T("常闭");
	}
	else
	{
		ProduceType = _T("常开");
	}

	if(G_PdfPrintFormat == 1 )
	{
		i=	dc->StartPage();
		PrintReport_ByGroup(dc,PdfFileName, BoxNr,
					CTime(STD_TIME)+CTimeSpan(0,0,record->StartTime,0),CTime(STD_TIME)+CTimeSpan(0,0,record->EndTime,0),
					record->ParaName,ProduceType, record->Memo,
					ConfigPara_All,TestResult_Ex,
					TRUE,(BOX_TYPE)record->BoxType,0);
		i =	dc->EndPage(); 
	}
	else if(G_PdfPrintFormat == 2 )
	{

		i = dc->StartPage();
		
		
		PrintReport_ByGroup_Ticket(dc,PdfFileName, BoxNr,
					CTime(STD_TIME)+CTimeSpan(0,0,record->StartTime,0),CTime(STD_TIME)+CTimeSpan(0,0,record->EndTime,0),
					record->ParaName,ProduceType, record->Memo,
					ConfigPara_All,TestResult_Ex,
					TRUE,(BOX_TYPE)record->BoxType,0);
		

		i =	dc->EndPage(); 
	}
	
	if(G_iMaxLedNr[BoxNr] > 128)
	{
		i = dc->StartPage();
	
		PrintReport_ByGroup(dc,PdfFileName, BoxNr,
				CTime(STD_TIME)+CTimeSpan(0,0,record->StartTime,0),CTime(STD_TIME)+CTimeSpan(0,0,record->EndTime,0),
					record->ParaName,ProduceType, record->Memo,
				ConfigPara_All,TestResult_Ex,
				TRUE,(BOX_TYPE)record->BoxType,0);

		i =	dc->EndPage(); 

		
	}

	i =	dc->EndDoc(); 
}


void CRecordViewDlg::OnButtonPrn() 
{
	CListDlg dlg_list;

	dlg_list.m_Tiltle = _T("选择 格式");
	
	dlg_list.m_ListItem.Add(_T("表格"));
	dlg_list.m_ListItem.Add(_T("收据"));
	//dlg_list.m_ListItem.Add(_T("串口票据打印机"));
	
	if(dlg_list.DoModal() != IDOK)
	{
		return;
	}

	if(dlg_list.m_iSelectItem == 2)
	{
	//	CONTROL_TEMP_RANGE	TempRange;
	
	//	HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(m_SwitchConfigPara.para  , &TempRange,G_NormalConfigPara[m_BoxNr].BoxType);

	//	G_MainDlg->m_LPM260_Printer.PrintTestResult(m_BoxNr,&record,0,0,TempRange,::G_NormalConfigPara[m_BoxNr].BoxType,0); 

	//	return;
	}

	// TODO: Add your control notification handler code here
	CPrintDialog prnDlg(true);//false);
	CString printer;
	CDC dc;
	int i;
	
	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	
	//prnDlg.SetWindowTextW(_T("选择PDF打印机"));

	if(prnDlg.DoModal() != IDOK)
	{
		AfxMessageBox(_T("打印取消!"));
		return;
	}

	
	
	printer  = prnDlg.GetDeviceName();


	if(!dc.CreateDC(_T(""),printer,_T(""),NULL))
	{
		AfxMessageBox(_T("请设置打印机"));
		return;
	}	
	
	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	CString strTitle;
	strTitle.Format(_T("%04d%02d%02d%02d%02d"),
		m_BeginTestTime.GetYear(),
		m_BeginTestTime.GetMonth(), 
		m_BeginTestTime.GetDay(), 
		m_BeginTestTime.GetHour(), 
		m_BeginTestTime.GetMinute()); 
	//this->GetWindowText(strTitle); 
	CString app_name = _T("热保护器测试记录");
	//AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += "[";
	app_name += strTitle;
	app_name += "]";
	app_name = _T("C:\\a.pdf");
	docInfo.lpszDocName =   app_name;

//	docInfo.lpszOutput = app_name;


	i=dc.StartDoc(&docInfo);  

	if(dlg_list.m_iSelectItem == 0)
	{
		i=	dc.StartPage();

		PrintReport_ByGroup(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_TestParaName,m_TestProductType, m_TestMemo,
					m_SwitchConfigPara,m_SwitchTestResult_Ex,
					TRUE,(BOX_TYPE)m_HeatBoxType,0);
		i =	dc.EndPage(); 
	}


	if(dlg_list.m_iSelectItem == 1)
	{
		i = dc.StartPage();
		
		PrintReport_ByGroup_Ticket(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_TestParaName,m_TestProductType,m_TestMemo, 
					m_SwitchConfigPara,m_SwitchTestResult_Ex,
					TRUE,(BOX_TYPE)m_HeatBoxType,0);

		

		i =	dc.EndPage(); 
	}
	
	if(G_iMaxLedNr[m_BoxNr] > 128)
	{
		i = dc.StartPage();
	
		PrintReport_ByGroup(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_TestParaName,m_TestProductType,m_TestMemo, 
					m_SwitchConfigPara,m_SwitchTestResult_Ex,
					TRUE,(BOX_TYPE)m_HeatBoxType,0);

		i =	dc.EndPage(); 

		
	}

	i =	dc.EndDoc(); 
	
	dc.DeleteDC();

}

void CRecordViewDlg::TongJi(RECORD	*Record,float *open_max_temp, float *open_min_temp, float *open_avr_temp, float *close_max_temp, float *close_min_temp, float *close_avr_temp, int *total_product, int *total_main_level)
{
		CString str;
		(*open_max_temp) = 0.0f;
		(*open_min_temp) = 999.9f;
		(*open_avr_temp) = 0.0f;
		(*close_max_temp)= 0.0f;
		(*close_min_temp)= 999.9f;
		(*close_avr_temp)= 0.0f;
		(*total_product) = G_iMaxLedNr[0];
		(*total_main_level) = 0;
		int  open_count=0,close_count=0;  
		int j;
		float temp;

		CONTROL_TEMP_RANGE temp_range;

		HEAT_COOL_ORDER HeatOrCool = ::CheckHeatOrCoolMode(0,Record->ConfigPara,&temp_range,Record->BoxType); 

		for(j = 0; j < G_iMaxLedNr[0];j++)
		{
			
			TEST_RESULT_LEVEL level =CheckTestLevel(Record->ConfigPara,Record->TestResult[j],HeatOrCool,temp_range,Record->TestResult[j],Record->BoxType);
								
			if(level == MAIN_LEVEL)
			{
				(*total_main_level) ++;
			}
			else if(level == TEST_RESULT_HIGH_LEVEL)
			{
				(*total_main_level) ++;
			}
			else if(level == LOW_LEVEL)
			{
				(*total_main_level) ++;
			}
			
			if(level == ERROR_LEVEL || level == NOT_USED)
			{
				(*total_product) --;
			}
			else 
			{
				temp = Record->TestResult[j].OpenTemp;
				if(temp > 0.0f && temp < 200.0f)
				{
					if((*open_max_temp) < temp)
					{
						(*open_max_temp) = temp;
					}
					if((*open_min_temp) > temp)
					{
						(*open_min_temp) = temp;
					}
					(*open_avr_temp) += temp;
					open_count ++;
				}
			
				temp = Record->TestResult[j].CloseTemp;
				if(temp > 0.0f && temp < 200.0f)
				{
					if((*close_max_temp) < temp)
					{
						(*close_max_temp) = temp;
					}
					if((*close_min_temp) > temp)
					{
						(*close_min_temp) = temp;
					}
					(*close_avr_temp) += temp;
					close_count ++;
				}
			}
			
		}
		if(open_count > 0)
		{
			(*open_avr_temp) = (*open_avr_temp) / open_count;
		}
		if(close_count > 0)
		{
			(*close_avr_temp) = (*close_avr_temp) / close_count;
		}
}


void CRecordViewDlg::OnButtonTempBar() 
{
	// TODO: Add your control notification handler code here

	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	int j;
	CString str;

	CProductTempBar dlg;

	for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
	{
		memcpy(&dlg.m_SwitchTestResult[j], &m_SwitchTestResult_Ex[j].result,sizeof(dlg.m_SwitchTestResult[j])) ;
	}
	dlg.m_BeginTestTime		=	m_BeginTestTime;
	dlg.m_SwitchConfigPara	=	m_SwitchConfigPara.para;
	dlg.m_BoxType			=	m_SelectedRecord.BoxType; 
	dlg.m_BoxNr				=	m_BoxNr;
	dlg.DoModal(); 


}

void CRecordViewDlg::OnButtonPrnPreview() 
{
	// TODO: Add your control notification handler code here
	CPrnPreview dlg;
	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	dlg.m_TestParaName		= m_TestParaName;
	dlg.m_EndTestTime		= m_EndTestTime;
	dlg.m_BeginTestTime		= m_BeginTestTime;
	dlg.m_BoxNr				= m_BoxNr;
	dlg.m_SwitchConfigPara	= m_SwitchConfigPara;
	dlg.m_TestMemo			= m_TestMemo;


	memcpy(dlg.m_SwitchTestResult_Ex,m_SwitchTestResult_Ex,sizeof(m_SwitchTestResult_Ex));
	
	CString ProductType ;

	dlg.m_TestProductType= m_TestProductType;
	
	CString strTitle;
	this->GetWindowText(strTitle); 
	CString app_name;
	AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += _T("[");
	app_name += strTitle;
	app_name += _T("]");
	
	dlg.m_Title = app_name;

	dlg.DoModal(); 
}

void CRecordViewDlg::OnButtonTongji() 
{
	// TODO: Add your control notification handler code here
	CChanLiangTongJiDlg dlg;
	dlg.m_BoxNr = this->m_BoxNr;
	dlg.DoModal(); 
}

void CRecordViewDlg::OnButtonSaveas() 
{
	// TODO: Add your control notification handler code here

	CListDlg dlg_list;

	dlg_list.m_Tiltle = _T("选择 保存格式");
	
	dlg_list.m_ListItem.Add(_T("保存为 Excel(CSV)"));
	dlg_list.m_ListItem.Add(_T("保存为 PDF 表格"));
	dlg_list.m_ListItem.Add(_T("保存为 PDF 收据"));
	
	if(dlg_list.DoModal() != IDOK)
	{
		return;
	}

	
	dlg_list.m_SelectString.Trim(); 
	if(dlg_list.m_SelectString.GetLength() <= 0)
	{
		return;
	}

	if(dlg_list.m_iSelectItem < 0)
	{
		return;
	}

	G_PdfPrintFormat = dlg_list.m_iSelectItem;

	if(G_PdfPrintFormat == 1 || G_PdfPrintFormat == 2)
	{
		CPrintDialog prnDlg(true);//false);
		CString printer;
		
		int i;
		
		

		
		//prnDlg.SetWindowTextW(_T("选择PDF打印机"));

		if(prnDlg.DoModal() != IDOK)
		{
			AfxMessageBox(_T("打印取消!"));
			return;
		}

		
		
		G_PdfPrinterName  = prnDlg.GetDeviceName();

		if(G_PdfPrinterDC.GetSafeHdc())
		{
			G_PdfPrinterDC.DeleteDC();
		}

		if(!G_PdfPrinterDC.CreateDC(_T(""),G_PdfPrinterName,_T(""),NULL))
		{
			AfxMessageBox(_T("请设置打印机"));
			return;
		}	
	}

	HTREEITEM treeitem = this->m_DirTreeCtrl.GetSelectedItem(); 
	CString filename;
	



	if(treeitem != 0)
	{
		filename = m_DirTreeCtrl.GetItemText(treeitem);
	
		if(filename.Find(_T("年"))> 0)
		{
			SaveYearCSV(treeitem);
			AfxMessageBox(filename + _T("保存成功"));
			return;
		}
		else if(filename.Find(_T("月"))> 0)
		{
			SaveMonthCSV(treeitem);
			CString s = m_DirTreeCtrl.GetItemText(m_DirTreeCtrl.GetParentItem(treeitem));
			
			AfxMessageBox(s + filename + _T("保存成功"));
			return;
			
		}
		else if(filename.Find(_T("日")) > 0)
		{
			SaveDayCSV(treeitem);
			AfxMessageBox(filename + _T("保存成功"));
			return;
		}
		
		
	}

	
	int j;
	TCHAR str[1000];
	TCHAR str1[1000];
	CString szFilter = _T("Excel Files (*.csv)|*.csv|");
	CString  file_name;
	CTime cur = cur.GetCurrentTime();


	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	CString s ;
	s.Format(_T("生产记录%04d%02d%02d%02d%02d%02d"),
		m_BeginTestTime.GetYear(), 
		m_BeginTestTime.GetMonth(), 
		m_BeginTestTime.GetDay(), 
		m_BeginTestTime.GetHour(), 
		m_BeginTestTime.GetMinute(), 
		m_BeginTestTime.GetSecond()); 
	CString ExName;
	if(G_PdfPrintFormat == 1 || G_PdfPrintFormat == 2)
	{
		ExName = _T("*.pdf");
		szFilter = _T("Excel Files (*.pdf)|*.pdf|");
	}
	else
	{
		ExName = _T("*.csv");
		szFilter = _T("Excel Files (*.csv)|*.csv|");
	}
	CFileDialog dlg(false,ExName,s,NULL,szFilter);

	int ret = dlg.DoModal();
	
	CFile RecFile;

	CONTROL_TEMP_RANGE temp_range;
	HEAT_COOL_ORDER	HeatOrCool = ::CheckHeatOrCoolMode(m_BoxNr,m_SelectedRecord.ConfigPara,&temp_range,m_SelectedRecord.BoxType);   
	
	if(ret == IDOK)
	{
		file_name = dlg.GetPathName();
		if(G_PdfPrintFormat == 1 || G_PdfPrintFormat == 2)
		{
			::SavePDF(&::G_PdfPrinterDC,_T(""),m_BoxNr,file_name,&m_SelectedRecord);
			return;
		}
		
	
		if(RecFile.Open(file_name,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{

			
			{
				RecFile.Write(&UNICODE_FILE_HEAR,2);
				RecFile.Write(&CSV_COMMMA,2);
				RecFile.Write(&CSV_COMMMA,2);
				s = _T("热保护器测试报告\r\n");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );


				s = _T("公司名称");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
				s = AfxGetApp()->GetProfileStringW(_T("SETTING"),_T("USER"),_T("常州市润邦电子科技有限公司")) ;
				s += _T("\r\n");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
		

				//产品规格
				s = _T("产品规格");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
				s = m_TestParaName + _T("\r\n"); 
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
				
				//产品类型
				s = _T("产品类型");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
				s = m_TestProductType + _T("\r\n");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
				
				
				s = _T("测试说明");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
				s = m_TestMemo + _T("\r\n");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
				

				s = _T("烘箱编号");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
				s.Format(_T("%d\r\n"), m_BoxNr + 1); 
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
				s = _T("开始测试时间");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
				s.Format(_T("%04d-%02d-%02d %02d:%02d\r\n"),m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),m_BeginTestTime.GetDay(),m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute());
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );

				s = _T("结束测试时间");
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
				s.Format(_T("%04d-%02d-%02d %02d:%02d\r\n"),m_EndTestTime.GetYear(),m_EndTestTime.GetMonth(),m_EndTestTime.GetDay(),m_EndTestTime.GetHour(),m_EndTestTime.GetMinute()); 
				RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );

				_stprintf_s(str,_T("序号%c动作温度%c复位温度%c规格\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
				RecFile.Write(str,wcslen(str) * sizeof(TCHAR));
					
				for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
				{
					if(m_SwitchTestResult_Ex[j].result.IsUsed)
					{
						if(G_iMaxLedNr[m_BoxNr] == 192)
						{
							if((j % 12) < 9)
							{
								_stprintf_s(str,_T("%d%d%c"),m_SwitchTestResult_Ex[j].ID/12,(j%12)+1,CSV_COMMMA);				
							}
							else if((j % 12) == 9)
							{
								_stprintf_s(str,_T("%d0%c"),m_SwitchTestResult_Ex[j].ID/12 + 1,CSV_COMMMA);				
							}
							else if((j % 12) == 10)
							{
								_stprintf_s(str,_T("%dA%c"),m_SwitchTestResult_Ex[j].ID/12 + 1,CSV_COMMMA);				
							}
							else if((j % 12) == 11)
							{
								_stprintf_s(str,_T("%dB%c"),m_SwitchTestResult_Ex[j].ID/12 +1,CSV_COMMMA);				
							}
						}
						else
						{
							_stprintf_s(str,_T("%d%c"),m_SwitchTestResult_Ex[j].ID + 1,CSV_COMMMA);				
						}
						CTime t = GetRecordTime(m_SwitchTestResult_Ex[j].result.OpenTime); 
						CTime t1 = GetRecordTime(m_SwitchTestResult_Ex[j].result.CloseTime); 

						TEST_RESULT_LEVEL level = CheckTestLevel(m_SwitchConfigPara.para,m_SwitchTestResult_Ex[j].result,HeatOrCool,temp_range,m_SelectedRecord.TestResult[j],m_SelectedRecord.BoxType);

						_stprintf_s(str1,_T("%5.1f%c%5.1f%c%s%c\r\n"),
							m_SwitchTestResult_Ex[j].result.OpenTemp,CSV_COMMMA,
							m_SwitchTestResult_Ex[j].result.CloseTemp,CSV_COMMMA,
							GetLevelName(level),CSV_COMMMA);   
						wcscat_s(str,str1);
						RecFile.Write(str,wcslen(str) * sizeof(TCHAR));
					}
					else
					{
						
					}
				}
			}

			RecFile.Close();
		}
	}
	
	

}

CString CreateDir(int BoxNr,CString curDir,CString sYear,CString sMonth,CString sDay)
{
	CString sBox;
	sBox.Format(_T("Box%d"),BoxNr +1);
		CString saveFileName = curDir + sBox;
		if (!::PathIsDirectory(saveFileName))
		{
			::CreateDirectory(saveFileName, NULL);
		}
		saveFileName += _T("\\") + sYear;
		if (!::PathIsDirectory(saveFileName))
		{
			::CreateDirectory(saveFileName, NULL);
		}
		saveFileName += _T("\\") + sMonth;
		if (!::PathIsDirectory(saveFileName))
		{
			::CreateDirectory(saveFileName, NULL);
		}
		saveFileName += _T("\\") + sDay;
		if (!::PathIsDirectory(saveFileName))
		{
			::CreateDirectory(saveFileName, NULL);
		}
		saveFileName += _T("\\");
		return saveFileName;
}



void CRecordViewDlg::SaveTreeReocord2CSV(int BoxNr,CString recordDir,CString csvDir,CString recordName,CString sYear,CString sMonth,CString sDay)
{
		CFile f;
		RECORD record;
		memset(&record,0,sizeof(RECORD));
		
		int pos = recordName.Find(_T("#"));
		if(pos > 0)
		{
			recordName = recordName.Right(recordName.GetLength() - pos - 2);
		}

		CString Old_recordName = recordName;

		pos = recordName.ReverseFind('_');
		if(pos > 0)
		{
			recordName = recordName.Left(pos);
		}

		

		if(f.Open(recordDir + _T("\\") + recordName,CFile::modeRead))
		{
			f.SeekToBegin();
			f.Read(&record,sizeof(RECORD));

			if(G_PdfPrintFormat == 1 || G_PdfPrintFormat == 2)
			{
				if(G_PdfPrintFormat == 1)
				{
					Old_recordName += _T("表格.pdf");
				}
				else
				{
					Old_recordName += _T("收据.pdf");
				}

				CString saveFileName = CreateDir(BoxNr,csvDir,sYear,sMonth,sDay);
				saveFileName += Old_recordName;

				SavePDF(&G_PdfPrinterDC,_T("sada"),BoxNr,saveFileName,&record);

			}
			else
			{
				Old_recordName += _T(".csv");

				CString saveFileName = CreateDir(BoxNr,csvDir,sYear,sMonth,sDay);
				saveFileName += Old_recordName;
				::SaveRecord2ExcelCSV(record,BoxNr,saveFileName);
				this->m_Para = saveFileName +  _T("\r\n") + m_Para;
				
				this->UpdateData(false);
			}
			

			f.Close();
		}
}
void CRecordViewDlg::SaveMonthCSV(HTREEITEM monthItem)
{
	if (m_DirTreeCtrl.ItemHasChildren(monthItem))
	{
			HTREEITEM hNextItemm_DirTreeCtrl;
			HTREEITEM hChildItem = m_DirTreeCtrl.GetChildItem(monthItem);
			
			if( hChildItem != NULL)
			{
				SaveDayCSV(hChildItem);	
			}

			while (hChildItem != NULL)
			{
					hNextItemm_DirTreeCtrl = m_DirTreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
				
					if(hNextItemm_DirTreeCtrl != NULL)
					{
						SaveDayCSV(hNextItemm_DirTreeCtrl);	

						hChildItem = hNextItemm_DirTreeCtrl;
					}
					else
					{
						break;
					}
			}
	}
}
void CRecordViewDlg::SaveYearCSV(HTREEITEM yearItem)
{
	if (m_DirTreeCtrl.ItemHasChildren(yearItem))
	{
			HTREEITEM hNextItemm_DirTreeCtrl;
			HTREEITEM hChildItem = m_DirTreeCtrl.GetChildItem(yearItem);
			
			if( hChildItem != NULL)
			{
				SaveMonthCSV(hChildItem);	
			}

			while (hChildItem != NULL)
			{
					hNextItemm_DirTreeCtrl = m_DirTreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
				
					if(hNextItemm_DirTreeCtrl != NULL)
					{
						SaveMonthCSV(hNextItemm_DirTreeCtrl);	

						hChildItem = hNextItemm_DirTreeCtrl;
					}
					else
					{
						break;
					}
			}
	}
}
void CRecordViewDlg::SaveDayCSV(HTREEITEM dayItem)
{
	CString sYear;
	CString sMonth;
	CString sDay;

			HTREEITEM treeitem_month = m_DirTreeCtrl.GetParentItem(dayItem);
			HTREEITEM treeitem_year  = m_DirTreeCtrl.GetParentItem(treeitem_month);
			sYear = m_DirTreeCtrl.GetItemText(treeitem_year);
			sMonth= m_DirTreeCtrl.GetItemText(treeitem_month);
			sDay  = m_DirTreeCtrl.GetItemText(dayItem);
			CString sBox;
			sBox.Format(_T("Box%d\\"),m_BoxNr+1);
			CString dirName = theAppDirectory + _T("log\\") + sBox + sYear + _T("\\") + sMonth + _T("\\") + sDay;
	
			if (m_DirTreeCtrl.ItemHasChildren(dayItem))
			{
				HTREEITEM hNextItemm_DirTreeCtrl;
				HTREEITEM hChildItem = m_DirTreeCtrl.GetChildItem(dayItem);
				
				if( hChildItem != NULL)
				{
					CString fName = m_DirTreeCtrl.GetItemText(hChildItem);
					SaveTreeReocord2CSV(m_BoxNr,dirName,_T("C:\\"),fName,sYear,sMonth,sDay);
					
				}

				while (hChildItem != NULL)
				{
						hNextItemm_DirTreeCtrl = m_DirTreeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
					
						if(hNextItemm_DirTreeCtrl != NULL)
						{
							CString fName = m_DirTreeCtrl.GetItemText(hNextItemm_DirTreeCtrl);

							SaveTreeReocord2CSV(m_BoxNr,dirName,_T("C:\\"),fName,sYear,sMonth,sDay);

							hChildItem = hNextItemm_DirTreeCtrl;
						}
						else
						{
							break;
						}
				}
			}

}


void CRecordViewDlg::OnBnClickedButtonSaveas2()
{
	// TODO: 在此添加控件通知处理程序代码

	
	

	HTREEITEM treeitem = this->m_DirTreeCtrl.GetSelectedItem(); 
	CString filename;
	
	if(treeitem != 0)
	{
		filename = m_DirTreeCtrl.GetItemText(treeitem);
		if(filename.Find(_T("年"))> 0)
		{
			SaveYearCSV(treeitem);
			AfxMessageBox( _T("保存成功"));
		}
		else if(filename.Find(_T("月"))> 0)
		{
			SaveMonthCSV(treeitem);
			AfxMessageBox(_T("保存成功"));
			
		}
		else if(filename.Find(_T("日")) > 0)
		{
			SaveDayCSV(treeitem);
			AfxMessageBox(_T("保存成功"));
		}

	}


}

void CRecordViewDlg::OnBnClickedDelete()
{
	RemoveDirectory(_T("c:\\a"));
}

void CRecordViewDlg::OnBnClickedButtonTakedlg()
{
	// TODO: 在此添加控件通知处理程序代码
 

	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	CTakeoutDlg dlg;
	dlg.m_BoxNr = 5;
	dlg.m_WindowText	= _T("历史记录查看"); 

	for(int j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
	{
		memcpy(&G_TakeDlgSwitchTestResult[1][j], &m_SwitchTestResult_Ex[j].result, sizeof(G_TakeDlgSwitchTestResult[1][j]));
		memcpy(&G_TakeDlgSwitchTestResult_Bak[1][j], &m_SwitchTestResult_Ex[j].result, sizeof(G_TakeDlgSwitchTestResult_Bak[1][j]));
	}

	dlg.m_iTakeBoardType = 1;
	dlg.m_fOpenTempShow  = 999.99f;
	dlg.m_BeginTestTime		= m_BeginTestTime;
	dlg.m_SwitchConfigPara	= m_SwitchConfigPara;
	dlg.DoModal(); 

}
SWITCH_TEST_RESULT_EX G_SwitchTestResult_Ex[192];

void CRecordViewDlg::SortRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	int id = 0;

	SWITCH_TEST_RESULT_EX temp_result;

	switch(m_ComboBoxSortType.GetCurSel())
	{
	case 1:
		for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
		{
			for(int j = i+1; j < G_iMaxLedNr[m_BoxNr]; j++)
			{
				if(m_SwitchTestResult_Ex[i].result.OpenTemp <  m_SwitchTestResult_Ex[j].result.OpenTemp)
				{
					temp_result = m_SwitchTestResult_Ex[i];
					m_SwitchTestResult_Ex[i] = m_SwitchTestResult_Ex[j];
					m_SwitchTestResult_Ex[j] = temp_result;
				}
			}
		}
		break;
	case 2:
		
		for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
		{
			for(int j = i+1; j < G_iMaxLedNr[m_BoxNr]; j++)
			{
				if(m_SwitchTestResult_Ex[i].result.OpenTemp >  m_SwitchTestResult_Ex[j].result.OpenTemp)
				{
					temp_result = m_SwitchTestResult_Ex[i];
					m_SwitchTestResult_Ex[i] = m_SwitchTestResult_Ex[j];
					m_SwitchTestResult_Ex[j] = temp_result;
				}
			}
		}
		break;
	case 3:
		//按组号
		id = 0;

		for(int group = 0; group <= 10; group++)
		{
			for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
			{
				if(m_SwitchTestResult_Ex[i].result.Group == group)
				{
					
					G_SwitchTestResult_Ex[id] = m_SwitchTestResult_Ex[i];
					id ++;
				}
			}
		}

		for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
		{
			m_SwitchTestResult_Ex[i] = G_SwitchTestResult_Ex[i];
				
		}

		break;
	default:
		for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
		{
			for(int j = i+1; j < G_iMaxLedNr[m_BoxNr]; j++)
			{
				if(m_SwitchTestResult_Ex[i].ID >  m_SwitchTestResult_Ex[j].ID)
				{
					temp_result = m_SwitchTestResult_Ex[i];
					m_SwitchTestResult_Ex[i] = m_SwitchTestResult_Ex[j];
					m_SwitchTestResult_Ex[j] = temp_result;
				}
			}
		}
		break;
	}
}
void CRecordViewDlg::OnCbnSelchangeComboSortType()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateRecordDetail(m_SelectedFileName); 
}


void CRecordViewDlg::OnCbnSelchangeComboPrintType()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_ComboPrintType.GetCurSel())
	{
	case 1:
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("PRINT TYPE"),1); 
		break;
	case 2:
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("PRINT TYPE"),2);
		break;
	default:
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("PRINT TYPE"),0);
		break;
	}
}

void CRecordViewDlg::OnBnClickedButtonOpenTempShow()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	CTakeoutDlg dlg;
	dlg.m_BoxNr = 5;
	dlg.m_WindowText	= _T("历史记录查看"); 

	for(int j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
	{
		memcpy(&G_TakeDlgSwitchTestResult[1][j], &m_SwitchTestResult_Ex[j].result, sizeof(G_TakeDlgSwitchTestResult[1][j]));
		memcpy(&G_TakeDlgSwitchTestResult_Bak[1][j], &m_SwitchTestResult_Ex[j].result, sizeof(G_TakeDlgSwitchTestResult_Bak[1][j]));
	}

	dlg.m_iTakeBoardType = 1;
	dlg.m_fOpenTempShow  = 0;
	dlg.m_BeginTestTime		= m_BeginTestTime;
	dlg.m_SwitchConfigPara	= m_SwitchConfigPara;
	dlg.SetAllSelected_NoNotUsed();
	dlg.DoModal(); 
}

void CRecordViewDlg::SaveRecord2CSV(CString str_Dir, HTREEITEM tree_Root)
{
    HTREEITEM tree_Temp;
	HANDLE hFile;
	WIN32_FIND_DATA file_find_data;
	DWORD errorcode = 0;

    //判断输入目录最后是否存在'\'，不存在则补充
    if (str_Dir.Right(1) != _T('\\'))
        str_Dir += _T("\\");
   
	hFile=FindFirstFile(str_Dir + _T("*.*") ,&file_find_data);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	tree_Temp = tree_Root;


	CString FileName = file_find_data.cFileName;

	if((file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	&& (FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) 
	{
		CString strPath  =str_Dir + FileName ; //得到路径，做为递归调用的开始
		CString strTitle = FileName ;//得到目录名，做为树控的结点
		//if(FileName.Find(_T("record")) >= 0)
		{
			tree_Temp = m_DirTreeCtrl.InsertItem(strTitle, 0, 0, tree_Root);
			ShowFile(strPath, tree_Temp);
		}
	}
	else if((FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) //如果是文件
	{
		//得到目录名，做为树控的结点
		m_DirTreeCtrl.InsertItem(FileName, 0, 0, tree_Temp);
	}
	
	if(::FindNextFile(hFile,&file_find_data) == false)
	{
		errorcode = GetLastError();
	}

	while((hFile!=INVALID_HANDLE_VALUE) && (errorcode!=ERROR_NO_MORE_FILES))
    {
       tree_Temp = tree_Root;
      
	   CString FileName = file_find_data.cFileName;
	
	   if((file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
       && (FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) 
		{
			CString strPath  = str_Dir + FileName ; //得到路径，做为递归调用的开始
            CString strTitle = FileName;			//得到目录名，做为树控的结点
			//if(FileName.Find(_T("record")) >= 0)
			{
				tree_Temp = m_DirTreeCtrl.InsertItem(strTitle, 0, 0, tree_Root);
				ShowFile(strPath, tree_Temp);
			}
        }
        else if((FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) //如果是文件
        {
			if(FileName.Find(_T("record")) >= 0)
			{
				m_DirTreeCtrl.InsertItem(FileName, 0, 0, tree_Temp);
			}
	     }

	   if(::FindNextFile(hFile,&file_find_data) == false)
	   {
			errorcode = GetLastError();
			break;
	   }
    }

	::FindClose(hFile); 
}

void CRecordViewDlg::ShowFile(CString str_Dir, HTREEITEM tree_Root)
{
    HTREEITEM tree_Temp;
	HANDLE hFile;
	WIN32_FIND_DATA file_find_data;
	DWORD errorcode = 0;

    //判断输入目录最后是否存在'\'，不存在则补充
    if (str_Dir.Right(1) != _T('\\'))
        str_Dir += _T("\\");
   
	hFile=FindFirstFile(str_Dir + _T("*.*") ,&file_find_data);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	tree_Temp = tree_Root;


	CString FileName = file_find_data.cFileName;

	if((file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	&& (FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) 
	{
		CString strPath  =str_Dir + FileName ; //得到路径，做为递归调用的开始
		CString strTitle = FileName ;//得到目录名，做为树控的结点
		//if(FileName.Find(_T("record")) >= 0)
		{
			tree_Temp = m_DirTreeCtrl.InsertItem(strTitle, 0, 0, tree_Root);
			ShowFile(strPath, tree_Temp);
		}
	}
	else if((FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) //如果是文件
	{
		//得到目录名，做为树控的结点
		m_DirTreeCtrl.InsertItem(FileName, 0, 0, tree_Temp);
	}
	
	if(::FindNextFile(hFile,&file_find_data) == false)
	{
		errorcode = GetLastError();
	}

	int ID = 0;
	while((hFile!=INVALID_HANDLE_VALUE) && (errorcode!=ERROR_NO_MORE_FILES))
    {
       tree_Temp = tree_Root;
      
	   CString FileName = file_find_data.cFileName;
	
	   if((file_find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
       && (FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) 
		{
			CString strPath  = str_Dir + FileName ; //得到路径，做为递归调用的开始
            CString strTitle = FileName;			//得到目录名，做为树控的结点
			//if(FileName.Find(_T("record")) >= 0)
			ID = 0;
			{
				tree_Temp = m_DirTreeCtrl.InsertItem(strTitle, 0, 0, tree_Root);
				ShowFile(strPath, tree_Temp);
			}
        }
        else if((FileName.Compare(_T(".")) != 0)  && (FileName.Compare(_T("..")) != 0) ) //如果是文件
        {
			if(FileName.Find(_T("record")) >= 0)
			{
				HTREEITEM tree_Temp2 = tree_Root;
				CString dir = FileName;
				while(1)
				{
					dir =  m_DirTreeCtrl.GetItemText(tree_Temp2) + _T("\\") + dir;
					tree_Temp2 = m_DirTreeCtrl.GetParentItem(tree_Temp2);
					if(tree_Temp2 == 0)
					{
						break;
					}
					
				}
				CString s;
				s.Format(_T("log\\box%d\\"), this->m_BoxNr + 1);
				dir = ::theAppDirectory  + s + dir;
				CFile RecFile;
				
				if(RecFile.Open(dir,CFile::modeRead))
				{
					RecFile.SeekToBegin();
					RecFile.Read(&m_SelectedRecord,sizeof(m_SelectedRecord));
					RecFile.Close();
				}
				
			
				ID ++;
				
				s.Format(_T("%03d#_%s_%5.1f"),ID,FileName,m_SelectedRecord.ConfigPara.MainLevel);
				m_DirTreeCtrl.InsertItem(s, 0, 0, tree_Temp);
			}
	     }

	   if(::FindNextFile(hFile,&file_find_data) == false)
	   {
			errorcode = GetLastError();
			break;
	   }
    }

	::FindClose(hFile); 
}
void CRecordViewDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	
	
}

void CRecordViewDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	HTREEITEM treeitem = this->m_DirTreeCtrl.GetSelectedItem(); 
	CString filename;

	if(treeitem != 0)
	{
		filename = m_DirTreeCtrl.GetItemText(treeitem);
	}
	if(m_DirTreeCtrl.GetChildItem(treeitem) == 0)
	{
		while(1)
		{
			
			treeitem = m_DirTreeCtrl.GetParentItem(treeitem);
			if(treeitem == 0)
			{
				break;
			}
			else
			{
				filename = m_DirTreeCtrl.GetItemText(treeitem) + _T("\\") + filename;
			}
		}

	}

	if(filename.Find(_T("record")) > 0)
	{
		int pos = filename.Find(_T("#"));
		if(pos > 0)
		{
			m_SelectedFileName  = filename.Left(pos-3);
			m_SelectedFileName += filename.Right(filename.GetLength() - pos - 2);
		}
		m_SelectedFileName = m_SelectedFileName.Left(m_SelectedFileName.GetLength() - 6);
		//AfxMessageBox(m_SelectedFileName);
		this->UpdateRecordDetail(m_SelectedFileName); 
	}
	else
	{//目录,遍历所有的文件

		//ShowFile(::theAppDirectory + sDir , m_TreeRoot);//以E:\\test为根目录进行遍历

	}
}

void CRecordViewDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CRecordViewDlg::OnBnClickedButtonTempCurve()
{
	// TODO: 在此添加控件通知处理程序代码

	CFullScreenCurve dlg;

	CString CurName = m_SelectedFileName;
	
	CurName.Replace(_T("record"),_T("cur"));

	CFile RecFile;
	CString s;
	TEMP_RECORD tr[10000];
	
	s.Format(_T("box%d\\"),m_BoxNr + 1); 
	CurName = theAppDirectory + _T("log\\") + s + CurName;
	if(RecFile.Open(CurName,CFile::modeRead))
	{
		RecFile.SeekToBegin();
		while(1)
		{
			DWORD len = RecFile.Read(&tr,sizeof(tr));
			
			if(len > 0)
			{
				for(int i = 0 ; i < (len / sizeof(TEMP_RECORD)); i++)
				{
					dlg.m_TempArray.Add(tr[i]);  
				}
			}
			else
			{
				break;
			}
		}

		RecFile.Close();

		dlg.m_BoxNr = -1;
		dlg.DoModal(); 
	}
	else
	{
		AfxMessageBox(_T("没有找到‘温度曲线’文件"));
		return;
	}


}

void CRecordViewDlg::OnBnClickedButtonMes()
{
	// TODO: 在此添加控件通知处理程序代码

	CWorkMode::Send_MES(m_SelectedRecord,false);
	

}

void   CRecordViewDlg::CreateMES_String( RECORD	*Record,CString * MES_String)
{
    int i = 0,j;

	CString					Para = _T("");
	SWITCH_CONFIG_PARA_ALL	SwitchConfigPara;
	SWITCH_TEST_RESULT_EX   SwitchTestResult_Ex[192];

						CString strTemp;
						
						SwitchConfigPara.para			= Record->ConfigPara;
						SwitchConfigPara.para_ex		= Record->ConfigParaEx;
						/*
						if(Record->SwitchType == 0)
						{
							m_TestProductType  = _T("常闭");
						}
						else
						{
							m_TestProductType  = _T("常开");
						}

						m_TestParaName.Format(_T("%s"),Record->ParaName); 
						
						m_TestMemo.Format(_T("%s"),Record->Memo);  
					 */
						for(j = 0; j < G_iMaxLedNr[0];j++)
						{	
							//m_SwitchTestResult[j] = record.TestResult[j];
							SwitchTestResult_Ex[j].ID = j;
							SwitchTestResult_Ex[j].result = Record->TestResult[j];
						}
						CONTROL_TEMP_RANGE temp_range;

						HEAT_COOL_ORDER HeatOrCool = ::CheckHeatOrCoolMode(0,Record->ConfigPara,&temp_range,Record->BoxType); 

						//SortRecord();
	
						CString s,s1;
						
						CTime BeginTestTime	= STD_TIME + CTimeSpan(0,0,Record->StartTime ,0);
						CTime EndTestTime	= STD_TIME + CTimeSpan(0,0,Record->EndTime ,0);
						
						
						s = BeginTestTime.Format("开始测试时间:%Y-%m-%d %H:%M:%S;");
					    Para += s;
						s = EndTestTime.Format("结束测试时间:%Y-%m-%d %H:%M:%S;");
					    Para += s;
						
						Para += _T("测试参数:");
						Para += Record->ParaName;
						
						
						if(Record->SwitchType == 0)
						{
							Para += _T("产品类型:常闭;");
						}
						else
						{
							Para += _T("产品类型:常开;");
						}
						
						
						for(j = 0; j < G_iMaxLedNr[0]/2;j++)
						{
							
							
							
							if(SwitchTestResult_Ex[j].result.IsUsed)
							{
								

								s.Format(_T("%d,%5.1f,%5.1f;"),SwitchTestResult_Ex[j].ID+1,SwitchTestResult_Ex[j].result.OpenTemp,SwitchTestResult_Ex[j].result.CloseTemp);
								
								
								TEST_RESULT_LEVEL level =CheckTestLevel(Record->ConfigPara,SwitchTestResult_Ex[j].result,HeatOrCool,temp_range,SwitchTestResult_Ex[j].result,Record->BoxType);
								
						
							}
							else
							{
								
								s .Format(_T("%d,无产品;"),SwitchTestResult_Ex[j].ID+1);
								
							}
							
						 Para += s;

						}

						
						float open_max_temp, open_min_temp,open_avr_temp,close_max_temp,close_min_temp,close_avr_temp;
						int total_product,total_main_level;

						CString str;
						
						str.Format(_T("主规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f;"),Record->ConfigPara.MainLevel,Record->ConfigPara.MainLevelSpanMinus,Record->ConfigPara.MainLevelSpanPlus);      
						Para += str;
						str.Format(_T("主复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f;"),Record->ConfigPara.CloseLevel,Record->ConfigPara.CloseLevelSpanMinus,Record->ConfigPara.CloseLevelSpanPlus);      
						Para += str;
						str.Format(_T("高规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f;"),Record->ConfigPara.HighLevel,Record->ConfigPara.HighLevelSpanMinus,Record->ConfigPara.HighLevelSpanPlus);      
						Para += str;
						str.Format(_T("高复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f;"),Record->ConfigPara.CloseHighLevel,Record->ConfigPara.CloseHighLevelSpanMinus,Record->ConfigPara.CloseHighLevelSpanPlus);      
						Para += str;

						str.Format(_T("低规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f;"),Record->ConfigPara.LowLevel,Record->ConfigPara.LowLevelSpanMinus,Record->ConfigPara.LowLevelSpanPlus);      
						Para += str;
						str.Format(_T("低复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f;"),Record->ConfigPara.CloseLowLevel,Record->ConfigPara.CloseLowLevelSpanMinus,Record->ConfigPara.CloseLowLevelSpanPlus);      
						Para += str;

						TongJi(Record,&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level);
						
						str.Format(_T("产品数:%d 合格品:%d 合格率:%5.1f%%\r\n动作最高温:%5.1f 动作最低温:%5.1f  动作平均温:%5.1f \r\n复位最高温:%5.1f 复位最低温:%5.1f  复位平均温:%5.1f"),total_product,total_main_level,(float)(total_main_level*100.0f/total_product),
							open_max_temp,open_min_temp,(open_avr_temp),close_max_temp,close_min_temp,(close_avr_temp));
						Para += str;

						

						*MES_String = Para;
}
