#pragma once


// COtherConfigDlg 对话框

class COtherConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(COtherConfigDlg)

public:
	COtherConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COtherConfigDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_OTHER_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioCom1();
	afx_msg void OnBnClickedRadioCom2();
	afx_msg void OnBnClickedRadioCom3();
	afx_msg void OnBnClickedRadioCom4();
	afx_msg void OnBnClickedRadioCom5();
	afx_msg void OnBnClickedRadioCom6();
	afx_msg void OnBnClickedRadioCom7();
	afx_msg void OnBnClickedRadioCom8();
};
