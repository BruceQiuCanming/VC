#if !defined(AFX_TEMPPARADLG_H__1AEC1F1C_0F0B_482D_A5BF_72090548CC64__INCLUDED_)
#define AFX_TEMPPARADLG_H__1AEC1F1C_0F0B_482D_A5BF_72090548CC64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TempParaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTempParaDlg dialog

class CTempParaDlg : public CDialog
{
// Construction
public:
	CTempParaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTempParaDlg)
	enum { IDD = IDD_DIALOG_TEMP_PARA };
	CString	m_MainLevelStr;
	CString	m_LowLevelStr;
	CString	m_HighLevelStr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTempParaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTempParaDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_ParaName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPPARADLG_H__1AEC1F1C_0F0B_482D_A5BF_72090548CC64__INCLUDED_)
