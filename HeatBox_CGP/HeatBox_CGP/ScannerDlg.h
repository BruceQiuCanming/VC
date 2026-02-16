#pragma once


// CScannerDlg 对话框

class CScannerDlg : public CDialog
{
	DECLARE_DYNAMIC(CScannerDlg)

public:
	CScannerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScannerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SCANNER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_SwitchPos;
	CString m_Barcode;
};
