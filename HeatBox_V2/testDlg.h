#pragma once


// CtestDlg 对话框

class CtestDlg : public CDialog
{
	DECLARE_DYNAMIC(CtestDlg)

public:
	CtestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CtestDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

	void CtestDlg::DrawLED_JiaJu(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
