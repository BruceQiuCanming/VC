#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_RECORDVIEWDLG_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_)
#define AFX_RECORDVIEWDLG_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordViewDlg dialog

class CRecordViewDlg : public CDialog
{
// Construction
public:
	RECORD	m_SelectedRecord;
	int		m_HeatBoxType;
	CString m_TestParaName;
	CString m_TestMemo;
	CTime m_BeginTestTime;
	CTime m_EndTestTime;
	CString m_TestProductType;
	int m_BoxNr;
	SWITCH_CONFIG_PARA_ALL m_SwitchConfigPara;
	
	SWITCH_TEST_RESULT_EX m_SwitchTestResult_Ex[192];

	void UpdateRecordDetail(CString begin,CString end);

	CRecordViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordViewDlg)
	enum { IDD = IDD_DIALOG_RECORD_VIEW };
	CListCtrl	m_ListCtrlRecord;
	CString		m_SelectedFileName;
	CString		m_Para;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDblclkListRecordTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPrn();
	afx_msg void OnButtonTempBar();
	afx_msg void OnButtonPrnPreview();
	afx_msg void OnButtonTongji();
	afx_msg void OnButtonSaveas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	public:
	static void TongJi(RECORD	*Record,float *open_max_temp,float *open_min_temp,float *open_avr_temp,float *close_max_temp,float *close_min_temp,float *close_avr_temp,int *total_product,int *total_main_level);


	afx_msg void OnBnClickedButtonSaveas2();
	void SaveDayCSV(HTREEITEM dayItem);
	void SaveMonthCSV(HTREEITEM monthItem);
	void SaveYearCSV(HTREEITEM yearItem);

	void CRecordViewDlg::UpdateRecordDetail(CString FileName);
	afx_msg void OnBnClickedButtonTakedlg();
	CComboBox m_ComboBoxSortType;
	void SortRecord();
	afx_msg void OnCbnSelchangeComboSortType();
	afx_msg void OnCbnSelchangeComboPrintType();
	CComboBox m_ComboPrintType;
	afx_msg void OnBnClickedButtonOpenTempShow();
	void ShowFile(CString str_Dir, HTREEITEM tree_Root);
	void CRecordViewDlg::SaveRecord2CSV(CString str_Dir, HTREEITEM tree_Root);
	CTreeCtrl m_DirTreeCtrl;
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	void CRecordViewDlg::OnBnClickedDelete();
	void CRecordViewDlg::SaveTreeReocord2CSV(int BoxNr,CString recordDir,CString csvDir,CString recordName,CString sYear,CString sMonth,CString sDay);
	static void CRecordViewDlg::CreateMES_String(RECORD	*Record,CString * MES_String);

	afx_msg void OnBnClickedButtonTempCurve();
	afx_msg void OnBnClickedButtonMes();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDVIEWDLG_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_)
