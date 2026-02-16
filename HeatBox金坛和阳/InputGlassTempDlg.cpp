// InputGlassTempDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "InputGlassTempDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGlassTempDlg dialog


CInputGlassTempDlg::CInputGlassTempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputGlassTempDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGlassTempDlg)
	m_Temp1 = "";
	m_Temp2 = "";
	m_Temp3 = "";
	m_Temp4 = "";
	//}}AFX_DATA_INIT
	m_InputGlassTemp_ID = 0;
}


void CInputGlassTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGlassTempDlg)
	DDX_Text(pDX, IDC_EDIT_GLASS_1, m_Temp1);
	DDX_Text(pDX, IDC_EDIT_GLASS_2, m_Temp2);
	DDX_Text(pDX, IDC_EDIT_GLASS_3, m_Temp3);
	DDX_Text(pDX, IDC_EDIT_GLASS_4, m_Temp4);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_GLASS_2, m_InputGlassTemp_2);
	DDX_Control(pDX, IDC_EDIT_GLASS_1, m_InputGlassTemp_1);
	DDX_Control(pDX, IDC_EDIT_GLASS_3, m_InputGlassTemp_3);
	DDX_Control(pDX, IDC_EDIT_GLASS_4, m_InputGlassTemp_4);
}


BEGIN_MESSAGE_MAP(CInputGlassTempDlg, CDialog)
	//{{AFX_MSG_MAP(CInputGlassTempDlg)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_2, &CInputGlassTempDlg::OnEnSetfocusEditGlass2)
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_1, &CInputGlassTempDlg::OnEnSetfocusEditGlass1)
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_3, &CInputGlassTempDlg::OnEnSetfocusEditGlass3)
	ON_EN_SETFOCUS(IDC_EDIT_GLASS_4, &CInputGlassTempDlg::OnEnSetfocusEditGlass4)
	ON_BN_CLICKED(IDC_BUTTON_1, &CInputGlassTempDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CInputGlassTempDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CInputGlassTempDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CInputGlassTempDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CInputGlassTempDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CInputGlassTempDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CInputGlassTempDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CInputGlassTempDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CInputGlassTempDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CInputGlassTempDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON_DOT, &CInputGlassTempDlg::OnBnClickedButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CInputGlassTempDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDOK, &CInputGlassTempDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGlassTempDlg message handlers

void CInputGlassTempDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData();
	CDialog::OnOK();
}

void CInputGlassTempDlg::OnEnSetfocusEditGlass2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 2;
}

void CInputGlassTempDlg::OnEnSetfocusEditGlass1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 1;
}

void CInputGlassTempDlg::OnEnSetfocusEditGlass3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 3;
}

void CInputGlassTempDlg::OnEnSetfocusEditGlass4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_InputGlassTemp_ID = 4;
}
void CInputGlassTempDlg::OnBnClickedButton0_9(CString num)
{
	switch(m_InputGlassTemp_ID)
	{
	case 1:
		m_Temp1 += num;
		break;
	case 2:
		m_Temp2 += num;
		break;
	case 3:
		m_Temp3 += num;
		break;
	case 4:
		m_Temp4 += num;
		break;
	}
	this->UpdateData(false);

}
void CInputGlassTempDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("1");
	
}

void CInputGlassTempDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("2");
}

void CInputGlassTempDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("3");
}

void CInputGlassTempDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("4");
}

void CInputGlassTempDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("5");
}

void CInputGlassTempDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("6");
}

void CInputGlassTempDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("7");
}

void CInputGlassTempDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("8");
}

void CInputGlassTempDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("9");
}

void CInputGlassTempDlg::OnBnClickedButton0()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9("0");
}

void CInputGlassTempDlg::OnBnClickedButtonDot()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButton0_9(".");
}

void CInputGlassTempDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_InputGlassTemp_ID)
	{
	case 1:
		m_Temp1 = "";
		break;
	case 2:
		m_Temp2 = "";
		break;
	case 3:
		m_Temp3 = "";
		break;
	case 4:
		m_Temp4 = "";
		break;
	}
	this->UpdateData(false);
}


BOOL CInputGlassTempDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(this->m_Title); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CInputGlassTempDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	m_Temp1.TrimLeft();
	m_Temp1.TrimRight();

	double f ;
	f = _tcstod(m_Temp1,NULL);
	if(f < 30 || f > 160)
	{
		AfxMessageBox(_T("1号水银温度计的温度有误，请修改!"));
		return;
	}

	f = _tcstod(m_Temp2,NULL);
	if(f < 30 || f > 160)
	{
		AfxMessageBox(_T("2号水银温度计的温度有误，请修改!"));
		return;
	}

	f = _tcstod(m_Temp3,NULL);
	if(f < 30 || f > 160)
	{
		AfxMessageBox(_T("3号水银温度计的温度有误，请修改!"));
		return;
	}

	f = _tcstod(m_Temp4,NULL);
	if(f < 30 || f > 160)
	{
		AfxMessageBox(_T("4号水银温度计的温度有误，请修改!"));
		return;
	}






	CDialog::OnOK();
}
