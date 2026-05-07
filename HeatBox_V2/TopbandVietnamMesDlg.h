#pragma once


// CTopbandVietnamMesDlg 对话框

class CTopbandVietnamMesDlg : public CDialog
{
	DECLARE_DYNAMIC(CTopbandVietnamMesDlg)

public:
	CTopbandVietnamMesDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTopbandVietnamMesDlg();

// 对话框数据
	enum { IDD = IDD_TOPBAND_VIATNAM_MES_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_URL_1;
	CString m_URL_2;
	afx_msg void OnEnChangeEditUrl1();
	afx_msg void OnEnChangeEditUrl2();
	CString m_Site;
	CString m_Resrce;
	CString m_Barcode;
	CString m_Password;
	CString m_Operation;
	CString m_Userid;
	CString m_ShopOrder;

	afx_msg void OnEnSetfocusEditshoporder();

	void SaveConfig(void);
	CString m_Sfcs;
	afx_msg void OnEnSetfocusEditUrl1();
	afx_msg void OnEnSetfocusEditUrl2();
	afx_msg void OnEnSetfocusEditsite();
	afx_msg void OnEnSetfocusEditoperation();
	afx_msg void OnEnSetfocusEditresrce();
	afx_msg void OnEnSetfocusEditsfcs();
	afx_msg void OnEnSetfocusEditbarcode();
	afx_msg void OnEnSetfocusEdituserid();
	afx_msg void OnEnSetfocusEditpassword();
	afx_msg void OnBnClickedButtonUpload();

	static void MES_Collection(int BoxNr,RECORD *record,int start_Nr,int end_Nr);
	void MES_Collection_TEST(void);
	static void MES_Check(void);
};
