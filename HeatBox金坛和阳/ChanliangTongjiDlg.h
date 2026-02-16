#if !defined(AFX_CHANLIANGTONGJIDLG_H__2CD10C4A_C2C4_4848_A1F6_5468A3F801CA__INCLUDED_)
#define AFX_CHANLIANGTONGJIDLG_H__2CD10C4A_C2C4_4848_A1F6_5468A3F801CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChanliangTongjiDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChanliangTongjiDlg dialog

class CChanliangTongjiDlg : public CDialog
{
// Construction
public:
	void FindRecord(void);
	CChanliangTongjiDlg(CWnd* pParent = NULL);   // standard constructor
	int m_BoxNr;
// Dialog Data
	//{{AFX_DATA(CChanliangTongjiDlg)
	enum { IDD = IDD_DIALOG_CHANLIANG_TONGJI };
	CMonthCalCtrl	m_EndDay;
	CMonthCalCtrl	m_BeginDay;
	int		m_iCloseFlashLevel;
	int		m_iCloseHighLevel;
	int		m_iCloseLowLevel;
	int		m_iHighLevel;
	int		m_iLowLevel;
	int		m_iMainLevel;
	int		m_iNotUsedLevel;
	int		m_iOpenDoorResetLevel;
	int		m_iOpenFlashLevel;
	int		m_iOverHighLevel;
	int		m_iOverLowLevel;
	int		m_iTotal;
	CTime	m_BeginTime;
	CTime	m_EndTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChanliangTongjiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChanliangTongjiDlg)
	afx_msg void OnButtonView();
	afx_msg void OnGetdaystateMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSaveCsv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANLIANGTONGJIDLG_H__2CD10C4A_C2C4_4848_A1F6_5468A3F801CA__INCLUDED_)
