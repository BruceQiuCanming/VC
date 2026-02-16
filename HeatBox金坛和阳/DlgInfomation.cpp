// DlgInfomation.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "DlgInfomation.h"


// CDlgInfomation 对话框

IMPLEMENT_DYNAMIC(CDlgInfomation, CDialog)

CDlgInfomation::CDlgInfomation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfomation::IDD, pParent)
	, m_Info(_T(""))
{

}

CDlgInfomation::~CDlgInfomation()
{
}

void CDlgInfomation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MEMO, m_Memo);
	DDX_Text(pDX, IDC_STATIC_INFO, m_Info);
}


BEGIN_MESSAGE_MAP(CDlgInfomation, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInfomation::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInfomation 消息处理程序

void CDlgInfomation::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData();
	
	CString s;
	
	s.Format(_T("TEST MEMO %d"), m_BoxNr);
	
	AfxGetApp()->WriteProfileStringW(_T("SETTING"),s,m_Memo);

	OnOK();
}

BOOL CDlgInfomation::OnInitDialog() 
{

	
	CDialog::OnInitDialog();
	



	return true;
}