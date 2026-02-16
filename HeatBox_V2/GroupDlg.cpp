// GroupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "GroupDlg.h"
#include "HeatBoxDlg.h"
#include "common.h"

// CGroupDlg 对话框

IMPLEMENT_DYNAMIC(CGroupDlg, CDialog)

CGroupDlg::CGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupDlg::IDD, pParent)
{

}

CGroupDlg::~CGroupDlg()
{
}

void CGroupDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GROUP_NR, m_ComboBoxNr);
	DDX_Control(pDX, IDC_LIST_NO_GROUP, m_ListNoGroup);
	DDX_Control(pDX, IDC_LIST_GROUP, m_ListGroup);

}


BEGIN_MESSAGE_MAP(CGroupDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGroupDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST_NO_GROUP, &CGroupDlg::OnLbnDblclkListNoGroup)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP_NR, &CGroupDlg::OnCbnSelchangeComboGroupNr)
	ON_LBN_DBLCLK(IDC_LIST_GROUP, &CGroupDlg::OnLbnDblclkListGroup)
END_MESSAGE_MAP()


// CGroupDlg 消息处理程序

void CGroupDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
 
	for(int i = 0; i < ::G_iMaxLedNr[m_BoxNr] ; i++)
	{
		G_NormalConfigPara[this->m_BoxNr].Group[i] = m_Group[i];
	}
	::SaveNormalConfigPara(); 

	//OnOK();
}


BOOL CGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	this->m_ComboBoxNr.AddString(_T("01 组"));  
	this->m_ComboBoxNr.AddString(_T("02 组")); 
	this->m_ComboBoxNr.AddString(_T("03 组")); 
	this->m_ComboBoxNr.AddString(_T("04 组")); 
	this->m_ComboBoxNr.AddString(_T("05 组")); 
	this->m_ComboBoxNr.AddString(_T("06 组")); 
	this->m_ComboBoxNr.AddString(_T("07 组")); 
	this->m_ComboBoxNr.AddString(_T("08 组")); 
	this->m_ComboBoxNr.AddString(_T("09 组")); 
	this->m_ComboBoxNr.AddString(_T("10 组")); 



	for(int i = 0; i < ::G_iMaxLedNr[m_BoxNr] ; i++)
	{
		
		if(::G_NormalConfigPara[this->m_BoxNr].Group[i] <= 0 ||  ::G_NormalConfigPara[this->m_BoxNr].Group[i] > 10)
		{
			CString s;
			s.Format(_T("%03d"), i+1); 
			this->m_ListNoGroup.AddString(s);  
			m_Group[i] = 0;
		}
		else 
		{
			m_Group[i] =  G_NormalConfigPara[this->m_BoxNr].Group[i];
		}
	}
	
	return true;
}

void CGroupDlg::OnLbnDblclkListNoGroup()
{
	// TODO: 在此添加控件通知处理程序代码

	if(this->m_ComboBoxNr.GetCurSel() >= 0)
	{
		int cur = this->m_ListNoGroup.GetCurSel();
		if(cur >= 0)
		{
			TCHAR buf[100];
			TCHAR *pstr = NULL;
			m_ListNoGroup.GetText(cur,buf);  
			
			int i = _tcstol(buf,&pstr,10);

			m_Group[i-1] = m_ComboBoxNr.GetCurSel() + 1;
			ShowGroup();
		}
	}
	else
	{
	}
}

void CGroupDlg::ShowGroup(void)
{

	m_ListNoGroup.ResetContent();
	m_ListGroup.ResetContent();

	for(int k = 0; k < ::G_iMaxLedNr[m_BoxNr] ; k++)
	{
		
		if(m_Group[k] <= 0 ||  m_Group[k] > 10)
		{
			CString s;
			s.Format(_T("%03d"), k+1); 
			this->m_ListNoGroup.AddString(s);  
	
		}
		else if(this->m_ComboBoxNr.GetCurSel() >= 0)
		{
			if(this->m_ComboBoxNr.GetCurSel() == (m_Group[k] - 1))
			{
				CString s;
				s.Format(_T("%03d"), k+1); 
				this->m_ListGroup.AddString(s);
			}
		}
	}
}

void CGroupDlg::OnCbnSelchangeComboGroupNr()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowGroup();
}

void CGroupDlg::OnLbnDblclkListGroup()
{
	// TODO: 在此添加控件通知处理程序代码


	if(this->m_ComboBoxNr.GetCurSel() >= 0)
	{
		int cur = this->m_ListGroup.GetCurSel();
		if(cur >= 0)
		{
			TCHAR buf[100];
			TCHAR *pstr = NULL;
			m_ListGroup.GetText(cur,buf);  
			
			int i = _tcstol(buf,&pstr,10);

			m_Group[i-1] = 0;
			ShowGroup();
		}
	}
	else
	{
	}

}
