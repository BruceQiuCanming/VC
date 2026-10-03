#pragma once

#include "HeatBox_CGPDlg.h"
// CFullScreenCurve 对话框

class CFullScreenCurve : public CDialog
{
	DECLARE_DYNAMIC(CFullScreenCurve)

public:
	CFullScreenCurve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFullScreenCurve();

	CTempRecordArray m_TempRecordArray;
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
	PBITMAPINFO CFullScreenCurve::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	BOOL CFullScreenCurve::CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC);
	afx_msg void OnBnClicked1023();
};
