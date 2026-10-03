// NumInputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "HeatBox_CGPDlg.h"
#include "NumInputDlg.h"


// CNumInputDlg 对话框

IMPLEMENT_DYNAMIC(CNumInputDlg, CDialog)

CNumInputDlg::CNumInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumInputDlg::IDD, pParent)
{
		m_Input[0]= _T("");
		m_Msg[0] = "";
		m_Input[1]= _T("");
		m_Msg[1] = "";
		m_Input[2]= _T("");
		m_Msg[2] = "";

		m_CurInput = 0;

}

CNumInputDlg::~CNumInputDlg()
{
}

void CNumInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Input[0]);
	DDX_Text(pDX, IDC_EDIT2, m_Input[1]);
	DDX_Text(pDX, IDC_EDIT3, m_Input[2]);
	DDX_Text(pDX, IDC_STATIC_MSG1, m_Msg[0]);
	DDX_Text(pDX, IDC_STATIC_MSG2, m_Msg[1]);
	DDX_Text(pDX, IDC_STATIC_MSG3, m_Msg[2]);
}


BEGIN_MESSAGE_MAP(CNumInputDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CNumInputDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CNumInputDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CNumInputDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CNumInputDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CNumInputDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CNumInputDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CNumInputDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CNumInputDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CNumInputDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CNumInputDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CNumInputDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CNumInputDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDOK, &CNumInputDlg::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT1, &CNumInputDlg::OnEnSetfocusEdit1)
	ON_BN_CLICKED(IDOK3, &CNumInputDlg::OnBnClickedOk3)
	ON_EN_SETFOCUS(IDC_EDIT2, &CNumInputDlg::OnEnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &CNumInputDlg::OnEnSetfocusEdit3)
END_MESSAGE_MAP()


// CNumInputDlg 消息处理程序

void CNumInputDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("1");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("2");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("3");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("4");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("5");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("6");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("7");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("8");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("9");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T("0");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] += _T(".");
	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Input[m_CurInput].GetLength() == 0)
	{
		m_Input[m_CurInput] += _T("+");
	}
	else if(m_Input[m_CurInput].GetAt(0) == '+') 
	{
		m_Input[m_CurInput].SetAt(0, '-');
	}
	else if(m_Input[m_CurInput].GetAt(0) == '-') 
	{
		m_Input[m_CurInput].SetAt(0, '+'); 
	}
	else
	{
		m_Input[m_CurInput] =_T("+")+m_Input[m_CurInput];
	}

	this->UpdateData(false); 
}

void CNumInputDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CNumInputDlg::OnInitDialog()
{
	CWnd *wnd;
	CDialog::OnInitDialog();

	this->SetWindowTextW(m_Title);

	wnd = this->GetDlgItem(IDC_EDIT2);
	
	if(m_Msg[1].GetLength() > 0)
	{
		wnd->ShowWindow(SW_SHOW);
	}
	else
	{
		wnd->ShowWindow(SW_HIDE);
	}

	wnd = this->GetDlgItem(IDC_EDIT3);
	
	if(m_Msg[2].GetLength() > 0)
	{
		wnd->ShowWindow(SW_SHOW);
	}
	else
	{
		wnd->ShowWindow(SW_HIDE);
	}

	
	switch(::G_NormalConfigPara.Language)
	{
	case 0:
		break;
	case 1:
		wnd = this->GetDlgItem(IDOK);
		wnd->SetWindowTextW(_T("OK"));
		wnd = this->GetDlgItem(IDOK3);
		wnd->SetWindowTextW(_T("DEL. ALL"));
		wnd = this->GetDlgItem(IDCANCEL);
		wnd->SetWindowTextW(_T("CANCLE"));
		break;
	}

	return true;
}
void CNumInputDlg::OnEnSetfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CurInput = 0;
}

void CNumInputDlg::OnBnClickedOk3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Input[m_CurInput] = ""; 
	this->UpdateData(false); 
}

void CNumInputDlg::OnEnSetfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CurInput = 1;
}

void CNumInputDlg::OnEnSetfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CurInput = 2;
}
