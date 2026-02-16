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
#include "GroupDlg.h"
#include "common.h"

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
	,m_PrinterType(0)
	, m_OpenDoorHeatDegree(0)
	, m_OpenDoorCoolDegree(0)
	, m_MainLevelAvrSpanMinus(0)
	, m_MainLevelAvrSpanPlus(0)
	, m_CloseMainLevelAvrSpanMinus(0)
	, m_CloseMainLevelAvrSpanPlus(0)
	, m_SoundType(0)
	,m_SoundStrong(0)
	, m_OverHeatGap(0)
	, m_AutoPrintType(0)
	, m_bCoolDownAutoOpendoor(FALSE)
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
	DDV_MinMaxDouble(pDX, m_HighLevelSpanMinus, 0., 199.9);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_SPAN_PLUS, m_HighLevelSpanPlus);
	DDV_MinMaxDouble(pDX, m_HighLevelSpanPlus, 0., 199.9);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_LOW, m_LowLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_SPAN_MINUS, m_LowLevelSpanMinus);
	DDV_MinMaxDouble(pDX, m_LowLevelSpanMinus, 0., 199.9);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_SPAN_PLUS, m_LowLevelSpanPlus);
	DDV_MinMaxDouble(pDX, m_LowLevelSpanPlus, 0., 199.9);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL_SPAN_MINUS, m_MainLevelSpanMinus);
	DDV_MinMaxDouble(pDX, m_MainLevelSpanMinus, 0., 199.9);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL_SPAN_PLUS, m_MainLevelSpanPlus);
	DDV_MinMaxDouble(pDX, m_MainLevelSpanPlus, 0., 199.9);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL, m_MainLevel);

	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PID_MINUTES, m_iNormalPidMinutes);
	DDX_CBIndex(pDX, IDC_COMBO_HEAT_BOX_TYPE, m_HeatBoxType);
	DDX_CBIndex(pDX, IDC_COMBO_PRINTER_TYPE, m_PrinterType);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_TEMP_TYPE, m_ComboBoxDisplayTempType);
	DDX_Control(pDX, IDC_EDIT_CONTINUE_TEST_COUNT, m_EditContinueTestCount);
	DDX_Control(pDX, IDC_COMBO_END_TEST_DOOR_STATUS, m_Combo_EndTestDoorStatus);
	DDX_Control(pDX, IDC_CHECK_STOP_TEST_TYPE, m_ControlStopTestType);
	DDX_Control(pDX, IDC_COMBO_485_PROTOCOL, m_Combo485Protocol);
	DDX_Control(pDX, IDC_COMBO_COOL_DEGREE, m_ComboCoolDegree);
	DDX_Control(pDX, IDC_COMBO_OVER_HEAT, m_ComboOverHeat);
	DDX_Control(pDX, IDC_COMBO_COOL_SPEED, m_ComboBoxCoolSpeed);
	DDX_Text(pDX, IDC_EDIT_HEAT_DEGREE, m_OpenDoorHeatDegree);
	DDX_Text(pDX, IDC_EDIT_COOL_DEGREE, m_OpenDoorCoolDegree);
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_AVR_SPAN_MINUS, m_MainLevelAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_AVR_SPAN_PLUS, m_MainLevelAvrSpanPlus);
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_MINUS, m_CloseMainLevelAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_PLUS, m_CloseMainLevelAvrSpanPlus);
	DDX_CBIndex(pDX, IDC_COMBO_SOUND_TYPE, m_SoundType);
	DDX_CBIndex(pDX, IDC_COMBO_SOUND_STRONG, m_SoundStrong);
	DDX_CBIndex(pDX, IDC_COMBO_OVER_HEAT, m_OverHeatGap);
	DDX_CBIndex(pDX, IDC_COMBO_PRINT_TYPE2, m_AutoPrintType);
	DDX_Check(pDX, IDC_CHECK_COOL_DOWN_AUTO_OPEN_DOOR, m_bCoolDownAutoOpendoor);
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
	ON_CBN_SELCHANGE(IDC_COMBO_OPEN_DOOR_RESET_LEVEL,&CParaDlg::OnCbnSelchangeComboOpenDoorReset)
	ON_CBN_SELCHANGE(IDC_COMBO_END_TEST_DOOR_STATUS, &CParaDlg::OnCbnSelchangeComboEndTestDoorStatus)
	ON_EN_CHANGE(IDC_EDIT_CONTINUE_TEST_COUNT, &CParaDlg::OnEnChangeEditContinueTestCount)
	ON_BN_CLICKED(IDC_CHECK_RELAY, &CParaDlg::OnBnClickedCheckRelay)
	ON_BN_CLICKED(IDC_CHECK_STOP_TEST_TYPE, &CParaDlg::OnBnClickedCheckStopTestType)
	ON_EN_SETFOCUS(IDC_EDIT_PID_MINUTES, &CParaDlg::OnEnSetfocusEditPidMinutes)
	ON_EN_SETFOCUS(IDC_EDIT_MAIN_LEVEL, &CParaDlg::OnEnSetfocusEditMainLevel2)
	ON_CBN_SETFOCUS(IDC_COMBO_HEAT_SPEED, &CParaDlg::OnCbnSetfocusComboHeatSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_CONTINUE_TEST_COUNT, &CParaDlg::OnEnSetfocusEditContinueTestCount)
	ON_CBN_SELCHANGE(IDC_COMBO_485_PROTOCOL, &CParaDlg::OnCbnSelchangeCombo485Protocol)
	ON_CBN_SELCHANGE(IDC_COMBO_COOL_DEGREE, &CParaDlg::OnCbnSelchangeComboCoolDegree)
	ON_CBN_SELCHANGE(IDC_COMBO_COOL_SPEED, &CParaDlg::OnCbnSelchangeComboCoolSpeed)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_ADJUST, &CParaDlg::OnBnClickedButtonTempAdjust)
	ON_BN_CLICKED(IDC_BUTTON_GROUP, &CParaDlg::OnBnClickedButtonGroup)
	ON_CBN_SELCHANGE(IDC_COMBO_SOUND_TYPE, &CParaDlg::OnCbnSelchangeComboSoundType)
	ON_CBN_SELENDOK(IDC_COMBO_OVER_HEAT, &CParaDlg::OnCbnSelendokComboOverHeat)
	ON_CBN_SELCHANGE(IDC_COMBO_SOUND_STRONG, &CParaDlg::OnCbnSelchangeComboSoundStrong)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINTER_TYPE, &CParaDlg::OnCbnSelchangeComboPrinterType)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINT_TYPE2, &CParaDlg::OnCbnSelchangeComboAutoPrintType)
	ON_BN_CLICKED(IDC_CHECK_COOL_DOWN_AUTO_OPEN_DOOR, &CParaDlg::OnBnClickedCheckCoolDownAutoOpenDoor)
	ON_CBN_SETFOCUS(IDC_COMBO_HEAT_BOX_TYPE, &CParaDlg::OnCbnSetfocusComboHeatBoxType)
	ON_CBN_SELENDOK(IDC_COMBO_HEAT_BOX_TYPE, &CParaDlg::OnCbnSelendokComboHeatBoxType)
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
void CParaDlg::UpdateSetPara(void)
{
	CString s;

	m_iNormalPidMinutes =::G_NormalConfigPara[m_BoxNr].PidTime;   
	
	
	m_TestModeList.SetCurSel(G_NormalConfigPara[m_BoxNr].TestMode);  
	
	if(G_NormalConfigPara[m_BoxNr].TestMode == TEST_MODE_TOP_TEMP_PID)
	{
		m_OpenDoorHeatDegree =	::G_NormalConfigPara[m_BoxNr].TopPidMinutes;
	}
	else
	{
		m_OpenDoorHeatDegree =	::G_NormalConfigPara[m_BoxNr].OpenDoorReset_HeatDegree;
	}
	
	m_OpenDoorCoolDegree =    G_NormalConfigPara[m_BoxNr].OpenDoorReset_CoolDegree;

	m_CurMaxHeatBox.SetCurSel(G_CurMaxHeatBox - 1);	
	

	
	m_Combo_EndTestDoorStatus.SetCurSel(::G_NormalConfigPara[m_BoxNr].StopTestAutoCloseDoor );  

	
	
	if(::G_NormalConfigPara[m_BoxNr].StopTestType != 0)   
	{
		m_ControlStopTestType.SetCheck(true); 
	}
	else
	{
		m_ControlStopTestType.SetCheck(false);
	}
	
	 
	CString str;
	str.Format(_T("IS CMD ASCII %d"),m_BoxNr); 

	G_iCmdAscii[m_BoxNr] = AfxGetApp()->GetProfileIntW(_T("SETTING"),str,0);
	if(G_iCmdAscii[m_BoxNr] > 2)
	{
		G_iCmdAscii[m_BoxNr] = 0;
	}

	m_Combo485Protocol.SetCurSel(G_iCmdAscii[m_BoxNr]);

	m_ComboCoolDegree.ResetContent(); 
	m_ComboCoolDegree.AddString(_T("无压缩机"));
	for(int i = 1; i <= 80; i++)
	{
		str.Format(_T("低于%d度"),i);
		m_ComboCoolDegree.AddString(str);
	}

	m_ComboCoolDegree.SetCurSel(G_NormalConfigPara[m_BoxNr].AirControlEquipment); 

	m_ComboOverHeat.ResetContent(); 
	for(int i = 0; i <= 10; i++)
	{
		str.Format(_T("%d度"),i);
		this->m_ComboOverHeat.AddString(str);
	}

	m_ComboOverHeat.SetCurSel(G_NormalConfigPara[m_BoxNr].OverHeatGap); 
	

#ifdef _CHENG_HONG_	
	CWnd *w;

	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_OPEN);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_CLOSE);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_OPEN);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_CLOSE);
	w->ShowWindow(SW_HIDE);
	
	w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_LOW);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_SPAN_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_SPAN_PLUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL2);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_PLUS);
	w->ShowWindow(SW_HIDE);

	w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL_LOW);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL_SPAN_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL_SPAN_PLUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL3);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS3);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS3);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_PLUS);
	w->ShowWindow(SW_HIDE);


		
	
