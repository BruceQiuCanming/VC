#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CViewTestRecordDlg 对话框

class CViewTestRecordDlg : public CDialog
{
	DECLARE_DYNAMIC(CViewTestRecordDlg)

public:
	CViewTestRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewTestRecordDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_VIEW_RECORDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListBox m_RecordDetailList;
	//TEST_RECORD m_Record;
	afx_msg void OnLbnDblclkListRecord();
	afx_msg void OnLbnSelchangeListRecord();
	CEdit m_EditPara;
	afx_msg void OnBnClickedPrint();
	afx_msg void OnBnClickedSave();
	void CViewTestRecordDlg::PrintReport(CDC *dc,
				 TEST_RECORD record);

	CTreeCtrl m_DirTreeCtrl;
	void CViewTestRecordDlg::ShowFile(CString str_Dir, HTREEITEM tree_Root);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCur();
};
