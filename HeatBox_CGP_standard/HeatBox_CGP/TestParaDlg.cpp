// TestParaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "TestParaDlg.h"
#include "NumInputDlg.H"
#include "HeatBox_CGPDlg.h"
#include "shlwapi.h"

// CTestParaDlg 对话框

IMPLEMENT_DYNAMIC(CTestParaDlg, CDialog)

CTestParaDlg::CTestParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestParaDlg::IDD, pParent)
	, m_Pro1Cycle(0)
	, m_Pro2Cycle(0)
	, m_Pro3Cycle(0)
	, m_Pro4Cycle(0)
	, m_Pro1TestPoint(0)
	, m_Pro1OpenTemp(0)
	, m_Pro1CloseTemp(0)
	, m_Pro2OpenTemp(0)
	, m_Pro2CloseTemp(0)
	, m_Pro3OpenTemp(0)
	, m_Pro3CloseTemp(0)
	, m_Pro4OpenTemp(0)
	, m_Pro4CloseTemp(0)
	, m_Pro1OpenCloseGapLow(0)
	, m_Pro2OpenCloseGapLow(0)
	, m_Pro3OpenCloseGapLow(0)
	, m_Pro4OpenCloseGapLow(0)
	, m_Pro1OpenCloseGapHigh(0)
	, m_Pro2OpenCloseGapHigh(0)
	, m_Pro3OpenCloseGapHigh(0)
	, m_Pro4OpenCloseGapHigh(0)
	, m_Pro1OpenTempMinus(0)
	, m_Pro2OpenTempMinus(0)
	, m_Pro3OpenTempMinus(0)
	, m_Pro4OpenTempMinus(0)
	, m_Pro1OpenTempPlus(0)
	, m_Pro2OpenTempPlus(0)
	, m_Pro3OpenTempPlus(0)
	, m_Pro4OpenTempPlus(0)

	, m_FlashGap(0)
	, m_ProductorNo(_T(""))
	, m_Productor(_T(""))
	, m_AdjustTemp(0)
	, m_SwitchNr(0)
	, m_HeatPercent(0)
	, m_CoolPercent(0)
{
	m_Pro1CloseTempMinus = 0.0f;
	m_Pro2CloseTempMinus = 0.0f;
	m_Pro3CloseTempMinus = 0.0f;
	m_Pro4CloseTempMinus = 0.0f;
	m_Pro1CloseTempPlus = 0.0f;
	m_Pro2CloseTempPlus = 0.0f;
	m_Pro3CloseTempPlus = 0.0f;
	m_Pro4CloseTempPlus = 0.0f;


}

CTestParaDlg::~CTestParaDlg()
{
}

void CTestParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PRO1_CYCLE, m_Pro1Cycle);
	DDV_MinMaxUInt(pDX, m_Pro1Cycle, 1, 9);
	DDX_Text(pDX, IDC_EDIT_PRO2_CYCLE, m_Pro2Cycle);
	DDV_MinMaxUInt(pDX, m_Pro2Cycle, 1, 9);
	DDX_Text(pDX, IDC_EDIT_PRO3_CYCLE, m_Pro3Cycle);
	DDV_MinMaxUInt(pDX, m_Pro3Cycle, 1, 9);
	DDX_Text(pDX, IDC_EDIT_PRO4_CYCLE, m_Pro4Cycle);
	DDV_MinMaxUInt(pDX, m_Pro4Cycle, 1, 9);
	DDX_Text(pDX, IDC_EDIT_PRO1_OPEN, m_Pro1OpenTemp);
	DDX_Text(pDX, IDC_EDIT_PRO1_CLOSE, m_Pro1CloseTemp);
	DDX_Text(pDX, IDC_EDIT_PRO2_OPEN, m_Pro2OpenTemp);
	DDX_Text(pDX, IDC_EDIT_PRO2_CLOSE, m_Pro2CloseTemp);
	DDX_Text(pDX, IDC_EDIT_PRO3_OPEN, m_Pro3OpenTemp);
	DDX_Text(pDX, IDC_EDIT_PRO3_CLOSE, m_Pro3CloseTemp);
	DDX_Text(pDX, IDC_EDIT_PRO4_OPEN, m_Pro4OpenTemp);
	DDX_Text(pDX, IDC_EDIT_PRO4_CLOSE, m_Pro4CloseTemp);
	DDX_Text(pDX, IDC_EDIT_PRO1_GAP1, m_Pro1OpenCloseGapLow);
	DDX_Text(pDX, IDC_EDIT_PRO2_GAP1, m_Pro2OpenCloseGapLow);
	DDX_Text(pDX, IDC_EDIT_PRO3_GAP1, m_Pro3OpenCloseGapLow);
	DDX_Text(pDX, IDC_EDIT_PRO4_GAP1, m_Pro4OpenCloseGapLow);
	DDX_Text(pDX, IDC_EDIT_PRO1_GAP2, m_Pro1OpenCloseGapHigh);
	DDX_Text(pDX, IDC_EDIT_PRO2_GAP2, m_Pro2OpenCloseGapHigh);
	DDX_Text(pDX, IDC_EDIT_PRO3_GAP2, m_Pro3OpenCloseGapHigh);
	DDX_Text(pDX, IDC_EDIT_PRO4_GAP2, m_Pro4OpenCloseGapHigh);
	DDX_Text(pDX, IDC_EDIT_PRO1_OPEN_GAP_LOW, m_Pro1OpenTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO2_OPEN_GAP_LOW, m_Pro2OpenTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO3_OPEN_GAP_LOW, m_Pro3OpenTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO4_OPEN_GAP_LOW, m_Pro4OpenTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO1_OPEN_GAP_HIGH, m_Pro1OpenTempPlus);
	DDX_Text(pDX, IDC_EDIT_PRO2_OPEN_GAP_HIGH, m_Pro2OpenTempPlus);
	DDX_Text(pDX, IDC_EDIT_PRO3_OPEN_GAP_HIGH, m_Pro3OpenTempPlus);
	DDX_Text(pDX, IDC_EDIT_PRO4_OPEN_GAP_HIGH, m_Pro4OpenTempPlus);
	DDX_Control(pDX, IDC_COMBO2, m_ControlParaName);
	DDX_Text(pDX, IDC_EDIT_FLASH, m_FlashGap);
	DDX_Text(pDX, IDC_EDIT1, m_ProductorNo);
	DDX_Text(pDX, IDC_EDIT2, m_Productor);
	DDX_Text(pDX, IDC_EDIT3, m_SwitchNr);
	DDX_Text(pDX, IDC_EDIT_PRO1_CLOSE_GAP_LOW, m_Pro1CloseTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO1_CLOSE_GAP_HIGH, m_Pro1CloseTempPlus);
	DDX_Text(pDX, IDC_EDIT_PRO2_CLOSE_GAP_LOW, m_Pro2CloseTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO2_CLOSE_GAP_HIGH, m_Pro2CloseTempPlus);
	DDX_Text(pDX, IDC_EDIT_PRO3_CLOSE_GAP_LOW, m_Pro3CloseTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO3_CLOSE_GAP_HIGH, m_Pro3CloseTempPlus);
	DDX_Text(pDX, IDC_EDIT_PRO4_CLOSE_GAP_LOW, m_Pro4CloseTempMinus);
	DDX_Text(pDX, IDC_EDIT_PRO4_CLOSE_GAP_HIGH, m_Pro4CloseTempPlus);
	DDX_Control(pDX, IDC_COMBO_HEAT_SPEED, m_ComboHeatSpeed);
	DDX_Control(pDX, IDC_COMBO_COOL_SPEED, m_ComboCoolSpeed);
	DDX_Text(pDX, IDC_EDIT_HEAT_PERCENT, m_HeatPercent);
	DDX_Text(pDX, IDC_EDIT_COOL_PERCENT, m_CoolPercent);
	DDV_MinMaxUInt(pDX, m_HeatPercent, 0, 100);
	DDV_MinMaxUInt(pDX, m_CoolPercent, 0, 100);
}


