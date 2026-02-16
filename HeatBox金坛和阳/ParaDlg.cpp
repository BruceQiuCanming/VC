// ParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "ParaDlg.h"
#include "HeatBoxDlg.h"
//#include "ParaRecordset.h"
#include "CreateModifyParaDlg.h"
#include "InputDlg.h"
#include "shlwapi.h"
#include "TempAdjustDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParaDlg dialog


CParaDlg::CParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParaDlg::IDD, pParent)
	, m_iNormalPidMinutes(0)
	, m_HeatBoxType(0)
	, m_bFirstCoolSencondHeat_CloseAC_Before_Heat(FALSE)
{
	

	//{{AFX_DATA_INIT(CParaDlg)
	m_CloseMainLevel = 0.0;
	m_CloseMainLevelSpanPlus = 0.0;
	m_CloseMainLevelSpanMinus = 0.0;
	m_HighLevel = 0.0;
	m_HighLevelSpanMinus = 0.0;
	m_HighLevelSpanPlus = 0.0;
	m_LowLevel = 0.0;
	m_LowLevelSpanMinus = 0.0;
	m_LowLevelSpanPlus = 0.0;
	m_MainLevelSpanMinus = 0.0;
	m_MainLevelSpanPlus = 0.0;
	m_MainLevel = 0.0;
	m_ProductYear = 0;
	m_ProductMonth = 0;
	m_ProductDay = 0;
	//}}AFX_DATA_INIT
	CTime t = t.GetCurrentTime();
	m_ProductYear = t.GetYear();
	m_ProductMonth = t.GetMonth();
	m_ProductDay = t.GetDay();

	m_bAllOpenned_Closed_To_StopTest = false;
}


void CParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	//{{AFX_DATA_MAP(CParaDlg)
	DDX_Control(pDX, IDC_COMBO_CUR_MAX_HEAT_BOX, m_CurMaxHeatBox);
	DDX_Control(pDX, IDC_COMBO_TEST_MODE, m_TestModeList);
	DDX_Control(pDX, IDC_COMBO_HEAT_SPEED, m_ComboBoxHeatSpeed);
	DDX_Control(pDX, IDC_COMBO_OPEN_DOOR_RESET_LEVEL, m_OpenDoorResetList);
	DDX_Control(pDX, IDC_COMBO_PARA_NAME, m_MainLevelList);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL, m_CloseMainLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS, m_CloseMainLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS, m_CloseMainLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_DEVIATION_PLUS, m_CloseMainLevelDeviationPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_DEVIATION_MINUS, m_CloseMainLevelDeviationMinus);

	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL2, m_CloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2, m_CloseHighLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2, m_CloseHighLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_PLUS, m_CloseHighLevelDeviationPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_MINUS, m_CloseHighLevelDeviationMinus);

	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL3, m_CloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS3, m_CloseLowLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS3, m_CloseLowLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_PLUS, m_CloseLowLevelDeviationPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_MINUS, m_CloseLowLevelDeviationMinus);


	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_LOW, m_HighLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_SPAN_MINUS, m_HighLevelSpanMinus);
	DDV_MinMaxDouble(pDX, m_HighLevelSpanMinus, 0., 99.9);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_SPAN_PLUS, m_HighLevelSpanPlus);
	DDV_MinMaxDouble(pDX, m_HighLevelSpanPlus, 0., 99.9);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_LOW, m_LowLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_SPAN_MINUS, m_LowLevelSpanMinus);
	DDV_MinMaxDouble(pDX, m_LowLevelSpanMinus, 0., 99.9);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_SPAN_PLUS, m_LowLevelSpanPlus);
	DDV_MinMaxDouble(pDX, m_LowLevelSpanPlus, 0., 99.9);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL_SPAN_MINUS, m_MainLevelSpanMinus);
	DDV_MinMaxDouble(pDX, m_MainLevelSpanMinus, 0., 99.9);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL_SPAN_PLUS, m_MainLevelSpanPlus);
	DDV_MinMaxDouble(pDX, m_MainLevelSpanPlus, 0., 99.9);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL2, m_MainLevel);

	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PID_MINUTES, m_iNormalPidMinutes);
	DDX_CBIndex(pDX, IDC_COMBO_HEAT_BOX_TYPE, m_HeatBoxType);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_TEMP_TYPE, m_ComboBoxDisplayTempType);
	DDX_Control(pDX, IDC_EDIT_CONTINUE_TEST_COUNT, m_EditContinueTestCount);
	DDX_Control(pDX, IDC_COMBO_END_TEST_DOOR_STATUS, m_Combo_EndTestDoorStatus);
	DDX_Control(pDX, IDC_CHECK_STOP_TEST_TYPE, m_ControlStopTestType);
	DDX_Control(pDX, IDC_COMBO_485_PROTOCOL, m_Combo485Protocol);
	DDX_Control(pDX, IDC_COMBO_COOL_DEGREE, m_ComboCoolDegree);
	DDX_Control(pDX, IDC_COMBO_COOL_SPEED, m_ComboBoxCoolSpeed);
	DDX_Check(pDX, IDC_CHECK_STOP_TEST_TYPE2, m_bFirstCoolSencondHeat_CloseAC_Before_Heat);
}


