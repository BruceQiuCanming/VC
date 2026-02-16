// InputDlg.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog


CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
	m_Input = _T("");
	//}}AFX_DATA_INIT
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CInputDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Input);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
	ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, OnButton0)
	ON_BN_CLICKED(IDC_BUTTON_DOT, OnButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PLUS_MINUS, &CInputDlg::OnBnClickedButtonPlusMinus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers

void CInputDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("1");
	this->UpdateData(false);
}

void CInputDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("2");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("3");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("4");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("5");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("6");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("7");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("8");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("9");
	this->UpdateData(false);
	
}

void CInputDlg::OnButton0() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T("0");
	this->UpdateData(false);
	
}

void CInputDlg::OnButtonDot() 
{
	// TODO: Add your control notification handler code here
	this->m_Input += _T(".");
	this->UpdateData(false);
	
}

void CInputDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	this->m_Input = _T("");
	this->UpdateData(false);
	
}

BOOL CInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(this->m_Title); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_Input.TrimLeft();
	m_Input.TrimRight();

	if(this->m_Input.GetLength() >= 1)
	{
		CDialog::OnOK();
	}
}

void CInputDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input.TrimLeft();
	m_Input.TrimRight();
	this->UpdateData();
	
 	m_Input.Trim(); 
	

	this->UpdateData(false);

	OnOK();
}

void CInputDlg::OnBnClickedButtonPlusMinus()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input.Trim(); 
	if(m_Input.GetLength() > 0)
	{
		if(m_Input.GetAt(0) == '-') 
		{
			m_Input.SetAt(0, ' ');
			m_Input.Trim(); 
		}
		else
		{
			m_Input = _T("-") + m_Input;
		}
	}
	this->UpdateData(false);
}
