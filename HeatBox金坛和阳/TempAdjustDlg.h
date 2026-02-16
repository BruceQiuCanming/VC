#pragma once


// CTempAdjustDlg 对话框

class CTempAdjustDlg : public CDialog
{
	DECLARE_DYNAMIC(CTempAdjustDlg)

public:
	CTempAdjustDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTempAdjustDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEMP_ADJUST };
	
	virtual BOOL OnInitDialog();
	void CTempAdjustDlg::OnEditClicked(UINT id);
	bool   m_bSetFocusOnPara4;
	float  m_AdjustTemp[500][2];
	CEdit  m_Edit[500];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_TempAdjustType;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
