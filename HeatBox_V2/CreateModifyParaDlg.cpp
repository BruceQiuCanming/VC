// CreateModifyParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "CreateModifyParaDlg.h"
//#include "ParaRecordset.h"
#include "InputDlg.h"
#include "shlwapi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateModifyParaDlg dialog


CCreateModifyParaDlg::CCreateModifyParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateModifyParaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateModifyParaDlg)
	m_HighLevelSpanMinus = 0.0f;
	m_HighLevelSpanPlus = 0.0f;
	m_HighLevel = 0.0f;
	m_CloseMainLevel = 0.0f;
	m_CloseMainLevelSpanPlus = 0.0f;
	m_CloseMainLevelSpanMinus= 0.0f;
	m_CloseMainLevelDeviationPlus = 0.0f;
	m_CloseMainLevelDeviationMinus= 0.0f;
	m_CloseHighLevel = 0.0f;
	m_CloseHighLevelSpanPlus = 0.0f;
	m_CloseHighLevelSpanMinus= 0.0f;
	m_CloseHighLevelDeviationPlus = 0.0f;
	m_CloseHighLevelDeviationMinus= 0.0f;
	m_CloseLowLevel = 0.0f;
	m_CloseLowLevelSpanPlus = 0.0f;
	m_CloseLowLevelSpanMinus= 0.0f;
	m_CloseLowLevelDeviationPlus = 0.0f;
	m_CloseLowLevelDeviationMinus= 0.0f;
	m_LowLevel = 0.0f;
	m_LowLevelSpanMinus = 0.0f;
	m_LowLevelSpanPlus = 0.0f;
	m_MainLevel = 0.0f;
	m_MainLevelSpanMinus = 0.0f;
	m_MainLevelSpanPlus = 0.0f;
	m_TakeoutTime = 0;
	m_ParaName = _T("");

	m_MainLevelAvrSpanMinus				=	0;
	m_MainLevelAvrSpanPlus				=	0;
	m_MainLevelCloseAvrSpanMinus		=	0;
	m_MainLevelCLoseAvrSpanPlus			=	0;

	m_HighLevelAvrSpanMinus				=	0;
	m_HighLevelAvrSpanPlus				=	0;
	m_HighLevelCloseAvrSpanMinus		=	0;
	m_HighLevelCLoseAvrSpanPlus			=	0;

	m_LowLevelAvrSpanMinus				=	0;
	m_LowLevelAvrSpanPlus				=	0;
	m_LowLevelCloseAvrSpanMinus			=	0;
	m_LowLevelCLoseAvrSpanPlus			=	0;
	//}}AFX_DATA_INIT
}


void CCreateModifyParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateModifyParaDlg)
	DDX_Control(pDX, IDC_COMBO_OPEN_DOOR_RESET_LEVEL, m_ComboOpenDoorResetLevel);
	DDX_Control(pDX, IDC_COMBO_PARA_TYPE, m_ComboParaType);
	DDX_Control(pDX, IDC_LIST_PARA_NAME, m_ListParaName);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_SPAN_MINUS, m_HighLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_SPAN_PLUS, m_HighLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL, m_HighLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL, m_CloseMainLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS, m_CloseMainLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS, m_CloseMainLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2, m_CloseMainLevelDeviationPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2, m_CloseMainLevelDeviationMinus);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL, m_LowLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_SPAN_MINUS, m_LowLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_SPAN_PLUS, m_LowLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL, m_MainLevel);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL_SPAN_MINUS, m_MainLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL_SPAN_PLUS, m_MainLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_TAKEOUT_TIME, m_TakeoutTime);
	DDX_Text(pDX, IDC_EDIT_PARA_NAME, m_ParaName);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL, m_CloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS, m_CloseHighLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS, m_CloseHighLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS2, m_CloseHighLevelDeviationPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS2, m_CloseHighLevelDeviationMinus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL, m_CloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS, m_CloseLowLevelSpanPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS, m_CloseLowLevelSpanMinus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2, m_CloseLowLevelDeviationPlus);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS2, m_CloseLowLevelDeviationMinus);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_AVR_SPAN_MINUS,		m_MainLevelAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_AVR_SPAN_PLUS,		m_MainLevelAvrSpanPlus);
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_MINUS,	m_MainLevelCloseAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_PLUS,	m_MainLevelCLoseAvrSpanPlus);

	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_AVR_SPAN_MINUS,		m_HighLevelAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_AVR_SPAN_PLUS,		m_HighLevelAvrSpanPlus);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_AVR_SPAN_MINUS,	m_HighLevelCloseAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_AVR_SPAN_PLUS,	m_HighLevelCLoseAvrSpanPlus);

	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_AVR_SPAN_MINUS,		m_LowLevelAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_AVR_SPAN_PLUS,			m_LowLevelAvrSpanPlus);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_AVR_SPAN_MINUS,	m_LowLevelCloseAvrSpanMinus);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_AVR_SPAN_PLUS,	m_LowLevelCLoseAvrSpanPlus);
}