BEGIN_MESSAGE_MAP(CTestParaDlg, CDialog)
//	ON_NOTIFY(NM_THEMECHANGED, IDC_SPIN_PRO1_CYCLE, &CTestParaDlg::OnNMThemeChangedSpinPro1Cycle)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRO1_CYCLE, &CTestParaDlg::OnDeltaposSpinPro1Cycle)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRO2_CYCLE, &CTestParaDlg::OnDeltaposSpinPro2Cycle)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRO3_CYCLE, &CTestParaDlg::OnDeltaposSpinPro3Cycle)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRO4_CYCLE, &CTestParaDlg::OnDeltaposSpinPro4Cycle)
ON_BN_CLICKED(IDC_RADIO_PRO1_HOT, &CTestParaDlg::OnBnClickedRadioPro1Hot)
ON_BN_CLICKED(IDC_RADIO_PRO1_MID_1, &CTestParaDlg::OnBnClickedRadioPro1Mid1)
ON_BN_CLICKED(IDC_RADIO_PRO1_MID_2, &CTestParaDlg::OnBnClickedRadioPro1Mid2)
ON_BN_CLICKED(IDC_RADIO_PRO1_LOW, &CTestParaDlg::OnBnClickedRadioPro1Low)
ON_BN_CLICKED(IDC_RADIO_PRO1_PUPD, &CTestParaDlg::OnBnClickedRadioPro1Pupd)
ON_BN_CLICKED(IDC_RADIO_PRO2_NO, &CTestParaDlg::OnBnClickedRadioPro2No)
ON_BN_CLICKED(IDC_RADIO_PRO2_HOT, &CTestParaDlg::OnBnClickedRadioPro2Hot)
ON_BN_CLICKED(IDC_RADIO_PRO2_MID_1, &CTestParaDlg::OnBnClickedRadioPro2Mid1)
ON_BN_CLICKED(IDC_RADIO_PRO2_MID_2, &CTestParaDlg::OnBnClickedRadioPro2Mid2)
ON_BN_CLICKED(IDC_RADIO_PRO2_LOW, &CTestParaDlg::OnBnClickedRadioPro2Low)
ON_BN_CLICKED(IDC_RADIO_PRO2_PUPD, &CTestParaDlg::OnBnClickedRadioPro2Pupd)
ON_BN_CLICKED(IDC_RADIO_PRO3_NO, &CTestParaDlg::OnBnClickedRadioPro3No)
ON_BN_CLICKED(IDC_RADIO_PRO3_HOT, &CTestParaDlg::OnBnClickedRadioPro3Hot)
ON_BN_CLICKED(IDC_RADIO_PRO3_MID_1, &CTestParaDlg::OnBnClickedRadioPro3Mid1)
ON_BN_CLICKED(IDC_RADIO_PRO3_MID_2, &CTestParaDlg::OnBnClickedRadioPro3Mid2)
ON_BN_CLICKED(IDC_RADIO_PRO3_LOW, &CTestParaDlg::OnBnClickedRadioPro3Low)
ON_BN_CLICKED(IDC_RADIO_PRO3_PUPD, &CTestParaDlg::OnBnClickedRadioPro3Pupd)
ON_BN_CLICKED(IDC_RADIO_PRO4_NO, &CTestParaDlg::OnBnClickedRadioPro4No)
ON_BN_CLICKED(IDC_RADIO_PRO4_HOT, &CTestParaDlg::OnBnClickedRadioPro4Hot)
ON_BN_CLICKED(IDC_RADIO_PRO4_MID_1, &CTestParaDlg::OnBnClickedRadioPro4Mid1)
ON_BN_CLICKED(IDC_RADIO_PRO4_MID_2, &CTestParaDlg::OnBnClickedRadioPro4Mid2)
ON_BN_CLICKED(IDC_RADIO_PRO4_LOW, &CTestParaDlg::OnBnClickedRadioPro4Low)
ON_BN_CLICKED(IDC_RADIO_PRO4_PUPD, &CTestParaDlg::OnBnClickedRadioPro4Pupd)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN, &CTestParaDlg::OnBnClickedButtonPro1Open)
ON_BN_CLICKED(IDC_RADIO_SPEED1, &CTestParaDlg::OnBnClickedRadioSpeed1)
ON_BN_CLICKED(IDC_RADIO_SPEED2, &CTestParaDlg::OnBnClickedRadioSpeed2)
ON_BN_CLICKED(IDC_RADIO_SPEED3, &CTestParaDlg::OnBnClickedRadioSpeed3)
ON_BN_CLICKED(IDC_RADIO_SPEED4, &CTestParaDlg::OnBnClickedRadioSpeed4)
ON_BN_CLICKED(IDC_RADIO_KEEP_WARM_1, &CTestParaDlg::OnBnClickedRadioKeepWarm1)
ON_BN_CLICKED(IDC_RADIO_KEEPWARM2, &CTestParaDlg::OnBnClickedRadioKeepwarm2)
ON_BN_CLICKED(IDC_RADIO_SWITCH_TYPE_C_LH, &CTestParaDlg::OnBnClickedRadioSwitchTypeCLh)
ON_BN_CLICKED(IDC_RADIO_SWITCH_TYPE_C_LS, &CTestParaDlg::OnBnClickedRadioSwitchTypeCLs)
ON_BN_CLICKED(IDC_BUTTON_PRO1_CLOSE, &CTestParaDlg::OnBnClickedButtonPro1Close)
ON_BN_CLICKED(IDC_BUTTON_PRO2_OPEN, &CTestParaDlg::OnBnClickedButtonPro2Open)
ON_BN_CLICKED(IDC_BUTTON_PRO2_CLOSE, &CTestParaDlg::OnBnClickedButtonPro2Close)
ON_BN_CLICKED(IDC_BUTTON_PRO3_OPEN, &CTestParaDlg::OnBnClickedButtonPro3Open)
ON_BN_CLICKED(IDC_BUTTON_PRO3_CLOSE, &CTestParaDlg::OnBnClickedButtonPro3Close)
ON_BN_CLICKED(IDC_BUTTON_PRO4_OPEN, &CTestParaDlg::OnBnClickedButtonPro4Open)
ON_BN_CLICKED(IDC_BUTTON_PRO4_CLOSE, &CTestParaDlg::OnBnClickedButtonPro4Close)
ON_BN_CLICKED(IDC_BUTTON_PRO1_GAP1, &CTestParaDlg::OnBnClickedButtonPro1Gap1)
ON_BN_CLICKED(IDC_BUTTON_PRO2_GAP1, &CTestParaDlg::OnBnClickedButtonPro2Gap1)
ON_BN_CLICKED(IDC_BUTTON_PRO3_GAP1, &CTestParaDlg::OnBnClickedButtonPro3Gap1)
ON_BN_CLICKED(IDC_BUTTON_PRO4_GAP1, &CTestParaDlg::OnBnClickedButtonPro4Gap1)
ON_BN_CLICKED(IDC_BUTTON_PRO1_GAP2, &CTestParaDlg::OnBnClickedButtonPro1Gap2)
ON_BN_CLICKED(IDC_BUTTON_PRO2_GAP2, &CTestParaDlg::OnBnClickedButtonPro2Gap2)
ON_BN_CLICKED(IDC_BUTTON_PRO3_GAP2, &CTestParaDlg::OnBnClickedButtonPro3Gap2)
ON_BN_CLICKED(IDC_BUTTON_PRO4_GAP2, &CTestParaDlg::OnBnClickedButtonPro4Gap2)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN2, &CTestParaDlg::OnBnClickedButtonPro1Open2)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN4, &CTestParaDlg::OnBnClickedButtonPro1Open4)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN6, &CTestParaDlg::OnBnClickedButtonPro1Open6)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN8, &CTestParaDlg::OnBnClickedButtonPro1Open8)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN3, &CTestParaDlg::OnBnClickedButtonPro1Open3)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN5, &CTestParaDlg::OnBnClickedButtonPro1Open5)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN7, &CTestParaDlg::OnBnClickedButtonPro1Open7)
ON_BN_CLICKED(IDC_BUTTON_PRO1_OPEN9, &CTestParaDlg::OnBnClickedButtonPro1Open9)
ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTestParaDlg::OnBnClickedButtonSave)
ON_BN_CLICKED(IDC_BUTTON_DELETE, &CTestParaDlg::OnBnClickedButtonDelete)
ON_CBN_SELCHANGE(IDC_COMBO2, &CTestParaDlg::OnCbnSelchangeCombo2)
ON_EN_SETFOCUS(IDC_EDIT4, &CTestParaDlg::OnEnSetfocusEdit4)
ON_BN_CLICKED(IDC_BUTTON_SWITCH_NR, &CTestParaDlg::OnBnClickedButtonSwitchNr)
ON_BN_CLICKED(IDC_BUTTON_PRODUCTOR, &CTestParaDlg::OnBnClickedButtonProductor)
ON_BN_CLICKED(IDC_BUTTON_PARA_NAME, &CTestParaDlg::OnBnClickedButtonParaName)
ON_BN_CLICKED(IDC_BUTTON_PRODUCT_NO, &CTestParaDlg::OnBnClickedButtonProductNo)
ON_BN_CLICKED(IDC_CHECK_WARM_AUTO_READY, &CTestParaDlg::OnBnClickedCheckWarmAutoReady)
ON_BN_CLICKED(IDC_CHECK_AUTO_ENTER, &CTestParaDlg::OnBnClickedCheckAutoEnter)
ON_BN_CLICKED(10001, &CTestParaDlg::OnBnClickedButtonHeatCoolTypePro1)
ON_BN_CLICKED(10002, &CTestParaDlg::OnBnClickedButtonHeatCoolTypePro2)
ON_BN_CLICKED(10003, &CTestParaDlg::OnBnClickedButtonHeatCoolTypePro3)
ON_BN_CLICKED(10004, &CTestParaDlg::OnBnClickedButtonHeatCoolTypePro4)
ON_BN_CLICKED(IDC_RADIO_SWITCH_TYPE_3, &CTestParaDlg::OnBnClickedRadioSwitchType3)
ON_BN_CLICKED(IDC_RADIO_SWITCH_TYPE_4, &CTestParaDlg::OnBnClickedRadioSwitchType4)
ON_BN_CLICKED(IDC_RADIO_SWITCH_TYPE_1, &CTestParaDlg::OnBnClickedRadioSwitchType1)
ON_BN_CLICKED(IDC_RADIO_SWITCH_TYPE_2, &CTestParaDlg::OnBnClickedRadioSwitchType2)
ON_BN_CLICKED(IDOK, &CTestParaDlg::OnBnClickedOk)
ON_CBN_SELCHANGE(IDC_COMBO_HEAT_SPEED, &CTestParaDlg::OnCbnSelchangeComboHeatSpeed)
ON_CBN_SELCHANGE(IDC_COMBO_COOL_SPEED, &CTestParaDlg::OnCbnSelchangeComboCoolSpeed)
ON_EN_CHANGE(IDC_EDIT_HEAT_PERCENT, &CTestParaDlg::OnEnChangeEditHeatPercent)
ON_EN_CHANGE(IDC_EDIT_COOL_PERCENT, &CTestParaDlg::OnEnChangeEditCoolPercent)
END_MESSAGE_MAP()


