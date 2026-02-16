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
	CRect		m_LED_Rect[192];


//	
	MODBUS_RS485_TEMP_ANSWER_PARA m_TempPara;
	float		m_Speed;
	
	bool		m_IsTaking;

	CTime		m_LedAnswerTime[2];
	CTime		m_MeterAnswerTime;
	
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

#ifndef _WORK_MODE_H_

	CString					m_Printer;

	int		m_OpenDoor_SwitchClosed[3];
	CTime	m_OpenDoor_SwitchClosed_StartTime[3];

	float	m_OpenDoor_Temp;
	CTime	m_OpenDoor_StartTime;
	BOOL	m_OpenDoorIng;

	CTime	m_EndTestTime;
	CTime	m_BeginTestTime;

	float	m_CloseDoorTopTemp;
	float	m_CloseDoorStartTemp;
	CTime	m_CloseDoorStartTime;
		
	double GetControlGoalTemp(SUB_MODE /*AUTO_TEST_STEP*/ step);
	int		GetHighestCloseTemp();
	double		GetLowestCloseTemp(void);

	void PrintTestResult(void) ;
	BOOL OpennedAndNotFlashTested();
	void OpenDoor_For_OpenFlashTest(void);
	void OpenDoor_For_CloseFlashTest(void);
	float m_LastClosePIDTemp;
	CTime m_ClosePIDStartTime;
	CTime m_TopPIDStartTime;

	bool	IsAllClosed(void);
	bool	IsAllOpened(void);

	CTime   m_CurSubModeBeginTime;

	void	SendReadOpenFlash(void);
	void	SendReadCloseFlash(void);
	void	SendStartOpenFlashTest(void);
	void	SendStartCloseFlashTest(void);


	bool CSwitchDlg::IsPT100Caled(void);
	void ExitAutoTest(void);
	void EnterAutoTest(void);

	bool m_StopTestType;

	void AutoTestStep_DirectHeat(void);
	void AuotTestStep_NormalPID(void);
	void AutoTestStep_StepPID(void);
	void AutoTestStep_TopPID(void);
	void AuotTestStep_NormalPID_2(void);
	void AutoTestStep_StepPID_2(void);
	void AutoTestStep_DircetCool(void);
	void AutoTestStep_HeatAgain(void);
	void AutoTestStep_StepCool(void);
#endif

	
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWITCHDLG_H__C320EC1D_9F5E_433C_B948_FD285D7B5F8C__INCLUDED_)