#endif

	m_HeatBoxType = ::G_NormalConfigPara[m_BoxNr].BoxType;
	m_PrinterType = ::G_NormalConfigPara[m_BoxNr].PrinterType;
		
	s.Format(_T("%d"),G_NormalConfigPara[m_BoxNr].RepeatTestCounts);
	m_EditContinueTestCount.SetWindowTextW(s);

	this->UpdateData(false); 
}

BOOL CParaDlg::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg); 
}

BOOL CParaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//G_MainDlg->ReadSwitchConfigPara();
	
	// TODO: Add extra initialization here

	CComboBox *wnd = (CComboBox *)this->GetDlgItem(IDC_COMBO_HEAT_BOX_TYPE); 
	wnd->AddString(_T("普通箱"));
	wnd->AddString(_T("PTC箱 测PTC"));
	wnd->AddString(_T("PTC箱 不测PTC"));
	wnd->AddString(_T("仅用于夹具焊接"));
	wnd->AddString(_T("双片-40"));
	wnd->AddString(_T("夹具测试"));
	wnd->AddString(_T("128点 老显示"));
	wnd->AddString(_T("PTC箱 逆向测PTC"));
	wnd->AddString(_T("NTC箱 测试电阻"));
	wnd->AddString(_T("双片-16"));
	wnd->AddString(_T("双片-24"));
	wnd->AddString(_T("桌面小烘箱"));
	wnd->AddString(_T("KW-2T03 双路保护器"));
	
	wnd->AddString(_T("九阳温度保险丝"));

	wnd->AddString(_T("8PT00 8SSR"));
	wnd->AddString(_T("DELTA PLC 8PT00 8SSR"));

	wnd->AddString(_T("油浴(油槽)"));
	wnd->AddString(_T("普通箱,带废品箱"));
	wnd->AddString(_T("普通箱,初校"));
	wnd->AddString(_T("TEMP2_10IN_12OUT"));
	wnd->AddString(_T("森萨塔 32 PT100"));
	wnd->AddString(_T("192点 控制箱"));
	wnd->AddString(_T("300点 控制箱"));
	wnd->AddString(_T("TX300W 恒流源"));


	m_bSetFocusOnEditMainLevel2 = true;

	wnd = (CComboBox *)this->GetDlgItem(IDC_COMBO_TEST_MODE); 

	wnd->AddString(_T("升降温，测动作复位温度"));
	wnd->AddString(_T("升降温，反复开关门测复位闪动"));
	wnd->AddString(_T("升降温，再次升温测复位不同步"));
	wnd->AddString(_T("升温，反复开关门测开门复位，全速降温"));
	wnd->AddString(_T("恒温计时间"));
	wnd->AddString(_T("升降温，反复开关门测动复闪"));
	wnd->AddString(_T("升降温，最高温恒温"));


	wnd = (CComboBox *)this->GetDlgItem(IDC_COMBO_PRINTER_TYPE); 
	wnd->AddString(_T("LPM260"));
	wnd->AddString(_T("PM-24"));
	
	wnd->SetCurSel(::G_NormalConfigPara[0].PrinterType);

	wnd = (CComboBox *)this->GetDlgItem(IDC_COMBO_PRINT_TYPE2); 
	wnd->AddString(_T("不打印"));
	wnd->AddString(_T("打印，含温度分布图"));
	wnd->AddString(_T("打印，除温度分布图"));

	this->m_AutoPrintType = ::G_NormalConfigPara[m_BoxNr].AutoPrintType;
	
	wnd->SetCurSel(::G_NormalConfigPara[this->m_BoxNr].AutoPrintType);
	m_bCoolDownAutoOpendoor = G_NormalConfigPara[m_BoxNr].CoolDownAutoOpenDoor;

	CFile ParaFile;
	CFileFind ff;
	CString fName;
	CString sDir,s;
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

	m_SoundStrong = ::G_NormalConfigPara[m_BoxNr].SoundLevel_Heat ;
	
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

}

