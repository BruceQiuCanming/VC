// AdjustWizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "AdjustWizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjustWizardDlg dialog


CAdjustWizardDlg::CAdjustWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdjustWizardDlg::IDD, pParent)
{
	for(int i = 0; i < 4; i++)
	{
		m_MeterTemp[i] = 0.0f;
		m_GlassTemp[i] = 0.0f;
		m_AdjustLow[i] = 0.0f;
		m_AdjustHigh[i] = 0.0f;
	}
	//{{AFX_DATA_INIT(CAdjustWizardDlg)
	m_Msg = _T("");
	//}}AFX_DATA_INIT
}


void CAdjustWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_METER_TEMP_1, m_MeterTemp[0]);
	DDX_Text(pDX, IDC_EDIT_METER_TEMP_2, m_MeterTemp[1]);
	DDX_Text(pDX, IDC_EDIT_METER_TEMP_3, m_MeterTemp[2]);
	DDX_Text(pDX, IDC_EDIT_METER_TEMP_4, m_MeterTemp[3]);
	DDX_Text(pDX, IDC_EDIT_GLASS_TEMP_1, m_GlassTemp[0]);
	DDX_Text(pDX, IDC_EDIT_GLASS_TEMP_2, m_GlassTemp[1]);
	DDX_Text(pDX, IDC_EDIT_GLASS_TEMP_3, m_GlassTemp[2]);
	DDX_Text(pDX, IDC_EDIT_GLASS_TEMP_4, m_GlassTemp[3]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_HIGH_1,m_AdjustHigh[0]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_HIGH_2,m_AdjustHigh[1]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_HIGH_3,m_AdjustHigh[2]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_HIGH_4,m_AdjustHigh[3]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_LOW_1,m_AdjustLow[0]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_LOW_2,m_AdjustLow[1]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_LOW_3,m_AdjustLow[2]);
	DDX_Text(pDX, IDC_EDIT_METER_ADJUS_LOW_4,m_AdjustLow[3]);
	DDX_Text(pDX, IDC_EDIT_MSG, m_Msg);
	DDX_Control(pDX, IDC_EDIT_GLASS_TEMP_1, m_GlassTempEdit[0]);
	DDX_Control(pDX, IDC_EDIT_GLASS_TEMP_2, m_GlassTempEdit[1]);
	DDX_Control(pDX, IDC_EDIT_GLASS_TEMP_3, m_GlassTempEdit[2]);
	DDX_Control(pDX, IDC_EDIT_GLASS_TEMP_4, m_GlassTempEdit[3]);
	//{{AFX_DATA_MAP(CAdjustWizardDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjustWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CAdjustWizardDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdjustWizardDlg message handlers
