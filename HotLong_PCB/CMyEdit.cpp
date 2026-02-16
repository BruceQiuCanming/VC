#include "stdafx.h"
#include "CMyEdit.h"

//#include "afxdialogex.h"



CMyEdit::CMyEdit()
{

}
CMyEdit::~CMyEdit()
{

}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP 
END_MESSAGE_MAP()

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlcolor)
{
	// TODO: Change any attributes of the DC here
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	//set text color
	pDC->SetTextColor(m_crText);
	//set the text's background color 
	pDC->SetBkColor(m_crBackGnd);
	//return the brush used for background this sets control background 
	return m_brBackGnd;
}

void CMyEdit::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background) 
	m_crBackGnd =rgb;
	//free brush
	if (m_brBackGnd.GetSafeHandle())
		m_brBackGnd.DeleteObject(); //set brush to new color
	m_brBackGnd.CreateSolidBrush(rgb);
	//redraw
	Invalidate(TRUE);
}

void CMyEdit::SetTextColor(COLORREF rgb)
{
	//set text color ref 
	m_crText = rgb;
	//redraw
	Invalidate(TRUE);
}