void CParaDlg::OnChangeEditMainLevelSpanMinus() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

}

void CParaDlg::OnChangeEditMainLevelSpanPlus() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

}

LRESULT CParaDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CParaDlg::UpdateModifyPara() 
{
	
	ReadNormalConfigPara(); 

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

	
}
void CParaDlg::OnButtonCreateModifyPara() 
{
	// TODO: Add your control notification handler code here

	


	CCreateModifyParaDlg dlg;

	dlg.m_BoxNr = m_BoxNr;
	dlg.DoModal(); 

	

	for(int i = 0; i < G_CurMaxHeatBox; i++)
	{
		G_MainDlg->m_ParaDlg[i].UpdateModifyPara();
	}

}

void CParaDlg::OnSelchangeComboHeatSpeed() 
{
	// TODO: Add your control notification handler code here
	CInputDlg dlg;

		
	dlg.m_Title = _T("请输入密码");
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_Input == _T("1357"))
		{
		}
		else
		{
			m_ComboBoxHeatSpeed.SetCurSel(G_NormalConfigPara[m_BoxNr].HeatSpeed);
			return;
		}
	}

	int curSel = this->m_ComboBoxHeatSpeed.GetCurSel(); 
	if(curSel < 0)
	{
		curSel = 19;
	}
	
	CString s;
	
	
	
	
