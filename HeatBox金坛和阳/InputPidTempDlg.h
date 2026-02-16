#if !defined(AFX_INPUTPIDTEMPDLG_H__D7F0048A_DF9D_44E7_B570_EFC50BB55C69__INCLUDED_)
#define AFX_INPUTPIDTEMPDLG_H__D7F0048A_DF9D_44E7_B570_EFC50BB55C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPidTempDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputPidTempDlg dialog

class CInputPidTempDlg : public CDialog
{
// Construction
public:
	CString m_WndText;
	CInputPidTempDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputPidTempDlg)
	enum { IDD = IDD_DIALOG_INPUT_PID_TEMP };
	UINT	m_PidTemp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPidTempDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputPidTempDlg)
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPIDTEMPDLG_H__D7F0048A_DF9D_44E7_B570_EFC50BB55C69__INCLUDED_)
