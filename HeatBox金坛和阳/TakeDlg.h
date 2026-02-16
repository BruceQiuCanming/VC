#if !defined(AFX_TAKEDLG_H__DBDB3852_5059_4CD3_9E82_A403D1021407__INCLUDED_)
#define AFX_TAKEDLG_H__DBDB3852_5059_4CD3_9E82_A403D1021407__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TakeDlg.h : header file
//
#include "ProductTempBar.h"

#define PIC_GAP 0


/////////////////////////////////////////////////////////////////////////////
// CTakeoutDlg dialog
extern COLORREF level_color[LOW_LEVEL_CLOSE_LOW_LEVEL - NO_LEVEL+1];
extern CBrush color_brush_0;
extern CBrush color_brush_1;
extern CBrush color_brush_2;
extern CBrush color_brush_3;
extern CBrush color_brush_4;
extern CBrush color_brush_5;
extern CBrush color_brush_6;
extern CBrush color_brush_7;
extern CBrush color_brush_8;
extern CBrush color_brush_9;
extern CBrush color_brush_10;

extern SWITCH_TEST_RESULT G_TakeDlgSwitchTestResult[2][500];
extern SWITCH_TEST_RESULT G_TakeDlgSwitchTestResult_Bak[2][500];

extern CBrush* level_brush[LOW_LEVEL_CLOSE_LOW_LEVEL-NO_LEVEL+1];

extern TEST_RESULT_LEVEL CheckTestLevel(SWITCH_CONFIG_PARA SwitchConfigPara,
			SWITCH_TEST_RESULT SwitchTestResult,HEAT_BOX_TYPE HeatBoxType);


class CTakeoutDlg : public CDialog
{
// Construction
public:
	int m_iTakeBoardType; //0 在线 ，1历史记录
	int	m_iDisplayTempType;
	int m_SwitchNormalOpen;
	bool m_IsVK;
	int m_FlashGap;
	CString m_WindowText;
	CString m_ParaName;
	int m_BoxNr;
	CTime	m_EndTestTime;
	CTime	m_BeginTestTime;
	//int		m_TestProductType;
	SWITCH_CONFIG_PARA m_SwitchConfigPara;

	void ShowTestResult_HeYang(bool bDraw,bool level_text = false);
	void ShowTestResult(bool bDraw,bool level_text = false);
	CTakeoutDlg(CWnd* pParent = NULL);   // standard constructor
	HEAT_BOX_TYPE	m_HeatBoxType;
// Dialog Data
	//{{AFX_DATA(CTakeoutDlg)
	enum { IDD = IDD_DIALOG_TAKEOUT };
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
	BOOL	m_bNoPTCLevel;
	UINT	m_iCloseFlashLevel;
	UINT	m_iCloseHighLevel;
	UINT	m_iCloseLowLevel;
	UINT	m_iHighLevel;
	UINT	m_iLowLevel;
	UINT	m_iMainLevel;
	UINT	m_iNotUsed;
	UINT	m_iOpenFlashLevel;
	UINT	m_iOverHighLevel;
	UINT	m_iOverLowLevel;
	UINT	m_iNoPTCLevel;
	CString	m_Msg;
	int		m_iSelect;
	BOOL	m_bOpenDoorResetLevel;
	UINT	m_iOpenDoorResetLevel;
	BOOL	m_bNoLevel;
	UINT	m_iNoLevel;
	BOOL	m_bHighHighLevel;
	UINT	m_iHighHighLevel;
	BOOL	m_bHighLevelCloseHighLevel;
	BOOL	m_bHighLevelCloseLowLevel;
	BOOL	m_bLowLevelCloseHighLevel;
	BOOL	m_bLowLevelCloseLowLevel;
	UINT	m_iHighLevelCloseHighLevel;
	UINT	m_iHighLevelCloseLowLevel;
	UINT	m_iLowLevelCloseHighLevel;
	int		m_iLowLevelCloseLowLevel;
	//}}AFX_DATA

	void ClearAllSelected(void);
	void SetAllSelected(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTakeoutDlg)
	public:
	virtual int DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTakeoutDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCheckMainLevel();
	afx_msg void OnCheckOverHighLevel();
	afx_msg void OnCheckCloseHighLevel();
	afx_msg void OnCheckCloseLowLevel();
	afx_msg void OnCheckHighLevel();
	afx_msg void OnCheckLowLevel();
	afx_msg void OnCheckOverLowLevel();
	afx_msg void OnCheckOpenFlash();
	afx_msg void OnCheckNoAction();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckOpenDoorReset();
	afx_msg void OnRadioOnlyOne();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonRecordView2();
	afx_msg void OnCheckCloseFlash();
	afx_msg void OnRadioAllSelect();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCheckNoLevel();
	afx_msg void OnButton2();
	afx_msg void OnCheckHighHighLevel();
	afx_msg void OnCheckHighLevelCloseHighLevel();
	afx_msg void OnCheckHighLevelCloseLowLevel();
	afx_msg void OnCheckLowLevelCloseHighLevel();
	afx_msg void OnCheckLowLevelCloseLowLevel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawOneSwitch(int switcNr,float openTemp,float closeTemp,TEST_RESULT_LEVEL level, bool bDraw, bool level_text=false);
public:
	CProductTempBar m_ProductTempBarDlg;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckNoPtcLevel();
	afx_msg void OnBnClickedButtonTempView();
	afx_msg void OnBnClickedButtonSaveExcel();
	afx_msg void OnBnClickedButtonPreview();
	afx_msg void OnBnClickedButtonTodayChanliang();
	afx_msg void OnBnClickedButtonPrint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAKEDLG_H__DBDB3852_5059_4CD3_9E82_A403D1021407__INCLUDED_)