#ifdef _OIL_
	curSel+=2;
#endif

	
	::G_NormalConfigPara[m_BoxNr].HeatSpeed = curSel; 

	::SaveNormalConfigPara();  
	
	ReadNormalConfigPara();
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

	G_NormalConfigPara[m_BoxNr].TestMode = curSel;
	::SaveNormalConfigPara(); 
	ReadNormalConfigPara();
}

void CParaDlg::OnSelchangeComboTestMode() 
{
	// TODO: Add your control notification handler code here

	CWnd *w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_LOW); 
	w->SetFocus(); 

	CInputDlg dlg_psw;

		
	dlg_psw.m_Title = _T("请输入密码");
	if(dlg_psw.DoModal() == IDOK)
	{
		if(dlg_psw.m_Input == _T("1357"))
		{

		}
		else
		{
			m_TestModeList.SetCurSel(G_NormalConfigPara[m_BoxNr].TestMode);
			return;
		}
	}


	CWnd *wnd;
	CString s;
	int curSel = this->m_TestModeList.GetCurSel();  

	::G_NormalConfigPara[m_BoxNr].TestMode = curSel;

	if(curSel == TEST_MODE_OPEN_DOOR_RESET || curSel == TEST_MODE_OPEN_DOOR_RESET_AND_CLOSE_DOOR_RESET)
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
	else if(curSel == TEST_MODE_TOP_TEMP_PID)
	{
		wnd = this->GetDlgItem(IDC_STATIC_HEAT);
		wnd->ShowWindow(SW_SHOW);
		wnd->SetWindowTextW(_T("恒温时间"));
		wnd = this->GetDlgItem(IDC_STATIC_COOL);
		wnd->ShowWindow(SW_HIDE);
		wnd = this->GetDlgItem(IDC_EDIT_HEAT_DEGREE);
		wnd->ShowWindow(SW_SHOW);
		wnd = this->GetDlgItem(IDC_EDIT_COOL_DEGREE);
		wnd->ShowWindow(SW_HIDE);
	}
	else if(curSel == TEST_MODE_CLOSE_DOOR_REPEAT)
	{
		wnd = this->GetDlgItem(IDC_STATIC_HEAT);
		wnd->ShowWindow(SW_SHOW);
		wnd->SetWindowTextW(_T("恒温时间"));
		wnd = this->GetDlgItem(IDC_STATIC_COOL);
		wnd->ShowWindow(SW_HIDE);
		wnd = this->GetDlgItem(IDC_EDIT_HEAT_DEGREE);
		wnd->ShowWindow(SW_SHOW);
		wnd = this->GetDlgItem(IDC_EDIT_COOL_DEGREE);
		wnd->ShowWindow(SW_HIDE);
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
	::SaveNormalConfigPara(); 
	ReadNormalConfigPara();
}

