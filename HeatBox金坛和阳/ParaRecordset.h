#if !defined(AFX_PARARECORDSET_H__C6787875_DDA5_4A33_B35F_FA6B2FC315D1__INCLUDED_)
#define AFX_PARARECORDSET_H__C6787875_DDA5_4A33_B35F_FA6B2FC315D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaRecordset.h : header file
//

#include <afxdao.h>
/////////////////////////////////////////////////////////////////////////////
// CParaRecordset DAO recordset

class CParaRecordset : public CDaoRecordset
{
public:
	CParaRecordset(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CParaRecordset)
	
// Field/Param Data
	//{{AFX_FIELD(CParaRecordset, CDaoRecordset)
	CString	m_para_name;
	long	m_ParaType;
	double	m_MainLevel;
	double	m_MainLevelSpanMinus;
	double	m_MainLevelSpanPlus;
	double	m_HighLevel;
	double	m_HighLevelSpanMinus;
	double	m_HighLevelSpanPlus;
	double	m_LowLevel;
	double	m_LowLevelSpanMinus;
	double	m_LowLevelSpanPlus;
	double	m_CloseMainLevel;
	double	m_CloseMainLevelSpanPlus;
	double	m_CloseMainLevelSpanMinus;
	double	m_CloseHighLevel;
	double	m_CloseHighLevelSpanPlus;
	double	m_CloseHighLevelSpanMinus;
	double	m_CloseLowLevel;
	double	m_CloseLowLevelSpanPlus;
	double	m_CloseLowLevelSpanMinus;
	long	m_OpenDoorResetLevel;
	long	m_TakeoutTime;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaRecordset)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARARECORDSET_H__C6787875_DDA5_4A33_B35F_FA6B2FC315D1__INCLUDED_)
