#if !defined(AFX_TAKEOUTPAGE_H__6E2C917A_0915_4F61_B38F_E172A4F99A02__INCLUDED_)
#define AFX_TAKEOUTPAGE_H__6E2C917A_0915_4F61_B38F_E172A4F99A02__INCLUDED_

#include "TakeDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TakeoutPage.h : header file
//

#include "ColorListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTakeoutPage dialog

class CTakeoutPage : public CDialog
{
// Construction
public:
	HEAT_COOL_ORDER		m_HeatCoolType;
	CONTROL_TEMP_RANGE	m_TempRange;
	SWITCH_TEST_RESULT	m_SwitchTestResult_Ex;
	BOX_TYPE			m_BoxType;


	CString GetSwitchNrString(int i);
	CTakeoutDlg G_TakeoutDlg;
	void ShowTestResult(void);
	void ClearAllSelected(void);
	void CalTestResultLevel(SWITCH_TEST_RESULT_AND_LEVEL * source);
	void SortTestResult(void);
	void DrawColorList(void);
	void DrawColorListItem(CDC MemDC, int switchNr, float openTemp, float closeTemp,TEST_RESULT_LEVEL level,bool bDraw);
	CTakeoutPage(CWnd* pParent = NULL);   // standard constructor
	BOX_TYPE m_HeatBoxType;
	int m_BoxNr;
	SWITCH_CONFIG_PARA_ALL m_SwitchConfigPara;
	SWITCH_TEST_RESULT m_SwitchTestResult[192];
	SWITCH_TEST_RESULT_AND_LEVEL m_SwitchTestResult_Sorted[192];

// Dialog Data
	//{{AFX_DATA(CTakeoutPage)
	enum { IDD = IDD_DIALOG_TAKEOUT_PAGE };
	CColorListCtrl	m_ListTestResult;
	CScrollBar	m_ScrollBar_Switch_ID;
	int		m_iSelect;
	BOOL	m_bMainLevel;
	BOOL	m_bLowLevel;
	BOOL	m_bHighLevel;
	BOOL	m_bCloseHighLevel;
	BOOL	m_bCloseLowLevel;
	BOOL	m_bOverHighLevel;
	BOOL	m_bOverLowLevel;
	BOOL	m_bNotUsed;
	BOOL	m_bOpenFlashLevel;
	BOOL	m_bCloseFlashLevel;
	UINT	m_iCloseHighLevel;
	UINT	m_iCloseLowLevel;
	UINT	m_iHighLevel;
	UINT	m_iLowLevel;
	UINT	m_iMainLevel;
	UINT	m_iNotUsed;
	UINT	m_iOpenFlashLevel;
	UINT	m_iOverHighLevel;
	UINT	m_iOverLowLevel;
	BOOL	m_bOpenDoorResetLevel;
	UINT	m_iOpenDoorResetLevel;
	UINT	m_iCloseFlashLevel;
	BOOL	m_bNoLevel;
	UINT	m_iNoLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTakeoutPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTakeoutPage)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckOverLowLevel();
	afx_msg void OnCheckOverHighLevel();
	afx_msg void OnCheckLowLevel();
	afx_msg void OnCheckHighLevel();
	afx_msg void OnCheckMainLevel();
	afx_msg void OnCheckCloseHighLevel();
	afx_msg void OnCheckCloseLowLevel();
	afx_msg void OnCheckOpenFlash();
	afx_msg void OnCheckOpenDoorReset();
	afx_msg void OnCheckNoAction();
	afx_msg void OnRadioOnlyOne();
	afx_msg void OnButtonRecordView2();
	afx_msg void OnButtonTakeoutDlg();
	afx_msg void OnCheckNoLevel();
	afx_msg void OnCheckCloseFlash();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bHighHighLevel;
	UINT m_iHighHighLevel;
	afx_msg void OnBnClickedCheckHighHighLevel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAKEOUTPAGE_H__6E2C917A_0915_4F61_B38F_E172A4F99A02__INCLUDED_)
