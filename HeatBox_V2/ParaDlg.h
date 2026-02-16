#if !defined(AFX_PARADLG_H__611BCBC1_7BBC_4CB0_8B76_060C5A573A06__INCLUDED_)
#define AFX_PARADLG_H__611BCBC1_7BBC_4CB0_8B76_060C5A573A06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaDlg.h : header file
//

#include "afxwin.h"





/////////////////////////////////////////////////////////////////////////////
// CParaDlg dialog

class CParaDlg : public CDialog
{
// Construction
public:
	CParaDlg(CWnd* pParent = NULL);   // standard constructor

	int m_BoxNr;
	//SWITCH_CONFIG_PARA m_SwitchConfigPara;


	bool m_bSetFocusOnEditMainLevel2;
// Dialog Data
	//{{AFX_DATA(CParaDlg)
	enum { IDD = IDD_DIALOG_PARA };
	CComboBox	m_CurMaxHeatBox;
	CComboBox	m_TestModeList;
	CComboBox	m_ComboBoxHeatSpeed;
	CComboBox	m_OpenDoorResetList;
	CComboBox	m_MainLevelList;
	double	m_CloseMainLevel;
	double	m_CloseMainLevelSpanPlus;
	double	m_CloseMainLevelSpanMinus;
	double	m_CloseMainLevelDeviationPlus;
	double	m_CloseMainLevelDeviationMinus;

	double	m_CloseHighLevel;
	double	m_CloseHighLevelSpanPlus;
	double	m_CloseHighLevelSpanMinus;
	double	m_CloseHighLevelDeviationPlus;
	double	m_CloseHighLevelDeviationMinus;

	double	m_CloseLowLevel;
	double	m_CloseLowLevelSpanPlus;
	double	m_CloseLowLevelSpanMinus;
	double	m_CloseLowLevelDeviationPlus;
	double	m_CloseLowLevelDeviationMinus;

	double	m_HighLevel;
	double	m_HighLevelSpanMinus;
	double	m_HighLevelSpanPlus;
	double	m_LowLevel;
	double	m_LowLevelSpanMinus;
	double	m_LowLevelSpanPlus;
	double	m_MainLevelSpanMinus;
	double	m_MainLevelSpanPlus;
	double	m_MainLevel;
	UINT	m_ProductYear;
	UINT	m_ProductMonth;
	UINT	m_ProductDay;
	BOOL	m_bAllOpenned_Closed_To_StopTest;
	//}}AFX_DATA

	SWITCH_CONFIG_PARA_ALL	m_SwitchConfigPara;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParaDlg)
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg void OnDblclkComboMainLevel();
	afx_msg void OnChangeEditMainLevelSpan();
	afx_msg void OnChangeEditHighLevelSpan();
	afx_msg void OnChangeEditLowLevelSpan();
	afx_msg void OnChangeEditMainLevelSpanMinus();
	afx_msg void OnChangeEditMainLevelSpanPlus();
	afx_msg void OnButtonCreateModifyPara();
	afx_msg void OnSelchangeComboHeatSpeed();
	afx_msg void OnChangeEditYear();
	afx_msg void OnEditchangeComboTestMode();
	afx_msg void OnSelchangeComboTestMode();
	afx_msg void OnChangeEditHeatDegree();
	afx_msg void OnChangeEditCoolDegree();
	afx_msg void OnSelchangeComboCurMaxHeatBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnEnChangeEditPidMinutes();
	UINT m_iNormalPidMinutes;
//	afx_msg void OnCbnSelendcancelComboParaName();
	afx_msg void OnCbnSelchangeComboParaName();
	afx_msg void OnCbnSelchangeComboHeatBoxType();
	int m_HeatBoxType;
	int m_PrinterType;
	afx_msg void OnCbnSelchangeComboDisplayTempType();

	afx_msg void OnCbnSelchangeComboOpenDoorReset();

	CComboBox m_ComboBoxDisplayTempType;
	void ShowCloseLevelEdit(void);
	afx_msg void OnCbnSelchangeComboContinueTestCount();
	CEdit	m_EditContinueTestCount;
	afx_msg void OnCbnSelchangeComboEndTestDoorStatus();
	CComboBox m_Combo_EndTestDoorStatus;
	afx_msg void OnEnChangeEditContinueTestCount();
	void UpdateModifyPara() ;
	afx_msg void OnBnClickedCheckRelay();
	CButton m_ControlStopTestType;
	afx_msg void OnBnClickedCheckStopTestType();
	afx_msg void OnEnSetfocusEditPidMinutes();
	afx_msg void OnEnSetfocusEditMainLevel2();
	afx_msg void OnCbnSetfocusComboHeatSpeed();
	afx_msg void OnEnSetfocusEditContinueTestCount();
	afx_msg void OnCbnSelchangeCombo485Protocol();
	CComboBox m_Combo485Protocol;
	CComboBox m_ComboCoolDegree;
	CComboBox m_ComboOverHeat;
	afx_msg void OnCbnSelchangeComboCoolDegree();
	afx_msg void OnCbnSelchangeComboCoolSpeed();
	CComboBox m_ComboBoxCoolSpeed;
	afx_msg void OnBnClickedButtonTempAdjust();
	float m_OpenDoorHeatDegree;
	void CParaDlg::UpdateSetPara(void);
	float m_OpenDoorCoolDegree;
	afx_msg void OnBnClickedButtonGroup();
	double m_MainLevelAvrSpanMinus;
	double m_MainLevelAvrSpanPlus;
	double m_CloseMainLevelAvrSpanMinus;
	double m_CloseMainLevelAvrSpanPlus;
	int m_SoundType;
	int	m_SoundStrong;
	afx_msg void OnCbnSelchangeComboSoundType();
	afx_msg void OnCbnSelendokComboOverHeat();
	int m_OverHeatGap;
	afx_msg void OnCbnSelchangeComboSoundStrong();
	afx_msg void OnCbnSelchangeComboPrinterType();
	afx_msg void OnCbnSelchangeComboAutoPrintType();
	int m_AutoPrintType;
	afx_msg void OnBnClickedCheckCoolDownAutoOpenDoor();
	BOOL m_bCoolDownAutoOpendoor;
	afx_msg void OnCbnSetfocusComboHeatBoxType();
	afx_msg void OnCbnSelendokComboHeatBoxType();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARADLG_H__611BCBC1_7BBC_4CB0_8B76_060C5A573A06__INCLUDED_)
