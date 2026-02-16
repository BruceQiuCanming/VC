// OtherConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "HeatBox_CGPDlg.h"
#include "OtherConfigDlg.h"


// COtherConfigDlg 对话框

IMPLEMENT_DYNAMIC(COtherConfigDlg, CDialog)

COtherConfigDlg::COtherConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COtherConfigDlg::IDD, pParent)
{

}

COtherConfigDlg::~COtherConfigDlg()
{
}

void COtherConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COtherConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_COM1, &COtherConfigDlg::OnBnClickedRadioCom1)
	ON_BN_CLICKED(IDC_RADIO_COM2, &COtherConfigDlg::OnBnClickedRadioCom2)
	ON_BN_CLICKED(IDC_RADIO_COM3, &COtherConfigDlg::OnBnClickedRadioCom3)
	ON_BN_CLICKED(IDC_RADIO_COM4, &COtherConfigDlg::OnBnClickedRadioCom4)
	ON_BN_CLICKED(IDC_RADIO_COM5, &COtherConfigDlg::OnBnClickedRadioCom5)
	ON_BN_CLICKED(IDC_RADIO_COM6, &COtherConfigDlg::OnBnClickedRadioCom6)
	ON_BN_CLICKED(IDC_RADIO_COM7, &COtherConfigDlg::OnBnClickedRadioCom7)
	ON_BN_CLICKED(IDC_RADIO_COM8, &COtherConfigDlg::OnBnClickedRadioCom8)
END_MESSAGE_MAP()


// COtherConfigDlg 消息处理程序
extern NORMAL_CONFIG_PARA	G_NormalConfigPara;
void COtherConfigDlg::OnBnClickedRadioCom1()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 0; 
	::G_MainDlg->SaveConfig();
}

void COtherConfigDlg::OnBnClickedRadioCom2()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 1; 
	::G_MainDlg->SaveConfig();
}

void COtherConfigDlg::OnBnClickedRadioCom3()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 2; 
	::G_MainDlg->SaveConfig();
}

void COtherConfigDlg::OnBnClickedRadioCom4()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 3; 
	::G_MainDlg->SaveConfig();
}

void COtherConfigDlg::OnBnClickedRadioCom5()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 4; 
	::G_MainDlg->SaveConfig();
}

void COtherConfigDlg::OnBnClickedRadioCom6()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 5; 
	::G_MainDlg->SaveConfig();
}

void COtherConfigDlg::OnBnClickedRadioCom7()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 6; 
	::G_MainDlg->SaveConfig();
}

void COtherConfigDlg::OnBnClickedRadioCom8()
{
	// TODO: 在此添加控件通知处理程序代码
	::G_NormalConfigPara.CommPort = 7; 
	::G_MainDlg->SaveConfig();
}
