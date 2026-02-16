#pragma once
#include "resource.h"

// CMyMsgBox 对话框

class CMyMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CMyMsgBox)

public:
	CMyMsgBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyMsgBox();

// 对话框数据
	enum { IDD = IDD_DIALOG_MSG_BOX };
	CString		m_Title;

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNcPaint(WPARAM,LPARAM);
	CString m_Msg;
};
