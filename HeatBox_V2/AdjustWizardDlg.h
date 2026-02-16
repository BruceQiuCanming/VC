#if !defined(AFX_ADJUSTWIZARDDLG_H__6B36DB60_10E2_4DBF_93BA_27AD1A5CEC5C__INCLUDED_)
#define AFX_ADJUSTWIZARDDLG_H__6B36DB60_10E2_4DBF_93BA_27AD1A5CEC5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjustWizardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjustWizardDlg dialog

class CAdjustWizardDlg : public CDialog
{
// Construction
public:
	CAdjustWizardDlg(CWnd* pParent = NULL);   // standard constructor
	float	m_MeterTemp[4];
	float	m_GlassTemp[4];
	float   m_AdjustHigh[4];
	float   m_AdjustLow[4];
	CEdit	m_GlassTempEdit[4];
	
// Dialog Data
	//{{AFX_DATA(CAdjustWizardDlg)
	enum { IDD = IDD_DIALOG_ADJUST_WIZARD_1 };
	CString	m_Msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjustWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjustWizardDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJUSTWIZARDDLG_H__6B36DB60_10E2_4DBF_93BA_27AD1A5CEC5C__INCLUDED_)
