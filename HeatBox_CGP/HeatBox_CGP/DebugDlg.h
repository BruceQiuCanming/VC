#pragma once


// CDebugDlg 对话框

class CDebugDlg : public CDialog
{
	DECLARE_DYNAMIC(CDebugDlg)

public:
	CDebugDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebugDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFullHeat();
	bool  m_bSetFocusOnEditCurTemp;
	float m_FullOutTemp;
	float m_FullCoolTemp;
	float m_StepCoolTemp;
	float m_PidCoolTemp;
	float m_NormalPidTemp;
	afx_msg void OnBnClickedButtonPidHeat();
	afx_msg void OnBnClickedButtonStepPidHeat();
	float m_StepPidTopTemp;
	short m_StepPidTime;
	afx_msg void OnBnClickedButtonReadPid();
	afx_msg void OnBnClickedButtonWritePid();
	 short m_Read_P;
	 short m_Read_I;
	 short m_Read_D;
	 short m_Write_P;
	 short m_Write_I;
	 short m_Write_D;
	 short m_Read_P_Cool;
	 short m_Read_I_Cool;
	 short m_Read_D_Cool;
	 short m_Write_P_Cool;
	 short m_Write_I_Cool;
	 short m_Write_D_Cool;
	UINT m_AutoCalStep;
	float  m_AutoCalMeterLow;
	float  m_AutoCalMeterHigh;
	float  m_AutoCalGlassLow;
	float  m_AutoCalGlassHigh;
	float  m_Adjust_400,m_Adjust_0;

	afx_msg void OnBnClickedButtonReadAdjustLow();
	afx_msg void OnBnClickedButtonReadAdjustLow2();
	float m_ReadAdjustLow_1;
	//float m_ReadAdjustLow_2;
	float m_ReadAdjustHigh_1;
	//float m_ReadAdjustHigh_2;
	float m_WriteAdjustHigh_1;
	//float m_WriteAdjustHigh_2;
	float m_WriteAdjustLow_1;
	//float m_WriteAdjustLow_2;
	afx_msg void OnBnClickedButtonWriteAdjustLow1();
	afx_msg void OnBnClickedButtonWriteAdjustHigh();
	afx_msg void OnBnClickedButtonDipslaySwitchStatus();
	afx_msg void OnBnClickedButtonDisplayAll();
	afx_msg void OnBnClickedButtonAutoDisplayNo();
	afx_msg void OnBnClickedButtonAutoAdjustTemp();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
		
	virtual BOOL OnInitDialog();
	float m_CurTemp;
	bool  m_bShowed;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	void CalAdjustTemp(void);
	afx_msg void OnBnClickedButtonStopHeat();
	afx_msg void OnBnClickedButtonCool();
	afx_msg void OnBnClickedButtonStepCool();
	afx_msg void OnBnClickedButtonPidCool();
	afx_msg void OnBnClickedButtonRelayOn();
	afx_msg void OnBnClickedButtonRelayOff();
	afx_msg void OnBnClickedButtonUpdateFirmware();
	UINT m_StepCoolMinutes;

	float m_AT_Temp;
	UINT m_StepPercent;
	UINT m_CoolStepPercent;
	void DrawLED(void);
	void DrawLED_JiaJu(void);
	bool m_bJiajuTesting;
	unsigned char m_JiaJuTested[32];
	bool CheckJiaJuPass(unsigned char ID);
	afx_msg void OnBnClickedCheckJiajuTest();
	afx_msg void OnBnClickedButtonReadCurrentMeterTempLow();
	afx_msg void OnBnClickedButtonReadCurrentMeterTempHigh();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonInitAdjustTemp();
	afx_msg void OnBnClickedButtonPidAutotune();
	afx_msg void OnBnClickedButtonCommSpeed();
	UINT m_CommSpeed;
	afx_msg void OnEnSetfocusEditCommSpeed();
	afx_msg void OnEnSetfocusEditCurTemp();
	afx_msg void OnEnSetfocusEditFullOutTemp();
	afx_msg void OnEnSetfocusEditPidOut();
};
