#pragma once


// CKeyBoardDlg 对话框

class CKeyBoardDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyBoardDlg)

public:
	CKeyBoardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeyBoardDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_KEYBOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton0();

	afx_msg void OnBnClickedButtonA();
	afx_msg void OnBnClickedButtonB();
	afx_msg void OnBnClickedButtonC();
	afx_msg void OnBnClickedButtonD();
	afx_msg void OnBnClickedButtonE();
	afx_msg void OnBnClickedButtonF();
	afx_msg void OnBnClickedButtonG();
	afx_msg void OnBnClickedButtonH();
	afx_msg void OnBnClickedButtonI();
	afx_msg void OnBnClickedButtonJ();
	afx_msg void OnBnClickedButtonK();

	afx_msg void OnBnClickedButtonL();
	afx_msg void OnBnClickedButtonM();
	afx_msg void OnBnClickedButtonN();
	afx_msg void OnBnClickedButtonO();
	afx_msg void OnBnClickedButtonP();
	afx_msg void OnBnClickedButtonQ();
	afx_msg void OnBnClickedButtonR();
	afx_msg void OnBnClickedButtonS();
	afx_msg void OnBnClickedButtonT();

	afx_msg void OnBnClickedButtonU();
	afx_msg void OnBnClickedButtonV();
	afx_msg void OnBnClickedButtonW();
	afx_msg void OnBnClickedButtonX();
	afx_msg void OnBnClickedButtonY();
	afx_msg void OnBnClickedButtonZ();


	afx_msg void OnBnClickedButton_PLUS();
	afx_msg void OnBnClickedButton_MINUS();
	afx_msg void OnBnClickedButton_DOT();
	afx_msg void OnBnClickedButton_CLEAR();

	CString m_Input;
	CString m_Title;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
