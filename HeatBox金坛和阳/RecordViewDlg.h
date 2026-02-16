#include "afxwin.h"
#if !defined(AFX_RECORDVIEWDLG_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_)
#define AFX_RECORDVIEWDLG_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordViewDlg.h : header file
//
void PrintReport_bmp(CDC *dc,CRect page_rect,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,HEAT_BOX_TYPE HeatBoxType);

void PrintReport(CDC *dc,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,HEAT_BOX_TYPE HeatBoxType);

/////////////////////////////////////////////////////////////////////////////
// CRecordViewDlg dialog

class CRecordViewDlg : public CDialog
{
// Construction
public:
	int		m_HeatBoxType;
	CString m_TestParaName;
	CString m_TestMemo;
	CTime m_BeginTestTime;
	CTime m_EndTestTime;
	CString m_TestProductType;
	int m_BoxNr;
	SWITCH_CONFIG_PARA m_SwitchConfigPara;
	
	SWITCH_TEST_RESULT_EX m_SwitchTestResult_Ex[500];

	void UpdateRecordDetail(CString begin,CString end);
	void UpdateListCtrlRecordTime(bool AllSelect = false);
	CRecordViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordViewDlg)
	enum { IDD = IDD_DIALOG_RECORD_VIEW };
	CListCtrl	m_ListCtrlRecordTime;
	CListCtrl	m_ListCtrlRecord;
	CTime	m_SelectTestTime;
	CString	m_Para;
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
	afx_msg void OnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPrn();
	afx_msg void OnButtonTempBar();
	afx_msg void OnButtonPrnPreview();
	afx_msg void OnButtonTongji();
	afx_msg void OnButtonSaveas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void TongJi(float *open_max_temp,float *open_min_temp,float *open_avr_temp,float *close_max_temp,float *close_min_temp,float *close_avr_temp,int *total_product,int *total_main_level);
public:
	afx_msg void OnLvnItemchangedListRecordTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSaveas2();
	void FindRecordDetail(CString begin, CString end);
	afx_msg void OnBnClickedButtonTakedlg();
	CComboBox m_ComboBoxSortType;
	void SortRecord();
	afx_msg void OnCbnSelchangeComboSortType();
	afx_msg void OnCbnSelchangeComboPrintType();
	CComboBox m_ComboPrintType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDVIEWDLG_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_)
