#if !defined(AFX_SWITCHDLG_H__C320EC1D_9F5E_433C_B948_FD285D7B5F8C__INCLUDED_)
#define AFX_SWITCHDLG_H__C320EC1D_9F5E_433C_B948_FD285D7B5F8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwitchDlg.h : header file
//

#include "takedlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSwitchDlg dialog
typedef enum 
{

	 TEST_MODE_NORMAL				= 0,
	 TEST_MODE_CLOSE_FLASH			= 1,
	 TEST_MODE_OPEN_AND_CLOSE_FLASH	= 2,
	 TEST_MODE_OPEN_FLASH			= 3,

}TEST_MODE;

typedef struct
{
	COMMAND_ID   CmdID;			//自动测试时已经发送的命令
	CTime        SendCmdTime;		//自动测试时发送命令时间
	char		 Para[8];			//自动测试时发送命令的参数
}SEND_COMMAND;	

CTime  GetRecordTime(long seconds);

extern SWITCH_TEST_RESULT	G_SwitchTestResult/*[6]*/[500]; //前5个对应5个烘箱，第6个是用于历史记录的查询

HEAT_COOL_ORDER	CheckHeatOrCoolMode(SWITCH_CONFIG_PARA	SwitchConfigPara,CONTROL_TEMP_RANGE *temp_range);

class CSwitchDlg : public CDialog
{
// Construction
public:
	CProductTempBar Dlg;
	double m_OpenDoorResetCoolDegree;
	double m_OpenDoorResetHeatDegree;
	void ClearPidStepCount(bool bClear);
	BOOL OpennedAndNotFlashTested();
	void OpenDoor_For_OpenFlashTest(void);
	void OpenDoor_For_CloseFlashTest(void);
	float m_LastClosePIDTemp;
	CTime m_ClosePIDStartTime;
	void SendStepPIDCmd(float TopTemp);
	void SendStepPIDCoolCmd(void);


	int		m_iTestMode;
	void	SendDirectHeatCmd(float temp);
	bool	m_bIsClosingDoor;
	float	m_CloseDoorTopTemp;
	float	m_CloseDoorStartTemp;
	CTime	m_CloseDoorStartTime;
//常达
//升1度，降2度，再升3度
	float	m_OpenDoor_Temp;
	CTime	m_OpenDoor_StartTime;
	BOOL	m_OpenDoorIng;
	CString m_ParaName;
	CString m_TestMemo;

//常胜，在主规格的动作温度下限，中间，上限分别开一次门
	int		m_OpenDoor_SwitchClosed[3];
	CTime	m_OpenDoor_SwitchClosed_StartTime[3];
	CONTROL_TEMP_RANGE	m_ControlTempRange;
	bool	m_bCoolRelay;
	int		m_iContinueTestCount;
	//int		m_iDisplayTempType;
	int		m_iCloseAgain[10];
	CTime	m_CloseAgainStartTime[10]; 
	bool	m_IsTaking;
	int		GetHighestCloseTemp();
	double		GetLowestCloseTemp(void);

	CTime	m_LedAnswerTime;
	CTime	m_MeterAnswerTime;


	void	SaveTestRecord(void);
	void	SaveTestRecord_Hex(void);

	void    SendStepPidHeatCmd(void);
	float   m_LastStepPidTemp;

	void    SendStepPidCoolCmd(void);
	float   m_LastStepPidCoolTemp;

	bool	IsAllClosed(void);
	bool	IsAllOpened(void);
	CTime	m_EndTestTime;
	CTime	m_BeginTestTime;
	void	SendReadOpenFlash(void);
	void	SendReadCloseFlash(void);
	void	SendStartOpenFlashTest(void);
	void	SendStartCloseFlashTest(void);
	void	SendCmdSP(float temp);
	int		m_BoxNr;
	int		m_HeatBoxType;
	CTime	m_NormalPidBeginTime;
	CTime	m_StepPidBeginTime;
	CTime	m_StepCoolPidBeginTime;

	CString m_Printer;
	int		m_Page;
	SWITCH_CONFIG_PARA	m_SwitchConfigPara;
	double				m_CurTemp[4];
	CTakeoutDlg			m_TakeoutDlg;
	//用于自动测试
//	AUTO_TEST_PARA		m_AutoTestPara[4];
	int					m_iStepPidSpeed;
	int					m_iStepDownPidSpeed;
	int					m_iNormalPidMinutes;
	SEND_COMMAND		m_SendCommand;
	HEAT_COOL_ORDER		m_HeatCoolOrder;
	WORK_STATUS m_WorkStatus;

	void DrawLED(void);
	void DealCmdAnswer(char *para, unsigned char *flash/*[16+16]*/,bool IsCmdAscii,char source_ID);
	CSwitchDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CSwitchDlg)
	enum { IDD = IDD_DIALOG_SWITCH };
	//BOOL	m_AutoTestFlag;
	CString	m_TestMsg;
	int		m_SwitchNormalOpen;
	float	m_PID_Temp;
	BOOL	m_bHighVolt;
	CString	m_TestTimeMsg;
	//}}AFX_DATA


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
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnButtonTakeout();
	afx_msg void OnRadioClose();
	afx_msg void OnRadioOpen();
	afx_msg void OnCheckHighVolt();
	afx_msg void OnChangeEdit1();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboFlashGap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ExitAutoTest(void);
	void EnterAutoTest(void);
	void EnterAutoTest_COOL_FIRST();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	
	double GetControlGoalTemp(AUTO_TEST_STEP step);
	void PrintTestResult(void) ;
	void CSwitchDlg::SaveCurve(void);


	bool m_StopTestType;
	bool m_bFirstCoolSencondHeat_CloseAC_Before_Heat;

	void AutoTestStep_DirectHeat(void);
	void AuotTestStep_NormalPID(void);
	void AutoTestStep_StepPID(void);
	void AutoTestStep_DircetCool(void);
	void AutoTestStep_HeatAgain(void);
	void AutoTestStep_StepCool(void);
	void CSwitchDlg::SaveExcel(void);

	void AutoTestCoolStep_DriectCool(void);
	void AutoTestCoolStep_PidCool(void);
	void AutoTestCoolStep_StepCool(void);
	void AutoTestCoolStep_DirectHeat(void);
	void AutoTestCoolStep_PidHeat(void);
	void AutoTestCoolStep_StepHeat(void);

	void CSwitchDlg::DealStatusNormalOpen(SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status);
	void CSwitchDlg::DealStatusNormalClose(SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status);
	void CSwitchDlg::ShowTestResult_HeYang(void);
	afx_msg void OnBnClickedCheckNextPage();
	afx_msg void OnBnClickedButtonPrint();
	float m_OpenTempLow;
	float m_OpenTempHigh;
	float m_CloseTempLow;
	float m_CloseTempHigh;
	afx_msg void OnEnChangeEditOpenTempLow();
	afx_msg void OnEnChangeEditOpenTempHigh();
	afx_msg void OnEnChangeEditCloseTempLow();
	afx_msg void OnEnChangeEditCloseTempHigh();
	afx_msg void OnBnClickedButtonRecord();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWITCHDLG_H__C320EC1D_9F5E_433C_B948_FD285D7B5F8C__INCLUDED_)