// CTestParaDlg 消息处理程序
BOOL CTestParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_AdjustTemp = G_NormalConfigPara.AdjustTemp; 

	m_SwitchNr = ::G_NormalConfigPara.SwitchCount;//  AfxGetApp()->GetProfileInt("SET","SWITCH_NR",64); 

	SelectLanguage();

	UpdateParaNameList();

	UpdateAllPara();

	CWnd * wnd ;
	CRect  rect;
	wnd = this->GetDlgItem(IDC_BUTTON_PRO1_GAP1);
	wnd->GetWindowRect(&rect); 
	m_bmpButton[0] = new CBitmapButton();
	// Create the bitmap button (must include the BS_OWNERDRAW style).
	m_bmpButton[0]->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, 
		CRect(rect.left,rect.top ,rect.left+50,rect.bottom + 60), this, 10001);
	if(m_Pro1HeatCoolType == 0) 
	{
		m_bmpButton[0]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	}
	else
	{
		m_bmpButton[0]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}
	
	wnd = this->GetDlgItem(IDC_BUTTON_PRO2_GAP1);
	wnd->GetWindowRect(&rect); 
	m_bmpButton[1] = new CBitmapButton();
	// Create the bitmap button (must include the BS_OWNERDRAW style).
	m_bmpButton[1]->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, 
		CRect(rect.left,rect.top ,rect.left+50,rect.bottom + 60), this, 10002);
	if(m_Pro2HeatCoolType == 0) 
	{
		m_bmpButton[1]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	}
	else
	{
		m_bmpButton[1]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}

	wnd = this->GetDlgItem(IDC_BUTTON_PRO3_GAP1);
	wnd->GetWindowRect(&rect); 
	m_bmpButton[2] = new CBitmapButton();
	// Create the bitmap button (must include the BS_OWNERDRAW style).
	m_bmpButton[2]->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, 
		CRect(rect.left,rect.top ,rect.left+50,rect.bottom + 60), this, 10003);
	if(m_Pro3HeatCoolType == 0) 
	{
		m_bmpButton[2]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	}
	else
	{
		m_bmpButton[2]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}

	wnd = this->GetDlgItem(IDC_BUTTON_PRO4_GAP1);
	wnd->GetWindowRect(&rect); 
	m_bmpButton[3] = new CBitmapButton();
	// Create the bitmap button (must include the BS_OWNERDRAW style).
	m_bmpButton[3]->Create(NULL, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, 
		CRect(rect.left,rect.top ,rect.left+50,rect.bottom + 60), this, 10004);
	if(m_Pro4HeatCoolType == 0) 
	{
		m_bmpButton[3]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	}
	else
	{
		m_bmpButton[3]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}

	this->UpdateData(false);

	return true;
}

void CTestParaDlg::OnDeltaposSpinPro1Cycle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_Pro1Cycle += pNMUpDown->iDelta;
	if(m_Pro1Cycle < 1)
		m_Pro1Cycle = 1;
	if(m_Pro1Cycle > 9)
		m_Pro1Cycle = 9;
	this->UpdateData(false); 
	*pResult = 0;
}

void CTestParaDlg::OnDeltaposSpinPro2Cycle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_Pro2Cycle += pNMUpDown->iDelta;
	if(m_Pro2Cycle < 1)
		m_Pro2Cycle = 1;
	if(m_Pro2Cycle > 9)
		m_Pro2Cycle = 9;
	this->UpdateData(false); 
	*pResult = 0;
}

void CTestParaDlg::OnDeltaposSpinPro3Cycle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_Pro3Cycle += pNMUpDown->iDelta;
	if(m_Pro3Cycle < 1)
		m_Pro3Cycle = 1;
	if(m_Pro3Cycle > 9)
		m_Pro3Cycle = 9;
	this->UpdateData(false); 
	*pResult = 0;
}

void CTestParaDlg::OnDeltaposSpinPro4Cycle(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_Pro4Cycle += pNMUpDown->iDelta;
	if(m_Pro4Cycle < 1)
		m_Pro4Cycle = 1;
	if(m_Pro4Cycle > 9)
		m_Pro4Cycle = 9;
	this->UpdateData(false); 
	*pResult = 0;
}

void CTestParaDlg::OnBnClickedRadioPro1Hot()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro1TestPoint = 0;
	
}

void CTestParaDlg::OnBnClickedRadioPro1Mid1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro1TestPoint = 1;
}

void CTestParaDlg::OnBnClickedRadioPro1Mid2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro1TestPoint = 2;
}

void CTestParaDlg::OnBnClickedRadioPro1Low()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro1TestPoint = 3;
}

void CTestParaDlg::OnBnClickedRadioPro1Pupd()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro1TestPoint = 4;
}

void CTestParaDlg::OnBnClickedRadioPro2No()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro2TestPoint = 0;
}

void CTestParaDlg::OnBnClickedRadioPro2Hot()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro2TestPoint = 1;
}

void CTestParaDlg::OnBnClickedRadioPro2Mid1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro2TestPoint = 2;
}

void CTestParaDlg::OnBnClickedRadioPro2Mid2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro2TestPoint = 3;
}

void CTestParaDlg::OnBnClickedRadioPro2Low()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro2TestPoint = 4;
}

void CTestParaDlg::OnBnClickedRadioPro2Pupd()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro2TestPoint = 5;
}

void CTestParaDlg::OnBnClickedRadioPro3No()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro3TestPoint = 0;
}

void CTestParaDlg::OnBnClickedRadioPro3Hot()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro3TestPoint = 1;
}

void CTestParaDlg::OnBnClickedRadioPro3Mid1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro3TestPoint = 2;
}

void CTestParaDlg::OnBnClickedRadioPro3Mid2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro3TestPoint = 3;
}

void CTestParaDlg::OnBnClickedRadioPro3Low()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro3TestPoint = 4;
}

void CTestParaDlg::OnBnClickedRadioPro3Pupd()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro3TestPoint = 5;
}

void CTestParaDlg::OnBnClickedRadioPro4No()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro4TestPoint = 0;
}

void CTestParaDlg::OnBnClickedRadioPro4Hot()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro4TestPoint = 1;
}

void CTestParaDlg::OnBnClickedRadioPro4Mid1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro4TestPoint = 2;
}

void CTestParaDlg::OnBnClickedRadioPro4Mid2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro4TestPoint = 3;
}

void CTestParaDlg::OnBnClickedRadioPro4Low()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro4TestPoint = 4;
}

void CTestParaDlg::OnBnClickedRadioPro4Pupd()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Pro4TestPoint = 5;
}

void CTestParaDlg::OnBnClickedButtonPro1Open()
{
	// TODO: 在此添加控件通知处理程序代码

	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = _T("过程1");
		dlg.m_Msg[0] =_T("断开温度"); 
		dlg.m_Msg[1] =_T("负偏差"); 
		dlg.m_Msg[2] =_T("正偏差"); 
		break;
	case 1:
		dlg.m_Title = _T("Pro 1");
		dlg.m_Msg[0] =_T("Open Temp"); 
		dlg.m_Msg[1] =_T("Gap -"); 
		dlg.m_Msg[2] =_T("Gap +"); 
		break;
	}
	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro1OpenTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro1OpenTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro1OpenTempPlus = _wtof(buf);

		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedRadioSpeed1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TestSpeed = 0;
}

void CTestParaDlg::OnBnClickedRadioSpeed2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TestSpeed = 1;
}

void CTestParaDlg::OnBnClickedRadioSpeed3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TestSpeed = 2;
}

void CTestParaDlg::OnBnClickedRadioSpeed4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_TestSpeed = 3;
}

void CTestParaDlg::OnBnClickedRadioKeepWarm1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EnterTestMode = 0;
}

void CTestParaDlg::OnBnClickedRadioKeepwarm2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EnterTestMode = 1;
}

void CTestParaDlg::OnBnClickedRadioSwitchTypeCLh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsDoubleSwitch = 0;
}

void CTestParaDlg::OnBnClickedRadioSwitchTypeCLs()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsDoubleSwitch = 1;
}

