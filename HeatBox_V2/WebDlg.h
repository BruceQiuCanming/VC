#pragma once
#include "explorer1.h"


// CWebDlg 对话框

class CWebDlg : public CDialog
{
	DECLARE_DYNAMIC(CWebDlg)

public:
	CWebDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWebDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_Web };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_Web;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