BEGIN_MESSAGE_MAP(CCreateModifyParaDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateModifyParaDlg)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_LIST_PARA_NAME, OnSelchangeListParaName)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_PARA_TYPE, &CCreateModifyParaDlg::OnCbnSelchangeComboParaType)
	ON_CONTROL_RANGE(EN_SETFOCUS,IDC_EDIT_MAIN_LEVEL,IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2,OnEnSetfocusEdit)
	ON_CONTROL_RANGE(EN_SETFOCUS,IDC_EDIT_LOW_LEVEL_AVR_SPAN_MINUS,IDC_EDIT_MIAN_LEVEL_AVR_SPAN_PLUS,OnEnSetfocusEdit_2)
	ON_CBN_SETFOCUS(IDC_COMBO_PARA_TYPE, &CCreateModifyParaDlg::OnCbnSetfocusComboParaType)
	ON_EN_SETFOCUS(IDC_EDIT_PARA_NAME, &CCreateModifyParaDlg::OnEnSetfocusEditParaName)
	ON_EN_KILLFOCUS(IDC_EDIT_PARA_NAME, &CCreateModifyParaDlg::OnEnKillfocusEditParaName)
	ON_EN_CHANGE(IDC_EDIT_LOW_LEVEL_SPAN_PLUS5, &CCreateModifyParaDlg::OnEnChangeEditLowLevelSpanPlus5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateModifyParaDlg message handlers

void CCreateModifyParaDlg::OnSave() 
{
	// TODO: Add your control notification handler code here

	
	CInputDlg dlg_psw;

		
	dlg_psw.m_Title = _T("请输入密码");
	if(dlg_psw.DoModal() == IDOK)
	{
		if(_ttoi( dlg_psw.m_Input) == ::G_NormalConfigPara[0].Para_Password)
		{
		}
		else
		{
			AfxMessageBox(_T("密码错误!"));
			return;
		}
	}
	else
	{
		return;
	}


	this->UpdateData(); 

	m_ParaName.Trim();

	if(m_ParaName.GetLength() == 0)
	{
		AfxMessageBox(_T("参数名不能为空!"));
		return;
	}

	double a,b;

	if(m_LowLevelSpanMinus != 0 && m_LowLevelSpanPlus)
	{
		a = (m_LowLevel + m_LowLevelSpanPlus); 
		b = (m_MainLevel - m_MainLevelSpanMinus);
		if(a < b)
		{
			CString s;
			s.Format(_T("温度设置值不连续\r\n\r\n低规格和主规格之间有温度间隙\r\n\r\n低规格的最高温度是 %5.1f\r\n\r\n主规格的最低温度是 %5.1f"),a,b); 
			AfxMessageBox(s,NULL,MB_ICONERROR);
			return;
		}
	}
	
	if(m_HighLevelSpanMinus != 0 && m_HighLevelSpanPlus)
	{
		a = (m_MainLevel + m_MainLevelSpanPlus); 
		b = (m_HighLevel - m_HighLevelSpanMinus);
		if( a < b )
		{
			CString s;
			s.Format(_T("温度设置值不连续\r\n\r\n主规格和高规格之间有温度间隙\r\n\r\n主规格的最高温度是 %5.1f\r\n\r\n高规格的最低温度是 %5.1f"),a,b); 
			AfxMessageBox(s,NULL,MB_ICONERROR);
			return;
		}
	}
	

	CFile ParaFile;
	SWITCH_CONFIG_PARA_ALL		para_all;
	CString sDir;
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	if(ParaFile.Open(sDir + m_ParaName + _T(".para"),CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
	{

		para_all.para.OpenDoorResetLevel=	m_ComboOpenDoorResetLevel.GetCurSel(); 
		if(para_all.para.OpenDoorResetLevel < 0 || para_all.para.OpenDoorResetLevel > 10)
		{
			para_all.para.OpenDoorResetLevel = 0;
		}
		para_all.para.CloseTempType	=	m_ComboParaType.GetCurSel(); 
		if(para_all.para.CloseTempType < 0 || para_all.para.CloseTempType > 2)
		{
			para_all.para.CloseTempType = 0;
		}
		para_all.para.MainLevel					=	m_MainLevel;
		para_all.para.MainLevelSpanMinus		=	m_MainLevelSpanMinus;
		para_all.para.MainLevelSpanPlus			=	m_MainLevelSpanPlus;
		para_all.para.CloseLevel				=	m_CloseMainLevel;
		para_all.para.CloseLevelSpanPlus		=	m_CloseMainLevelSpanPlus;
		para_all.para.CloseLevelSpanMinus		=	m_CloseMainLevelSpanMinus;
		para_all.para.CloseLevelDiffPlus	=	m_CloseMainLevelDeviationPlus;
		para_all.para.CloseLevelDiffMinus	=	m_CloseMainLevelDeviationMinus;

#ifdef _CHENG_HONG_
		para.HighLevelSpanMinus				=	0;
		para.HighLevelSpanPlus				=	0;
		
		para.HighLevel						=	0;
		
		para.CloseLowLevel					=	0;
		para.CloseLowLevelSpanPlus			=	0;
		para.CloseLowLevelSpanMinus			=	0;
		para.CloseLowLevelDiffPlus		=	0;
		para.CloseLowLevelDiffMinus	=	0;

		para.CloseHighLevel					=	0;
		para.CloseHighLevelSpanPlus			=	0;
		para.CloseHighLevelSpanMinus		=	0;
		para.CloseHighLevelDiffPlus	=	0;
		para.CloseHighLevelDiffMinus	=	0;


		
		para.LowLevel			=	0;
		para.LowLevelSpanMinus	=	0;
		para.LowLevelSpanPlus	=	0;
		para.TakeoutTime		=	0;
#else
		para_all.para.LowLevel						=	m_LowLevel;
		para_all.para.LowLevelSpanMinus				=	m_LowLevelSpanMinus;
		para_all.para.LowLevelSpanPlus				=	m_LowLevelSpanPlus;
		para_all.para.CloseLowLevel					=	m_CloseLowLevel;
		para_all.para.CloseLowLevelSpanPlus			=	m_CloseLowLevelSpanPlus;
		para_all.para.CloseLowLevelSpanMinus		=	m_CloseLowLevelSpanMinus;
		para_all.para.CloseLowLevelDiffPlus	=	m_CloseLowLevelDeviationPlus;
		para_all.para.CloseLowLevelDiffMinus	=	m_CloseLowLevelDeviationMinus;

		
		para_all.para.HighLevel						=	m_HighLevel;;
		para_all.para.HighLevelSpanPlus				=	m_HighLevelSpanPlus;
		para_all.para.HighLevelSpanMinus			=	m_HighLevelSpanMinus;
		para_all.para.CloseHighLevel				=	m_CloseHighLevel;
		para_all.para.CloseHighLevelSpanPlus		=	m_CloseHighLevelSpanPlus;
		para_all.para.CloseHighLevelSpanMinus		=	m_CloseHighLevelSpanMinus;
		para_all.para.CloseHighLevelDiffPlus	=	m_CloseHighLevelDeviationPlus;
		para_all.para.CloseHighLevelDiffMinus	=	m_CloseHighLevelDeviationMinus;

		
		
//		para_all.para.TakeoutTime					=	m_TakeoutTime;

		para_all.para_ex.MainLevelAvrSpanMinus		=	m_MainLevelAvrSpanMinus;
		para_all.para_ex.MainLevelAvrSpanPlus		=	m_MainLevelAvrSpanPlus;
		para_all.para_ex.MainLevelCloseAvrSpanMinus	=	m_MainLevelCloseAvrSpanMinus;
		para_all.para_ex.MainLevelCloseAvrSpanPlus	=	m_MainLevelCLoseAvrSpanPlus;

		para_all.para_ex.HighLevelAvrSpanMinus		=	m_HighLevelAvrSpanMinus;
		para_all.para_ex.HighLevelAvrSpanPlus		=	m_HighLevelAvrSpanPlus;
		para_all.para_ex.HighLevelCloseAvrSpanMinus	=	m_HighLevelCloseAvrSpanMinus;
		para_all.para_ex.HighLevelCloseAvrSpanPlus	=	m_HighLevelCLoseAvrSpanPlus;

		para_all.para_ex.LowLevelAvrSpanMinus		=	m_LowLevelAvrSpanMinus ;
		para_all.para_ex.LowLevelAvrSpanPlus		=	m_LowLevelAvrSpanPlus;
		para_all.para_ex.LowLevelCloseAvrSpanMinus	=	m_LowLevelCloseAvrSpanMinus;
		para_all.para_ex.LowLevelCloseAvrSpanPlus	=	m_LowLevelCLoseAvrSpanPlus;
#endif
		ParaFile.Write(&para_all,sizeof(para_all));
		
	}


	/*
	CParaRecordset para_rs;
	para_rs.Open();
	if(para_rs.IsEOF() && para_rs.IsBOF())
	{
	}
	else
	{
		para_rs.MoveFirst();
	}
	while(!para_rs.IsEOF())
	{
		para_rs.m_para_name.TrimRight();
		para_rs.m_para_name.TrimLeft();
		m_ParaName.TrimRight();
		m_ParaName.TrimLeft();
		if(para_rs.m_para_name == this->m_ParaName)
		{
			if(AfxMessageBox(_T("已经有相同的参数名，是否覆盖原来的？"),MB_YESNO | MB_ICONQUESTION,NULL) == IDYES)
			{
				para_rs.Edit();
				para_rs.m_OpenDoorResetLevel=	m_ComboOpenDoorResetLevel.GetCurSel(); 
				if(para_rs.m_OpenDoorResetLevel < 0 || para_rs.m_OpenDoorResetLevel > 10)
				{
					para_rs.m_OpenDoorResetLevel = 0;
				}
				para_rs.m_ParaType	=	m_ComboParaType.GetCurSel(); 
				if(para_rs.m_ParaType < 0 || para_rs.m_ParaType > 1)
				{
					para_rs.m_ParaType = 0;
				}
				para_rs.m_HighLevelSpanMinus=	m_HighLevelSpanMinus;
				para_rs.m_HighLevelSpanPlus =	m_HighLevelSpanPlus;
				para_rs.m_HighLevel			=	m_HighLevel;
				para_rs.m_CloseMainLevel		=	m_CloseMainLevel;
				para_rs.m_CloseMainLevelSpanPlus=	m_CloseMainLevelSpanPlus;
				para_rs.m_CloseMainLevelSpanMinus=	m_CloseMainLevelSpanMinus;
				para_rs.m_CloseLowLevel			=	m_CloseLowLevel;
				para_rs.m_CloseLowLevelSpanPlus=	m_CloseLowLevelSpanPlus;
				para_rs.m_CloseLowLevelSpanMinus=	m_CloseLowLevelSpanMinus;
				para_rs.m_CloseHighLevel		=	m_CloseHighLevel;
				para_rs.m_CloseHighLevelSpanPlus=	m_CloseHighLevelSpanPlus;
				para_rs.m_CloseHighLevelSpanMinus=	m_CloseHighLevelSpanMinus;
				
				para_rs.m_LowLevel			=	m_LowLevel;
				para_rs.m_LowLevelSpanMinus	=	m_LowLevelSpanMinus;
				para_rs.m_LowLevelSpanPlus	=	m_LowLevelSpanPlus;
				para_rs.m_MainLevel			=	m_MainLevel;
				para_rs.m_MainLevelSpanMinus=	m_MainLevelSpanMinus;
				para_rs.m_MainLevelSpanPlus =	m_MainLevelSpanPlus;
				para_rs.m_TakeoutTime		=	m_TakeoutTime;
				para_rs.Update();
				para_rs.Close(); 
				return;
			}
			para_rs.Close(); 
			return;
		}
		para_rs.MoveNext(); 
	}
	
	para_rs.AddNew();
	para_rs.m_para_name = this->m_ParaName;
	
//	CListBox	m_ListParaName;
	para_rs.m_OpenDoorResetLevel	= m_ComboOpenDoorResetLevel.GetCurSel(); 
	if(para_rs.m_OpenDoorResetLevel < 0 || para_rs.m_OpenDoorResetLevel > 5)
	{
		para_rs.m_OpenDoorResetLevel = 0;
	}

	para_rs.m_ParaType	=	m_ComboParaType.GetCurSel(); 
	if(para_rs.m_ParaType < 0 || para_rs.m_ParaType > 1)
	{
		para_rs.m_ParaType = 0;
	}

	para_rs.m_HighLevelSpanMinus	=	m_HighLevelSpanMinus;
	para_rs.m_HighLevelSpanPlus		=	m_HighLevelSpanPlus;
	para_rs.m_HighLevel				=	m_HighLevel;
	para_rs.m_CloseMainLevel			=	m_CloseMainLevel;
	para_rs.m_CloseMainLevelSpanPlus	=	m_CloseMainLevelSpanPlus;
	para_rs.m_CloseMainLevelSpanMinus	=	m_CloseMainLevelSpanMinus;
	para_rs.m_LowLevel				=	m_LowLevel;
	para_rs.m_LowLevelSpanMinus		=	m_LowLevelSpanMinus;
	para_rs.m_LowLevelSpanPlus		=	m_LowLevelSpanPlus;
	para_rs.m_MainLevel =	m_MainLevel;
	para_rs.m_MainLevelSpanMinus =	m_MainLevelSpanMinus;
	para_rs.m_MainLevelSpanPlus =	m_MainLevelSpanPlus;
	para_rs.m_TakeoutTime =	m_TakeoutTime;

	para_rs.m_CloseHighLevel			=	m_CloseHighLevel;
	para_rs.m_CloseHighLevelSpanPlus	=	m_CloseHighLevelSpanPlus;
	para_rs.m_CloseHighLevelSpanMinus	=	m_CloseHighLevelSpanMinus;
	
	para_rs.m_CloseLowLevel				=	m_CloseLowLevel;
	para_rs.m_CloseLowLevelSpanPlus		=	m_CloseLowLevelSpanPlus;
	para_rs.m_CloseLowLevelSpanMinus	=	m_CloseLowLevelSpanMinus;
	

	para_rs.Update(); 
	para_rs.Close(); 
	*/

	UpdateParaNameList();
	AfxMessageBox(_T("参数添加或更新成功!"));

}
void CCreateModifyParaDlg::Show_BOX_128_ONLY_OPENTEMP_TEST(void)
{
	if(::G_NormalConfigPara[m_BoxNr].BoxType == ::BOX_128_ONLY_OPENTEMP_TEST)
	{
		CWnd *w;
		w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_AVR_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_AVR_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_HIDE);
			


		w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_HIDE);


		w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_STATIC_MAIN_LEVEL_OPEN_CENTER_SPAN);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_CLOSE_AVR3);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_STATIC_MAIN_LEVEL_OPEN_CLOSE_SPAN);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_OPEN_CLOSE_SPAN);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_OPEN_CLOSE_SPAN);
		w->ShowWindow(SW_HIDE);
		
		w = this->GetDlgItem(IDC_STATIC_GROUP_MAIN);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_STATIC_GROUP_HIGH);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_STATIC_GROUP_LOW);
		w->ShowWindow(SW_HIDE);
		

		w = this->GetDlgItem(IDC_STATIC_MAIN_LEVEL_OPEN);
		w->SetWindowTextW(_T("低温品"));
		w = this->GetDlgItem(IDC_STATIC_MAIN_LEVEL_CLOSE);
		w->SetWindowTextW(_T("主规格"));
		w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_OPEN);
		w->SetWindowTextW(_T("高规格1"));
		w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_CLOSE_1);
		w->SetWindowTextW(_T("高规格2"));
		w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_OPEN);
		w->SetWindowTextW(_T("高规格3"));
		w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_CLOSE_1);
		w->SetWindowTextW(_T("高规格4"));

		
		
	}
}