void CTestParaDlg::OnBnClickedButtonPro1Close()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = "过程1";
		dlg.m_Msg[0] ="接通温度"; 
		dlg.m_Msg[1] ="负偏差"; 
		dlg.m_Msg[2] ="正偏差"; 
		break;
	case 1:
		dlg.m_Title = "Pro 1";
		dlg.m_Msg[0] ="Close Temp"; 
		dlg.m_Msg[1] ="Gap -"; 
		dlg.m_Msg[2] ="Gap +"; 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro1CloseTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro1CloseTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro1CloseTempPlus = _wtof(buf);
		
		if((m_Pro1CloseTempMinus != 0) && (m_Pro1CloseTempPlus != 0))
		{
			this->m_Pro1OpenCloseGapLow = 0;
			this->m_Pro1OpenCloseGapHigh = 0;
		}

		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro2Open()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = "过程2";
		dlg.m_Msg[0] ="断开温度"; 
		dlg.m_Msg[1] ="负偏差"; 
		dlg.m_Msg[2] ="正偏差"; 
		break;
	case 1:
		dlg.m_Title = "Pro 2";
		dlg.m_Msg[0] ="Open Temp"; 
		dlg.m_Msg[1] ="Gap -"; 
		dlg.m_Msg[2] ="Gap +"; 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro2OpenTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro2OpenTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro2OpenTempPlus = _wtof(buf);

		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro2Close()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = "过程2";
		dlg.m_Msg[0] ="接通温度"; 
		dlg.m_Msg[1] ="负偏差"; 
		dlg.m_Msg[2] ="正偏差"; 
		break;
	case 1:
		dlg.m_Title = "Pro 2";
		dlg.m_Msg[0] ="Close Temp"; 
		dlg.m_Msg[1] ="Gap -"; 
		dlg.m_Msg[2] ="Gap +"; 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro2CloseTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro2CloseTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro2CloseTempPlus = _wtof(buf);

		if((m_Pro2CloseTempMinus != 0) && (m_Pro2CloseTempPlus != 0))
		{
			this->m_Pro2OpenCloseGapLow = 0;
			this->m_Pro2OpenCloseGapHigh = 0;
		}

		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro3Open()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = _T("过程3");
		dlg.m_Msg[0] =_T("断开温度"); 
		dlg.m_Msg[1] =_T("负偏差"); 
		dlg.m_Msg[2] =_T("正偏差"); 
		break;
	case 1:
		dlg.m_Title = _T("Pro 3");
		dlg.m_Msg[0] =_T("Open Temp"); 
		dlg.m_Msg[1] =_T("Gap -"); 
		dlg.m_Msg[2] =_T("Gap +"); 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro3OpenTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro3OpenTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro3OpenTempPlus = _wtof(buf);

		
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro3Close()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = "过程3";
		dlg.m_Msg[0] ="接通温度"; 
		dlg.m_Msg[1] ="负偏差"; 
		dlg.m_Msg[2] ="正偏差"; 
		break;
	case 1:
		dlg.m_Title = "Pro 3";
		dlg.m_Msg[0] ="Close Temp"; 
		dlg.m_Msg[1] ="Gap -"; 
		dlg.m_Msg[2] ="Gap +"; 
		break;
	}
	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro3CloseTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro3CloseTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro3CloseTempPlus = _wtof(buf);

		if((m_Pro3CloseTempMinus != 0) && (m_Pro3CloseTempPlus != 0))
		{
			this->m_Pro3OpenCloseGapLow = 0;
			this->m_Pro3OpenCloseGapHigh = 0;
		}
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro4Open()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = "过程4";
		dlg.m_Msg[0] ="断开温度"; 
		dlg.m_Msg[1] ="负偏差"; 
		dlg.m_Msg[2] ="正偏差"; 
		break;
	case 1:
		dlg.m_Title = "Pro 4";
		dlg.m_Msg[0] ="Open Temp"; 
		dlg.m_Msg[1] ="Gap -"; 
		dlg.m_Msg[2] ="Gap +"; 
		break;
	}
	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro4OpenTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro4OpenTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro4OpenTempPlus = _wtof(buf);

		this->m_Pro4OpenCloseGapLow = 0;
		this->m_Pro4OpenCloseGapHigh = 0;

		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro4Close()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	switch(G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = "过程4";
		dlg.m_Msg[0] ="接通温度"; 
		dlg.m_Msg[1] ="负偏差"; 
		dlg.m_Msg[2] ="正偏差"; 
		break;
	case 1:
		dlg.m_Title = "Pro 4";
		dlg.m_Msg[0] ="Close Temp"; 
		dlg.m_Msg[1] ="Gap -"; 
		dlg.m_Msg[2] ="Gap +"; 
		break;
	} 

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		m_Pro4CloseTemp = _wtof(buf);

		buf = dlg.m_Input[1].GetBuffer(dlg.m_Input[1].GetLength()); 
		m_Pro4CloseTempMinus = _wtof(buf);

		buf = dlg.m_Input[2].GetBuffer(dlg.m_Input[2].GetLength()); 
		m_Pro4CloseTempPlus = _wtof(buf);
		if((m_Pro4CloseTempMinus != 0) && (m_Pro4CloseTempPlus != 0))
		{
			this->m_Pro4OpenCloseGapLow = 0;
			this->m_Pro4OpenCloseGapHigh = 0;
		}
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Gap1()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "过程1的开停差";
	dlg.m_Msg[0] =  "最小值";
	dlg.m_Msg[1] =  "最大值";
	dlg.m_Msg[2] =  "";

	if(G_NormalConfigPara.Language == 1)
	{
		dlg.m_Title = "Pro 1: Close Gap";
		dlg.m_Msg[0] =  "Min Val";
		dlg.m_Msg[1] =  "Max Val";
		dlg.m_Msg[2] =  "";
	}

	if(dlg.DoModal() == IDOK)
	{
		m_Pro1OpenCloseGapLow = _wtof(dlg.m_Input[0]); 
		m_Pro1OpenCloseGapHigh = _wtof(dlg.m_Input[1]); 

		if((m_Pro1OpenCloseGapLow != 0) &&  (m_Pro1OpenCloseGapHigh != 0))
		{
				m_Pro1CloseTemp = 0;
				m_Pro1CloseTempMinus = 0;
				m_Pro1CloseTempPlus  = 0;
		}
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro2Gap1()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "过程2的开停差";
	dlg.m_Msg[0] =  "最小值";
	dlg.m_Msg[1] =  "最大值";
	dlg.m_Msg[2] =  "";

	if(G_NormalConfigPara.Language == 1)
	{
		dlg.m_Title = "Pro 2: Close Gap";
		dlg.m_Msg[0] =  "Min Val";
		dlg.m_Msg[1] =  "Max Val";
		dlg.m_Msg[2] =  "";
	}

	if(dlg.DoModal() == IDOK)
	{
		m_Pro2OpenCloseGapLow = _wtof(dlg.m_Input[0]); 
		m_Pro2OpenCloseGapHigh = _wtof(dlg.m_Input[1]); 

		if((m_Pro2OpenCloseGapLow != 0) &&  (m_Pro2OpenCloseGapHigh != 0))
		{
			m_Pro2CloseTemp = 0;
			m_Pro2CloseTempMinus = 0;
			m_Pro2CloseTempPlus  = 0;
		}
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro3Gap1()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "过程3的开停差";
	dlg.m_Msg[0] =  "最小值";
	dlg.m_Msg[1] =  "最大值";
	dlg.m_Msg[2] =  "";

	if(G_NormalConfigPara.Language == 1)
	{
		dlg.m_Title = "Pro 3: Close Gap";
		dlg.m_Msg[0] =  "Min Val";
		dlg.m_Msg[1] =  "Max Val";
		dlg.m_Msg[2] =  "";
	}

	if(dlg.DoModal() == IDOK)
	{
		m_Pro3OpenCloseGapLow = _wtof(dlg.m_Input[0]); 
		m_Pro3OpenCloseGapHigh = _wtof(dlg.m_Input[1]); 

		if((m_Pro3OpenCloseGapLow != 0) &&  (m_Pro3OpenCloseGapHigh != 0))
		{
			m_Pro3CloseTemp = 0;
			m_Pro3CloseTempMinus = 0;
			m_Pro3CloseTempPlus  = 0;
		}
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro4Gap1()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "过程4的开停差";
	dlg.m_Msg[0] =  "最小值";
	dlg.m_Msg[1] =  "最大值";
	dlg.m_Msg[2] =  "";

	if(G_NormalConfigPara.Language == 1)
	{
		dlg.m_Title = "Pro 4: Close Gap";
		dlg.m_Msg[0] =  "Min Val";
		dlg.m_Msg[1] =  "Max Val";
		dlg.m_Msg[2] =  "";
	}

	if(dlg.DoModal() == IDOK)
	{
		m_Pro4OpenCloseGapLow = _wtof(dlg.m_Input[0]); 
		m_Pro4OpenCloseGapHigh = _wtof(dlg.m_Input[1]); 

		if((m_Pro4OpenCloseGapLow != 0) &&  (m_Pro4OpenCloseGapHigh != 0))
		{
			m_Pro4CloseTemp = 0;
			m_Pro4CloseTempMinus = 0;
			m_Pro4CloseTempPlus  = 0;
		}
		this->UpdateData(false); 
	}

}

void CTestParaDlg::OnBnClickedButtonPro1Gap2()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程1的开停差的最大值";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro1OpenCloseGapHigh = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro2Gap2()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程2的开停差的最大值";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro2OpenCloseGapHigh = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro3Gap2()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程3的开停差的最大值";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro3OpenCloseGapHigh = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro4Gap2()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程4的开停差的最大值";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro4OpenCloseGapHigh = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open2()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程1的断开温度负偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro1OpenTempMinus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open4()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程2的断开温度负偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro2OpenTempMinus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open6()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程3的断开温度的负偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro3OpenTempMinus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open8()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程4的断开温度负偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro4OpenTempMinus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open3()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程1的断开温度正偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro1OpenTempPlus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open5()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程2的断开温度正偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro2OpenTempPlus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open7()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程3的断开温度正偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro3OpenTempPlus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

void CTestParaDlg::OnBnClickedButtonPro1Open9()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	dlg.m_Title = "输入过程4的断开温度正偏差";
	if(dlg.DoModal() == IDOK)
	{
		this->m_Pro4OpenTempPlus = _wtof(dlg.m_Input[0]); 
		this->UpdateData(false); 
	}
}

int CTestParaDlg::SetRadio(void)
{
		CButton *btn;
	//
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_HOT); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_MID_1); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_MID_2); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_LOW); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_PUPD); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_NO); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_HOT); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_MID_1); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_MID_2); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_LOW); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_PUPD); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_NO); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_HOT); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_MID_1); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_MID_2); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_LOW); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_PUPD); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_NO); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_HOT); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_MID_1); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_MID_2); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_LOW); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_PUPD); 
		btn->SetCheck(BST_UNCHECKED); 
		
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_KEEP_WARM_1); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_KEEPWARM2); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_1); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_2); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_3); 
		btn->SetCheck(BST_UNCHECKED); 
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_4); 
		btn->SetCheck(BST_UNCHECKED); 
	
	
	//


	switch(m_Pro1TestPoint)
	{
	case 0:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_HOT); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 1:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_MID_1); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 2:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_MID_2); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 3:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_LOW); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 4:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO1_PUPD); 
		btn->SetCheck(BST_CHECKED); 
		break;
	}
	
	switch(m_Pro2TestPoint)
	{
	case 0:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_NO); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 1:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_HOT); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 2:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_MID_1); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 3:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_MID_2); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 4:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_LOW); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 5:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO2_PUPD); 
		btn->SetCheck(BST_CHECKED); 
		break;
	}
	
	switch(m_Pro3TestPoint)
	{
	case 0:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_NO); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 1:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_HOT); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 2:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_MID_1); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 3:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_MID_2); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 4:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_LOW); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 5:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO3_PUPD); 
		btn->SetCheck(BST_CHECKED); 
		break;
	}

	switch(m_Pro4TestPoint)
	{
	case 0:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_NO); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 1:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_HOT); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 2:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_MID_1); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 3:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_MID_2); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 4:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_LOW); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 5:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_PRO4_PUPD); 
		btn->SetCheck(BST_CHECKED); 
		break;
	}
	
	switch(this->m_EnterTestMode)
	{
	case 0:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_KEEP_WARM_1); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 1:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_KEEPWARM2); 
		btn->SetCheck(BST_CHECKED); 
		break;
	}
	switch(this->m_IsDoubleSwitch)
	{
	case 0:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_1); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 1:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_2); 
		btn->SetCheck(BST_CHECKED); 
		break;
	}

	switch(this->m_IsNormalOpen)
	{
	case 0:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_3); 
		btn->SetCheck(BST_CHECKED); 
		break;
	case 1:
		btn = (CButton *)this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_4); 
		btn->SetCheck(BST_CHECKED); 
		break;
	}
	this->UpdateData(false); 
	return 0;
}