BEGIN_MESSAGE_MAP(CParaDlg, CDialog)
	//{{AFX_MSG_MAP(CParaDlg)
	ON_EN_CHANGE(IDC_EDIT_MAIN_LEVEL_SPAN, OnChangeEditMainLevelSpan)
	ON_EN_CHANGE(IDC_EDIT_HIGH_LEVEL_SPAN_MINUS, OnChangeEditHighLevelSpan)
	ON_EN_CHANGE(IDC_EDIT_LOW_LEVEL_SPAN, OnChangeEditLowLevelSpan)
	ON_EN_CHANGE(IDC_EDIT_MAIN_LEVEL_SPAN_MINUS, OnChangeEditMainLevelSpanMinus)
	ON_EN_CHANGE(IDC_EDIT_MAIN_LEVEL_SPAN_PLUS, OnChangeEditMainLevelSpanPlus)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_MODIFY_PARA, OnButtonCreateModifyPara)
	ON_CBN_SELCHANGE(IDC_COMBO_HEAT_SPEED, OnSelchangeComboHeatSpeed)
	ON_EN_CHANGE(IDC_EDIT_YEAR, OnChangeEditYear)
	ON_CBN_EDITCHANGE(IDC_COMBO_TEST_MODE, OnEditchangeComboTestMode)
	ON_CBN_SELCHANGE(IDC_COMBO_TEST_MODE, OnSelchangeComboTestMode)
	ON_EN_CHANGE(IDC_EDIT_HEAT_DEGREE, OnChangeEditHeatDegree)
	ON_EN_CHANGE(IDC_EDIT_COOL_DEGREE, OnChangeEditCoolDegree)
	ON_CBN_SELCHANGE(IDC_COMBO_CUR_MAX_HEAT_BOX, OnSelchangeComboCurMaxHeatBox)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
