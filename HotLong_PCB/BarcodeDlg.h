#pragma once


// CBarcodeDlg 对话框

class CBarcodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CBarcodeDlg)

public:
	CBarcodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBarcodeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BARCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Barcode;
};