BOOL CCreateModifyParaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bSetFocusOnEdit = true;

	// TODO: Add extra initialization here
	this->m_ComboOpenDoorResetLevel.SetCurSel(3);  
	m_ComboParaType.SetCurSel(0);
	this->UpdateParaNameList();
	

	Show_BOX_128_ONLY_OPENTEMP_TEST();

#ifdef _CHENG_HONG_	
	CWnd *w;
	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_1);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_1);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_OPEN);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_CLOSE_1);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_HIGH_LEVEL_CLOSE_2);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_OPEN);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_CLOSE_1);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_STATIC_LOW_LEVEL_CLOSE_2);
	w->ShowWindow(SW_HIDE);

	w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_SPAN_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_HIGH_LEVEL_SPAN_PLUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS2);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS2);
	w->ShowWindow(SW_HIDE);

	w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL_SPAN_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_LOW_LEVEL_SPAN_PLUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS2);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2);
	w->ShowWindow(SW_HIDE);

	w = this->GetDlgItem(IDC_STATIC_OPEN_RESET);
	w->ShowWindow(SW_HIDE);
	w = this->GetDlgItem(IDC_COMBO_OPEN_DOOR_RESET_LEVEL);
	w->ShowWindow(SW_HIDE);

	
		
	
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateModifyParaDlg::OnSelchangeListParaName() 
{
	// TODO: Add your control notification handler code here

	CFile ParaFile;
	CFileFind ff;
	//SWITCH_CONFIG_PARA		para;
	//SWITCH_CONFIG_PARA_EX	para_ex;
	SWITCH_CONFIG_PARA_ALL		para_all;
	CString fName;
	CString sDir;

	
	sDir = theAppDirectory +_T("para\\");

	
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	//TraverseFolder(sDir,_T("*.para"),&sa);


	{
		CString para_name;
		m_ListParaName.GetText(m_ListParaName.GetCurSel(),para_name);

		//para_name += _T(".para");
	
		if(ParaFile.Open(sDir + para_name + _T(".para"),CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
		{
			
			memset(&para_all,0,sizeof(para_all));
			
			if(ParaFile.Read(&para_all,sizeof(SWITCH_CONFIG_PARA_ALL)) <= sizeof(SWITCH_CONFIG_PARA_ALL)) 
			{
				
				this->m_ComboParaType.SetCurSel(para_all.para.CloseTempType); 
				m_ComboOpenDoorResetLevel.SetCurSel(para_all.para.OpenDoorResetLevel); 
				m_HighLevelSpanMinus			= para_all.para.HighLevelSpanMinus;
				m_HighLevelSpanPlus				= para_all.para.HighLevelSpanPlus;
				m_HighLevel						= para_all.para.HighLevel;
				m_CloseMainLevel				= para_all.para.CloseLevel;
				m_CloseMainLevelSpanPlus		= para_all.para.CloseLevelSpanPlus;
				m_CloseMainLevelSpanMinus		= para_all.para.CloseLevelSpanMinus;
				m_CloseMainLevelDeviationPlus	= para_all.para.CloseLevelDiffPlus;
				m_CloseMainLevelDeviationMinus	= para_all.para.CloseLevelDiffMinus;

				m_LowLevel					= para_all.para.LowLevel;
				m_LowLevelSpanMinus			= para_all.para.LowLevelSpanMinus;
				m_LowLevelSpanPlus			= para_all.para.LowLevelSpanPlus;
				
				m_MainLevel					= para_all.para.MainLevel;
				m_MainLevelSpanMinus		= para_all.para.MainLevelSpanMinus;
				m_MainLevelSpanPlus			= para_all.para.MainLevelSpanPlus;
//				m_TakeoutTime				= para_all.para.TakeoutTime;
				this->m_ParaName			= para_name; 

				m_CloseHighLevel			= para_all.para.CloseHighLevel;
				m_CloseHighLevelSpanPlus	= para_all.para.CloseHighLevelSpanPlus;
				m_CloseHighLevelSpanMinus	= para_all.para.CloseHighLevelSpanMinus;
				m_CloseHighLevelDeviationPlus	= para_all.para.CloseHighLevelDiffPlus;
				m_CloseHighLevelDeviationMinus	= para_all.para.CloseHighLevelDiffMinus;

				m_CloseLowLevel					= para_all.para.CloseLowLevel;
				m_CloseLowLevelSpanPlus			= para_all.para.CloseLowLevelSpanPlus;
				m_CloseLowLevelSpanMinus		= para_all.para.CloseLowLevelSpanMinus;
				m_CloseLowLevelDeviationPlus	= para_all.para.CloseLowLevelDiffPlus;
				m_CloseLowLevelDeviationMinus	= para_all.para.CloseLowLevelDiffMinus;
				

				m_MainLevelAvrSpanMinus			= para_all.para_ex.MainLevelAvrSpanMinus ;
				m_MainLevelAvrSpanPlus			= para_all.para_ex.MainLevelAvrSpanPlus;
				m_MainLevelCloseAvrSpanMinus	= para_all.para_ex.MainLevelCloseAvrSpanMinus;
				m_MainLevelCLoseAvrSpanPlus		= para_all.para_ex.MainLevelCloseAvrSpanPlus;

				m_HighLevelAvrSpanMinus			= para_all.para_ex.HighLevelAvrSpanMinus;
				m_HighLevelAvrSpanPlus			= para_all.para_ex.HighLevelAvrSpanPlus ;
				m_HighLevelCloseAvrSpanMinus	= para_all.para_ex.HighLevelCloseAvrSpanMinus ;
				m_HighLevelCLoseAvrSpanPlus		= para_all.para_ex.HighLevelCloseAvrSpanPlus ;

				m_LowLevelAvrSpanMinus			= para_all.para_ex.LowLevelAvrSpanMinus ;
				m_LowLevelAvrSpanPlus			= para_all.para_ex.LowLevelAvrSpanPlus ;
				m_LowLevelCloseAvrSpanMinus		= para_all.para_ex.LowLevelCloseAvrSpanMinus ;
				m_LowLevelCLoseAvrSpanPlus		= para_all.para_ex.LowLevelCloseAvrSpanPlus ;

				this->UpdateData(false);
			
				ShowItems(para_all.para.CloseTempType);
				


			}
			else
			{
				memset(&para_all,0,sizeof(para_all));
				para_all.para.CloseTempType						=	0;
				para_all.para.OpenDoorResetLevel			=   0; 
				para_all.para.MainLevel						=	90;
				para_all.para.MainLevelSpanMinus			=	5;
				para_all.para.MainLevelSpanPlus				=	5;
				para_all.para.HighLevel						=	95;
				para_all.para.HighLevelSpanMinus			=	5;
				para_all.para.HighLevelSpanPlus				=	5;
				para_all.para.LowLevel						=	85;
				para_all.para.LowLevelSpanMinus				=	5;
				para_all.para.LowLevelSpanPlus				=	5;
				para_all.para.CloseLevel					=	50;
				para_all.para.CloseLevelSpanPlus			=	5;
				para_all.para.CloseLevelSpanMinus			=	5;
				para_all.para.CloseHighLevel				=	55;
				para_all.para.CloseHighLevelSpanPlus		=	5;
				para_all.para.CloseHighLevelSpanMinus		=	5;
				para_all.para.CloseLowLevel					=	45;
				para_all.para.CloseLowLevelSpanPlus			=	5;
				para_all.para.CloseLowLevelSpanMinus		=	5;
				
				para_all.para.OpenDoorResetLevel			=   0;
//				para_all.para.TakeoutTime					=	0;

				para_all.para.CloseLowLevel					=	0;
				para_all.para.CloseLowLevelSpanPlus			=	0;
				para_all.para.CloseLowLevelSpanMinus		=	0;
				para_all.para.CloseLowLevelDiffPlus			=	0;
				para_all.para.CloseLowLevelDiffMinus		=	0;
				

				para_all.para_ex.MainLevelAvrSpanMinus		=	0;
				para_all.para_ex.MainLevelAvrSpanPlus		=	0;
				para_all.para_ex.MainLevelCloseAvrSpanMinus	=	0;
				para_all.para_ex.MainLevelCloseAvrSpanPlus	=	0;

				para_all.para_ex.HighLevelAvrSpanMinus		=	0;
				para_all.para_ex.HighLevelAvrSpanPlus		=	0;
				para_all.para_ex.HighLevelCloseAvrSpanMinus	=	0;
				para_all.para_ex.HighLevelCloseAvrSpanPlus	=	0;

				para_all.para_ex.LowLevelAvrSpanMinus		=	0;
				para_all.para_ex.LowLevelAvrSpanPlus		=	0;
				para_all.para_ex.LowLevelCloseAvrSpanMinus	=	0;
				para_all.para_ex.LowLevelAvrSpanPlus		=	0;

				ParaFile.SeekToBegin();
				ParaFile.Write(&para_all,sizeof(para_all)); 

				ShowItems(para_all.para.CloseTempType);
			}


			ParaFile.Close(); 
		}
	}


}

extern void TraverseFolder(LPCTSTR lpPath,LPCTSTR file_ext,CStringArray *fileStr);

void CCreateModifyParaDlg::UpdateParaNameList()
{
	this->m_ListParaName.ResetContent(); 

	CFile ParaFile;
	CFileFind ff;

	CString fName;
	CString sDir;
	CStringArray sa;

	
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	TraverseFolder(sDir,_T("*.para"),&sa);

	for(int i = 0; i < sa.GetSize(); i ++)
	{
		CString s = sa.GetAt(i);
		int pos  = s.Find(_T("."));
		if(pos > 0)
		{
			s = s.Left(pos); 
		}
		this->m_ListParaName.AddString(s);
	}

	/*
	CParaRecordset para_rs;
	para_rs.Open();
	if(para_rs.IsEOF() && para_rs.IsBOF())
	{
	}
	else
	{
		para_rs.MoveFirst();
	}
	while(!para_rs.IsEOF())
	{
		para_rs.m_para_name.TrimLeft(); 
		para_rs.m_para_name.TrimRight(); 
		m_ListParaName.AddString(para_rs.m_para_name);  
		para_rs.MoveNext(); 
	}*/

}

void CCreateModifyParaDlg::OnDelete() 
{
	CFile ParaFile;
	CFileFind ff;

	CString fName;
	CString sDir;
	CStringArray sa;

	
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	CString para_name;
	m_ListParaName.GetText(m_ListParaName.GetCurSel(),para_name);

	CFile::Remove(sDir + para_name + _T(".para")); 

	/*
	CParaRecordset para_rs;
	para_rs.Open();
	if(para_rs.IsEOF() && para_rs.IsBOF())
	{
	}
	else
	{
		para_rs.MoveFirst();
	}
	while(!para_rs.IsEOF())
	{
		para_rs.m_para_name.TrimRight();
		para_rs.m_para_name.TrimLeft();
		m_ParaName.TrimRight();
		m_ParaName.TrimLeft();
		if(para_rs.m_para_name == this->m_ParaName)
		{
			para_rs.Delete();
			break;
		}
		para_rs.MoveNext();
	}
	para_rs.Close();

	*/

	UpdateParaNameList();
}
void CCreateModifyParaDlg::OnCbnSelchangeComboParaType()
{
	ShowItems(this->m_ComboParaType.GetCurSel());
}

void CCreateModifyParaDlg::ShowItems(int para_type)
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd * w;
	
	if( para_type == 0)
	{
		//温度范围

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_SHOW); 


		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW); 
#ifndef _CHENG_HONG_
		
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW);

		//温度差
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_HIDE); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_HIDE);
#endif

	}
	else if( para_type == 1)
	{
		//温度范围
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_AVR_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_AVR_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		


		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_HIDE);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_HIDE);

		//温度差
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW);

	}
	else if( para_type == 2)
	{
		//温度范围
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS);
		w->ShowWindow(SW_SHOW);

		//温度差
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW); 
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW); 

		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW);

		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_MINUS2);
		w->ShowWindow(SW_SHOW);
		w = this->GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2);
		w->ShowWindow(SW_SHOW);
	}		


	if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_ONLY_OPENTEMP_TEST)
	{

		Show_BOX_128_ONLY_OPENTEMP_TEST();
	}
}

