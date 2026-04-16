#pragma once
#include "afxcmn.h"


// CDongguanBetterMesMsgDlg 对话框

class CDongguanBetterMesMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CDongguanBetterMesMsgDlg)

public:
	CDongguanBetterMesMsgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDongguanBetterMesMsgDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DONGGUAN_BETTER_MES_MESSAGE };

	CString GetLocalIP(void);
	CString Get_MES_IP(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	BYTE m_LocalIP[4];

	UINT m_MES_TCP_Port;
	
	afx_msg void OnIpnFieldchangedIpaddressMes(NMHDR *pNMHDR, LRESULT *pResult);

//	afx_msg void OnEnChangeEditTcpPort();

	CString m_EquipmentCode;
	CString m_WorkStation_Code;
//	afx_msg void OnEnChangeEditEquipmentCode();
//	afx_msg void OnEnChangeEditEquipmentAddr();
	CString m_WorkStation_Name;
	CString m_WorkSheet_Code;
	CString m_Product_Code;
	CString m_Product_Name;
};
