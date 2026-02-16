#include "afxwin.h"
#if !defined(AFX_CREATEMODIFYPARADLG_H__C5C568D4_5789_4CBA_957B_EA619F4DCC57__INCLUDED_)
#define AFX_CREATEMODIFYPARADLG_H__C5C568D4_5789_4CBA_957B_EA619F4DCC57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateModifyParaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateModifyParaDlg dialog

class CCreateModifyParaDlg : public CDialog
{
// Construction
public:
	void UpdateParaNameList(void);
	CCreateModifyParaDlg(CWnd* pParent = NULL);   // standard constructor
	bool m_bSetFocusOnEdit;
// Dialog Data
	//{{AFX_DATA(CCreateModifyParaDlg)
	enum { IDD = IDD_DIALOG_CREAT_MODIFY_PARA };
	CComboBox	m_ComboOpenDoorResetLevel;
	CListBox	m_ListParaName;
	double	m_HighLevelSpanMinus;
	double	m_HighLevelSpanPlus;
	double	m_HighLevel;
	double	m_CloseMainLevel;
	double	m_CloseMainLevelSpanPlus;
	double	m_CloseMainLevelSpanMinus;
	double	m_CloseMainLevelDeviationPlus;
	double	m_CloseMainLevelDeviationMinus;
	double	m_CloseHighLevel;
	double	m_CloseHighLevelSpanPlus;
	double	m_CloseHighLevelSpanMinus;
	double	m_CloseHighLevelDeviationPlus;
	double	m_CloseHighLevelDeviationMinus;
	double	m_CloseLowLevel;
	double	m_CloseLowLevelSpanPlus;
	double	m_CloseLowLevelSpanMinus;
	double	m_CloseLowLevelDeviationPlus;
	double	m_CloseLowLevelDeviationMinus;
	double	m_LowLevel;
	double	m_LowLevelSpanMinus;
	double	m_LowLevelSpanPlus;
	double	m_MainLevel;
	double	m_MainLevelSpanMinus;
	double	m_MainLevelSpanPlus;
	UINT	m_TakeoutTime;
	CString	m_ParaName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateModifyParaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateModifyParaDlg)
	afx_msg void OnSave();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListParaName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboParaType;
	afx_msg void OnCbnSelchangeComboParaType();
	afx_msg void OnEnSetfocusEditMainLevel();
	void OnEnSetfocusEdit(UINT id);
	afx_msg void OnCbnSetfocusComboParaType();
	afx_msg void OnEnSetfocusEditParaName();
	afx_msg void OnEnKillfocusEditParaName();
	void ShowItems(int para_type);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEMODIFYPARADLG_H__C5C568D4_5789_4CBA_957B_EA619F4DCC57__INCLUDED_)