void CCreateModifyParaDlg::OnEnSetfocusEdit_2(UINT id)
{

	/*
#define IDC_EDIT_LOW_LEVEL_AVR_SPAN_MINUS			3025
#define IDC_EDIT_LOW_LEVEL_AVR_SPAN_PLUS			3026
#define IDC_EDIT_LOW_LEVEL_CLOSE_AVR_SPAN_MINUS		3027
#define IDC_EDIT_LOW_LEVEL_CLOSE_AVR_SPAN_PLUS		3028

#define IDC_EDIT_HIGH_LEVEL_AVR_SPAN_MINUS			3029
#define IDC_EDIT_HIGH_LEVEL_AVR_SPAN_PLUS			3030
#define IDC_EDIT_HIGH_LEVEL_CLOSE_AVR_SPAN_MINUS	3031
#define IDC_EDIT_LOW_LEVEL_SPAN_PLUS5				3032
#define IDC_EDIT_HIGH_LEVEL_CLOSE_AVR_SPAN_PLUS		3032
#define IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_MINUS	3033
#define IDC_EDIT_MIAN_LEVEL_CLOSE_AVR_SPAN_PLUS		3034
#define IDC_EDIT_MIAN_LEVEL_AVR_SPAN_MINUS			3035
#define IDC_EDIT_MIAN_LEVEL_AVR_SPAN_PLUS			3036
	*/

	CString Title[] =
	{
		_T("设置低规格中心值负偏差"),
		_T("设置低规格中心值正偏差"),
		_T("设置低规格复位中心值负偏差"),
		_T("设置低规格复位中心值正偏差"),

		_T("设置高规格中心值负偏差"),
		_T("设置高规格中心值正偏差"),
		_T("设置高规格复位中心值负偏差"),
		_T("设置高规格复位中心值正偏差"),

		_T("设置主规格复位中心值负偏差"),
		_T("设置主规格复位中心值正偏差"),
		_T("设置主规格中心值负偏差"),
		_T("设置主规格中心值正偏差"),
		

	};


	if(m_bSetFocusOnEdit == false)
	{
		return;
	}
	m_bSetFocusOnEdit = false;
	CWnd *w = this->GetDlgItem(IDC_COMBO_PARA_TYPE); 

	
	CInputDlg dlg;
	dlg.m_Title = Title[id - IDC_EDIT_LOW_LEVEL_AVR_SPAN_MINUS];
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(id) ;
		w->SetWindowTextW(dlg.m_Input);
	}
	

	w->SetFocus(); 

}