//	ON_EN_CHANGE(IDC_EDIT_PID_MINUTES, &CParaDlg::OnEnChangeEditPidMinutes)
	ON_CBN_SELCHANGE(IDC_COMBO_PARA_NAME, &CParaDlg::OnCbnSelchangeComboParaName)
	ON_CBN_SELCHANGE(IDC_COMBO_HEAT_BOX_TYPE, &CParaDlg::OnCbnSelchangeComboHeatBoxType)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_TEMP_TYPE, &CParaDlg::OnCbnSelchangeComboDisplayTempType)
	ON_CBN_SELCHANGE(IDC_COMBO_END_TEST_DOOR_STATUS, &CParaDlg::OnCbnSelchangeComboEndTestDoorStatus)
	ON_EN_CHANGE(IDC_EDIT_CONTINUE_TEST_COUNT, &CParaDlg::OnEnChangeEditContinueTestCount)
	ON_BN_CLICKED(IDC_CHECK_RELAY, &CParaDlg::OnBnClickedCheckRelay)
	ON_BN_CLICKED(IDC_CHECK_STOP_TEST_TYPE, &CParaDlg::OnBnClickedCheckStopTestType)
	ON_EN_SETFOCUS(IDC_EDIT_PID_MINUTES, &CParaDlg::OnEnSetfocusEditPidMinutes)
	ON_EN_SETFOCUS(IDC_EDIT_MAIN_LEVEL2, &CParaDlg::OnEnSetfocusEditMainLevel2)
	ON_CBN_SETFOCUS(IDC_COMBO_HEAT_SPEED, &CParaDlg::OnCbnSetfocusComboHeatSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_CONTINUE_TEST_COUNT, &CParaDlg::OnEnSetfocusEditContinueTestCount)
	ON_CBN_SELCHANGE(IDC_COMBO_485_PROTOCOL, &CParaDlg::OnCbnSelchangeCombo485Protocol)
	ON_CBN_SELCHANGE(IDC_COMBO_COOL_DEGREE, &CParaDlg::OnCbnSelchangeComboCoolDegree)
	ON_CBN_SELCHANGE(IDC_COMBO_COOL_SPEED, &CParaDlg::OnCbnSelchangeComboCoolSpeed)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_ADJUST, &CParaDlg::OnBnClickedButtonTempAdjust)
	ON_BN_CLICKED(IDC_CHECK_STOP_TEST_TYPE2, &CParaDlg::OnBnClickedCheckStopTestType2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParaDlg message handlers



//遍历文件夹函数

void TraverseFolder(LPCTSTR lpPath,LPCTSTR file_ext,CStringArray *fileStr)

{

    TCHAR szFind[MAX_PATH] = {_T("\0")};

    WIN32_FIND_DATA findFileData;

    BOOL bRet;

 

    _tcscpy_s(szFind, MAX_PATH, lpPath);

    _tcscat_s(szFind, file_ext);     //这里一定要指明通配符，不然不会读取所有文件和目录

 

    HANDLE hFind = ::FindFirstFile(szFind, &findFileData);

    if (INVALID_HANDLE_VALUE == hFind)

    {

        return;

    }

 

    //遍历文件夹

    while (TRUE)

    {

        if (findFileData.cFileName[0] != _T('.'))

        {//不是当前路径或者父目录的快捷方式

           
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)

            {//这是一个普通目录

                //设置下一个将要扫描的文件夹路径

               // _tcscpy_s(szFind, MAX_PATH, lpPath);    

                //_tcscat_s(szFind, _T("\\"));    

               // _tcscat_s(szFind, findFileData.cFileName);

                ///_tcscat_s(szNextDir, _T("\\*"));

                //遍历该目录

                //TraverseFolder(szFind);

            }
			else
			{
				 CString s;
				s.Format(_T("%s"), findFileData.cFileName);
				fileStr->Add(s);
			}

        }

        //如果是当前路径或者父目录的快捷方式，或者是普通目录，则寻找下一个目录或者文件

        bRet = ::FindNextFile(hFind, &findFileData);

        if (!bRet)

        {//函数调用失败

            //cout << "FindNextFile failed, error code: " 

            //  << GetLastError() << endl;

            break;

        }

    }

 

    ::FindClose(hFind);

}

BOOL CParaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	// TODO: Add extra initialization here
	m_bSetFocusOnEditMainLevel2 = true;


	CFile ParaFile;
	CFileFind ff;
	CString fName;
	CString sDir;
	CStringArray sa;

	
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	TraverseFolder(sDir,_T("*.para"),&sa);

	for(int i = 0; i < sa.GetSize(); i ++)
	{
		CString s = sa.GetAt(i);
		int pos  = s.Find(_T("."));
		if(pos > 0)
		{
			s = s.Left(pos); 
		}
		this->m_MainLevelList.AddString(s);
	}

	/*
	CParaRecordset para_rs;
	para_rs.Open();


	if(para_rs.IsEOF() && para_rs.IsBOF())
	{
	}
	else
	{
		para_rs.MoveFirst();
	}
	
	while(!para_rs.IsEOF())
	{
		para_rs.m_para_name.TrimLeft(); 
		para_rs.m_para_name.TrimRight(); 
		this->m_MainLevelList.AddString(para_rs.m_para_name);
		para_rs.MoveNext(); 
	}

	*/

	m_CurMaxHeatBox.SetCurSel(G_CurMaxHeatBox - 1);	
	
	CString s;
	s.Format(_T("END_TEST_DOOR_STATUS_%d"),m_BoxNr); 
	
	int  door_status = AfxGetApp()->GetProfileIntW(_T("SETTING"),s,0); 
	m_Combo_EndTestDoorStatus.SetCurSel(door_status);  

	
	s.Format(_T("STOP_TEST_TYPE_%d"),m_BoxNr); 

	if(AfxGetApp()->GetProfileIntW(_T("SETTING"),s,0) != 0)   
	{
		m_ControlStopTestType.SetCheck(true); 
	}
	else
	{
		m_ControlStopTestType.SetCheck(false);
	}
	
	 
	CString str;
	str.Format(_T("IS CMD ASCII %d"),m_BoxNr); 

	
	
	m_ComboCoolDegree.AddString(_T("无压缩机"));
	for(int i = 1; i <= 100; i++)
	{
		str.Format(_T("低于%d度"),i);
		m_ComboCoolDegree.AddString(str);
	}

	m_ComboCoolDegree.SetCurSel( AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("AIR CONTROL START"),0) ); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CParaDlg::OnChangeEditMainLevelSpan() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
