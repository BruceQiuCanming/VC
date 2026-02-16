#pragma once
#include "afxwin.h"


// CGroupDlg 对话框

class CGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CGroupDlg)

public:
	CGroupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GROUP };
	int		m_BoxNr;
	unsigned char m_Group[192];
	void ShowGroup(void);
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBoxNr;
	afx_msg void OnBnClickedOk();
	CListBox m_ListNoGroup;
	afx_msg void OnLbnDblclkListNoGroup();
	CListBox m_ListGroup;
	afx_msg void OnCbnSelchangeComboGroupNr();
	afx_msg void OnLbnDblclkListGroup();
};
