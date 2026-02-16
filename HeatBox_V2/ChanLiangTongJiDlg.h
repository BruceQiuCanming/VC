#pragma once
#include "resource.h"
#include "afxdtctl.h"

// CChangLiangTongJi 对话框

class CChanLiangTongJiDlg : public CDialog
{
	DECLARE_DYNAMIC(CChanLiangTongJiDlg)

public:
	CChanLiangTongJiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChanLiangTongJiDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHANLIANG_TONGJI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonView();
	int		m_BoxNr;

	CMonthCalCtrl m_StartTime;
	CMonthCalCtrl m_EndTime;

	HEAT_COOL_ORDER		m_HeatCoolType;
	CONTROL_TEMP_RANGE	m_TempRange;
	SWITCH_TEST_RESULT	m_SwitchTestResult_Ex;
	BOX_TYPE			m_BoxType;

};
