#pragma once


// CNumInputDlg 对话框

class CNumInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CNumInputDlg)

public:
	CNumInputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNumInputDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_NUM_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Input[3];
	CString m_Msg[3];
	int     m_CurInput;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_Title;
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnEnSetfocusEdit2();
	afx_msg void OnEnSetfocusEdit3();
};
