// ProgramDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "ProgramDlg.h"


// CProgramDlg 对话框

IMPLEMENT_DYNAMIC(CProgramDlg, CDialog)

CProgramDlg::CProgramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgramDlg::IDD, pParent)
{

}

CProgramDlg::~CProgramDlg()
{
}

void CProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BOX_NR, m_Combo_BoxNr);
	DDX_Control(pDX, IDC_COMBO_TM7705_OSC, m_ComboTM7705_OSC);
	DDX_Control(pDX, IDC_COMBO_INTERFACE_BOARD_TYPE, m_ComboInterfaceBoardType);
}


BEGIN_MESSAGE_MAP(CProgramDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProgramDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CProgramDlg 消息处理程序


BOOL CProgramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->m_Combo_BoxNr.AddString(_T("1"));
	this->m_Combo_BoxNr.AddString(_T("2"));
	this->m_Combo_BoxNr.AddString(_T("3"));
	this->m_Combo_BoxNr.AddString(_T("4"));
	this->m_Combo_BoxNr.AddString(_T("5"));
	this->m_Combo_BoxNr.AddString(_T("6"));

	this->m_Combo_BoxNr.SetCurSel(0);


	m_ComboTM7705_OSC.AddString(_T("有晶振"));
	m_ComboTM7705_OSC.AddString(_T("无晶振"));

	m_ComboTM7705_OSC.SetCurSel(::G_NormalConfigPara[0].TM7705_NoOSC);

	for(int i = 0; i < sizeof(G_InterfacePcbTypeName)/sizeof(G_InterfacePcbTypeName[0]); i++)
	{
		this->m_ComboInterfaceBoardType.AddString(G_InterfacePcbTypeName[i].name);
	}

	m_ComboInterfaceBoardType.SetCurSel(G_NormalConfigPara[0].InterfaceBoardType); 

	return true;
}
void CProgramDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_BoxNr				= m_Combo_BoxNr.GetCurSel();
	m_TM7705_OSC		= m_ComboTM7705_OSC.GetCurSel();
	m_InterfaceBoardType = m_ComboInterfaceBoardType.GetCurSel();
	

	OnOK();
}
