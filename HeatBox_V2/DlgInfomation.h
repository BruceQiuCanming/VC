#pragma once


// CDlgInfomation 对话框

class CDlgInfomation : public CDialog
{
	DECLARE_DYNAMIC(CDlgInfomation)

public:
	CDlgInfomation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInfomation();

// 对话框数据
	enum { IDD = IDD_DIALOG_INFOMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString  m_Info;
	CString  m_Memo;
	int		m_BoxNr;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	CString m_Tester;
};
