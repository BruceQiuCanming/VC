#pragma once
#include "afxwin.h"


// CParaDlg 对话框

class CParaDlg : public CDialog
{
	DECLARE_DYNAMIC(CParaDlg)

public:
	CParaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaDlg();

		CComboBox m_Combo_COMM_AGING_1;
	CComboBox m_Combo_COMM_AGING_2;
	CComboBox m_Combo_COMM_AGING_3;
	CComboBox m_Combo_COMM_AGING_4;
	CComboBox m_Combo_COMM_AGING_5;
	CComboBox m_Combo_COMM_AGING_6;
	CComboBox m_Combo_COMM_AGING_7;
	CComboBox m_Combo_COMM_AGING_8;

	CComboBox m_Combo_PLC_COMM;

	CComboBox m_Combo_PROGRAM_COMM;

	afx_msg void OnBnClickedOk();
	CComboBox m_Combo_Camera;
	afx_msg void OnCbnSelendokComboCamera();


	afx_msg void OnCbnSelendokCombo_AGING_Comm1();
	afx_msg void OnCbnSelendokCombo_AGING_Comm2();
	afx_msg void OnCbnSelendokCombo_AGING_Comm3();
	afx_msg void OnCbnSelendokCombo_AGING_Comm4();
	afx_msg void OnCbnSelendokCombo_AGING_Comm5();
	afx_msg void OnCbnSelendokCombo_AGING_Comm6();
	afx_msg void OnCbnSelendokCombo_AGING_Comm7();
	afx_msg void OnCbnSelendokCombo_AGING_Comm8();

	afx_msg void OnCbnSelendokCombo_PLC_Comm();
	CComboBox m_Combo_Aging_Power_1;
	CComboBox m_Combo_Aging_Power_2;
	CComboBox m_Combo_Aging_Power_3;
	CComboBox m_Combo_Aging_Power_4;
	CComboBox m_Combo_Aging_Power_5;
	CComboBox m_Combo_Aging_Power_6;
	CComboBox m_Combo_Aging_Power_7;
	CComboBox m_Combo_Aging_Power_8;

	void CParaDlg::DisplaySend_PLC(MODBUS_RS485_CMD_ALL all);

	void DisplayText(CString *text,unsigned int counts ,MODBUS_RS485_CMD_ALL all);

	void DisplaySend_hotLong(int ID,MODBUS_RS485_CMD_ALL all);

	void DisplayRec_PLC(unsigned char *Data,int Date_Len);
	void CParaDlg::DisplayText(CString *text,unsigned int counts ,unsigned char *data,int date_len);
	void DisplayRec_hotLong(int ID,unsigned char *data,int date_len);


// 对话框数据
	enum { IDD = IDD_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_SendTxt_PLC;
	CString m_RecTxt_PLC;
	CString m_SendTxt_HotLong[9];
	CString m_RecTxt_HotLong[9];
	afx_msg void OnCbnSelendokComboPlcY_AGING_1();
	afx_msg void OnCbnSelendokComboPlcY_AGING_2();
	afx_msg void OnCbnSelendokComboPlcY_AGING_3();
	afx_msg void OnCbnSelendokComboPlcY_AGING_4();
	afx_msg void OnCbnSelendokComboPlcY_AGING_5();
	afx_msg void OnCbnSelendokComboPlcY_AGING_6();
	afx_msg void OnCbnSelendokComboPlcY_AGING_7();
	afx_msg void OnCbnSelendokComboPlcY_AGING_8();

	CComboBox m_Combo_PCB_Type;
	afx_msg void OnCbnSelendokComboPcbType();
	afx_msg void OnCbnSelendokComboProgramComm();
	CComboBox m_Combo_TEST_NEEDLE;
	CComboBox m_Combo_TEST_START;
	CComboBox m_Combo_SET_KEY;
	CComboBox m_Combo_PLUS_KEY;
	CComboBox m_Combo_MINUS_KEY;
	afx_msg void OnCbnSelendokComboPlcYTestNeedle();
	afx_msg void OnCbnSelendokComboPlcYTestStart();
	afx_msg void OnCbnSelendokComboPlcYSetKey();
	afx_msg void OnCbnSelendokComboPlcYPlusKey();
	afx_msg void OnCbnSelendokComboPlcYMinusKey();
	afx_msg void OnBnClickedCheckPower1();
	CButton m_CheckPower_1;
	CButton m_CheckPower_2;
	CButton m_CheckPower_3;
	CButton m_CheckPower_4;
	CButton m_CheckPower_5;
	CButton m_CheckPower_6;
	CButton m_CheckPower_7;
	CButton m_CheckPower_8;

	afx_msg void OnBnClickedCheckPower2();
	afx_msg void OnBnClickedCheckPower3();
	afx_msg void OnBnClickedCheckPower4();
	afx_msg void OnBnClickedCheckPower5();
	afx_msg void OnBnClickedCheckPower6();
	afx_msg void OnBnClickedCheckPower7();
	afx_msg void OnBnClickedCheckPower8();
	afx_msg void OnBnClickedCheckTestNeedle();
	afx_msg void OnBnClickedCheckProgramStart();
	afx_msg void OnBnClickedCheckProgramSet();
	afx_msg void OnBnClickedCheckProgramIncrease();
	afx_msg void OnBnClickedCheckProgramMinus();
	CButton m_Check_TestNeedle;
	CButton m_Check_ProgramStart;
	CButton m_Check_ProgramSet;
	CButton m_Check_ProgramIncrease;
	CButton m_Check_ProgramMinus;
};
