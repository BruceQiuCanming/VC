// ParaRecordset.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "ParaRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParaRecordset

IMPLEMENT_DYNAMIC(CParaRecordset, CDaoRecordset)

CParaRecordset::CParaRecordset(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CParaRecordset)
	m_para_name = "";
	m_ParaType	=0;
	m_MainLevel = 0.0;
	m_MainLevelSpanMinus = 0.0;
	m_MainLevelSpanPlus  = 0.0;
	m_HighLevel = 0.0;
	m_HighLevelSpanMinus = 0.0;
	m_HighLevelSpanPlus = 0.0;
	m_LowLevel = 0.0;
	m_LowLevelSpanMinus = 0.0;
	m_LowLevelSpanPlus = 0.0;
	m_CloseMainLevel = 0.0;
	m_CloseMainLevelSpanPlus = 0.0;
	m_CloseMainLevelSpanMinus = 0.0;
	m_CloseLowLevel = 0.0;
	m_CloseLowLevelSpanPlus = 0.0;
	m_CloseLowLevelSpanMinus = 0.0;
	m_CloseHighLevel = 0.0;
	m_CloseHighLevelSpanPlus = 0.0;
	m_CloseHighLevelSpanMinus = 0.0;
	m_OpenDoorResetLevel = 0;
	m_TakeoutTime = 0;
	m_nFields = 22;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;//dbOpenTable;
}


CString CParaRecordset::GetDefaultDBName()
{
	CString mdbFileName;
	mdbFileName.Format(_T("%s.mdb"),AfxGetApp()->m_pszExeName); 

	return (theAppDirectory+mdbFileName);
}

CString CParaRecordset::GetDefaultSQL()
{
	return _T("[switch_test_para]");
}

void CParaRecordset::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CParaRecordset)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[para_name]"), m_para_name);
	DFX_Long(pFX, _T("[ParaType]"), m_ParaType);

	DFX_Double(pFX, _T("[MainLevel]"), m_MainLevel);
	DFX_Double(pFX, _T("[MainLevelSpanMinus]"), m_MainLevelSpanMinus);
	DFX_Double(pFX, _T("[MainLevelSpanPlus]"), m_MainLevelSpanPlus);
	DFX_Double(pFX, _T("[HighLevel]"), m_HighLevel);
	DFX_Double(pFX, _T("[HighLevelSpanMinus]"), m_HighLevelSpanMinus);
	DFX_Double(pFX, _T("[HighLevelSpanPlus]"), m_HighLevelSpanPlus);
	DFX_Double(pFX, _T("[LowLevel]"), m_LowLevel);
	DFX_Double(pFX, _T("[LowLevelSpanMinus]"), m_LowLevelSpanMinus);
	DFX_Double(pFX, _T("[LowLevelSpanPlus]"), m_LowLevelSpanPlus);
	DFX_Double(pFX, _T("[CloseMainLevel]"),		    m_CloseMainLevel);
	DFX_Double(pFX, _T("[CloseMainLevelSpanPlus]"),	m_CloseMainLevelSpanPlus);
	DFX_Double(pFX, _T("[CloseMainLevelSpanMinus]"),m_CloseMainLevelSpanMinus);
	DFX_Double(pFX, _T("[CloseHighLevel]"),		    m_CloseHighLevel);
	DFX_Double(pFX, _T("[CloseHighLevelSpanPlus]"),	m_CloseHighLevelSpanPlus);
	DFX_Double(pFX, _T("[CloseHighLevelSpanMinus]"),m_CloseHighLevelSpanMinus);
	DFX_Double(pFX, _T("[CloseLowLevel]"),		    m_CloseLowLevel);
	DFX_Double(pFX, _T("[CloseLowLevelSpanPlus]"),	m_CloseLowLevelSpanPlus);
	DFX_Double(pFX, _T("[CloseLowLevelSpanMinus]"), m_CloseLowLevelSpanMinus);
	
	DFX_Long(pFX, _T("[OpenDoorResetLevel]"), m_OpenDoorResetLevel);
	DFX_Long(pFX, _T("[TakeoutTime]"), m_TakeoutTime);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CParaRecordset diagnostics

#ifdef _DEBUG
void CParaRecordset::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CParaRecordset::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
