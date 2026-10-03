#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CDeliveryNoteDlg 对话框

class CContractDlg : public CDialog
{
	DECLARE_DYNAMIC(CContractDlg)

public:
	CContractDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CContractDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTRACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void PrintContract(CString fName);
	void DrawRect(CDC *dc,CRect rect);
	void DrawText(CDC *dc,CRect rect,CString text,UINT format);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonPrint();
	void Save(CString fName);
	CString m_Production;
	int m_Counts;
	float m_Price;
	float m_TotalPrice;
	CString m_Memo;
	afx_msg void OnBnClickedOk();
	void SelectCustomer(CString customer);
	CString m_CustomerCompany;

	CStringArray	m_ReceiverArray;
	CStringArray	m_ProductionArray;
	CStringArray	m_TypeArray;
	CStringArray	m_UnitArray;

	void GetUser(void);
	void GetProduction(void);
	void GetType(void);
	void GetUnit(void);
	afx_msg void OnEnSetfocusEditNum();
	afx_msg void OnEnSetfocusEditPrice();
	CString m_Type;

	CListCtrl m_ListCtrl;
	afx_msg void OnBnClickedButtonProduction();
	afx_msg void OnBnClickedButtonType();
	CString m_Unit;
	afx_msg void OnBnClickedButtonUnit();
	CString m_Contract_Nr;
	CComboBox m_ComboProvince;
	afx_msg void OnBnClickedButtonWorkDir();
	CString m_WorkDir;
	
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_SendCompany;
	CString m_Sender;
	afx_msg void OnBnClickedButtonCompanySeal();
	CStatic m_CompanySeal;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonCustomer();
	afx_msg void OnBnClickedButtonContract();
	afx_msg void OnBnClickedButtonCustomer2();
	afx_msg void OnEnChangeEditSendAddress();
	CString m_SendTaxNr;
	CString m_SendAddress;
	CString m_SendBank;
	CString m_SendBankAccount;
	CString m_SendBankId;
	CDateTimeCtrl m_DateTimeCtrl;
	CString m_Customer;
	CString m_CustomerTaxNr;
	CString m_CustomerAddress;
	CString m_CustomerBank;
	CString m_CustomerBankAccount;
	CString m_CustomerBankId;
	CString m_Memo_1;
	CString m_SignedAddress;
	CString m_SendTel;
	CString m_CustomerTel;
	int CContractDlg::CalFontSize(CDC *dc,CString str,CRect rect,int size);

};
