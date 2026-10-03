#pragma once
#include "afxwin.h"
#include "HeatBox_CGPDlg.h"
#include "afxcmn.h"

// CTestParaDlg 对话框

class CTestParaDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestParaDlg)

public:
	CTestParaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestParaDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnNMThemeChangedSpinPro1Cycle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPro1Cycle(NMHDR *pNMHDR, LRESULT *pResult);
	CBitmapButton *m_bmpButton[4];
	UINT m_Pro1Cycle;
	UINT m_Pro2Cycle;
	UINT m_Pro3Cycle;
	UINT m_Pro4Cycle;
	afx_msg void OnDeltaposSpinPro2Cycle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPro3Cycle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPro4Cycle(NMHDR *pNMHDR, LRESULT *pResult);
	int m_Pro1TestPoint;
	int m_Pro2TestPoint;
	int m_Pro3TestPoint;
	int m_Pro4TestPoint;
	int m_IsDoubleSwitch;
	int	m_IsNormalOpen;
	int m_TestSpeed;
	int m_EnterTestMode;
	afx_msg void OnBnClickedRadioPro1Hot();
	afx_msg void OnBnClickedRadioPro1Mid1();
	afx_msg void OnBnClickedRadioPro1Mid2();
	afx_msg void OnBnClickedRadioPro1Low();
	afx_msg void OnBnClickedRadioPro1Pupd();
	afx_msg void OnBnClickedRadioPro2No();
	afx_msg void OnBnClickedRadioPro2Hot();
	afx_msg void OnBnClickedRadioPro2Mid1();
	afx_msg void OnBnClickedRadioPro2Mid2();
	afx_msg void OnBnClickedRadioPro2Low();
	afx_msg void OnBnClickedRadioPro2Pupd();
	afx_msg void OnBnClickedRadioPro3No();
	afx_msg void OnBnClickedRadioPro3Hot();
	afx_msg void OnBnClickedRadioPro3Mid1();
	afx_msg void OnBnClickedRadioPro3Mid2();
	afx_msg void OnBnClickedRadioPro3Low();
	afx_msg void OnBnClickedRadioPro3Pupd();
	afx_msg void OnBnClickedRadioPro4No();
	afx_msg void OnBnClickedRadioPro4Hot();
	afx_msg void OnBnClickedRadioPro4Mid1();
	afx_msg void OnBnClickedRadioPro4Mid2();
	afx_msg void OnBnClickedRadioPro4Low();
	afx_msg void OnBnClickedRadioPro4Pupd();
	afx_msg void OnBnClickedButtonPro1Open();
	float m_Pro1OpenTemp;
	float m_Pro1CloseTemp;
	float m_Pro2OpenTemp;
	float m_Pro2CloseTemp;
	float m_Pro3OpenTemp;
	float m_Pro3CloseTemp;
	float m_Pro4OpenTemp;
	float m_Pro4CloseTemp;
	afx_msg void OnBnClickedRadioSpeed1();
	afx_msg void OnBnClickedRadioSpeed2();
	afx_msg void OnBnClickedRadioSpeed3();
	afx_msg void OnBnClickedRadioSpeed4();
	afx_msg void OnBnClickedRadioKeepWarm1();
	afx_msg void OnBnClickedRadioKeepwarm2();
	afx_msg void OnBnClickedRadioSwitchTypeCLh();
	afx_msg void OnBnClickedRadioSwitchTypeCLs();
	afx_msg void OnBnClickedButtonPro1Close();
	afx_msg void OnBnClickedButtonPro2Open();
	afx_msg void OnBnClickedButtonPro2Close();
	afx_msg void OnBnClickedButtonPro3Open();
	afx_msg void OnBnClickedButtonPro3Close();
	afx_msg void OnBnClickedButtonPro4Open();
	afx_msg void OnBnClickedButtonPro4Close();
	float m_Pro1OpenCloseGapLow;
	float m_Pro2OpenCloseGapLow;
	float m_Pro3OpenCloseGapLow;
	float m_Pro4OpenCloseGapLow;
	float m_Pro1OpenCloseGapHigh;
	float m_Pro2OpenCloseGapHigh;
	float m_Pro3OpenCloseGapHigh;
	float m_Pro4OpenCloseGapHigh;
	
	afx_msg void OnBnClickedButtonPro1Gap1();
	afx_msg void OnBnClickedButtonPro2Gap1();
	afx_msg void OnBnClickedButtonPro3Gap1();
	afx_msg void OnBnClickedButtonPro4Gap1();
	afx_msg void OnBnClickedButtonPro1Gap2();
	afx_msg void OnBnClickedButtonPro2Gap2();
	afx_msg void OnBnClickedButtonPro3Gap2();
	afx_msg void OnBnClickedButtonPro4Gap2();
	float m_Pro1OpenTempMinus;
	float m_Pro2OpenTempMinus;
	float m_Pro3OpenTempMinus;
	float m_Pro4OpenTempMinus;
	float m_Pro1OpenTempPlus;
	float m_Pro2OpenTempPlus;
	float m_Pro3OpenTempPlus;
    float m_Pro4OpenTempPlus;
	float m_Pro1CloseTempMinus;
	float m_Pro2CloseTempMinus;
	float m_Pro3CloseTempMinus;
	float m_Pro4CloseTempMinus;

	float m_Pro1CloseTempPlus;
	float m_Pro2CloseTempPlus;
	float m_Pro3CloseTempPlus;
	float m_Pro4CloseTempPlus;

	char  m_Pro1HeatCoolType;
	char  m_Pro2HeatCoolType;
	char  m_Pro3HeatCoolType;
	char  m_Pro4HeatCoolType;
	afx_msg void OnBnClickedButtonPro1Open2();
	afx_msg void OnBnClickedButtonPro1Open4();
	afx_msg void OnBnClickedButtonPro1Open6();
	afx_msg void OnBnClickedButtonPro1Open8();
	afx_msg void OnBnClickedButtonPro1Open3();
	afx_msg void OnBnClickedButtonPro1Open5();
	afx_msg void OnBnClickedButtonPro1Open7();
	afx_msg void OnBnClickedButtonPro1Open9();
	CComboBox m_ControlParaName;
	int SetRadio(void);
	UINT m_FlashGap;
	CString m_ProductorNo;
	CString m_Productor;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDelete();
	TEST_PARA CopyControlData2TestPara(void);
	afx_msg void OnCbnSelchangeCombo2();
	void UpdateAllPara(void);
	afx_msg void OnEnSetfocusEdit4();
	afx_msg void OnBnClickedButtonAdj();
	float m_AdjustTemp;
	afx_msg void OnBnClickedButtonSwitchNr();
	UINT m_SwitchNr;
	afx_msg void OnBnClickedButtonProductor();
	afx_msg void OnBnClickedButtonParaName();
	afx_msg void OnBnClickedButtonProductNo();
	afx_msg void OnBnClickedCheckWarmAutoReady();
	afx_msg void OnBnClickedCheckAutoEnter();
	afx_msg void OnBnClickedButtonHeatCoolType();
	afx_msg void OnBnClickedButtonHeatCoolTypePro1();
	afx_msg void OnBnClickedButtonHeatCoolTypePro2();
	afx_msg void OnBnClickedButtonHeatCoolTypePro3();
	afx_msg void OnBnClickedButtonHeatCoolTypePro4();
	

	void SelectLanguage(void);
	afx_msg void OnBnClickedRadioSwitchType3();
	afx_msg void OnBnClickedRadioSwitchType4();
	afx_msg void OnBnClickedRadioSwitchType1();
	afx_msg void OnBnClickedRadioSwitchType2();
	afx_msg void OnBnClickedOk();
	CComboBox m_ComboHeatSpeed;
	CComboBox m_ComboCoolSpeed;

	UINT m_HeatPercent;

	UINT m_CoolPercent;
	afx_msg void OnCbnSelchangeComboHeatSpeed();
	afx_msg void OnCbnSelchangeComboCoolSpeed();
	afx_msg void OnEnChangeEditHeatPercent();
	afx_msg void OnEnChangeEditCoolPercent();
	void CTestParaDlg::UpdateParaNameList();

};
