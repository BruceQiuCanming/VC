#if !defined(AFX_INPUTDLG_H__E6F16A36_B056_43E0_A82F_0B07D01FA7B4__INCLUDED_)
#define AFX_INPUTDLG_H__E6F16A36_B056_43E0_A82F_0B07D01FA7B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : header file
//

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog
#ifdef WINCE
#include "stdafx.h"
#else

#endif


class CInputDlg : public CDialog
{
// Construction
public:
	CString m_Title;
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDlg)
	enum { IDD = IDD_INPUT_DIALOG };
	CString	m_Input;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDlg)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton0();
	afx_msg void OnButtonDot();
	afx_msg void OnButtonClear();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonPlusMinus();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__E6F16A36_B056_43E0_A82F_0B07D01FA7B4__INCLUDED_)
