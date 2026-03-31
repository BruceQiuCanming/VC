#pragma once


// CDongguanBetterMesMsgDlg 对话框

class CDongguanBetterMesMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CDongguanBetterMesMsgDlg)

public:
	CDongguanBetterMesMsgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDongguanBetterMesMsgDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DONGGUAN_BETTER_MES_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