void CParaDlg::OnChangeEditHeatDegree() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	this->UpdateData(); 
	if(G_NormalConfigPara[m_BoxNr].TestMode == TEST_MODE_TOP_TEMP_PID)
	{
		::G_NormalConfigPara[m_BoxNr].TopPidMinutes =   m_OpenDoorHeatDegree;
	}
	else
	{
		::G_NormalConfigPara[m_BoxNr].OpenDoorReset_HeatDegree =   m_OpenDoorHeatDegree;
	}
	
	::SaveNormalConfigPara(); 
//	G_MainDlg->ReadSwitchConfigPara();
}

void CParaDlg::OnChangeEditCoolDegree() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	this->UpdateData(); 
	::G_NormalConfigPara[m_BoxNr].OpenDoorReset_CoolDegree = m_OpenDoorCoolDegree;	
	::SaveNormalConfigPara();  	
//	G_MainDlg->ReadSwitchConfigPara();

 
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





void CParaDlg::OnCbnSelchangeComboParaName()
{
	// TODO: 在此添加控件通知处理程序代码

	
	
	CString str;

		
	CString name;
	
	str.Format(_T("SWITCH_CONFIG_PARA_BOX_NR_%d"),m_BoxNr);
	

	m_MainLevelList.GetLBText(m_MainLevelList.GetCurSel(),name);
	//AfxGetApp()->WriteProfileString(_T("SETTING"),
	 //		str,name);
	if(name.GetLength() > sizeof(G_NormalConfigPara[m_BoxNr].ParaName) / sizeof(TCHAR))
	{
		name = name.Left( sizeof(G_NormalConfigPara[m_BoxNr].ParaName) / sizeof(TCHAR));
	}
	TCHAR *buf = name.GetBuffer() ;
	memset(G_NormalConfigPara[m_BoxNr].ParaName,0,sizeof(G_NormalConfigPara[m_BoxNr].ParaName));
	memcpy(G_NormalConfigPara[m_BoxNr].ParaName,buf,name.GetLength() * sizeof(TCHAR));

	::SaveNormalConfigPara(); 

	ReadNormalConfigPara();

	


}

void CParaDlg::OnCbnSelchangeComboHeatBoxType()
{
	// TODO: 在此添加控件通知处理程序代码
	

	CWnd *w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_LOW); 
	w->SetFocus(); 

	CInputDlg dlg_psw;

		
	dlg_psw.m_Title = _T("请输入密码");
	if(dlg_psw.DoModal() == IDOK)
	{
		if(dlg_psw.m_Input == _T("1357"))
		{

		}
		else
		{
			::ReadNormalConfigPara();
			return;
		}
	}

	this->UpdateData();
	::G_NormalConfigPara[m_BoxNr].BoxType =   (BOX_TYPE)m_HeatBoxType;
	::SaveNormalConfigPara(); 
}

void CParaDlg::OnCbnSelchangeComboDisplayTempType()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	s.Format(_T("DISPLAY_TEMP_TYPE_%d"),m_BoxNr); 
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,(int)m_ComboBoxDisplayTempType.GetCurSel());	

}

void CParaDlg::OnCbnSelchangeComboOpenDoorReset()
{

	//::G_NormalConfigPara[m_BoxNr].Open_Close_Delta = m_OpenDoorResetList.GetCurSel();
	//::SaveNormalConfigPara();
}

