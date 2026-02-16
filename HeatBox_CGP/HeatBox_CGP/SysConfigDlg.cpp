// SysConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "SysConfigDlg.h"


// CSysConfigDlg 对话框

IMPLEMENT_DYNAMIC(CSysConfigDlg, CDialog)

CSysConfigDlg::CSysConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysConfigDlg::IDD, pParent)
{

}

CSysConfigDlg::~CSysConfigDlg()
{
}

void CSysConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CSysConfigDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSysConfigDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDCANCEL, &CSysConfigDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSysConfigDlg 消息处理程序
BOOL CSysConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Tab.ShowWindow(SW_SHOW);
	m_Tab.InsertItem(0,_T("工位开关"));
	m_Tab.InsertItem(1,_T("系统配置"));

	m_SwitchUsedDlg.Create(IDD_DIALOG_SWITCH_ONOFF,&m_Tab);
	m_OtherConfigDlg.Create(IDD_DIALOG_OTHER_CONFIG,&m_Tab);
	CRect rect;
	m_Tab.GetClientRect(&rect);
	rect.top += 30;
	m_SwitchUsedDlg.MoveWindow(&rect);
	m_OtherConfigDlg.MoveWindow(&rect);
	m_SwitchUsedDlg.ShowWindow(SW_SHOW);

	return true;
}

void CSysConfigDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	switch(m_Tab.GetCurSel())
	{
	case 0:
		m_SwitchUsedDlg.ShowWindow(SW_SHOW);
		m_OtherConfigDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_SwitchUsedDlg.ShowWindow(SW_HIDE);
		m_OtherConfigDlg.ShowWindow(SW_SHOW);
		break;
	}
}

void CSysConfigDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
