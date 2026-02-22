// HeatBoxDlg.h : header file
//

#if !defined(AFX_HEATBOXDLG_H__EEAF7D94_73E3_46C1_8CAE_B342D8D48DD0__INCLUDED_)
#define AFX_HEATBOXDLG_H__EEAF7D94_73E3_46C1_8CAE_B342D8D48DD0__INCLUDED_

#include "stdafx.h"

#include "comm.h"
#include "SwitchDlg.h"	// Added by ClassView
#include "CurveDlg.h"	// Added by ClassView
#include "ParaDlg.h"
#include "DebugDlg.h"	// Added by ClassView
#include "TakeoutPage.h"
#include "TempParaDlg.h"
#include "LPM260_Printer.h"
#include "SorterPlcDlg.h"

#include <sapi.h>				//导入语音头文件
#pragma comment(lib,"sapi.lib") //导入语音头文件库
#ifndef _DEBUG
#include <sphelper.h>			//语音识别头文件
#endif
//#include <atlstr.h>				//要用到CString

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXDAO.H>
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CHeatBoxDlg dialog
extern	CStringArray    G_SpeechArray;

extern	CTakeoutDlg		G_TakeoutDlg;

const int WM_RECORD = WM_USER + 100;		//定义消息


class SelfTuningPID 
{
	public:
	//构造函数，初始化PID参数和学习率
		SelfTuningPID(float Kp=1.0, float Ki= 0.1, float Kd = 0.01, float learning_rate = 0.01);

		~SelfTuningPID();
	//更新PID控制器，计算控制量
	float update(float setpoint, float current_value, float dt);
	float clamp(float value,float min,float max);
    void SelfTuningPID::PIDCal(float CurTemp,float fGoalTemp);

	float getKp() const {return m_Kp;}
	float getKi() const {return m_Ki;}
	float getKd() const {return m_Kd;}

	float	m_Kp;
	float	m_Ki;
	float	m_Kd;
	float	m_learning_rate;
	float	m_prev_error;
	float	m_integral;
};


class CHeatBoxDlg : public CDialog
{
// Construction
public:
	CHeatBoxDlg(CWnd* pParent = NULL);	// standard constructor


	
	
	
	
// Dialog Data
	//{{AFX_DATA(CHeatBoxDlg)
	enum { IDD = IDD_HEATBOX_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatBoxDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CHeatBoxDlg)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab8(NMHDR* pNMHDR, LRESULT* pResult);
	
	void  MSSListen(void);
	BOOL PreTranslateMessage(MSG *pMsg);
	void ReadSwitchConfigPara(void);
	//void SaveSwitchConfigPara(void);
	void TempCmd(void);

	void SelectTab(int tabnr);
	LRESULT OnComm(WPARAM wParam,LPARAM lParam);
	int InitRecord(void);
	LRESULT CHeatBoxDlg::OnRecord(WPARAM wParam,LPARAM lParam);
	bool CheckSameTemp(int BoxNr);
	void InitDatabase(void);
	void DisplaySendCmd(unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr);
	void DisplayReceiveCmd(unsigned char cmd,unsigned char *cmd_buf,int len,int BoxNr);

	RECORD_THREAD_PROC_PARA m_RecordThreadProcPara;

	CLPM260_Printer 	m_LPM260_Printer;
	int				G_CommTimeOut[MAX_HEAT_BOX];
	unsigned int	m_SN[MAX_HEAT_BOX];
	int				m_MeterID;
	int				m_CurRs485Nr;

	unsigned char	m_SwitchStatus[MAX_HEAT_BOX][192 / 8]; //用于192点灯板显示

	
	//double			m_CurTemp[MAX_HEAT_BOX][4];
	CFloatArray		m_LastCurTemp[MAX_HEAT_BOX][4];

	CTabCtrl		m_Tab[10];	//属性页不可变

	CSorterPlcDlg	m_SorterPlcDlg;

	CCurveDlg 		m_CurveDlg[MAX_HEAT_BOX];
	CSwitchDlg 		m_SwitchDlg[MAX_HEAT_BOX];
	CParaDlg   		m_ParaDlg[MAX_HEAT_BOX];
	CDebugDlg		m_DebugDlg[MAX_HEAT_BOX];
	CTakeoutPage	m_TakeoutPage[MAX_HEAT_BOX];
	CTempParaDlg	m_TempParaDlg[MAX_HEAT_BOX];
	int				m_NoPcbFlashTest;
	int				m_NoFlashTest[MAX_HEAT_BOX];
	SWITCH_TEST_RESULT m_SwitchTestResult[MAX_HEAT_BOX][192];

	CString ExecuteHttpPost(LPCTSTR strUrl, CString strPostData);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CComboBox	G_CommList;
	CString	m_CompileTime;
	CString	m_Version;
	CString	m_strSoftTime;
	CString	m_strPcTime;
	DWORD	m_PcSN;
	DWORD	m_SoftSN;
	CString	m_PcTime2;
	CString	m_SoftTime2;
	//}}AFX_DATA

	CTime	m_PcTime;
	CTime	m_SoftTime;
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButtonSaveSn();
	afx_msg void OnButtonCrack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_CopyrightDay;
	CString m_EditModify;
	afx_msg void OnEnChangeEditUser();
	afx_msg void OnEnChangeEditDialogTiltle();
	CComboBox m_MaxLedNrList;
	afx_msg void OnCbnSelchangeComboSwitchNr();
	CComboBox m_HighHighLevelDeltaList;
	afx_msg void OnCbnSelchangeComboHighHighLevelDelta();
	afx_msg void OnBnClickedButtonLastVersion();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnChangeEditCommTimer();
	afx_msg void OnEnChangeEdit_MES_URL();
	UINT G_CommTimer;
	afx_msg void OnBnClickedCheckNoPcbFlash();
	BOOL m_bNoPcbFlashTest;
	afx_msg void OnBnClickedCheckNoFlashTest1();
	BOOL m_bNoFlashTest1;
	BOOL m_bNoFlashTest2;
	BOOL m_bNoFlashTest3;
	BOOL m_bNoFlashTest4;
	afx_msg void OnBnClickedCheckNoFlashTest2();
	afx_msg void OnBnClickedCheckNoFlashTest3();
	afx_msg void OnBnClickedCheckNoFlashTest4();
	CComboBox m_ComboSpeech;

	CString G_CommRec;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck_Barcode();
	afx_msg void OnBnClickedCheck_ONE_BY_ONE_TAKEBOARD();
	afx_msg void OnBnClickedCheck_IDC_CHECK_DEBUG_MODE();
	BOOL G_CommCheck;
	LRESULT OnComm(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonClearRec();
	afx_msg void OnBnClickedButtonFirmware();
	afx_msg void OnBnClickedCheckSpeech();
	afx_msg void OnBnClickedCheckSpeechControl();
	CComboBox m_ComboBoxStepCoolDelta;
	afx_msg void OnCbnSelchangeComboStepCoolDelta();
	CComboBox m_ComboTempSensorDelay;
	afx_msg void OnCbnSelchangeComboTempSensorDelay();
	afx_msg void OnBnClickedButtonFireware();
	afx_msg void OnBnClickedButtonFireware2();
	afx_msg void OnBnClickedButtonModifyPassword();
	
	CString m_MES_URL;
};


#endif // !defined(AFX_HEATBOXDLG_H__EEAF7D94_73E3_46C1_8CAE_B342D8D48DD0__INCLUDED_)

extern CHeatBoxDlg *G_MainDlg;
extern CAboutDlg  G_AboutDlg;
