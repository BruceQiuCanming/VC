#pragma once


// CSwitchUsedDlg 对话框

class CSwitchUsedDlg : public CDialog
{
	DECLARE_DYNAMIC(CSwitchUsedDlg)

public:
	CSwitchUsedDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSwitchUsedDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SWITCH_ONOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
