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
	, m_Tester(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_TESTER, m_Tester);
}


BEGIN_MESSAGE_MAP(CDlgInfomation, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInfomation::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInfomation 消息处理程序

void CDlgInfomation::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData();
	

	memset(&G_NormalConfigPara[m_BoxNr].TestMemo,0,sizeof(G_NormalConfigPara[m_BoxNr].TestMemo));
	if(m_Memo.GetLength() > sizeof(G_NormalConfigPara[m_BoxNr].TestMemo) - 1)
	{
		m_Memo = m_Memo.Left(sizeof(G_NormalConfigPara[m_BoxNr].TestMemo) - 1);
	}
	TCHAR * buf = m_Memo.GetBuffer();

	memcpy(&G_NormalConfigPara[m_BoxNr].TestMemo,buf,m_Memo.GetLength() *sizeof(TCHAR));


	memset(&G_NormalConfigPara[m_BoxNr].Tester,0,sizeof(G_NormalConfigPara[m_BoxNr].Tester));
	if(m_Tester.GetLength() > sizeof(G_NormalConfigPara[m_BoxNr].Tester) - 1)
	{
		m_Tester = m_Tester.Left(sizeof(G_NormalConfigPara[m_BoxNr].Tester) - 1);
	}
	buf = m_Tester.GetBuffer();

	memcpy(&G_NormalConfigPara[m_BoxNr].Tester,buf,m_Tester.GetLength() *sizeof(TCHAR));



	if(G_NormalConfigPara[m_BoxNr].bBarcode > 0)
	{
		if(m_Memo.GetLength() <= 0)
		{
			AfxMessageBox(_T("请输入条码"));
			return;
		}
	}

	OnOK();
}

BOOL CDlgInfomation::OnInitDialog() 
{

	if(G_iLangID == 0)
	{
	}
	else
	{
		this->SetWindowTextW(_T("TEST INFO")); 
	}
	
	CDialog::OnInitDialog();
	

	if(::G_NormalConfigPara[0].bBarcode)
	{
		CWnd *w = this->GetDlgItem(IDC_STATIC_MEMO);
		w->SetWindowText(_T("输入条码"));
	}


	return true;
}