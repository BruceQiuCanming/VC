// BarcodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HotLong_PCB.h"
#include "BarcodeDlg.h"


// CBarcodeDlg 对话框

IMPLEMENT_DYNAMIC(CBarcodeDlg, CDialog)

CBarcodeDlg::CBarcodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarcodeDlg::IDD, pParent)
	, m_Barcode(_T(""))
{

}

CBarcodeDlg::~CBarcodeDlg()
{
}

void CBarcodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BARCODE, m_Barcode);
}


BEGIN_MESSAGE_MAP(CBarcodeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBarcodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBarcodeDlg 消息处理程序

void CBarcodeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CBarcodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd * w = this->GetDlgItem(IDC_EDIT_BARCODE);
	w->SetFocus();
	return false;
}