void CParaDlg::ShowCloseLevelEdit(void)
{
	CWnd * w;


	if( m_SwitchConfigPara.para.CloseTempType == CLOSE_TEMP_TYPE_RANGE) 
	{
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL2);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL3);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS3);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS3);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_HIDE);

		w =GetDlgItem(IDC_STATIC_DEVIATION);
		w->ShowWindow(SW_HIDE);
		w = GetDlgItem(IDC_STATIC_DEVIATION_LOW);
		w->ShowWindow(SW_HIDE);
		w = GetDlgItem(IDC_STATIC_DEVIATION_HIGH);
		w->ShowWindow(SW_HIDE);


	}
	else if(m_SwitchConfigPara.para.CloseTempType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
	{
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL2);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL3);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS3);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS3);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_SHOW);

		w = GetDlgItem(IDC_STATIC_DEVIATION);
		w->ShowWindow(SW_SHOW);
		w = GetDlgItem(IDC_STATIC_DEVIATION_LOW);
		w->ShowWindow(SW_SHOW);
		w = GetDlgItem(IDC_STATIC_DEVIATION_HIGH);
		w->ShowWindow(SW_SHOW);

	}
	else
	{
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE); 


		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL2);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL3);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS3);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS3);
		w->ShowWindow(SW_HIDE); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_DEVIATION_PLUS);
		w->ShowWindow(SW_SHOW);

		w = GetDlgItem(IDC_STATIC_DEVIATION);
		w->ShowWindow(SW_SHOW);
		w = GetDlgItem(IDC_STATIC_DEVIATION_LOW);
		w->ShowWindow(SW_SHOW);
		w = GetDlgItem(IDC_STATIC_DEVIATION_HIGH);
		w->ShowWindow(SW_SHOW);

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
 
	if(i >= 0)
	{
		G_NormalConfigPara[m_BoxNr].StopTestAutoCloseDoor = i;
		::SaveNormalConfigPara();
		::ReadNormalConfigPara(); 
	}

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

	G_NormalConfigPara[m_BoxNr].RepeatTestCounts = i;

	::SaveNormalConfigPara(); 

}

void CParaDlg::OnBnClickedCheckRelay()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CParaDlg::OnBnClickedCheckStopTestType()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_ControlStopTestType.GetCheck())
	{
		G_NormalConfigPara[m_BoxNr].StopTestType = 1;   
	}
	else
	{
		G_NormalConfigPara[m_BoxNr].StopTestType = 0; 
	}

	::SaveNormalConfigPara(); 

}

void CParaDlg::OnEnSetfocusEditPidMinutes()
{
	// TODO: 在此添加控件通知处理程序代码

	CWnd *w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_LOW); 
	w->SetFocus(); 

	CInputDlg dlg_psw;

		
	dlg_psw.m_Title = _T("请输入密码");
	if(dlg_psw.DoModal() == IDOK)
	{
		if(dlg_psw.m_Input == _T("1357"))
		{
		}
		else
		{
			return;
		}
	}

	
	

	
	CInputDlg dlg;
	dlg.m_Title = _T("设置恒温时间(分钟)");
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(IDC_EDIT_PID_MINUTES) ;
		w->SetWindowTextW(dlg.m_Input);

		
		UpdateData(true);
		::G_NormalConfigPara[m_BoxNr].PidTime = m_iNormalPidMinutes;
		::SaveNormalConfigPara(); 
		ReadNormalConfigPara();
	}
	
	
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
		G_NormalConfigPara[m_BoxNr].RepeatTestCounts = i;   

		::SaveNormalConfigPara(); 

		
	
	}
	

	w->SetFocus(); 

}

void CParaDlg::OnCbnSelchangeCombo485Protocol()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str;
	str.Format(_T("IS CMD ASCII %d"),m_BoxNr); 
	G_iCmdAscii[m_BoxNr] = m_Combo485Protocol.GetCurSel();
	

	AfxGetApp()->WriteProfileInt(_T("SETTING"),str,G_iCmdAscii[m_BoxNr]);

}