/*	this->UpdateData();
	
	m_HighLevel = m_MainLevelList.GetCurSel()+1 + m_MainLevelSpanPlus;
	m_LowLevel  = m_MainLevelList.GetCurSel()+1 - m_MainLevelSpanMinus;
	m_OverHighLevel = m_HighLevel + m_HighLevelSpan;
	m_OverLowLevel  = m_LowLevel  - m_LowLevelSpan;
	
	this->UpdateData(false);
*/
}

void CParaDlg::OnChangeEditHighLevelSpan() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
/*	this->UpdateData();
	
	m_HighLevel = m_MainLevelList.GetCurSel()+1 + m_MainLevelSpanPlus;
	m_LowLevel  = m_MainLevelList.GetCurSel()+1 - m_MainLevelSpanMinus;
	m_OverHighLevel = m_HighLevel + m_HighLevelSpan;
	m_OverLowLevel  = m_LowLevel  - m_LowLevelSpan;
	
	this->UpdateData(false);
*/
}

void CParaDlg::OnChangeEditLowLevelSpan() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
/*		this->UpdateData();
	
	m_HighLevel = m_MainLevelList.GetCurSel()+1 + m_MainLevelSpanPlus;
	m_LowLevel  = m_MainLevelList.GetCurSel()+1 - m_MainLevelSpanMinus;
	m_OverHighLevel = m_HighLevel + m_HighLevelSpan;
	m_OverLowLevel  = m_LowLevel  - m_LowLevelSpan;
	
	this->UpdateData(false);
*/
}

void CParaDlg::OnChangeEditMainLevelSpanMinus() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
/*	this->UpdateData();
	
	m_HighLevel = m_MainLevelList.GetCurSel()+1 + m_MainLevelSpanPlus;
	m_LowLevel  = m_MainLevelList.GetCurSel()+1 - m_MainLevelSpanMinus;
	m_OverHighLevel = m_HighLevel + m_HighLevelSpan;
	m_OverLowLevel  = m_LowLevel  - m_LowLevelSpan;
	
	this->UpdateData(false);
*/
}

void CParaDlg::OnChangeEditMainLevelSpanPlus() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
/*	this->UpdateData();
	
	m_HighLevel = m_MainLevelList.GetCurSel()+1 + m_MainLevelSpanPlus;
	m_LowLevel  = m_MainLevelList.GetCurSel()+1 - m_MainLevelSpanMinus;
	m_OverHighLevel = m_HighLevel + m_HighLevelSpan;
	m_OverLowLevel  = m_LowLevel  - m_LowLevelSpan;
	
	this->UpdateData(false);	
	*/
}

LRESULT CParaDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CParaDlg::UpdateModifyPara() 
{
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	maindlg->ReadSwitchConfigPara(); 

	m_MainLevelList.ResetContent();

	CFile ParaFile;
	CFileFind ff;
	CString fName;
	CString sDir;
	CStringArray sa;

	
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	TraverseFolder(sDir,_T("*.para"),&sa);

	for(int i = 0; i < sa.GetSize(); i ++)
	{
		CString s = sa.GetAt(i);
		int pos  = s.Find(_T("."));
		if(pos > 0)
		{
			s = s.Left(pos); 
		}
		this->m_MainLevelList.AddString(s);
	}

	/*
	CParaRecordset para_rs;
	para_rs.Open();
	if(para_rs.IsEOF() && para_rs.IsBOF())
	{
	}
	else
	{
		para_rs.MoveFirst();
	}
	while(!para_rs.IsEOF())
	{
		para_rs.m_para_name.TrimLeft(); 
		para_rs.m_para_name.TrimRight(); 
		this->m_MainLevelList.AddString(para_rs.m_para_name);
		para_rs.MoveNext(); 
	}
	para_rs.Close(); 
	*/
}
void CParaDlg::OnButtonCreateModifyPara() 
{
	// TODO: Add your control notification handler code here

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	

	CCreateModifyParaDlg dlg;
	dlg.DoModal(); 

	

	for(int i = 0; i < G_CurMaxHeatBox; i++)
	{
		maindlg->m_ParaDlg.UpdateModifyPara();
	}

}

