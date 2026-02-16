// TempAdjustDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "TempAdjustDlg.h"
#include "InputDlg.h"

// CTempAdjustDlg 对话框

IMPLEMENT_DYNAMIC(CTempAdjustDlg, CDialog)

CTempAdjustDlg::CTempAdjustDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTempAdjustDlg::IDD, pParent)
	, m_TempAdjustType(0)
{
	m_bSetFocusOnPara4 = true;
	m_TempAdjustType   = 0;
}

CTempAdjustDlg::~CTempAdjustDlg()
{
}

void CTempAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTempAdjustDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTempAdjustDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(EN_SETFOCUS,10001,10200,OnEditClicked)
	ON_BN_CLICKED(IDC_RADIO1, &CTempAdjustDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTempAdjustDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CTempAdjustDlg 消息处理程序
void CTempAdjustDlg::OnEditClicked(UINT id)
{

	if(m_bSetFocusOnPara4 == false)
	{
		return;
	}
	m_bSetFocusOnPara4 = false;

	CWnd *w = this->GetDlgItem(IDOK); 
	
	CInputDlg dlg;

	dlg.m_Title.Format(_T("%d: = %5.1f"),id - 10000,this->m_AdjustTemp[id - 10001][m_TempAdjustType]);

	if(dlg.DoModal() == IDOK)
	{
		this->m_AdjustTemp[id - 10001][m_TempAdjustType] = _tcstod(dlg.m_Input,NULL);//atof(buf);

		CWnd * edit = this->GetDlgItem(id);
		
		CString s;
		s.Format(_T("%d#= %5.1f"),id - 10000 ,m_AdjustTemp[id - 10001][m_TempAdjustType]);  
		
		edit->SetWindowTextW(s);
		
	}
	

	w->SetFocus(); 

	m_bSetFocusOnPara4 = true;

}

BOOL CTempAdjustDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->ShowWindow( SW_SHOWMAXIMIZED);

	RECT wndRect;
	this->GetWindowRect(&wndRect); 



	for(int i = 0; i < G_iLedLines; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine; j++)
		{
			RECT rect;
			rect.left  = j * (wndRect.right / G_iLedNrPerLine);
			rect.right = rect.left + (wndRect.right / G_iLedNrPerLine) *0.9;
			rect.top   = (i + 2)* (wndRect.bottom / (G_iLedLines + 2));
			rect.bottom = rect.top + (wndRect.bottom / (G_iLedLines + 2)) * 0.9;

			m_Edit[i*G_iLedNrPerLine + j].Create(0x50010080,rect,this,10001 + i*G_iLedNrPerLine + j); 
			
			CString s;
			s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine + j +1 ,this->m_AdjustTemp[i*G_iLedNrPerLine + j][0]);  
			m_Edit[i*G_iLedNrPerLine + j].SetWindowTextW(s);

		}
	}
	
	CButton* b = (CButton*) GetDlgItem(IDC_RADIO1);
	b->SetCheck(1);

	return true;
}
void CTempAdjustDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CTempAdjustDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码

	m_TempAdjustType = 0;

	for(int i = 0; i < G_iLedLines; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine; j++)
		{
			CString s;
			s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine + j +1 ,this->m_AdjustTemp[i*G_iLedNrPerLine + j][0]);  
			m_Edit[i*G_iLedNrPerLine + j].SetWindowTextW(s);
		}
	} 

}

void CTempAdjustDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码

	m_TempAdjustType = 1;

	for(int i = 0; i < G_iLedLines; i++)
	{
		for(int j = 0; j < G_iLedNrPerLine; j++)
		{
			CString s;
			s.Format(_T("%d#= %5.1f"),i*G_iLedNrPerLine + j +1 ,this->m_AdjustTemp[i*G_iLedNrPerLine + j][1]);  
			m_Edit[i*G_iLedNrPerLine + j].SetWindowTextW(s);
		}
	} 

}
