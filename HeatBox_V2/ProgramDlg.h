#pragma once
#include "afxwin.h"


// CProgramDlg 对话框

class CProgramDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgramDlg)

public:
	CProgramDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgramDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_Program };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	
	virtual BOOL CProgramDlg::OnInitDialog() ;
	CComboBox m_Combo_BoxNr;
	CComboBox m_ComboTM7705_OSC;
	CComboBox m_ComboInterfaceBoardType;
	int m_BoxNr;
	int m_TM7705_OSC;
	int m_InterfaceBoardType;
	afx_msg void OnBnClickedOk();
};
