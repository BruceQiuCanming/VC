#pragma once


// CFullScreenCurve 对话框

class CFullScreenCurve : public CDialog
{
	DECLARE_DYNAMIC(CFullScreenCurve)

public:
	CFullScreenCurve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFullScreenCurve();

	CTempArray			m_TempArray;
	int					m_BoxNr;

// 对话框数据
	enum { IDD = IDD_DIALOG_CURVE_FULL_SCREEN };

	virtual BOOL OnInitDialog();
	void CFullScreenCurve::DrawTempCurve(void);
	void CFullScreenCurve::ExportTempCurve(CString filePath);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
