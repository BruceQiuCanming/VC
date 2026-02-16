//{{AFX_INCLUDES()
//#include "mschart.h"
#include "afxwin.h"
//}}AFX_INCLUDES
#if !defined(AFX_CURVEDLG_H__50394443_57AF_4EE8_9D1E_204F5A1C5498__INCLUDED_)
#define AFX_CURVEDLG_H__50394443_57AF_4EE8_9D1E_204F5A1C5498__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurveDlg.h : header file
//
typedef CArray<double ,double> CDoubleArray;

/////////////////////////////////////////////////////////////////////////////
// CCurveDlg dialog
#define MAX_HISTORY 10000
	
class CCurveDlg : public CDialog
{
// Construction
public:
//	CMSChart	m_Chart;
	void UpdateTemp(double temp[4]);
	CCurveDlg(CWnd* pParent = NULL);   // standard constructor
	int m_BoxNr;
	double max,min;
// Dialog Data
	//{{AFX_DATA(CCurveDlg)
	enum { IDD = IDD_DIALOG_CURVE };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	BOOL CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC) ;
	afx_msg void OnBnClickedButtonSave();
	void CCurveDlg::OnExportChart(CString filePath);

	CComboBox m_ComboCurveType;
	CDoubleArray m_TempArray[4];
	void DrawTempCurve(void);
	void OnExportTempFlat(CString filePath);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURVEDLG_H__50394443_57AF_4EE8_9D1E_204F5A1C5498__INCLUDED_)