void CCreateModifyParaDlg::OnEnSetfocusEdit(UINT id)
{
	// TODO: 在此添加控件通知处理程序代码
	CString Title[] =
	{
		_T("设置主规格动作温度"),
		_T("设置主规格动作负偏差"),
		_T("设置主规格动作正偏差"),
		_T("设置主规格复位温度"),
		_T("设置主规格复位负偏差"),
		_T("设置主规格复位正偏差"),

		_T("设置高规格动作温度"),
		_T("设置高规格动作负偏差"),
		_T("设置高规格动作正偏差"),
		_T("设置高规格复位温度"),
		_T("设置高规格复位负偏差"),
		_T("设置高规格复位正偏差"),

		_T("设置低规格动作温度"),
		_T("设置低规格动作负偏差"),
		_T("设置低规格动作正偏差"),
		_T("设置低规格复位温度"),
		_T("设置低规格复位负偏差"),
		_T("设置低规格复位正偏差"),

		_T("设置主规格复位负温差"),
		_T("设置主规格复位正温差"),

		_T("设置高规格复位负温差"),
		_T("设置高规格复位正温差"),

		_T("设置低规格复位负温差"),
		_T("设置低规格复位正温差"),

	};

	if(m_bSetFocusOnEdit == false)
	{
		return;
	}
	m_bSetFocusOnEdit = false;
	CWnd *w = this->GetDlgItem(IDC_COMBO_PARA_TYPE); 

	
	CInputDlg dlg;
	dlg.m_Title = Title[id - IDC_EDIT_MAIN_LEVEL];
	if(dlg.DoModal() == IDOK)
	{
		CWnd * w = this->GetDlgItem(id) ;
		w->SetWindowTextW(dlg.m_Input);
	}
	

	w->SetFocus(); 
}

void CCreateModifyParaDlg::OnCbnSetfocusComboParaType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSetFocusOnEdit = true;
}

void CCreateModifyParaDlg::OnEnSetfocusEditParaName()
{
	// TODO: 在此添加控件通知处理程序代码

//	WinExec("osk.exe",SW_NORMAL);
	ShellExecute(GetSafeHwnd(), NULL, _T("osk.exe"),NULL,NULL,SW_NORMAL);
//	ShellExecute(GetSafeHwnd(), _T("open"), _T("osk.exe"), _T(""),_T(""),SW_SHOW);

}

void CCreateModifyParaDlg::OnEnKillfocusEditParaName()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	HWND hwnd = ::FindWindow(NULL, _T("屏幕键盘"));
	if (hwnd)
	{
		::SendMessage((HWND)hwnd, WM_CLOSE, 0, 0);
	}
	*/
}

void CCreateModifyParaDlg::OnEnChangeEditLowLevelSpanPlus5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
