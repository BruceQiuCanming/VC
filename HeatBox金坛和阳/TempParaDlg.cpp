// TempParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "TempParaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTempParaDlg dialog


CTempParaDlg::CTempParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTempParaDlg::IDD, pParent)
	, m_ParaName(_T(""))
{
	//{{AFX_DATA_INIT(CTempParaDlg)
	m_MainLevelStr = _T("");
	m_LowLevelStr = _T("");
	m_HighLevelStr = _T("");
	//}}AFX_DATA_INIT
}


void CTempParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTempParaDlg)
	DDX_Text(pDX, IDC_STATIC_MAIN_LEVEL, m_MainLevelStr);
	DDX_Text(pDX, IDC_STATIC_LOW_LEVEL, m_LowLevelStr);
	DDX_Text(pDX, IDC_STATIC_HIGH_LEVEL, m_HighLevelStr);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_PARA_NAME, m_ParaName);
}


BEGIN_MESSAGE_MAP(CTempParaDlg, CDialog)
	//{{AFX_MSG_MAP(CTempParaDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTempParaDlg message handlers

HBRUSH CTempParaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
		switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		hbr = G_bkBrush;
		break;

	case CTLCOLOR_BTN:	//    Button control
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		break;
	case CTLCOLOR_EDIT:	//    Edit control
		//hbr = m_bkBrush;
		 
		break;
	case CTLCOLOR_LISTBOX:	//    List-box control
		break;
	case CTLCOLOR_MSGBOX:	//    Message box
		break;
	case CTLCOLOR_SCROLLBAR:	//    Scroll-bar control
		break;
	case CTLCOLOR_STATIC:	//    Static control
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		pDC->SetTextColor(RGB(255, 255, 60));
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