extern CString theAppDirectory;

void CTestParaDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码

	if(!UpdateData())
	{
		return;
	}


	CFile ParaFile;
	
	TEST_PARA	m_TestPara;

	CString sDir;

	
	sDir = theAppDirectory +_T("para\\");

	
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}
	memset(G_NormalConfigPara.ParaName,0,sizeof(G_NormalConfigPara.ParaName));

	this->m_ControlParaName.GetWindowText(G_NormalConfigPara.ParaName,sizeof(G_NormalConfigPara.ParaName));  
	_tcscat(G_NormalConfigPara.ParaName,_T(".para"));	

	if(ParaFile.Open(sDir + G_NormalConfigPara.ParaName,CFile::modeReadWrite | CFile::modeCreate))
	{

		m_TestPara	= CopyControlData2TestPara();	
		ParaFile.Write(&m_TestPara,sizeof(m_TestPara));
		ParaFile.Flush();
		ParaFile.Close();
		this->UpdateAllPara(); 
	}



	::G_MainDlg->SaveConfig();

	
	UpdateParaNameList();
	
	UpdateAllPara();



	CString s = G_NormalConfigPara.ParaName;

	if(s.GetLength() > 5)
	{
		s = s.Left(s.GetLength() - 5);
	}

	for(int i = 0; i < m_ControlParaName.GetCount(); i++)
	{
		CString text;
		m_ControlParaName.GetLBText(i,text);
		if(text.Compare(s) == 0)
		{
			m_ControlParaName.SetCurSel( i);
			break;
		}
	
	}

	::G_MainDlg->SetTimer(1,(m_TestPara.FlashGap  + 1) *100 ,NULL);
}

void CTestParaDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码

	CFile ParaFile;
	
	TEST_PARA	m_TestPara;

	CString sDir;

	
	sDir = theAppDirectory +_T("para\\");

	
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}
	CString s;
	m_ControlParaName.GetWindowText(s);  
	s.Trim();
	if(s.GetLength() == 0)
	{
		AfxMessageBox(_T("没有选择测试参数"));
		return;
	}

	memset(G_NormalConfigPara.ParaName,0,sizeof(G_NormalConfigPara.ParaName));

	memcpy(G_NormalConfigPara.ParaName,s.GetBuffer(),sizeof(G_NormalConfigPara.ParaName));
	

	s+=_T(".para");	
	s = sDir + _T("\\") + s;
	CFile::Remove(s);

	UpdateParaNameList();
	
	UpdateAllPara();

}

TEST_PARA CTestParaDlg::CopyControlData2TestPara(void)
{
	TCHAR *buf;
	TEST_PARA m_TestPara;

	CString s;
	

	this->m_ControlParaName.GetWindowText(s);   
	s.Trim();
	buf = s.GetBuffer(s.GetLength());
	//memcpy(m_TestPara.ParaName,buf,sizeof(m_TestPara.ParaName)) ;
	
	m_Productor.Trim(); 
	buf = m_Productor.GetBuffer(m_Productor.GetLength());
	memcpy(m_TestPara.Productor,buf,sizeof(m_TestPara.Productor)) ;

	m_ProductorNo.Trim(); 
	buf = m_ProductorNo.GetBuffer(m_ProductorNo.GetLength() );
	memcpy(m_TestPara.ProductorNo,buf,sizeof(m_TestPara.ProductorNo)) ;
	

	m_TestPara.TestPoint[0]			= m_Pro1TestPoint;
	m_TestPara.Cycle[0]				= m_Pro1Cycle;
	m_TestPara.HeatCoolType[0]		= m_Pro1HeatCoolType;
	m_TestPara.OpenTemp[0]			= m_Pro1OpenTemp;
	m_TestPara.OpenTempMinus[0]		= m_Pro1OpenTempMinus;
	m_TestPara.OpenTempPlus[0]		= m_Pro1OpenTempPlus;
	m_TestPara.CloseTemp[0]			= m_Pro1CloseTemp;
	m_TestPara.CloseTempMinus[0]	= m_Pro1CloseTempMinus;
	m_TestPara.CloseTempPlus[0]		= m_Pro1CloseTempPlus;
	m_TestPara.OpenCloseGapLow[0]	= m_Pro1OpenCloseGapLow;
	m_TestPara.OpenCloseGapHigh[0]	= m_Pro1OpenCloseGapHigh;

	m_TestPara.TestPoint[1]			= m_Pro2TestPoint;
	m_TestPara.Cycle[1]				= m_Pro2Cycle;
	m_TestPara.HeatCoolType[1]		= m_Pro2HeatCoolType;
	m_TestPara.OpenTemp[1]			= m_Pro2OpenTemp;
	m_TestPara.OpenTempMinus[1]		= m_Pro2OpenTempMinus;
	m_TestPara.OpenTempPlus[1]		= m_Pro2OpenTempPlus;
	m_TestPara.CloseTemp[1]			= m_Pro2CloseTemp;
	m_TestPara.CloseTempMinus[1]	= m_Pro2CloseTempMinus;
	m_TestPara.CloseTempPlus[1]		= m_Pro2CloseTempPlus;
	m_TestPara.OpenCloseGapLow[1]	= m_Pro2OpenCloseGapLow;
	m_TestPara.OpenCloseGapHigh[1]	= m_Pro2OpenCloseGapHigh;


	m_TestPara.TestPoint[2]			= m_Pro3TestPoint;
	m_TestPara.Cycle[2]				= m_Pro3Cycle;
	m_TestPara.HeatCoolType[2]		= m_Pro3HeatCoolType;
	m_TestPara.OpenTemp[2]			= m_Pro3OpenTemp;
	m_TestPara.OpenTempMinus[2]		= m_Pro3OpenTempMinus;
	m_TestPara.OpenTempPlus[2]		= m_Pro3OpenTempPlus;
	m_TestPara.CloseTemp[2]			= m_Pro3CloseTemp;
	m_TestPara.CloseTempMinus[2]	= m_Pro3CloseTempMinus;
	m_TestPara.CloseTempPlus[2]		= m_Pro3CloseTempPlus;
	m_TestPara.OpenCloseGapLow[2]	= m_Pro3OpenCloseGapLow;
	m_TestPara.OpenCloseGapHigh[2]	= m_Pro3OpenCloseGapHigh;

	m_TestPara.TestPoint[3]			= m_Pro4TestPoint;
	m_TestPara.Cycle[3]				= m_Pro4Cycle;
	m_TestPara.HeatCoolType[3]		= m_Pro4HeatCoolType;
	m_TestPara.OpenTemp[3]			= m_Pro4OpenTemp;
	m_TestPara.OpenTempMinus[3]		= m_Pro4OpenTempMinus;
	m_TestPara.OpenTempPlus[3]		= m_Pro4OpenTempPlus;
	m_TestPara.CloseTemp[3]			= m_Pro4CloseTemp;
	m_TestPara.CloseTempMinus[3]	= m_Pro4CloseTempMinus;
	m_TestPara.CloseTempPlus[3]		= m_Pro4CloseTempPlus;
	m_TestPara.OpenCloseGapLow[3]	= m_Pro4OpenCloseGapLow;
	m_TestPara.OpenCloseGapHigh[3]	= m_Pro4OpenCloseGapHigh;

	m_TestPara.TestSpeed			= m_TestSpeed;
	m_TestPara.KeepWarmMode			= m_EnterTestMode;
	m_TestPara.IsDoubleSwitch		= m_IsDoubleSwitch;
	m_TestPara.FlashGap				= m_FlashGap;

	m_TestPara.IsNormalOpen			= m_IsNormalOpen;

	m_TestPara.HeatSpeed			= m_ComboHeatSpeed.GetCurSel() ;
	m_TestPara.CoolSpeed			= m_ComboCoolSpeed.GetCurSel() ;
	m_TestPara.HeatPercent			= m_HeatPercent;
	m_TestPara.CoolPercent			= m_CoolPercent;


	return m_TestPara;
}