void CParaDlg::OnSelchangeComboHeatSpeed() 
{
	// TODO: Add your control notification handler code here
	int curSel = this->m_ComboBoxHeatSpeed.GetCurSel(); 
	if(curSel < 0)
	{
		curSel = 19;
	}
	
	CString s;
	
	
	
	
#ifdef _OIL_
	curSel+=2;
#endif

	s.Format(_T("TEST_SPEED_%d"),m_BoxNr);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,curSel);


	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	maindlg->ReadSwitchConfigPara();
}

void CParaDlg::OnChangeEditYear() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	this->UpdateData(); 
}

void CParaDlg::OnEditchangeComboTestMode() 
{
	// TODO: Add your control notification handler code here
	CString s;
	int curSel = this->m_TestModeList.GetCurSel();  

	s.Format(_T("TEST_MODE_%d"),m_BoxNr);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,curSel);

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	maindlg->ReadSwitchConfigPara();
}

void CParaDlg::OnSelchangeComboTestMode() 
{
	// TODO: Add your control notification handler code here
	CWnd *wnd;
	CString s;
	int curSel = this->m_TestModeList.GetCurSel();  

	s.Format(_T("TEST_MODE_%d"),m_BoxNr);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,curSel);

	if(curSel != TEST_MODE_NORMAL)
	{
		wnd = this->GetDlgItem(IDC_STATIC_HEAT);
		wnd->ShowWindow(SW_SHOW);
		wnd = this->GetDlgItem(IDC_STATIC_COOL);
		wnd->ShowWindow(SW_SHOW);
		wnd = this->GetDlgItem(IDC_EDIT_HEAT_DEGREE);
		wnd->ShowWindow(SW_SHOW);
		wnd = this->GetDlgItem(IDC_EDIT_COOL_DEGREE);
		wnd->ShowWindow(SW_SHOW);
	}
	else
	{
		wnd = this->GetDlgItem(IDC_STATIC_HEAT);
		wnd->ShowWindow(SW_HIDE);
		wnd = this->GetDlgItem(IDC_STATIC_COOL);
		wnd->ShowWindow(SW_HIDE);
		wnd = this->GetDlgItem(IDC_EDIT_HEAT_DEGREE);
		wnd->ShowWindow(SW_HIDE);
		wnd = this->GetDlgItem(IDC_EDIT_COOL_DEGREE);
		wnd->ShowWindow(SW_HIDE);
	}

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	maindlg->ReadSwitchConfigPara();
}

void CParaDlg::OnChangeEditHeatDegree() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	TCHAR buf[100];
	
	CWnd * wnd = this->GetDlgItem(IDC_EDIT_HEAT_DEGREE);
	wnd->GetWindowText(buf,sizeof(buf));
	
	double m_OpenDoorResetHeatDegree = _tcstod(buf,NULL);//atof(buf);

	CString s;
	s.Format(_T("OPEN_DOOR_RESET_HEAT_DEGREE_%d"),m_BoxNr);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,(int)((m_OpenDoorResetHeatDegree + 0.05) * 10.0f));	

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	maindlg->m_SwitchDlg.m_OpenDoorResetHeatDegree = m_OpenDoorResetHeatDegree;   
}

void CParaDlg::OnChangeEditCoolDegree() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	TCHAR buf[100];
	
	CWnd * wnd = this->GetDlgItem(IDC_EDIT_COOL_DEGREE);
	wnd->GetWindowText(buf,sizeof(buf));
	
	double m_OpenDoorResetCoolDegree = _tcstod(buf,NULL);//atof(buf);

	CString s;
	s.Format(_T("OPEN_DOOR_RESET_COOL_DEGREE_%d"),m_BoxNr);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,(int)((m_OpenDoorResetCoolDegree + 0.05) * 10.0f));	

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	maindlg->m_SwitchDlg.m_OpenDoorResetCoolDegree = m_OpenDoorResetCoolDegree;   
}

//将TCHAR转为char   
//*tchar是TCHAR类型指针，*_char是char类型指针   
void TcharToChar (const TCHAR * tchar, char * _char)  
{  
    int iLength ;  
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);  
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);   
}  

