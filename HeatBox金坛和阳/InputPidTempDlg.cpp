// InputPidTempDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "InputPidTempDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputPidTempDlg dialog


CInputPidTempDlg::CInputPidTempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputPidTempDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputPidTempDlg)
	m_PidTemp = 0;
	//}}AFX_DATA_INIT
}


void CInputPidTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPidTempDlg)
	DDX_Text(pDX, IDC_EDIT_PID_TEMP, m_PidTemp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPidTempDlg, CDialog)
	//{{AFX_MSG_MAP(CInputPidTempDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPidTempDlg message handlers

void CInputPidTempDlg::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(true);
	CDialog::OnOK();
}

void CInputPidTempDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

int CInputPidTempDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetWindowText(m_WndText);
	return 0;
}