void CTestParaDlg::OnCbnSelchangeCombo2()
{
	memset(G_NormalConfigPara.ParaName,0,sizeof(G_NormalConfigPara.ParaName));
	
	m_ControlParaName.GetLBText(m_ControlParaName.GetCurSel(),G_NormalConfigPara.ParaName); 
	

	_tcscat(G_NormalConfigPara.ParaName,_T(".para"));	

	this->UpdateAllPara(); 
}

void CTestParaDlg::UpdateAllPara(void)
{
	

	CFile ParaFile;
	
	TEST_PARA	m_TestPara;

	CString sDir;

	
	sDir = theAppDirectory +_T("para\\");

	
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	
		
		if(ParaFile.Open(sDir + G_NormalConfigPara.ParaName,CFile::modeReadWrite))
		{

			ParaFile.SeekToBegin();
			memset(&m_TestPara,0,sizeof(m_TestPara));
			UINT i = ParaFile.Read(&m_TestPara,sizeof(m_TestPara));
			if(i <= sizeof(m_TestPara))
			{
				
					CString paraName = G_NormalConfigPara.ParaName;
					paraName = paraName.Left(paraName.GetLength() - 5);

					m_ControlParaName.SetWindowTextW(paraName) ;

					m_Productor				=	m_TestPara.Productor;
					m_ProductorNo			=	m_TestPara.ProductorNo;

					m_Pro1TestPoint			=	m_TestPara.TestPoint[0];
					m_Pro1Cycle				=	m_TestPara.Cycle[0];
					if(m_Pro1Cycle <= 0)
					{
						m_Pro1Cycle = 1;
					}
					m_Pro1HeatCoolType		=	m_TestPara.HeatCoolType[0];
					m_Pro1OpenTemp			=	m_TestPara.OpenTemp[0];
					m_Pro1OpenTempMinus		=	m_TestPara.OpenTempMinus[0];
					m_Pro1OpenTempPlus		=	m_TestPara.OpenTempPlus[0];
					m_Pro1CloseTemp			=	m_TestPara.CloseTemp[0];
					m_Pro1CloseTempMinus	=	 m_TestPara.CloseTempMinus[0];
					m_Pro1CloseTempPlus		=	m_TestPara.CloseTempPlus[0];
					m_Pro1OpenCloseGapLow	=	m_TestPara.OpenCloseGapLow[0];
					m_Pro1OpenCloseGapHigh	=	m_TestPara.OpenCloseGapHigh[0];

					m_Pro2TestPoint			=	m_TestPara.TestPoint[1];
					m_Pro2Cycle				=	m_TestPara.Cycle[1];
					if(m_Pro2Cycle <= 0)
					{
						m_Pro2Cycle = 1;
					}
					m_Pro2HeatCoolType		=   m_TestPara.HeatCoolType[1];
					m_Pro2OpenTemp			=	m_TestPara.OpenTemp[1];
					m_Pro2OpenTempMinus		=	m_TestPara.OpenTempMinus[1];
					m_Pro2OpenTempPlus		=	m_TestPara.OpenTempPlus[1];
					m_Pro2CloseTemp			=	m_TestPara.CloseTemp[1];
					m_Pro2CloseTempMinus	=	m_TestPara.CloseTempMinus[1];
					m_Pro2CloseTempPlus		=	m_TestPara.CloseTempPlus[1];
					m_Pro2OpenCloseGapLow	=	m_TestPara.OpenCloseGapLow[1];
					m_Pro2OpenCloseGapHigh	=	m_TestPara.OpenCloseGapHigh[1];

					m_Pro3TestPoint			=	m_TestPara.TestPoint[2];
					m_Pro3Cycle				=	m_TestPara.Cycle[2];
					if(m_Pro3Cycle <= 0)
					{
						m_Pro3Cycle = 1;
					}
					m_Pro3HeatCoolType		=   m_TestPara.HeatCoolType[2];
					m_Pro3OpenTemp			=	m_TestPara.OpenTemp[2];
					m_Pro3OpenTempMinus		=   m_TestPara.OpenTempMinus[2];
					m_Pro3OpenTempPlus		=   m_TestPara.OpenTempPlus[2];
					m_Pro3CloseTemp			=   m_TestPara.CloseTemp[2];
					m_Pro3CloseTempMinus	=	m_TestPara.CloseTempMinus[2];
					m_Pro3CloseTempPlus		=	m_TestPara.CloseTempPlus[2];
					m_Pro3OpenCloseGapLow	=	m_TestPara.OpenCloseGapLow[2];
					m_Pro3OpenCloseGapHigh	=	m_TestPara.OpenCloseGapHigh[2];

					m_Pro4TestPoint			=	m_TestPara.TestPoint[3];
					m_Pro4Cycle				=	m_TestPara.Cycle[3];
					if(m_Pro4Cycle <= 0)
					{
						m_Pro4Cycle = 1;
					}
					m_Pro4HeatCoolType		=	m_TestPara.HeatCoolType[3];
					m_Pro4OpenTemp			=	m_TestPara.OpenTemp[3];
					m_Pro4OpenTempMinus		=	m_TestPara.OpenTempMinus[3];
					m_Pro4OpenTempPlus		=	m_TestPara.OpenTempPlus[3];
					m_Pro4CloseTemp			=	m_TestPara.CloseTemp[3];
					m_Pro4CloseTempMinus	=	m_TestPara.CloseTempMinus[3];
					m_Pro4CloseTempPlus		=	m_TestPara.CloseTempPlus[3];
					m_Pro4OpenCloseGapLow	=	m_TestPara.OpenCloseGapLow[3];
					m_Pro4OpenCloseGapHigh	=	m_TestPara.OpenCloseGapHigh[3];

					m_TestSpeed				=	m_TestPara.TestSpeed;
					m_EnterTestMode			=	m_TestPara.KeepWarmMode;
					m_IsDoubleSwitch		=	m_TestPara.IsDoubleSwitch;
					m_FlashGap				=	m_TestPara.FlashGap;

					m_IsNormalOpen			=	m_TestPara.IsNormalOpen;

					SetRadio();

					m_ComboHeatSpeed.SetCurSel( m_TestPara.HeatSpeed);
					m_ComboCoolSpeed.SetCurSel( m_TestPara.CoolSpeed);
					m_HeatPercent			=	m_TestPara.HeatPercent;
					m_CoolPercent			=	m_TestPara.CoolPercent;

					
					this->UpdateData(false); 
				}

		}


	
}

void CTestParaDlg::OnEnSetfocusEdit4()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void CTestParaDlg::OnBnClickedButtonAdj()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	if(dlg.DoModal() == IDOK) 
	{
		this->m_AdjustTemp = _wtof(dlg.m_Input[0]);
		this->UpdateData(false);
		::G_NormalConfigPara.AdjustTemp =  m_AdjustTemp;
		::G_MainDlg->SaveConfig();  
	}
}