void CParaDlg::OnSelchangeComboCurMaxHeatBox() 
{
	// TODO: Add your control notification handler code here

	
	AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("MAX_HEAT_BOX"),(int)m_CurMaxHeatBox.GetCurSel());	

	AfxMessageBox(_T("控制箱数量已经改变\r\n程序需要关闭重启!"));
	exit(1);

}

HBRUSH CParaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

//void CParaDlg::OnEnChangeEditPidMinutes()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
//	// 发送该通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
//	
//	UpdateData(true);
//	CString s;
//	s.Format(_T("NORMAL_PID_MINUTES_%d"),m_BoxNr);
//	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,m_iNormalPidMinutes);
//
//	maindlg->ReadSwitchConfigPara();
//}

//void CParaDlg::OnCbnSelendcancelComboParaName()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//}

void CParaDlg::OnCbnSelchangeComboParaName()
{
	// TODO: 在此添加控件通知处理程序代码

	
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	CString str;

		
	
	CString name;
	
	str.Format(_T("SWITCH_CONFIG_PARA_BOX_NR_%d"),m_BoxNr);
	

	m_MainLevelList.GetLBText(m_MainLevelList.GetCurSel(),name);
	AfxGetApp()->WriteProfileString(_T("SETTING"),
	 		str,name);

	maindlg->SaveSwitchConfigPara(m_BoxNr);

	maindlg->ReadSwitchConfigPara();

	


}

void CParaDlg::OnCbnSelchangeComboHeatBoxType()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(); 
	CString s;
	s.Format(_T("HEAT_BOX_TYPE_%d"),m_BoxNr);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,(int)m_HeatBoxType);	
	
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	maindlg->ReadSwitchConfigPara();
}

void CParaDlg::OnCbnSelchangeComboDisplayTempType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	s.Format(_T("DISPLAY_TEMP_TYPE_%d"),m_BoxNr); 
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,(int)m_ComboBoxDisplayTempType.GetCurSel());	

}


void CParaDlg::ShowCloseLevelEdit(void)
{
	CWnd * w;
	
	if( m_SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_RANGE)
	{
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL2);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL3);
		w->ShowWindow(SW_SHOW);
	}
	else
	{
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL2);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL3);
		w->ShowWindow(SW_HIDE);
	}
}
void CParaDlg::OnCbnSelchangeComboContinueTestCount()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CParaDlg::OnCbnSelchangeComboEndTestDoorStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = this->m_Combo_EndTestDoorStatus.GetCurSel();  
	CString s;
	s.Format(_T("END_TEST_DOOR_STATUS_%d"),m_BoxNr); 
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,i); 

}

void CParaDlg::OnEnChangeEditContinueTestCount()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString s;
	TCHAR buf[100];
	TCHAR *pstr = NULL;

	s.Format(_T("CONTINUE_TEST_COUNT_%d"),m_BoxNr); 
	m_EditContinueTestCount.GetWindowTextW(buf,100);
	int i = _tcstol(buf,&pstr,10);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,i);    

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	maindlg->m_SwitchDlg.m_iContinueTestCount = i;

}

void CParaDlg::OnBnClickedCheckRelay()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CParaDlg::OnBnClickedCheckStopTestType()
{
	// TODO: 在此添加控件通知处理程序代码
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	CString s;
	s.Format(_T("STOP_TEST_TYPE_%d"),m_BoxNr); 

	if(m_ControlStopTestType.GetCheck())
	{
		AfxGetApp()->WriteProfileInt(_T("SETTING"),s,1);  
		maindlg->m_SwitchDlg.m_StopTestType = 1;   
	}
	else
	{
		AfxGetApp()->WriteProfileInt(_T("SETTING"),s,0); 
		maindlg->m_SwitchDlg.m_StopTestType = 0; 
	}
}

void CParaDlg::OnEnSetfocusEditPidMinutes()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bSetFocusOnEditMainLevel2 == false)
	{
		return;
	}
	m_bSetFocusOnEditMainLevel2 = false;
	CWnd *w = this->GetDlgItem(IDC_COMBO_HEAT_SPEED); 
	

	
	CInputDlg dlg;
	dlg.m_Title = _T("设置恒温时间(分钟)");
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(IDC_EDIT_PID_MINUTES) ;
		w->SetWindowTextW(dlg.m_Input);

		CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
		UpdateData(true);
		CString s;
		s.Format(_T("NORMAL_PID_MINUTES_%d"),m_BoxNr);
		AfxGetApp()->WriteProfileInt(_T("SETTING"),s,m_iNormalPidMinutes);

		maindlg->ReadSwitchConfigPara();
	}
	

	w->SetFocus(); 
}

