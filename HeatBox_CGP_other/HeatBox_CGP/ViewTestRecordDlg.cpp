// ViewTestRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "HeatBox_CGPDlg.h"
#include "ViewTestRecordDlg.h"
#include <math.h>
#include "shlwapi.h"
#include "FullScreenCurve.h"

extern CString theAppDirectory;
extern int CheckLevelItem(TEST_PARA	m_TestPara, SWITCH_TEST_RESULT m_SwitchStatus, int m_LastTestPro);

// CViewTestRecordDlg 对话框

IMPLEMENT_DYNAMIC(CViewTestRecordDlg, CDialog)

CViewTestRecordDlg::CViewTestRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewTestRecordDlg::IDD, pParent)
{

}

CViewTestRecordDlg::~CViewTestRecordDlg()
{
}

void CViewTestRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECORD_DETAIL, m_RecordDetailList);
	DDX_Control(pDX, IDC_EDIT_PARA, m_EditPara);
	DDX_Control(pDX, IDC_TREE1, m_DirTreeCtrl);
}


BEGIN_MESSAGE_MAP(CViewTestRecordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CViewTestRecordDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST_RECORD, &CViewTestRecordDlg::OnLbnDblclkListRecord)
	ON_LBN_SELCHANGE(IDC_LIST_RECORD, &CViewTestRecordDlg::OnLbnSelchangeListRecord)
	ON_BN_CLICKED(IDC_PRINT, &CViewTestRecordDlg::OnBnClickedPrint)
	ON_BN_CLICKED(IDC_SAVE, &CViewTestRecordDlg::OnBnClickedSave)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CViewTestRecordDlg::OnNMDblclkTree1)
	ON_BN_CLICKED(IDCANCEL, &CViewTestRecordDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CUR, &CViewTestRecordDlg::OnBnClickedButtonCur)
END_MESSAGE_MAP()


// CViewTestRecordDlg 消息处理程序

void CViewTestRecordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

extern void TraverseFolder(LPCTSTR lpPath,LPCTSTR file_ext,CStringArray *fileStr);

BOOL CViewTestRecordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd *wnd;

	HTREEITEM m_TreeRoot = NULL;//m_DirTreeCtrl.InsertItem(_T("测试记录"),0,0);//插入根节点
	CString sDir = _T("\\log\\");
	
	ShowFile(::theAppDirectory + sDir , m_TreeRoot);//以E:\\test为根目录进行遍历

	m_DirTreeCtrl.Expand(m_TreeRoot,TVE_EXPAND);


	/*
	
	TEST_RECORD record;
	TEST_RECORD_FILE_HEADER head; 
	if(m_RecordList.GetCount() > 0)
	{
		m_RecordList.ResetContent();
	}

	CFile test_result_file;
	if(test_result_file.Open(theAppDirectory + "Records_CGP.mdb", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		
		test_result_file.SeekToBegin();
		int head_len = test_result_file.Read(&head,sizeof(head));

		if(head_len == 0)
		{
			head.FILE_VER = 1;
			head.FILE_SUB_VER = 0;
			head.TEST_RECORD_SIZE = sizeof(TEST_RECORD);
			_stprintf(head.NAME,sizeof(head.NAME),"%s","常州市润邦电子科技有限公司 TEL:013915838598");
			test_result_file.Write(&head,sizeof(head)); 
		}
		while(1)
		{
			if(test_result_file.Read(&record,sizeof(TEST_RECORD)) ==  sizeof(TEST_RECORD))
			{
				CString s;
				CTime t(record.BeginTime); 
				s.Format("%04d/%02d/%02d %02d:%02d:%02d %s", t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),::record.TestPara.ParaName);
				m_RecordList.AddString(s); 
			}
			else
			{
				break;
			}
		}
	}
	*/

	
	
	if(::G_NormalConfigPara.Language  == 1)
	{
		wnd = this->GetDlgItem(IDOK);
		wnd->SetWindowTextW(_T("Quit")); 
		wnd = this->GetDlgItem(IDC_PRINT);
		wnd->SetWindowTextW(_T("Print"));
		wnd = this->GetDlgItem(IDC_SAVE);
		wnd->SetWindowTextW(_T("Save Excel"));

	}
	return true;
}
void CViewTestRecordDlg::OnLbnDblclkListRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void CViewTestRecordDlg::OnLbnSelchangeListRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	OnLbnDblclkListRecord();
}

