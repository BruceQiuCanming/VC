// PasswordDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "PasswordDlg.h"
//#include "afxdialogex.h"


// CPasswordDlg 对话框

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_PASSWORD, pParent)
	, m_Password(_T(""))
{

}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Password);
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPasswordDlg 消息处理程序


void CPasswordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	m_Password.Trim();
	if (m_Password == _T("147258"))
	{
		CDialog::OnOK();
	}
	else
	{
		CDialog::OnCancel();
	}
}
