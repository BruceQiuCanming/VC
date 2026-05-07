// KeyBoardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "KeyBoardDlg.h"


// CKeyBoardDlg 对话框

IMPLEMENT_DYNAMIC(CKeyBoardDlg, CDialog)

CKeyBoardDlg::CKeyBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyBoardDlg::IDD, pParent)
	, m_Input(_T(""))
{

}

CKeyBoardDlg::~CKeyBoardDlg()
{
}

void CKeyBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_Input);
}


BEGIN_MESSAGE_MAP(CKeyBoardDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_1, &CKeyBoardDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, &CKeyBoardDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, &CKeyBoardDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, &CKeyBoardDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, &CKeyBoardDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, &CKeyBoardDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, &CKeyBoardDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, &CKeyBoardDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, &CKeyBoardDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON_0, &CKeyBoardDlg::OnBnClickedButton0)

	ON_BN_CLICKED(IDC_BUTTON_A, &CKeyBoardDlg::OnBnClickedButtonA)
	ON_BN_CLICKED(IDC_BUTTON_B, &CKeyBoardDlg::OnBnClickedButtonB)
	ON_BN_CLICKED(IDC_BUTTON_C, &CKeyBoardDlg::OnBnClickedButtonC)
	ON_BN_CLICKED(IDC_BUTTON_D, &CKeyBoardDlg::OnBnClickedButtonD)
	ON_BN_CLICKED(IDC_BUTTON_E, &CKeyBoardDlg::OnBnClickedButtonE)
	ON_BN_CLICKED(IDC_BUTTON_F, &CKeyBoardDlg::OnBnClickedButtonF)
	ON_BN_CLICKED(IDC_BUTTON_G, &CKeyBoardDlg::OnBnClickedButtonG)
	ON_BN_CLICKED(IDC_BUTTON_H, &CKeyBoardDlg::OnBnClickedButtonH)
	ON_BN_CLICKED(IDC_BUTTON_I, &CKeyBoardDlg::OnBnClickedButtonI)
	ON_BN_CLICKED(IDC_BUTTON_J, &CKeyBoardDlg::OnBnClickedButtonJ)

	ON_BN_CLICKED(IDC_BUTTON_K, &CKeyBoardDlg::OnBnClickedButtonK)
	ON_BN_CLICKED(IDC_BUTTON_L, &CKeyBoardDlg::OnBnClickedButtonL)
	ON_BN_CLICKED(IDC_BUTTON_M, &CKeyBoardDlg::OnBnClickedButtonM)
	ON_BN_CLICKED(IDC_BUTTON_N, &CKeyBoardDlg::OnBnClickedButtonN)
	ON_BN_CLICKED(IDC_BUTTON_O, &CKeyBoardDlg::OnBnClickedButtonO)
	ON_BN_CLICKED(IDC_BUTTON_P, &CKeyBoardDlg::OnBnClickedButtonP)
	ON_BN_CLICKED(IDC_BUTTON_Q, &CKeyBoardDlg::OnBnClickedButtonQ)
	ON_BN_CLICKED(IDC_BUTTON_R, &CKeyBoardDlg::OnBnClickedButtonR)
	ON_BN_CLICKED(IDC_BUTTON_S, &CKeyBoardDlg::OnBnClickedButtonS)
	ON_BN_CLICKED(IDC_BUTTON_T, &CKeyBoardDlg::OnBnClickedButtonT)

	ON_BN_CLICKED(IDC_BUTTON_U, &CKeyBoardDlg::OnBnClickedButtonU)
	ON_BN_CLICKED(IDC_BUTTON_V, &CKeyBoardDlg::OnBnClickedButtonV)
	ON_BN_CLICKED(IDC_BUTTON_W, &CKeyBoardDlg::OnBnClickedButtonW)
	ON_BN_CLICKED(IDC_BUTTON_X, &CKeyBoardDlg::OnBnClickedButtonX)
	ON_BN_CLICKED(IDC_BUTTON_Y, &CKeyBoardDlg::OnBnClickedButtonY)
	ON_BN_CLICKED(IDC_BUTTON_Z, &CKeyBoardDlg::OnBnClickedButtonZ)
	ON_BN_CLICKED(IDC_BUTTON_PLUS, &CKeyBoardDlg::OnBnClickedButton_PLUS)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, &CKeyBoardDlg::OnBnClickedButton_MINUS)
	ON_BN_CLICKED(IDC_BUTTON_DOT, &CKeyBoardDlg::OnBnClickedButton_DOT)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CKeyBoardDlg::OnBnClickedButton_CLEAR)




	ON_BN_CLICKED(IDOK, &CKeyBoardDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CKeyBoardDlg 消息处理程序

void CKeyBoardDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("1");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("2");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("3");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("4");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("5");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("6");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("7");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("8");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("9");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButton0()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("0");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonA()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("A");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonB()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("B");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonC()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("C");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonD()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("D");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonE()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("E");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonF()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("F");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonG()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("G");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonH()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("H");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonI()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("I");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonJ()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("J");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonK()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("K");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonM()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("M");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonN()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("N");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonO()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("O");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonP()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("P");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonQ()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("Q");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonR()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("R");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonS()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("S");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonT()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("T");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonU()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("U");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonL()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("L");
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButtonV()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("V");
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButtonW()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("W");
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButtonX()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("X");
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButtonY()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("Y");
	this->UpdateData(false);

}
void CKeyBoardDlg::OnBnClickedButtonZ()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T("Z");
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButton_PLUS()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input = _T("+") + this->m_Input;
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButton_MINUS()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input = _T("-") + this->m_Input;
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButton_DOT()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input += _T(".");
	this->UpdateData(false);

}

void CKeyBoardDlg::OnBnClickedButton_CLEAR()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_Input = _T("");
	this->UpdateData(false);

}


void CKeyBoardDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
