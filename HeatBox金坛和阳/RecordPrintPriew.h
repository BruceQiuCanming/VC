#if !defined(AFX_PRNPREVIEW_H__63320212_F46F_4695_AF42_4EEB4469C18E__INCLUDED_)
#define AFX_PRNPREVIEW_H__63320212_F46F_4695_AF42_4EEB4469C18E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrnPreview.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrnPreview dialog

class CPrnPreview : public CDialog
{
// Construction
public:
	HEAT_BOX_TYPE m_HeatBoxType;
	void DisplayPrnView(void);
	CString m_TestParaName;
	CString m_TestProductType;
	CString m_TestMemo;
	CString m_Title;
	BOOL  m_bTestCloseLevel;
	CTime m_EndTestTime;
	CTime m_BeginTestTime;
	int   m_PageNr;
	int m_BoxNr;
	CString m_WindowText;
	SWITCH_CONFIG_PARA m_SwitchConfigPara;
	SWITCH_TEST_RESULT_EX m_SwitchTestResult_Ex[500];

	CPrnPreview(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrnPreview)
	enum { IDD = IDD_DIALOG_PRN_PREVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrnPreview)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void OnButtonNext();
	// Generated message map functions
	//{{AFX_MSG(CPrnPreview)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRNPREVIEW_H__63320212_F46F_4695_AF42_4EEB4469C18E__INCLUDED_)