void CParaDlg::OnCbnSelchangeComboCoolDegree()
{
	// TODO: 在此添加控件通知处理程序代码

	CWnd *w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_LOW); 
	w->SetFocus(); 

	CInputDlg dlg_psw;

		
	dlg_psw.m_Title = _T("请输入密码");
	if(dlg_psw.DoModal() == IDOK)
	{
		if(dlg_psw.m_Input == _T("1357"))
		{

		}
		else
		{
			m_ComboCoolDegree.SetCurSel(G_NormalConfigPara[m_BoxNr].AirControlEquipment);
			return;
		}
	}


	G_NormalConfigPara[m_BoxNr].AirControlEquipment = m_ComboCoolDegree.GetCurSel(); 
	::SaveNormalConfigPara(); 
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


	::G_NormalConfigPara[m_BoxNr].CoolSpeed = curSel;
	::SaveNormalConfigPara(); 
	ReadNormalConfigPara();
}

void CParaDlg::OnBnClickedButtonTempAdjust()
{
	// TODO: 在此添加控件通知处理程序代码

	CTempAdjustDlg dlg;


	for(int i = 0; i < 192; i++)
	{
		dlg.m_AdjustTemp[0][i] = ::G_AdjustTemp[m_BoxNr][0][i]; 
		dlg.m_AdjustTemp[1][i] = ::G_AdjustTemp[m_BoxNr][1][i]; 
	}

	dlg.m_BoxNr = m_BoxNr;

	if(dlg.DoModal() == IDOK)
	{
		for(int i = 0; i < 192; i++)
		{
			G_AdjustTemp[m_BoxNr][0][i] = dlg.m_AdjustTemp[0][i]; 
			G_AdjustTemp[m_BoxNr][1][i] = dlg.m_AdjustTemp[1][i];
		}

		::SaveNormalConfigPara(); 
	}

}

void CParaDlg::OnBnClickedButtonGroup()
{
	// TODO: 在此添加控件通知处理程序代码

	CGroupDlg dlg;

	dlg.m_BoxNr = m_BoxNr;
	dlg.DoModal();
}

void CParaDlg::OnCbnSelchangeComboSoundType()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData(); 

	::G_NormalConfigPara[m_BoxNr].SoundOutputType =  m_SoundType;

	::SaveNormalConfigPara(); 

}

void CParaDlg::OnCbnSelendokComboOverHeat()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(); 

	::G_NormalConfigPara[m_BoxNr].OverHeatGap =  m_OverHeatGap;

	::SaveNormalConfigPara(); 
}

void CParaDlg::OnCbnSelchangeComboSoundStrong()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(); 

	::G_NormalConfigPara[m_BoxNr].SoundLevel_Heat =  m_SoundStrong;

	::SaveNormalConfigPara(); 

}

void CParaDlg::OnCbnSelchangeComboPrinterType()
{
	// TODO: 在此添加控件通知处理程序代码

	

	CWnd *w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_LOW); 
	w->SetFocus(); 

	CInputDlg dlg_psw;

		
	dlg_psw.m_Title = _T("请输入密码");
	if(dlg_psw.DoModal() == IDOK)
	{
		if(dlg_psw.m_Input == _T("1357"))
		{

		}
		else
		{
			::ReadNormalConfigPara();
			return;
		}
	}

	this->UpdateData();
	::G_NormalConfigPara[m_BoxNr].PrinterType =   (BOX_TYPE)m_PrinterType;
	::SaveNormalConfigPara(); 
}

void CParaDlg::OnCbnSelchangeComboAutoPrintType()
{
	// TODO: 在此添加控件通知处理程序代码

	

	CWnd *w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_LOW); 
	w->SetFocus(); 

	CInputDlg dlg_psw;

		
	dlg_psw.m_Title = _T("请输入密码");
	if(dlg_psw.DoModal() == IDOK)
	{
		if(dlg_psw.m_Input == _T("1357"))
		{

		}
		else
		{
			::ReadNormalConfigPara();
			return;
		}
	}
	this->UpdateData();
	::G_NormalConfigPara[m_BoxNr].AutoPrintType =   (BOX_TYPE)m_AutoPrintType;
	::SaveNormalConfigPara(); 
}

void CParaDlg::OnBnClickedCheckCoolDownAutoOpenDoor()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(); 
	CFile para_file;

	::G_NormalConfigPara[m_BoxNr].CoolDownAutoOpenDoor =   m_bCoolDownAutoOpendoor;
	::SaveNormalConfigPara(); 
}

void CParaDlg::OnCbnSetfocusComboHeatBoxType()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("1"));
}

void CParaDlg::OnCbnSelendokComboHeatBoxType()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("2"));
}