void CTestParaDlg::OnBnClickedButtonSwitchNr()
{
	// TODO: 在此添加控件通知处理程序代码
	CNumInputDlg dlg;
	if(dlg.DoModal() == IDOK) 
	{
		this->m_SwitchNr = _wtol(dlg.m_Input[0]);
		this->UpdateData(false);
		::G_NormalConfigPara.SwitchCount =  m_SwitchNr;
		::G_MainDlg->SaveConfig();  
	}
}

void CTestParaDlg::OnBnClickedButtonProductor()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * wnd = this->GetDlgItem(IDC_EDIT2);
	wnd->SetFocus(); 
	WinExec("osk.exe",SW_NORMAL);

}

void CTestParaDlg::OnBnClickedButtonParaName()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * wnd = this->GetDlgItem(IDC_COMBO2);
	wnd->SetFocus(); 
	WinExec("osk.exe",SW_NORMAL);
	
}

void CTestParaDlg::OnBnClickedButtonProductNo()
{
	// TODO: 在此添加控件通知处理程序代码
		CWnd * wnd = this->GetDlgItem(IDC_EDIT3);
	wnd->SetFocus(); 
	WinExec("osk.exe",SW_NORMAL);
}

void CTestParaDlg::OnBnClickedCheckWarmAutoReady()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CTestParaDlg::OnBnClickedCheckAutoEnter()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTestParaDlg::OnBnClickedButtonHeatCoolType()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CTestParaDlg::OnBnClickedButtonHeatCoolTypePro1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Pro1HeatCoolType == 0)
	{
		m_Pro1HeatCoolType = 1;
		m_bmpButton[0]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}
	else
	{
		m_Pro1HeatCoolType = 0;
		m_bmpButton[0]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	
	}
	m_bmpButton[0]->RedrawWindow(); 
	
}

void CTestParaDlg::OnBnClickedButtonHeatCoolTypePro2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Pro2HeatCoolType == 0)
	{
		m_Pro2HeatCoolType = 1;
		m_bmpButton[1]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}
	else
	{
		m_Pro2HeatCoolType = 0;
		m_bmpButton[1]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	
	}
	m_bmpButton[1]->RedrawWindow(); 
	
}
void CTestParaDlg::OnBnClickedButtonHeatCoolTypePro3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Pro3HeatCoolType == 0)
	{
		m_Pro3HeatCoolType = 1;
		m_bmpButton[2]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}
	else
	{
		m_Pro3HeatCoolType = 0;
		m_bmpButton[2]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	
	}
	m_bmpButton[2]->RedrawWindow(); 
	
}
void CTestParaDlg::OnBnClickedButtonHeatCoolTypePro4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Pro4HeatCoolType == 0)
	{
		m_Pro4HeatCoolType = 1;
		m_bmpButton[3]->LoadBitmaps(IDB_BITMAP_COOL_HEAT);
	}
	else
	{
		m_Pro4HeatCoolType = 0;
		m_bmpButton[3]->LoadBitmaps(IDB_BITMAP_HEAT_COOL);
	
	}
	m_bmpButton[3]->RedrawWindow(); 
	
}