void CParaDlg::OnEnSetfocusEditMainLevel2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSetFocusOnEditMainLevel2 = true;
}

void CParaDlg::OnCbnSetfocusComboHeatSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSetFocusOnEditMainLevel2 = true;
}

void CParaDlg::OnEnSetfocusEditContinueTestCount()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_bSetFocusOnEditMainLevel2 == false)
	{
		return;
	}
	m_bSetFocusOnEditMainLevel2 = false;
	CWnd *w = this->GetDlgItem(IDC_COMBO_HEAT_SPEED); 

	
	CInputDlg dlg;
	dlg.m_Title = _T("设置连续测试次数");
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(IDC_EDIT_CONTINUE_TEST_COUNT) ;
		w->SetWindowTextW(dlg.m_Input);

		CString s;
		TCHAR buf[100];
		TCHAR *pstr = NULL;

		s.Format(_T("CONTINUE_TEST_COUNT_%d"),m_BoxNr); 
		m_EditContinueTestCount.GetWindowTextW(buf,100);
		int i = _tcstol(buf,&pstr,10);
		AfxGetApp()->WriteProfileInt(_T("SETTING"),s,i);    

		CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
		maindlg->m_SwitchDlg.m_iContinueTestCount = i;

		
	
	}
	

	w->SetFocus(); 

}

void CParaDlg::OnCbnSelchangeCombo485Protocol()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CParaDlg::OnCbnSelchangeComboCoolDegree()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("AIR CONTROL START"),m_ComboCoolDegree.GetCurSel()); 
}

void CParaDlg::OnCbnSelchangeComboCoolSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	int curSel = this->m_ComboBoxCoolSpeed.GetCurSel(); 
	if(curSel < 0)
	{
		curSel = 19;
	}
	
	CString s;
	
	
	
	
#ifdef _OIL_
	curSel+=2;
#endif

	s.Format(_T("TEST_COOL_SPEED_%d"),m_BoxNr);
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,curSel);


	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	
	maindlg->ReadSwitchConfigPara();
}

void CParaDlg::OnBnClickedButtonTempAdjust()
{
	// TODO: 在此添加控件通知处理程序代码

	CTempAdjustDlg dlg;

	for(int i = 0; i < 500; i++)
	{
		dlg.m_AdjustTemp[i][0] = G_SwitchTempAdjust[m_BoxNr][i][0]; 
		dlg.m_AdjustTemp[i][1] = G_SwitchTempAdjust[m_BoxNr][i][1]; 
	}

	if(dlg.DoModal() == IDOK)
	{
		for(int i = 0; i < 500; i++)
		{
			G_SwitchTempAdjust[m_BoxNr][i][0] = dlg.m_AdjustTemp[i][0]; 
			G_SwitchTempAdjust[m_BoxNr][i][1] = dlg.m_AdjustTemp[i][1];
		}

		CFile ParaFile;
	
		CString sDir = theAppDirectory +_T("para\\");

		if(!PathIsDirectory(sDir))
		{
			::CreateDirectory(sDir,NULL); 
		}

			
		if(ParaFile.Open(sDir + _T("工位修正") + _T(".para"),CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
		{
			
			ParaFile.SeekToBegin();
			
			ParaFile.Write(G_SwitchTempAdjust, sizeof(G_SwitchTempAdjust));
		
			ParaFile.Close();
			
		}
	}

}

void CParaDlg::OnBnClickedCheckStopTestType2()
{
	// TODO: 在此添加控件通知处理程序代码
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	CString s;
	s = _T("FirstCoolSencondHeat_CloseAC_Before_Heat"); 

	this->UpdateData();

	if(this->m_bFirstCoolSencondHeat_CloseAC_Before_Heat)
	{
		AfxGetApp()->WriteProfileInt(_T("SETTING"),s,1);  
		maindlg->m_SwitchDlg.m_bFirstCoolSencondHeat_CloseAC_Before_Heat = 1;   
	}
	else
	{
		AfxGetApp()->WriteProfileInt(_T("SETTING"),s,0); 
		maindlg->m_SwitchDlg.m_bFirstCoolSencondHeat_CloseAC_Before_Heat = 0; 
	}
}
