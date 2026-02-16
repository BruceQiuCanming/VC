#if !defined(AFX_DEBUGDLG_H__6389673A_F1BF_433A_94B8_93402C2406C9__INCLUDED_)
#define AFX_DEBUGDLG_H__6389673A_F1BF_433A_94B8_93402C2406C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg dialog
#include "AdjustWizardDlg.h"
#include "afxwin.h"
#include "..\\..\\public_c\\typedefs.h"



class CDebugDlg : public CDialog
{
// Construction
public:
	float	m_CurNTC[64];
	int		m_StartPid;
	bool	m_OnTiming;
	bool    m_bSetFocusOnPara4;
	double	m_PidTemp;
	CTempTimeArray m_PidTempArray;
	//bool	IsAllPIDZero(void);
	bool	IsAdjustAllZero(void);

	BOOL	m_TimerBusy;
	int		m_NormalPidTime ;
	int		m_SetPidTempLow ,m_SetPidTempHigh;
	double	m_MeterPidTempLow[4],m_MeterPidTempHigh[4];
	double	m_GlassPidTempLow[4],m_GlassPidTempHigh[4];
	int		m_SendCmdCount;
	int		m_ReceiveCmdCount;
	int		m_TimerCount;


	float		 m_InputGlassLow[4];
	float		 m_InputGlassHigh[4];
	double		 m_MeterLow[4];
	double		 m_MeterHigh[4];
	float		 m_ReadAdjustLow[32];
	float		 m_ReadAdjustHigh[32];
	float		 m_WriteAdjustLow[32];
	float		 m_WriteAdjustHigh[32];
	int			 m_PidBeginTime;
	double       m_CurTemp[4];
	double		 m_AdjustLow[4];
	double		 m_AdjustHigh[4];
	double		 m_LowAutoAdjustTemp;
	double		 m_HighAutoAdjustTemp;
	bool		 m_JiaJuTestedPass[3];
	CRect		 m_LED_Rect[192];
	int m_BoxNr;
	CDebugDlg(CWnd* pParent = NULL);   // standard constructor
	void OnButtonAll(int index);
	void DealCmdAnswer(int cmd_addr,int data_len, unsigned char * para, unsigned char * flash,bool IsCmdAscii);
	bool CDebugDlg::CheckTempStable(void);
	void InputGlassLowTemp(void);

	void CalAdjustTemp(void);
	//bool CDebugDlg::CheckJiaJuPass(unsigned char ID);
// Dialog Data
	//{{AFX_DATA(CDebugDlg)
	enum { IDD = IDD_DIALOG_DEBUG };
	BOOL	m_bHighVolt;
	BOOL	m_bCheckRelay;
	BOOL	m_bCheckHighVolt;
	BOOL	m_bReset;
	CString	m_CalMsg;
	//}}AFX_DATA

	BOOL PreTranslateMessage(MSG *pMsg);

	afx_msg void OnRadioLedStatus();
	afx_msg void OnRadioLedAllOn();
	afx_msg void OnRadioLedAllOff();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugDlg)
	public:
	afx_msg void OnButton_PID();
	afx_msg void OnButton_STOP_OUT();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugDlg)
	
	afx_msg void OnButton_ReadAdjustLow();
	afx_msg void OnButton_ReadAdjustHigh();

	afx_msg void OnButton13();
	afx_msg void OnButton17();
	afx_msg void OnButton_Cal_Adjust();
	afx_msg void OnButton18();
	afx_msg void OnButton2();
	
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButtonSetPID();
	afx_msg void OnButton_SetAdjustLow();
	afx_msg void OnButton_SetAdjustHigh();
	afx_msg void OnButtonBoardAdjust();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnButton20();
	afx_msg void OnButtonSynchronizationBegin();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonReadSynchronization();
	afx_msg void OnButtonRecordView();
	afx_msg void OnButtonCopyright();
	afx_msg void OnButtonOpenDoor();
	afx_msg void OnButtonHighVolt();
	
	afx_msg void OnButtonAdjustWizard();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSetAdjustAll();
	afx_msg void OnButtonReadAdjustAll();
	afx_msg void OnCheckReset();
	afx_msg void OnButtonCalTemp();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckRelay();
	afx_msg void OnBnClickedCheckHighVolt();

	void DrawLED(void);
	void DrawLED_JiaJu(void);

	afx_msg void OnEnSetfocusEditPara4();
	afx_msg void OnEnSetfocusEditPara9();

	afx_msg void OnEnSetfocusEditPara25();
	afx_msg void OnEnSetfocusEditPara26();
	afx_msg void OnEnSetfocusEditPara27();
	afx_msg void OnEnSetfocusEditPara28();
	void CDebugDlg::OnEnSetfocusEditPara(UINT id);
	afx_msg void OnEnSetfocusEditSetAdjustAll();
	afx_msg void OnBnClickedCheckCoolRelay();
	BOOL m_bCheckCoolRelay;
	afx_msg void OnBnClickedButton14();
	CString G_CommRecString;
	CString G_CommSendString;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonSetAddr();
	afx_msg void OnBnClickedButtonUpdateFirmware();
	afx_msg void OnBnClickedButtonPrinterTest();
	afx_msg void OnBnClickedButtonAt();
	afx_msg void OnBnClickedCheckDeltaFrequnce();
	BOOL m_bDeltaFrequnce;
	afx_msg void OnEnSetfocusEditSetPt100Count();
	afx_msg void OnBnClickedButtonSetAPT100Count();

	afx_msg void OnBnClickedCheckJiajuTest();
	BOOL m_bJiajuTesting;
	CComboBox m_ComboInterfaceBoardType;
	afx_msg void OnCbnSelendokComboInterfaceBoardType();
	afx_msg void OnBnClickedButtonReadBimetallicStrip();
	afx_msg void OnEnChangeEditPara9();
	int m_LedType;
	CString m_CalText;
	CComboBox m_ComboTM7705_OSC;
	afx_msg void OnCbnSelchangeComboTm7705Osc();
	afx_msg void OnCbnSelendokComboTm7705Osc();
	afx_msg void OnBnClickedButtonUpdateBootloader();
	afx_msg void OnBnClickedCheck192InterfacePcbType();
	BOOL m_bCheck_192_Interface_PcbType;
	afx_msg void OnBnClickedCheckBeep();
	void CDebugDlg::ShowPCBType(void);
	BOOL m_bCheckBeep;
	afx_msg void OnBnClickedCheckFan();
	BOOL m_bCheckFan;
	afx_msg void OnBnClickedButtonClearSound();
	afx_msg void OnStnClickedStaticTest();
	afx_msg void OnEnSetfocusEditPara13();
	afx_msg void OnEnSetfocusEditPara14();
	afx_msg void OnBnClickedButtonCpuId();
	afx_msg void OnCbnSelchangeComboInterfaceBoardType();
	afx_msg void OnBnClickedButtonNtcRelayLed();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGDLG_H__6389673A_F1BF_433A_94B8_93402C2406C9__INCLUDED_)