void CTestParaDlg::SelectLanguage(void)
{
	CString s;
	CWnd * w;
	G_NormalConfigPara.Language = G_NormalConfigPara.Language; 

	switch(G_NormalConfigPara.Language)
	{
	case 0:
		this->m_ComboHeatSpeed.AddString(_T("比例控制"));
  
		
		for(int i = 0; i < 50; i ++)
		{
			s.Format(_T("%3.1f度/分钟"),(i + 1) / 10.0f); 
			m_ComboHeatSpeed.AddString(s);
		}

	 

		this->m_ComboCoolSpeed.AddString(_T("比例控制"));
	  

		for(int i = 0; i < 50; i ++)
		{
			s.Format(_T("%3.1f度/分钟"),(i + 1) / 10.0f); 
			m_ComboCoolSpeed.AddString(s);
		}

		break;
	case 1:

		w = this->GetDlgItem(IDC_STATIC_CYCLE1);
		w->SetWindowTextW(_T("CYCLE")); 
		w = this->GetDlgItem(IDC_STATIC_CYCLE2);
		w->SetWindowTextW(_T("CYCLE"));
		w = this->GetDlgItem(IDC_STATIC_CYCLE3);
		w->SetWindowTextW(_T("CYCLE"));
		w = this->GetDlgItem(IDC_STATIC_CYCLE4);
		w->SetWindowTextW(_T("CYCLE"));
		w = this->GetDlgItem(IDC_RADIO_PRO1_HOT);
		w->SetWindowTextW(_T("HIGH POINT"));
		w = this->GetDlgItem(IDC_RADIO_PRO1_MID_1);
		w->SetWindowTextW(_T("MID 1"));
		w = this->GetDlgItem(IDC_RADIO_PRO1_MID_2);
		w->SetWindowTextW(_T("MID 2"));
		w = this->GetDlgItem(IDC_RADIO_PRO1_LOW);
		w->SetWindowTextW(_T("LOW"));
		w = this->GetDlgItem(IDC_RADIO_PRO1_PUPD);
		w->SetWindowTextW(_T("PU/PD"));
		w = this->GetDlgItem(IDC_RADIO_PRO2_NO);
		w->SetWindowTextW(_T("N/C"));
		w = this->GetDlgItem(IDC_RADIO_PRO2_HOT);
		w->SetWindowTextW(_T("HIGH POINT"));
		w = this->GetDlgItem(IDC_RADIO_PRO2_MID_1);
		w->SetWindowTextW(_T("MID 1"));
		w = this->GetDlgItem(IDC_RADIO_PRO2_MID_2);
		w->SetWindowTextW(_T("MID 2"));
		w = this->GetDlgItem(IDC_RADIO_PRO2_LOW);
		w->SetWindowTextW(_T("LOW"));
		w = this->GetDlgItem(IDC_RADIO_PRO2_PUPD);
		w->SetWindowTextW(_T("PU/PD"));
		w = this->GetDlgItem(IDC_RADIO_PRO3_NO);
		w->SetWindowTextW(_T("N/C"));
		w = this->GetDlgItem(IDC_RADIO_PRO3_HOT);
		w->SetWindowTextW(_T("HIGH POINT"));
		w = this->GetDlgItem(IDC_RADIO_PRO3_MID_1);
		w->SetWindowTextW(_T("MID 1"));
		w = this->GetDlgItem(IDC_RADIO_PRO3_MID_2);
		w->SetWindowTextW(_T("MID 2"));
		w = this->GetDlgItem(IDC_RADIO_PRO3_LOW);
		w->SetWindowTextW(_T("LOW"));
		w = this->GetDlgItem(IDC_RADIO_PRO3_PUPD);
		w->SetWindowTextW(_T("PU/PD"));
		w = this->GetDlgItem(IDC_RADIO_PRO4_NO);
		w->SetWindowTextW(_T("N/C"));
		w = this->GetDlgItem(IDC_RADIO_PRO4_HOT);
		w->SetWindowTextW(_T("HIGH POINT"));
		w = this->GetDlgItem(IDC_RADIO_PRO4_MID_1);
		w->SetWindowTextW(_T("MID 1"));
		w = this->GetDlgItem(IDC_RADIO_PRO4_MID_2);
		w->SetWindowTextW(_T("MID 2"));
		w = this->GetDlgItem(IDC_RADIO_PRO4_LOW);
		w->SetWindowTextW(_T("LOW"));
		w = this->GetDlgItem(IDC_RADIO_PRO4_PUPD);
		w->SetWindowTextW(_T("PU/PD"));
		w = this->GetDlgItem(IDC_BUTTON_PRO1_OPEN);
		w->SetWindowTextW(_T("OPEN"));
		w = this->GetDlgItem(IDC_RADIO_KEEP_WARM_1);
		w->SetWindowTextW(_T("Keep Warm"));
		w = this->GetDlgItem(IDC_RADIO_KEEPWARM2);
		w->SetWindowTextW(_T("Open Temp"));
		w = this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_1);
		w->SetWindowTextW(_T("Single"));
		w = this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_2);
		w->SetWindowTextW(_T("Double"));
		w = this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_3);
		w->SetWindowTextW(_T("Normal Close"));
		w = this->GetDlgItem(IDC_RADIO_SWITCH_TYPE_4);
		w->SetWindowTextW(_T("Normal Open"));
		w = this->GetDlgItem(IDC_BUTTON_PRO1_CLOSE);
		w->SetWindowTextW(_T("CLOSE"));
		w = this->GetDlgItem(IDC_BUTTON_PRO2_OPEN);
		w->SetWindowTextW(_T("OPEN"));
		w = this->GetDlgItem(IDC_BUTTON_PRO2_CLOSE);
		w->SetWindowTextW(_T("CLOSE"));
		w = this->GetDlgItem(IDC_BUTTON_PRO3_OPEN);
		w->SetWindowTextW(_T("OPEN"));
		w = this->GetDlgItem(IDC_BUTTON_PRO3_CLOSE);
		w->SetWindowTextW(_T("CLOSE"));
		w = this->GetDlgItem(IDC_BUTTON_PRO4_OPEN);
		w->SetWindowTextW(_T("OPEN"));
		w = this->GetDlgItem(IDC_BUTTON_PRO4_CLOSE);
		w->SetWindowTextW(_T("CLOSE"));
		w = this->GetDlgItem(IDC_BUTTON_PRO1_GAP1);
		w->SetWindowTextW(_T("CLOSE GAP"));
		w = this->GetDlgItem(IDC_BUTTON_PRO2_GAP1);
		w->SetWindowTextW(_T("CLOSE GAP"));
		w = this->GetDlgItem(IDC_BUTTON_PRO3_GAP1);
		w->SetWindowTextW(_T("CLOSE GAP"));
		w = this->GetDlgItem(IDC_BUTTON_PRO4_GAP1);
		w->SetWindowTextW(_T("CLOSE GAP"));
		
		w = this->GetDlgItem(IDC_STATIC_PRO1);
		w->SetWindowTextW(_T("Pro 1"));
		w = this->GetDlgItem(IDC_STATIC_PRO2);
		w->SetWindowTextW(_T("Pro 2"));
		w = this->GetDlgItem(IDC_STATIC_PRO3);
		w->SetWindowTextW(_T("Pro 3"));
		w = this->GetDlgItem(IDC_STATIC_PRO4);
		w->SetWindowTextW(_T("Pro 4"));
		

		w = this->GetDlgItem(IDC_STATIC_MINUS_1);
		w->SetWindowTextW(_T("Gap-"));
		w = this->GetDlgItem(IDC_STATIC_MINUS_2);
		w->SetWindowTextW(_T("Gap-"));
		w = this->GetDlgItem(IDC_STATIC_MINUS_3);
		w->SetWindowTextW(_T("Gap-"));
		w = this->GetDlgItem(IDC_STATIC_MINUS_4);
		w->SetWindowTextW(_T("Gap-"));
		w = this->GetDlgItem(IDC_STATIC_MINUS_5);
		w->SetWindowTextW(_T("Gap-"));
		w = this->GetDlgItem(IDC_STATIC_MINUS_6);
		w->SetWindowTextW(_T("Gap-"));
		w = this->GetDlgItem(IDC_STATIC_MINUS_7);
		w->SetWindowTextW(_T("Gap-"));
		w = this->GetDlgItem(IDC_STATIC_MINUS_8);
		w->SetWindowTextW(_T("Gap-"));

		
		w = this->GetDlgItem(IDC_STATIC_PLUS_1);
		w->SetWindowTextW(_T("Gap+"));
		w = this->GetDlgItem(IDC_STATIC_PLUS_2);
		w->SetWindowTextW(_T("Gap+"));
		w = this->GetDlgItem(IDC_STATIC_PLUS_3);
		w->SetWindowTextW(_T("Gap+"));
		w = this->GetDlgItem(IDC_STATIC_PLUS_4);
		w->SetWindowTextW(_T("Gap+"));
		w = this->GetDlgItem(IDC_STATIC_PLUS_5);
		w->SetWindowTextW(_T("Gap+"));
		w = this->GetDlgItem(IDC_STATIC_PLUS_6);
		w->SetWindowTextW(_T("Gap+"));
		w = this->GetDlgItem(IDC_STATIC_PLUS_7);
		w->SetWindowTextW(_T("Gap+"));
		w = this->GetDlgItem(IDC_STATIC_PLUS_8);
		w->SetWindowTextW(_T("Gap+"));

		w = this->GetDlgItem(IDC_STATIC_ENTRY_1);
		w->SetWindowTextW(_T("Entry"));
		w = this->GetDlgItem(IDC_STATIC_ENTRY_2);
		w->SetWindowTextW(_T("Entry"));
		w = this->GetDlgItem(IDC_STATIC_ENTRY_3);
		w->SetWindowTextW(_T("Entry"));
		w = this->GetDlgItem(IDC_STATIC_ENTRY_4);
		w->SetWindowTextW(_T("Entry"));
		
		w = this->GetDlgItem(IDC_STATIC_SPEED);
		w->SetWindowTextW(_T("Speed"));
		w = this->GetDlgItem(IDC_STATIC_KEEP_TYPE);
		w->SetWindowTextW(_T("Constant temp Type"));
		w = this->GetDlgItem(IDC_STATIC_SWITCH_TYPE);
		w->SetWindowTextW(_T("Switch Type"));

		w = this->GetDlgItem(IDC_STATIC_OTHER);
		w->SetWindowTextW(_T("Other"));

		w = this->GetDlgItem(IDC_STATIC_FLASH);
		w->SetWindowTextW(_T("Flash"));
	
	
		w = this->GetDlgItem(IDC_BUTTON_SAVE);
		w->SetWindowTextW(_T("Save"));
		w = this->GetDlgItem(IDOK);
		w->SetWindowTextW(_T("Quit"));
		w = this->GetDlgItem(IDC_BUTTON_DELETE);
		w->SetWindowTextW(_T("Delete"));
		w = this->GetDlgItem(IDC_BUTTON_ADJ);
		w = this->GetDlgItem(IDC_BUTTON_SWITCH_NR);
		w->SetWindowTextW(_T("Count"));
		w = this->GetDlgItem(IDC_BUTTON_PRODUCTOR);
		w->SetWindowTextW(_T("Productor"));
		w = this->GetDlgItem(IDC_BUTTON_PARA_NAME);
		w->SetWindowTextW(_T("Para Name"));
		w = this->GetDlgItem(IDC_BUTTON_PRODUCT_NO);
		w->SetWindowTextW(_T("Pro. Nr."));
		w = this->GetDlgItem(IDC_CHECK_WARM_AUTO_READY);
		w->SetWindowTextW(_T("Auto Ready"));
		w = this->GetDlgItem(IDC_CHECK_AUTO_ENTER);
		w->SetWindowTextW(_T("Auto Entry"));

		w = this->GetDlgItem(IDC_STATIC_HEAT_SPEED);
		w->SetWindowTextW(_T("Heat Speed"));
		w = this->GetDlgItem(IDC_STATIC_HEAT_RATIO);
		w->SetWindowTextW(_T("Heat Ratio"));
		w = this->GetDlgItem(IDC_STATIC_COOL_SPEED);
		w->SetWindowTextW(_T("Cool Speed"));
		w = this->GetDlgItem(IDC_STATIC_COOL_RATIO);
		w->SetWindowTextW(_T("Cool Ratio"));

		this->m_ComboHeatSpeed.AddString(_T("RATIO"));
  

		for(int i = 0; i < 50; i ++)
		{
			s.Format(_T("%3.1f°C/ min"),(i + 1) / 10.0f); 
			m_ComboHeatSpeed.AddString(s);
		}

	 

		this->m_ComboCoolSpeed.AddString(_T("RATIO"));
	  

		for(int i = 0; i < 50; i ++)
		{
			s.Format(_T("%3.1f°C/ min"),(i + 1) / 10.0f); 
			m_ComboCoolSpeed.AddString(s);
		}


	break;
	}
}

void CTestParaDlg::OnBnClickedRadioSwitchType3()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_IsNormalOpen = 0;
}

void CTestParaDlg::OnBnClickedRadioSwitchType4()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_IsNormalOpen = 1;
}

void CTestParaDlg::OnBnClickedRadioSwitchType1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_IsDoubleSwitch = 0;
}

void CTestParaDlg::OnBnClickedRadioSwitchType2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_IsDoubleSwitch = 1;
}

void CTestParaDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s;
	int sel = m_ControlParaName.GetCurSel();
	if(sel < 0)
	{
		sel = 0;
	}
	m_ControlParaName.GetLBText(sel,s); 
	s.Trim();
	if(s.GetLength() == 0)
	{
		AfxMessageBox(_T("参数名不能为空"));
	}
	else
	{
		s += _T(".para");
		memcpy(G_NormalConfigPara.ParaName,s.GetBuffer(),sizeof(G_NormalConfigPara.ParaName));
		G_MainDlg->SaveConfig(); 
		::G_MainDlg->ReadConfig();  
		OnOK();
	}
}


void CTestParaDlg::OnCbnSelchangeComboHeatSpeed()
{
	// TODO: 在此添加控件通知处理程序代码

	//AfxGetApp()->WriteProfileInt(_T("SET"),_T("HEAT SPEED"), this->m_ComboHeatSpeed.GetCurSel() ); 
}

void CTestParaDlg::OnCbnSelchangeComboCoolSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxGetApp()->WriteProfileInt(_T("SET"),_T("COOL SPEED"), this->m_ComboCoolSpeed.GetCurSel() ); 

}

void CTestParaDlg::OnEnChangeEditHeatPercent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	//this->UpdateData();

	//AfxGetApp()->WriteProfileInt(_T("SET"),_T("HEAT PERCENT"), this->m_HeatPercent ); 

}

void CTestParaDlg::OnEnChangeEditCoolPercent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	
	//this->UpdateData();

	//AfxGetApp()->WriteProfileInt(_T("SET"),_T("COOL PERCENT"), this->m_CoolPercent ); 

}
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

void CTestParaDlg::UpdateParaNameList()
{
	m_ControlParaName.ResetContent(); 

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
		this->m_ControlParaName.AddString(s);
	}

	
}
