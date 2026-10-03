// ScannerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "ScannerDlg.h"


// CScannerDlg 对话框

IMPLEMENT_DYNAMIC(CScannerDlg, CDialog)

CScannerDlg::CScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScannerDlg::IDD, pParent)
	, m_SwitchPos(0)
	, m_Barcode(_T(""))
{

}

CScannerDlg::~CScannerDlg()
{
}

void CScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POS, m_SwitchPos);
	DDV_MinMaxInt(pDX, m_SwitchPos, 1, 60);
	DDX_Text(pDX, IDC_EDIT_BARCODE, m_Barcode);
}


BEGIN_MESSAGE_MAP(CScannerDlg, CDialog)
END_MESSAGE_MAP()


// CScannerDlg 消息处理程序