void CViewTestRecordDlg::OnBnClickedPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	CPrintDialog prnDlg(true);
	CString printer;
	CDC dc;
	int i;

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
		TEST_RECORD record; 
		
		CFile test_result_file;
		if(test_result_file.Open(::theAppDirectory + _T("log\\") + filename, /*CFile::modeCreate | CFile::modeNoTruncate |*/ CFile::modeReadWrite))
		{
			
			test_result_file.SeekToBegin();

			if(test_result_file.Read(&record,sizeof(TEST_RECORD)) ==  sizeof(TEST_RECORD))
			{

				if(prnDlg.DoModal() != IDOK)
				{
					AfxMessageBox(_T("没有选择打印机"));
					return;
				}
				printer  = prnDlg.GetDeviceName();
			//		AfxMessageBox(printer);

				if(!dc.CreateDC(_T(""),printer,_T(""),NULL))
				{
					if(::G_NormalConfigPara.Language == 0)
					{
						AfxMessageBox(_T("请设置打印机"));
					}
					else
					{
						AfxMessageBox(_T("Please select a PRINTER"));
					}
					return;
				}	
				
				DOCINFO docInfo;
				memset(&docInfo, 0, sizeof(DOCINFO));
				docInfo.cbSize = sizeof(DOCINFO);
				CString strTitle;
				this->GetWindowText(strTitle); 
				CString app_name;
				AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
				app_name += _T("[");
				app_name += strTitle;
				app_name += _T("]");
				docInfo.lpszDocName =   app_name;


				i=dc.StartDoc(&docInfo);  
				i=	dc.StartPage();

				CString BoxNr;

				
				PrintReport(&dc,record);

				


				i =	dc.EndPage(); 
				

				i =	dc.EndDoc(); 
				
				dc.DeleteDC();

			}
		}
	}
	else
	{
		if(::G_NormalConfigPara.Language  == 0)
		{
			AfxMessageBox(_T("请选择具体的生产记录"));
		}
		else
		{
			AfxMessageBox(_T("Please select a record"));
		}
	}

}

void CViewTestRecordDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码

	
	CTime cur = cur.GetCurrentTime();
	bool bDeltaType = false;

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

	TEST_RECORD record; 

	if(filename.Find(_T("record")) > 0)
	{
		CFile test_result_file;
		if(test_result_file.Open(::theAppDirectory + _T("log\\") + filename, /*CFile::modeCreate | CFile::modeNoTruncate |*/ CFile::modeReadWrite))
		{
			
			test_result_file.SeekToBegin();

			if(test_result_file.Read(&record,sizeof(TEST_RECORD)) ==  sizeof(TEST_RECORD))
			{

			}
			else
			{
				return;
			}
		}
	}
	else
	{
		AfxMessageBox(_T("没有找到测试记录!"));
		return;
	}

	CString s;
	if(::G_NormalConfigPara.Language  == 0)
	{
		s = "生产记录";
	}
	else
	{
		s = "TestRecord";
	}

	static TCHAR szFilter[] = _T("Excel Files (*.csv)|*.csv|");

	CFileDialog dlg(false,_T("*.csv"),s,NULL,szFilter);

	int ret = dlg.DoModal();
	
	CFile RecFile;

	if(ret == IDOK)
	{
		CString file_name = dlg.GetPathName();
		if(RecFile.Open(file_name,CFile::modeReadWrite | CFile::modeCreate))
		{
			RecFile.SeekToBegin(); 
			TCHAR buf[1024];
			TCHAR buf1[1024];

			unsigned short UNICODE_FILE_HEAR = 0xFEFF;
			unsigned short CSV_COMMMA		 = 0x0009;
			RecFile.Write(&UNICODE_FILE_HEAR,2);

			if(::G_NormalConfigPara.Language  == 0)
			{
				_stprintf(buf,_T("公司名称:%c %s\r\n") ,CSV_COMMMA, record.TestPara.Productor);
			}
			else
			{
				_stprintf(buf,_T("Productor:%c %s\r\n") ,CSV_COMMMA, record.TestPara.Productor);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR)); 

			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("产品规格:%c %s\r\n"),CSV_COMMMA,G_NormalConfigPara.ParaName);
			}
			else
			{
				_stprintf(buf,_T("ParaName:%c %s\r\n"),CSV_COMMMA,G_NormalConfigPara.ParaName);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR)); 
			
			_stprintf(buf,_T("测试速度:%c %5.1f度/分钟\r\n"),CSV_COMMMA,(record.TestPara.TestSpeed + 1)*0.5);
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR)); 
			

			if(record.TestPara.OpenCloseGapHigh[0] != 0 || record.TestPara.OpenCloseGapLow != 0)
			{
				bDeltaType = true;
			}
			else
			{
				bDeltaType = false;
			}
			if(record.TestPara.IsDoubleSwitch == 0)
			{
				if(::G_NormalConfigPara.Language  == 0)
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("产品类型:%c 单路 回差温度\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("产品类型:%c 单路 动作复位温度\r\n"),CSV_COMMMA);
					}
				}
				else
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("Switch Type:%c Single Delta\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("Switch Type:%c Single Reset\r\n"),CSV_COMMMA);
					}
					
				}
			}
			else
			{
				if(G_NormalConfigPara.Language == 0)
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("产品类型:,%c 双路 回差温度\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("产品类型:%c 双路 动作复位温度\r\n"),CSV_COMMMA);
					}
					
				}
				else
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("Switch Type:%c Double Delta\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("Switch Type:%c Double Reset\r\n"),CSV_COMMMA);
					}
					
				}
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR)); 
			
	
			if(G_NormalConfigPara.Language == 0)
			{
				CTime t(record.BeginTime);
				_stprintf(buf,_T("测试时间:%c %04d/%02d/%02d %02d:%02d:%02d\r\n"),CSV_COMMMA, t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());	
			}
			else
			{
				CTime t(record.BeginTime);
				_stprintf(buf,_T("Test Time:,%c %04d/%02d/%02d %02d:%02d:%02d\r\n"),CSV_COMMMA, t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());	
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));


			
			
	
			float open_max_temp = -999.0f;
			float open_min_temp = 999.0f;
			float close_max_temp = -999.0f;
			float close_min_temp = 999.0f;
			float delata_max_temp = -999.0f;
			float delata_min_temp = 999.0f;

			float open_avr_temp = 0,close_avr_temp = 0;
			int total_product = 0,total_main_level = 0;
			int total_product_Openned = 0;
			int total_product_Closed  = 0;
 
			for(int i = 0; i < MAX_LED_NR; i++)
			{
				if(record.SwitchStatus[0][i].IsUsed)
				{
					total_product ++;
					if(record.SwitchStatus[0][i].IsOpenned)
					{
						open_avr_temp += record.SwitchStatus[0][i].OpenTemp;
						total_product_Openned++;
					}
					if(record.SwitchStatus[0][i].IsClosed)
					{
						close_avr_temp+= record.SwitchStatus[0][i].CloseTemp;
						total_product_Closed++;
					}
					
					if(CheckLevelItem(record.TestPara,record.SwitchStatus[0][i],0 ) != 0)
					{
						total_main_level++;
					}
					if(record.SwitchStatus[0][i].IsOpenned)
					{
						if(record.SwitchStatus[0][i].OpenTemp < open_min_temp)
						{
							open_min_temp = record.SwitchStatus[0][i].OpenTemp;
						}
						if(record.SwitchStatus[0][i].OpenTemp > open_max_temp)
						{
							open_max_temp = record.SwitchStatus[0][i].OpenTemp;
						}
					}
					if(record.SwitchStatus[0][i].IsClosed)
					{
						if(record.SwitchStatus[0][i].CloseTemp < close_min_temp)
						{
							close_min_temp = record.SwitchStatus[0][i].CloseTemp;
						}
						if(record.SwitchStatus[0][i].CloseTemp > close_max_temp)
						{
							close_max_temp = record.SwitchStatus[0][i].CloseTemp;
						}
					}
					if(record.SwitchStatus[0][i].IsOpenned  && record.SwitchStatus[0][i].IsClosed)
					{
						if(fabs(record.SwitchStatus[0][i].OpenTemp - record.SwitchStatus[0][i].CloseTemp) > delata_max_temp)
						{
							delata_max_temp = fabs(record.SwitchStatus[0][i].OpenTemp - record.SwitchStatus[0][i].CloseTemp);
						}

						if(fabs(record.SwitchStatus[0][i].OpenTemp - record.SwitchStatus[0][i].CloseTemp) < delata_min_temp)
						{
							delata_min_temp = fabs(record.SwitchStatus[0][i].OpenTemp - record.SwitchStatus[0][i].CloseTemp);
						}
					}

				}
			}
			
			if(total_product > 0)
			{
				if(bDeltaType)
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f%c回差最低温度:%c%5.1f\r\n"),CSV_COMMMA,open_min_temp,CSV_COMMMA,CSV_COMMMA,delata_min_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c回差最高温度:%c%5.1f\r\n"),CSV_COMMMA,open_max_temp,CSV_COMMMA,CSV_COMMMA,delata_max_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
				else
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f,%c复位最低温度:%c%5.1f,\r\n"),CSV_COMMMA,open_min_temp,CSV_COMMMA,CSV_COMMMA,close_min_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c复位最高温度:%c%5.1f,\r\n"),CSV_COMMMA,open_max_temp,CSV_COMMMA,CSV_COMMMA,close_max_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
			}
			else
			{
				if(bDeltaType)
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f%c回差最低温度:%c%5.1f\r\n"),CSV_COMMMA,0,0,CSV_COMMMA,CSV_COMMMA);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c回差最高温度:%c%5.1f\r\n"),CSV_COMMMA,0,0,CSV_COMMMA,CSV_COMMMA);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
				else
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f%c复位最低温度:%c%5.1f,%c\r\n"),CSV_COMMMA,0,0,CSV_COMMMA,CSV_COMMMA);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c复位最高温度:%c%5.1f%c\r\n"),CSV_COMMMA,0,0,CSV_COMMMA,CSV_COMMMA);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
			}


			if(total_product > 0 )
			{
				open_avr_temp /= total_product_Openned;
				close_avr_temp/= total_product_Closed;
			}

			
			if(G_NormalConfigPara.Language == 0)
			{
				if(total_product > 0)
				{
					_stprintf(buf,_T("产品数:%c %d%c合格品:%c %d%c合格率:%c %5.1f%%\r\n"),
						CSV_COMMMA,total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,CSV_COMMMA,(float)(total_main_level*100.0f/total_product));
				}
				else
				{
					_stprintf(buf,_T("产品数:%c %d%c合格品:%c %d%c合格率:%c %5.1f%%\r\n"),
						CSV_COMMMA,total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,CSV_COMMMA,0.00);
				}
			}
			else
			{
				if(total_product > 0)
				{
					_stprintf(buf,_T("Total:%c %d%cPass:%c %d%c %5.1f%%\r\n"),
						CSV_COMMMA,total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,CSV_COMMMA,(float)(total_main_level*100.0f/total_product));
				}
				else
				{
					_stprintf(buf,_T("Total:%c %d%cPass:%c %d%c %5.1f%%\r\n"),
						CSV_COMMMA,total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,CSV_COMMMA,0.00);
				}
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
			
		
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("测试参数\r\n"));
			}
			else
			{
				_stprintf(buf,_T("Test Para\r\n"));
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
			
			if(::G_NormalConfigPara.Language  == 0)
			{
				_stprintf(buf,_T("断开温度:%c %5.1f%c负偏差:%c %5.1f%c正偏差:%c %5.1f\r\n"),
					CSV_COMMMA,record.TestPara.OpenTemp[0],CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenTempMinus[0],CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenTempPlus[0]);
			}
			else
			{
				_stprintf(buf,_T("Open Temp:%c %5.1f%cGap -:%c %5.1f%cGap +:%c %5.1f\r\n"),
					CSV_COMMMA,record.TestPara.OpenTemp[0],CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenTempMinus[0],CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenTempPlus[0]);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
			
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("%c%c回差下限:%c %5.1f%c回差上限:%c %5.1f\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenCloseGapLow[0],CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenCloseGapHigh[0]);
			}
			else
			{
				_stprintf(buf,_T("%c%cClose Gap Min:%c %5.1f%cMax:%c %5.1f\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenCloseGapLow[0],CSV_COMMMA,CSV_COMMMA,record.TestPara.OpenCloseGapHigh[0]);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
	
			
			if(G_NormalConfigPara.Language == 0)
			{
				if(bDeltaType)
				{
					_stprintf(buf,_T("编号 %c动作温度%c回差温度%c闪动%c判定结果\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
				}
				else
				{
					_stprintf(buf,_T("编号%c动作温度%c复位温度%c闪动%c判定结果\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
				}
			}
			else
			{
				_stprintf(buf,_T("Nr. %cOpen%cClose%cLevel\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));

			for(int i = 0; i < MAX_LED_NR; i++)
			{
				if(record.SwitchStatus[0][i].IsUsed == false)
				{
					continue;
				}

				
				_stprintf(buf,_T("%3d%c"),i+1,CSV_COMMMA); 
				
				
				if(record.SwitchStatus[0][i].OpenTemp >= record.SwitchStatus[0][i].CloseTemp)
					{
						TCHAR buf2[100];
						memset(buf1,0,sizeof(buf1));
						if(record.SwitchStatus[0][i].IsOpenned)
						{
							_stprintf(buf2,_T("%5.1f%c"),record.SwitchStatus[0][i].OpenTemp,CSV_COMMMA);
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没断开"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);

						if(record.SwitchStatus[0][i].IsClosed)
						{
							_stprintf(buf2,_T("%5.1f%c"),record.SwitchStatus[0][i].OpenTemp - record.SwitchStatus[0][i].CloseTemp,CSV_COMMMA );
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没复位"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);

						
					}
					else
					{
						TCHAR buf2[100];
						memset(buf1,0,sizeof(buf1));
						if(record.SwitchStatus[0][i].IsOpenned)
						{
							_stprintf(buf2,_T("%5.1f%c"),record.SwitchStatus[0][i].OpenTemp,CSV_COMMMA);
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没断开"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);

						if(record.SwitchStatus[0][i].IsClosed)
						{
							_stprintf(buf2,_T("%5.1f%c"),record.SwitchStatus[0][i].CloseTemp - record.SwitchStatus[0][i].OpenTemp,CSV_COMMMA  );
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没复位"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);
					}
				_tcscat(buf,buf1);
				
				if(record.SwitchStatus[0][i].IsOpenFlash || record.SwitchStatus[0][i].IsCloseFlash)
				{
					TCHAR buf2[100];
					_stprintf(buf2,_T("%c"),CSV_COMMMA);
					_tcscat(buf,buf2);
					_tcscat(buf,_T("F"));
				}
				else
				{
					TCHAR buf2[100];
					_stprintf(buf2,_T("%c"),CSV_COMMMA);
					_tcscat(buf,buf2);
					_tcscat(buf,_T(" "));
				}
				
				if(record.SwitchStatus[0][i].IsUsed)
				{
					if(CheckLevelItem(record.TestPara,record.SwitchStatus[0][i],0 ) != 0)
					{
						TCHAR buf2[100];
						_stprintf(buf2,_T("%c"),CSV_COMMMA);
						_tcscat(buf,buf2);
						_tcscat(buf,_T("合格 \r\n"));
					}
					else
					{
						TCHAR buf2[100];
						_stprintf(buf2,_T("%c"),CSV_COMMMA);
						_tcscat(buf,buf2);
						_tcscat(buf,_T("不合格 \r\n"));
					}
				}
				else
				{
					TCHAR buf2[100];
					_stprintf(buf2,_T("%c"),CSV_COMMMA);
					_tcscat(buf,buf2);
					_tcscat(buf,_T("--- \r\n"));
				}
				RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				
			}
		}
	}

}

void CViewTestRecordDlg::PrintReport(CDC *dc,
				 TEST_RECORD record)
{
	
	CString Title = _T("温控器测试报告");
//#define MAX_LED_NR_128	
	CFont *font=dc->GetCurrentFont();  
	LOGFONT logfont; 
	font->GetLogFont(&logfont); 

	CRect page_rect;
	CString str;
	
	page_rect.left = 0;
	page_rect.right = dc->GetDeviceCaps(HORZRES);
	page_rect.top = 0; 
	page_rect.bottom = dc->GetDeviceCaps(VERTRES);   

	page_rect.bottom = page_rect.right * 7008 / 4950 ;//A4

	CBrush whitebrush(COLORREF(RGB(255,255,255)));
	dc->FillRect(page_rect,&whitebrush);

	int FontPoint = page_rect.right / 2;

	
	CFont newFont;
	CFont* oldFont;
	CSize chnSize;
	while(1)
	{
		newFont.CreatePointFont(FontPoint,_T("宋体"),dc);
		oldFont = dc->SelectObject(&newFont);
		chnSize = dc->GetTextExtent(_T("编号 动作温 复位温 判定结果 "));
		if(page_rect.right  >= (4 *  chnSize.cx))
		{
			break;
		}
		FontPoint -= 20;
		newFont.DeleteObject();
		dc->SelectObject(oldFont);
	}
	oldFont->DeleteObject();
	
	CPen pen(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
	CPen *oldPen = dc->SelectObject(&pen);
	oldPen->DeleteObject();
	
	chnSize = dc->GetTextExtent(_T("国"));
	
	chnSize.cy  = (long)(chnSize.cy *1.5);
	int cur_x,cur_y;
	cur_x = page_rect.right/3;
	cur_y = chnSize.cy;

	dc->TextOut(cur_x,cur_y, Title);
	

	CString sCompany;
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	sCompany.Format(_T("公司名称: %s") , record.TestPara.Productor);
	dc->TextOut(cur_x,cur_y, sCompany);
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	

	CString sVal;
	//产品规格
	sVal.Format(_T("产品规格: %s"),G_NormalConfigPara.ParaName); 
	dc->TextOut(cur_x,cur_y, sVal);
	//产品类型
	if(record.TestPara.IsDoubleSwitch == 0)
	{
		sVal = _T("产品类型: 单路");
	}
	else
	{
		sVal = "产品类型: 双路";
	}
	dc->TextOut(cur_x+chnSize.cx * 10,cur_y,sVal);
	
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	CTime t(record.BeginTime); 
	sVal.Format(_T("测试时间: %04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
	str += sVal;
	dc->TextOut(cur_x,cur_y, str);

	CSize nr_Size = dc->GetTextExtent(_T("编号 "));
	CSize open_temp_Size = dc->GetTextExtent(_T("动作温 "));
	CSize close_temp_Size = dc->GetTextExtent(_T("复位温 "));
	CSize level_Size = dc->GetTextExtent(_T("判定结果 "));

	cur_x = 0;
	cur_y += (int)(chnSize.cy);
	if(record.TestPara.OpenCloseGapLow[0] != 0 || record.TestPara.OpenCloseGapHigh[0] != 0)
	{
		str = _T("编号 动作温 回差   判定结果 ");
	}
	else
	{
		str = _T("编号 动作温 复位温 判定结果 ");
	}
	CSize title_Size = dc->GetTextExtent(str);
	int prn_col = page_rect.right / title_Size.cx;
	int i;

	for(i = 0; i < prn_col; i++)
	{
		dc->TextOut(cur_x,cur_y + (int)(chnSize.cy* 0.3), str);
		cur_x += page_rect.right / prn_col;
	}

	int table_start_y = cur_y;

	cur_y += (int)(chnSize.cy *1.5);
	

	int prn_row = (int)((page_rect.bottom - cur_y) / (chnSize.cy *1.0));

	if(prn_row > (128 / prn_col))
	{
		prn_row = (128 / prn_col);
	}

	
	float open_max_temp = -999.0f;
	float open_min_temp = 999.0f;
	float close_max_temp = -999.0f;
	float close_min_temp = 999.0f;
	float open_avr_temp = 0,close_avr_temp = 0;
	int total_product = 0,total_main_level = 0;

	for(i = 0; i < MAX_LED_NR; i++)
	{
		if(record.SwitchStatus[0][i].IsUsed)
		{
			total_product ++;
			open_avr_temp += record.SwitchStatus[0][i].OpenTemp;
			close_avr_temp+= record.SwitchStatus[0][i].CloseTemp;

			if(CheckLevelItem(record.TestPara,record.SwitchStatus[0][i],0 ) != 0)
			{
				total_main_level++;
			}
			if(record.SwitchStatus[0][i].OpenTemp < open_min_temp)
			{
				open_min_temp = record.SwitchStatus[0][i].OpenTemp;
			}
			if(record.SwitchStatus[0][i].OpenTemp > open_max_temp)
			{
				open_max_temp = record.SwitchStatus[0][i].OpenTemp;
			}

			if(record.SwitchStatus[0][i].CloseTemp < close_min_temp)
			{
				close_min_temp = record.SwitchStatus[0][i].CloseTemp;
			}
			if(record.SwitchStatus[0][i].CloseTemp > close_max_temp)
			{
				close_max_temp = record.SwitchStatus[0][i].CloseTemp;
			}

		}
	}
	
	if(total_product > 0 )
	{
		open_avr_temp /= total_product;
		close_avr_temp/= total_product;
	}


	for(i = 0; i < prn_row; i++)
	{
		for(int j = 0; j < prn_col; j++)
		{
			cur_x = j * page_rect.right / prn_col;
			if(i+prn_row*j <= MAX_LED_NR)

			{
				int index = i+prn_row * j;
				
				if(MAX_LED_NR == 192)
				{
					if((index % 12) < 9)
					{
						str.Format(_T("%02d%d"),index/12,(index%12)+1 );				
					}
					else if((index % 12) == 9)
					{
						str.Format(_T("%02d0"),index/12 + 1);				
					}
					else if((index % 12) == 10)
					{
						str.Format(_T("%02dA"),index/12 + 1);				
					}
					else if((index % 12) == 11)
					{
						str.Format(_T("%02dB"),index/12 +1);				
					}
				}
				else
				{
					str.Format(_T("%3d"),index+1); 
				}
				

				dc->TextOut(cur_x,cur_y, str);
				cur_x += nr_Size.cx;
				
				
				float open_temp = record.SwitchStatus[0][index].OpenTemp;
				str.Format(_T("%5.1f"),open_temp);
				if(open_temp <= -50.0f || open_temp >= 400.0f)
				{
					str = "";
				}
				
				
				if(record.SwitchStatus[0][index].IsOpenFlash)
				{
					if(record.SwitchStatus[0][index].IsUsed)
					{
						str += "F";
					}
				}
				dc->TextOut(cur_x,cur_y, str);
				cur_x += open_temp_Size.cx;
				
				
				float close_temp = record.SwitchStatus[0][index].CloseTemp;;
				if(record.TestPara.OpenCloseGapLow[0] != 0 || record.TestPara.OpenCloseGapHigh[0] != 0)
				{
					float delta = record.SwitchStatus[0][index].CloseTemp - record.SwitchStatus[0][index].OpenTemp;

					if(delta < 0)
					{
						delta  = 0 - delta;
					}
					str.Format(_T("%5.1f"),delta);

					if(close_temp <= -50.0f || close_temp >= 400.0f)
					{
						str = "";
					}
				}
				else
				{
					str.Format(_T("%5.1f"),close_temp);
					if(close_temp <= -50.0f || close_temp >= 400.0f)
					{
						str = "";
					}
				}
				

				if(record.SwitchStatus[0][index].IsCloseFlash)
				{
					if(record.SwitchStatus[0][index].IsUsed) 
						str += "F";
				}
				if(record.SwitchStatus[0][index].IsUsed)
				{
					if(CheckLevelItem(record.TestPara,record.SwitchStatus[0][index],0 ) != 0)
					{
						str += "    合格";
					}
					else
					{
						str += "    不合格";
					}
				}
				else
				{
					str += "    ---";
				}
				dc->TextOut(cur_x,cur_y, str);
				cur_x += close_temp_Size.cx;
				
				str = "";

			}		
		}
		
		cur_y += (int)(chnSize.cy);
	}

	
	for(i = 0; i <= prn_row + 1; i++)
	{
		dc->MoveTo(0,table_start_y + i * chnSize.cy + (int)(chnSize.cy* 0.3));
		dc->LineTo(page_rect.right,table_start_y + i * chnSize.cy + (int)(chnSize.cy*0.3)); 
	}
	
	
	for(i = 0; i < prn_col; i++)
	{
		cur_x = i*page_rect.right / prn_col;
		int start_x = table_start_y + (int)(chnSize.cy* 0.3);
		int end_y = table_start_y + (prn_row + 1) * chnSize.cy + (int)(chnSize.cy*0.3);
		dc->MoveTo(cur_x, start_x);
		dc->LineTo(cur_x, end_y);

		dc->MoveTo(cur_x+nr_Size.cx, start_x);
		dc->LineTo(cur_x+nr_Size.cx, end_y);

		dc->MoveTo(cur_x+nr_Size.cx+ open_temp_Size.cx, start_x);
		dc->LineTo(cur_x+nr_Size.cx+ open_temp_Size.cx, end_y);
		
		dc->MoveTo(cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, start_x);
		dc->LineTo(cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, end_y);
		
	}
	

	table_start_y = cur_y;
	
	cur_x = chnSize.cx ;
	dc->TextOut(cur_x,cur_y, _T("产品数"));
	cur_x += page_rect.right /6;
	str.Format(_T("%d"),total_product);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("合格品"));
	cur_x += page_rect.right /6;
	str.Format(_T("%d"),total_main_level);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("合格率"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f%%"),(float)(total_main_level*100.0f/total_product));
	dc->TextOut(cur_x,cur_y, str);
	
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("动作最高温"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),open_max_temp);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("动作最低温"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),open_min_temp);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("动作平均温"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),open_avr_temp);
	dc->TextOut(cur_x,cur_y, str);
	
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("复位最高温"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),close_max_temp);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位最低温"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),close_min_temp);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位平均温"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),close_avr_temp);
	dc->TextOut(cur_x,cur_y, str);

	cur_y += (int)(chnSize.cy);

	//CPen pen2(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
	//oldPen = dc->SelectObject(&pen2);
	//oldPen->DeleteObject();

	for(i = 0; i <= 7; i++)
	{
		cur_x = i*page_rect.right / 6;
		int start_y = table_start_y;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, cur_y);
	}

	for(i = 0; i <= 3; i++)
	{
		int start_y = table_start_y + i * chnSize.cy ;
		//int end_y = cur_y;
		dc->MoveTo(0, start_y);
		dc->LineTo(page_rect.right, start_y);
	}

	
	cur_y += (int)(chnSize.cy / 2);
	table_start_y = cur_y;

	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("断开温度"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),record.TestPara.OpenTemp[0]);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("负偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),record.TestPara.OpenTempMinus[0]);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("正偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),record.TestPara.OpenTempPlus[0]);
	dc->TextOut(cur_x,cur_y, str);
	cur_y += (int)(chnSize.cy);
//
	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("开停差下限"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),record.TestPara.OpenCloseGapLow[0] );
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("开停差上限"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),record.TestPara.OpenCloseGapHigh[0]);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	cur_y += (int)(chnSize.cy);
	
	for(i = 0; i <= 7; i++)
	{
		cur_x = i*page_rect.right / 6;
		int start_y = table_start_y;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, cur_y);
	}
	for(i = 0; i <= 2; i++)
	{
		int start_y = table_start_y + i * chnSize.cy ;
		//int end_y = cur_y;
		dc->MoveTo(0, start_y);
		dc->LineTo(page_rect.right, start_y);
	}

	
	
	//dc->TextOut(page_rect.right/2,page_rect.bottom - chnSize.cy, "制表： 常州市润邦电子科技有限公司 TEL:018052531601,013915838598");

}

void CViewTestRecordDlg::ShowFile(CString str_Dir, HTREEITEM tree_Root)
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
void CViewTestRecordDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
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

			
		TEST_RECORD record; 
		
		CFile test_result_file;
		if(test_result_file.Open(::theAppDirectory + _T("log\\") + filename, /*CFile::modeCreate | CFile::modeNoTruncate |*/ CFile::modeReadWrite))
		{
			
			test_result_file.SeekToBegin();

				if(test_result_file.Read(&record,sizeof(TEST_RECORD)) ==  sizeof(TEST_RECORD))
				{
					CString s;
					CTime t(record.BeginTime);
					s.Format(_T("%04d/%02d/%02d %02d:%02d:%02d %s"), t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),record.ParaName);
					
					
					CString s1;
					CString sPara;
					
						if(m_RecordDetailList.GetCount() > 0)
						{
							m_RecordDetailList.ResetContent();	  
						}
						//m_Record.TestPara.
						for(int pro = 0; pro < 4; pro++)
						{
							if(pro != 0)
							{
								if(record.TestPara.TestPoint[pro] == 0)
								{
									break;
								}
							}

							for(int i = 0; i < MAX_LED_NR; i++)
							{
								s.Format(_T("%03d"), i+1);
								if(record.SwitchStatus[0][i].IsUsed)
								{
									
									if(::G_NormalConfigPara.Language   == 1)
									{
										s+=_T(",  Used");
									}
									else
									{
										s+=_T(",  有产品");
									}
								}
								else
								{

									continue;
									
									if(G_NormalConfigPara.Language == 1)
									{
										s+=_T(",  N/C ");
									}
									else
									{
										s+=_T(",  无产品");
									}
								}
								if(record.SwitchStatus[0][i].IsOpenned)
								{
									s1.Format(_T(",%8.1f"),record.SwitchStatus[0][i].OpenTemp);  
									s += s1;
								}
								else
								{
									s += _T(",    没断开");
								}
								if(record.SwitchStatus[0][i].IsClosed)
								{
									s1.Format(_T(",%8.1f"),record.SwitchStatus[0][i].CloseTemp);  
									s += s1;
								}
								else
								{
									s += _T(",    没复位");
								}
								if(record.SwitchStatus[0][i].IsOpenned && record.SwitchStatus[0][i].IsClosed)
								{
									if(record.TestPara.HeatCoolType[0] == 0)
									{
										s1.Format(_T(",%8.1f"),record.SwitchStatus[0][i].OpenTemp - record.SwitchStatus[0][i].CloseTemp);  
										s += s1;
									}
									else
									{
										s1.Format(_T(",%8.1f"),record.SwitchStatus[0][i].CloseTemp - record.SwitchStatus[0][i].OpenTemp);  
										s += s1;
									}
								}
								else
								{
									s += _T(",     --.--");
								}
								if(record.SwitchStatus[0][i].IsOpenFlash || record.SwitchStatus[0][i].IsCloseFlash)
								{
									
									if(G_NormalConfigPara.Language == 1)
									{
										s+=_T(",  Flash");
									}
									else
									{
										s += _T(",闪动");
									}
								}
								else
								{
									s += _T(",    ");
								}
								
								if(record.SwitchStatus[0][i].IsUsed)
								{
									if(CheckLevelItem(record.TestPara,record.SwitchStatus[0][i],0 ) != 0)
									{
										s += _T(",合格");
									}
									else
									{
										s += _T(",不合格");
									}
								}
								else
								{
									s += _T(",---");
								}

								m_RecordDetailList.AddString(s); 
							}
							sPara  += _T("测试参数: ");
							if(G_NormalConfigPara.Language == 1)
							{
								sPara +=_T("Test Para: ");
							}
							s1 = record.ParaName;
							sPara += s1;

							sPara+=_T("\r\n");
							s1.Format(_T("过程%d: \r\n断开温度%5.1f\r\n负偏差:%5.1f\r\n正偏差:%5.1f\r\n开停差:%5.1f~%5.1f\r\n"),
								pro+1,
								record.TestPara.OpenTemp[pro],
								record.TestPara.OpenTempMinus[pro],
								record.TestPara.OpenTempPlus[pro], 
								record.TestPara.OpenCloseGapLow[pro],
								record.TestPara.OpenCloseGapHigh[pro]); 
							if(G_NormalConfigPara.Language == 1)
							{
								s1.Format(_T("Pro %d: \r\nOpen:%5.1f\r\nGap -:%5.1f\r\nGap +:%5.1f\r\nClose Gap:%5.1f~%5.1f\r\n"),
									pro+1,
									record.TestPara.OpenTemp[pro],
									record.TestPara.OpenTempMinus[pro],
									record.TestPara.OpenTempPlus[pro], 
									record.TestPara.OpenCloseGapLow[pro],
									record.TestPara.OpenCloseGapHigh[pro]); 
							}
							sPara+=s1;

						}
						
						this->m_EditPara.SetWindowTextW(sPara);  

				
					}
				}
		}

}

void CViewTestRecordDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CViewTestRecordDlg::OnBnClickedButtonCur()
{
	// TODO: 在此添加控件通知处理程序代码

	CPrintDialog prnDlg(true);
	CString printer;
	CDC dc;
	int i;


	CFullScreenCurve dlg;
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
		filename.Replace(_T(".record"),_T(".cur")); 
		
		CFile RecFile;
		if(RecFile.Open(::theAppDirectory + _T("log\\") + filename, /*CFile::modeCreate | CFile::modeNoTruncate |*/ CFile::modeReadWrite))
		{
			
			TEMP_RECORD tr[10000];
			
			RecFile.SeekToBegin();
			while(1)
			{
				DWORD len = RecFile.Read(&tr,sizeof(tr));
				
				if(len > 0)
				{
					for(int i = 0 ; i < (len / sizeof(TEMP_RECORD)); i++)
					{
						dlg.m_TempRecordArray.Add(tr[i]);  
					}
				}
				else
				{
					break;
				}
			}

		}
	}

	dlg.DoModal();

}
