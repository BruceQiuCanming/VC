// HeatBoxDlg.h : header file
//

#if !defined(AFX_HEATBOXDLG_H__EEAF7D94_73E3_46C1_8CAE_B342D8D48DD0__INCLUDED_)
#define AFX_HEATBOXDLG_H__EEAF7D94_73E3_46C1_8CAE_B342D8D48DD0__INCLUDED_

#include "comm.h"	// Added by ClassView
#include "SwitchDlg.h"	// Added by ClassView
#include "CurveDlg.h"	// Added by ClassView
#include "ParaDlg.h"
#include "DebugDlg.h"	// Added by ClassView
#include "TakeoutPage.h"
#include "TempParaDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXDAO.H>
/////////////////////////////////////////////////////////////////////////////
// CHeatBoxDlg dialog
extern CCommCmdArray_500	m_SendCmdArray500;

class CHeatBoxDlg : public CDialog
{
// Construction
public:
	
#ifdef _HEATBOX_RESET_
	unsigned char LastSwitchStatus[4][40];
#endif
	
	int m_CommTimeOut[MAX_HEAT_BOX];
	unsigned int m_SN[MAX_HEAT_BOX];
	int			 m_MeterID;
	int				m_CurCmtType;
	CCommCmdProgram	  m_SendmCmdProgram;
	CCommCmdArray_OMRON m_SendCmdArray_OMRON;
	

	//CCommCmdArray_ASC m_SendCmdArrayASC[MAX_HEAT_BOX];

	int m_CurRs485Nr;
	unsigned char m_SwitchStatus[MAX_HEAT_BOX][65]; //用于500点灯板显示

	CComm m_Comm;
	double m_CurTemp[MAX_HEAT_BOX][4];
	CDoubleArray m_LastCurTemp[MAX_HEAT_BOX][4];

	CTabCtrl	m_Tab[10];	//属性页不可变
	
	CCurveDlg 	m_CurveDlg;
	CSwitchDlg 	m_SwitchDlg;
	CParaDlg   	m_ParaDlg;
	CDebugDlg	m_DebugDlg;
	CTakeoutPage	m_TakeoutPage;
	CTempParaDlg	m_TempParaDlg;

//	SWITCH_TEST_RESULT m_SwitchTestResult[(500 + 7)];
//	SWITCH_CONFIG_PARA m_SwitchConfigPara[MAX_HEAT_BOX];
	
	void ReadSwitchConfigPara(void);
	void SaveSwitchConfigPara(int BoxNr);
	void TempCmd(COMMAND_ID cmd_id,char* para);

	void CHeatBoxDlg::TempCmd_500(COMMAND_ID cmd_id,char* para);
	void SelectTab(int tabnr);
	LRESULT OnComm(WPARAM wParam,LPARAM lParam);
	bool CHeatBoxDlg::CheckSameTemp(int BoxNr);
	//LRESULT OnComm_ASCII(WPARAM wParam,LPARAM lParam);
	void SetCmdString_ASCII(ASCII_RS485_COMMAND *cmd,int equip, int cmdId, short para[4]);
	void SetCmdString(int equip, COMMAND_ID cmdId, char para[4]);

	void CHeatBoxDlg::SetCmdHeat(bool On);
	void CHeatBoxDlg::SetCmdFan(bool On);
	void CHeatBoxDlg::SetCmdAirControl(bool On);
	void CHeatBoxDlg::SetCmdAirValve(bool On);

	void SetCmdString_OMRON(int equip, COMMAND_ID cmdId, char para[4]);
	void InitDatabase(void);

	CHeatBoxDlg(CWnd* pParent = NULL);	// standard constructor
	
// Dialog Data
	//{{AFX_DATA(CHeatBoxDlg)
	enum { IDD = IDD_HEATBOX_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatBoxDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab8(NMHDR* pNMHDR, LRESULT* pResult);
	
	// Generated message map functions
	//{{AFX_MSG(CHeatBoxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	void CHeatBoxDlg::XModem_Start(unsigned char BoxNr);
	int CHeatBoxDlg::Hex2Bin(CString HexFile,char *binData,int MaxLen);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEATBOXDLG_H__EEAF7D94_73E3_46C1_8CAE_B342D8D48DD0__INCLUDED_)
