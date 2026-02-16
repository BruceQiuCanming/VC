//{{AFX_INCLUDES()

//}}AFX_INCLUDES
#if !defined(AFX_PRODUCTTEMPBAR_H__160DD8E4_2633_4DB1_84AD_5E85AEE863A0__INCLUDED_)
#define AFX_PRODUCTTEMPBAR_H__160DD8E4_2633_4DB1_84AD_5E85AEE863A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "StdAfx.h"
#include <atltime.h>
#include "..\\..\\public_c\\typedefs.h"
#include "..\\..\\public_c\\bmp.h"
#ifdef WINCE
#include "resource.h"
#else
#include "resource.h"
#endif
// ProductTempBar.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CProductTempBar dialog

class CProductTempBar : public CDialog
{
// Construction
public:
	CProductTempBar(CWnd* pParent = NULL);   // standard constructor
	SWITCH_TEST_RESULT  m_SwitchTestResult[192];
	SWITCH_CONFIG_PARA	m_SwitchConfigPara;
	CTime				m_BeginTestTime;
	bool				m_bOpenTemp;
	BOX_TYPE			m_BoxType;
	int					m_BoxNr;
	BOOL				m_bFontDirection;
	afx_msg void OnRadioOpenTemp();
// Dialog Data
	//{{AFX_DATA(CProductTempBar)
	enum { IDD = IDD_DIALOG_PRODUCT_BAR };
//	CMSChart	m_Chart1;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProductTempBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProductTempBar)
	afx_msg void OnRadioCloseTemp();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonFontDirection();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODUCTTEMPBAR_H__160DD8E4_2633_4DB1_84AD_5E85AEE863A0__INCLUDED_)
