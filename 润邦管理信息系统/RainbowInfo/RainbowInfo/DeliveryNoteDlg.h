#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CDeliveryNoteDlg 对话框

class CDeliveryNoteDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeliveryNoteDlg)

public:
	CDeliveryNoteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeliveryNoteDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_Delivery_Note };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void PrintDeliveryNote(CString fName);
	void CDeliveryNoteDlg::DrawRect(CDC *dc,CRect rect);
	void CDeliveryNoteDlg::DrawText(CDC *dc,CRect rect,CString text,UINT format);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonPrint();
	void CDeliveryNoteDlg::Save(CString fName);
	CString m_Production;
	int m_Counts;
	float m_Price;
	float m_TotalPrice;
	CString m_Memo;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonReceiver();
	void CDeliveryNoteDlg::SelectCustomer(CString customer);
	CString m_Receiver;

	CStringArray	m_ReceiverArray;
	CStringArray	m_ProductionArray;
	CStringArray	m_TypeArray;
	CStringArray	m_UnitArray;

	void CDeliveryNoteDlg::GetUser(void);
	void CDeliveryNoteDlg::GetProduction(void);
	void CDeliveryNoteDlg::GetType(void);
	void CDeliveryNoteDlg::GetUnit(void);
//	afx_msg void OnEnChangeEditNum();
//	afx_msg void OnEnChangeEditPrice();
	afx_msg void OnEnSetfocusEditNum();
	afx_msg void OnEnSetfocusEditPrice();
	CString m_Type;

	CListCtrl m_ListCtrl;
	afx_msg void OnBnClickedButtonProduction();
	afx_msg void OnBnClickedButtonType();
	CString m_SheetNr;
	CDateTimeCtrl m_DateTimeCtrl;
	CString m_Unit;
	afx_msg void OnBnClickedButtonUnit();
	CString m_Contract_Nr;
	CComboBox m_ComboProvince;
	afx_msg void OnBnClickedButtonWorkDir();
	CString m_WorkDir;
	afx_msg void OnBnClickedButtonReceiver2();

	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_SendCompany;
	CString m_Sender;
	afx_msg void OnBnClickedButtonCompanySeal();
	CStatic m_CompanySeal;
	afx_msg void OnPaint();
};
