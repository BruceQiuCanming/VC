// NumCharKeyBoardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "NumCharKeyBoardDlg.h"


// CNumCharKeyBoardDlg 对话框

IMPLEMENT_DYNAMIC(CNumCharKeyBoardDlg, CDialog)

CNumCharKeyBoardDlg::CNumCharKeyBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumCharKeyBoardDlg::IDD, pParent)
	, m_Input(_T(""))
{

}

CNumCharKeyBoardDlg::~CNumCharKeyBoardDlg()
{
}

void CNumCharKeyBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_Input);
}


BEGIN_MESSAGE_MAP(CNumCharKeyBoardDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_1, &CNumCharKeyBoardDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CNumCharKeyBoardDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CNumCharKeyBoardDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CNumCharKeyBoardDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CNumCharKeyBoardDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CNumCharKeyBoardDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CNumCharKeyBoardDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CNumCharKeyBoardDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CNumCharKeyBoardDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CNumCharKeyBoardDlg::OnBnClickedButton0)

	ON_BN_CLICKED(IDOK, &CNumCharKeyBoardDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_A, &CNumCharKeyBoardDlg::OnBnClickedButtonA)
	ON_BN_CLICKED(IDC_BUTTON_B, &CNumCharKeyBoardDlg::OnBnClickedButtonB)
	ON_BN_CLICKED(IDC_BUTTON_C, &CNumCharKeyBoardDlg::OnBnClickedButtonC)
	ON_BN_CLICKED(IDC_BUTTON_D, &CNumCharKeyBoardDlg::OnBnClickedButtonD)
	ON_BN_CLICKED(IDC_BUTTON_E, &CNumCharKeyBoardDlg::OnBnClickedButtonE)
	ON_BN_CLICKED(IDC_BUTTON_F, &CNumCharKeyBoardDlg::OnBnClickedButtonF)
	ON_BN_CLICKED(IDC_BUTTON_G, &CNumCharKeyBoardDlg::OnBnClickedButtonG)
	ON_BN_CLICKED(IDC_BUTTON_H, &CNumCharKeyBoardDlg::OnBnClickedButtonH)
	ON_BN_CLICKED(IDC_BUTTON_I, &CNumCharKeyBoardDlg::OnBnClickedButtonI)
	ON_BN_CLICKED(IDC_BUTTON_J, &CNumCharKeyBoardDlg::OnBnClickedButtonJ)
	ON_BN_CLICKED(IDC_BUTTON_K, &CNumCharKeyBoardDlg::OnBnClickedButtonK)
	ON_BN_CLICKED(IDC_BUTTON_L, &CNumCharKeyBoardDlg::OnBnClickedButtonL)
	ON_BN_CLICKED(IDC_BUTTON_M, &CNumCharKeyBoardDlg::OnBnClickedButtonM)
	ON_BN_CLICKED(IDC_BUTTON_N, &CNumCharKeyBoardDlg::OnBnClickedButtonN)
	ON_BN_CLICKED(IDC_BUTTON_O, &CNumCharKeyBoardDlg::OnBnClickedButtonO)
	ON_BN_CLICKED(IDC_BUTTON_P, &CNumCharKeyBoardDlg::OnBnClickedButtonP)
	ON_BN_CLICKED(IDC_BUTTON_Q, &CNumCharKeyBoardDlg::OnBnClickedButtonQ)
	ON_BN_CLICKED(IDC_BUTTON_R, &CNumCharKeyBoardDlg::OnBnClickedButtonR)
	ON_BN_CLICKED(IDC_BUTTON_S, &CNumCharKeyBoardDlg::OnBnClickedButtonS)
	ON_BN_CLICKED(IDC_BUTTON_T, &CNumCharKeyBoardDlg::OnBnClickedButtonT)
	ON_BN_CLICKED(IDC_BUTTON_U, &CNumCharKeyBoardDlg::OnBnClickedButtonU)
	ON_BN_CLICKED(IDC_BUTTON_V, &CNumCharKeyBoardDlg::OnBnClickedButtonV)
	ON_BN_CLICKED(IDC_BUTTON_W, &CNumCharKeyBoardDlg::OnBnClickedButtonW)
	ON_BN_CLICKED(IDC_BUTTON_X, &CNumCharKeyBoardDlg::OnBnClickedButtonX)
	ON_BN_CLICKED(IDC_BUTTON_Y, &CNumCharKeyBoardDlg::OnBnClickedButtonY)
	ON_BN_CLICKED(IDC_BUTTON_Z, &CNumCharKeyBoardDlg::OnBnClickedButtonZ)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, &CNumCharKeyBoardDlg::OnBnClickedButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CNumCharKeyBoardDlg::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CNumCharKeyBoardDlg 消息处理程序

void CNumCharKeyBoardDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("1");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("2");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("3");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("4");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("5");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("6");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("7");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("8");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("9");
	this->UpdateData(false);
}
void CNumCharKeyBoardDlg::OnBnClickedButton0()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("0");
	this->UpdateData(false);
}


void CNumCharKeyBoardDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CNumCharKeyBoardDlg::OnBnClickedButtonA()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("A");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonB()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("B");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonC()
{
	this->m_Input += _T("C");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonD()
{
	this->m_Input += _T("D");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonE()
{
	this->m_Input += _T("E");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonF()
{
	this->m_Input += _T("F");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonG()
{
	this->m_Input += _T("G");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonH()
{
	this->m_Input += _T("H");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonI()
{
	this->m_Input += _T("I");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonJ()
{
	this->m_Input += _T("J");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonK()
{
	this->m_Input += _T("K");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonL()
{
	this->m_Input += _T("L");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonM()
{
	this->m_Input += _T("M");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonN()
{
	this->m_Input += _T("N");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonO()
{
	this->m_Input += _T("O");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonP()
{
	this->m_Input += _T("P");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonQ()
{
	this->m_Input += _T("Q");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonR()
{
	this->m_Input += _T("R");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonS()
{
	this->m_Input += _T("S");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonT()
{
	this->m_Input += _T("T");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonU()
{
	this->m_Input += _T("U");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonV()
{
	this->m_Input += _T("V");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonW()
{
	this->m_Input += _T("W");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonX()
{
	this->m_Input += _T("X");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonY()
{
	this->m_Input += _T("Y");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonZ()
{
	this->m_Input += _T("Z");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonMinus()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input += _T("-");
	this->UpdateData(false);
}

void CNumCharKeyBoardDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_Input = _T("");
	this->UpdateData(false);
}
