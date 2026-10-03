#if !defined(AFX_SWITCHDLG_H__C320EC1D_9F5E_433C_B948_FD285D7B5F8C__INCLUDED_)
#define AFX_SWITCHDLG_H__C320EC1D_9F5E_433C_B948_FD285D7B5F8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwitchDlg.h : header file
//

#include "takedlg.h"
#include "..\\..\\public_c\\typedefs.h"
#include "..\\..\\public_c\\TempMeter.h"
#include "..\\..\\public_c\\WorkMode.h"

/////////////////////////////////////////////////////////////////////////////
// CSwitchDlg dialog



CTime  GetRecordTime(long seconds);

extern int	G_iCmdAscii[MAX_HEAT_BOX];
extern bool G_IsJustCmdAscii;
extern PID		G_PID[MAX_HEAT_BOX];
extern CNtcTempRecordArray	G_NtcTempRecordArray[MAX_HEAT_BOX];


class CSwitchDlg : public CDialog
{

public:

	CWorkMode	m_WorkMode;
//	
	CTempMeter	m_Meter;
	

	int			m_iTestMode;
	void		SendDirectHeatCmd(float temp);



//常达
//升1度，降2度，再升3度
	
	CString		m_ParaName;
	CString		m_TestMemo;
	CString     m_Barcode;
	CRect		m_LED_Rect[192];


//	
	MODBUS_RS485_TEMP_ANSWER_PARA m_TempPara;
	float		m_Speed;
	
	bool		m_IsTaking;


	
	void	DrawBiMetalLED(void);

	
	

	
	int			m_BoxNr;

	


	SWITCH_CONFIG_PARA_ALL	m_SwitchConfigPara;
	float					m_CurTemp[4];

	void DrawLED(void);

	void DealCmdAnswer(unsigned char *para, unsigned char *flash/*[16+16]*/,bool IsCmdAscii,int data_len = 0);
	CSwitchDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CSwitchDlg)
	enum { IDD = IDD_DIALOG_SWITCH };

	CString		m_TestMsg;

	float		m_PidTemp;

	CString		m_TestTimeMsg;
	
	//}}AFX_DATA


	void CSwitchDlg::Send_Better_MES(RECORD *record);

	afx_msg void OnCheckStartTest(void);
	afx_msg void OnButtonTakeout(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwitchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSwitchDlg)
	BOOL PreTranslateMessage(MSG *pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	
	afx_msg void OnRadioClose();
	afx_msg void OnRadioOpen();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	
	void CSwitchDlg::SaveExcel(void);


	void CSwitchDlg::DrawLED_128_OLD_DISPLAY(void);

	void CSwitchDlg::SaveCurve(void);



	
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWITCHDLG_H__C320EC1D_9F5E_433C_B948_FD285D7B5F8C__INCLUDED_)
