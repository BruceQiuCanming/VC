#pragma once
#include "afxcmn.h"
#include "SwitchUsedDlg.h"
#include "OtherConfigDlg.h"
#include "DebugDlg.H"
// CSysConfigDlg 对话框

class CSysConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CSysConfigDlg)

public:
	CSysConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysConfigDlg();
	
// 对话框数据
	enum { IDD = IDD_DIALOG_SYS_CONFIG };


	CSwitchUsedDlg  m_SwitchUsedDlg;
	COtherConfigDlg m_OtherConfigDlg;
	//CDebugDlg		m_DebugDlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
};
