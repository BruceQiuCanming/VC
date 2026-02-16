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
}


BEGIN_MESSAGE_MAP(CCreateModifyParaDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateModifyParaDlg)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_LIST_PARA_NAME, OnSelchangeListParaName)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_PARA_TYPE, &CCreateModifyParaDlg::OnCbnSelchangeComboParaType)
	ON_CONTROL_RANGE(EN_SETFOCUS,IDC_EDIT_MAIN_LEVEL,IDC_EDIT_CLOSE_LOW_LEVEL_SPAN_PLUS2,OnEnSetfocusEdit)
	ON_CBN_SETFOCUS(IDC_COMBO_PARA_TYPE, &CCreateModifyParaDlg::OnCbnSetfocusComboParaType)
	ON_EN_SETFOCUS(IDC_EDIT_PARA_NAME, &CCreateModifyParaDlg::OnEnSetfocusEditParaName)
	ON_EN_KILLFOCUS(IDC_EDIT_PARA_NAME, &CCreateModifyParaDlg::OnEnKillfocusEditParaName)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateModifyParaDlg message handlers

void CCreateModifyParaDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(); 

	m_ParaName.Trim();

	if(m_ParaName.GetLength() == 0)
	{
		AfxMessageBox(_T("参数名不能为空!"));
		return;
	}

	double a,b;

	if(m_LowLevel != 0)
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
	
	if(m_HighLevel != 0)
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
	SWITCH_CONFIG_PARA	para;
	CString sDir;
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	if(ParaFile.Open(sDir + m_ParaName + _T(".para"),CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
	{

		para.OpenDoorResetLevel=	m_ComboOpenDoorResetLevel.GetCurSel(); 
		if(para.OpenDoorResetLevel < 0 || para.OpenDoorResetLevel > 10)
		{
			para.OpenDoorResetLevel = 0;
		}
		para.ParaType	=	m_ComboParaType.GetCurSel(); 
		if(para.ParaType < 0 || para.ParaType > 2)
		{
			para.ParaType = 0;
		}
		para.HighLevelSpanMinus		=	m_HighLevelSpanMinus;
		para.HighLevelSpanPlus		=	m_HighLevelSpanPlus;
		para.HighLevel				=	m_HighLevel;
		para.CloseLevel				=	m_CloseMainLevel;
		para.CloseLevelSpanPlus		=	m_CloseMainLevelSpanPlus;
		para.CloseLevelSpanMinus	=	m_CloseMainLevelSpanMinus;
		para.CloseLevelDeviationPlus	=	m_CloseMainLevelDeviationPlus;
		para.CloseLevelDeviationMinus	=	m_CloseMainLevelDeviationMinus;

		para.CloseLowLevel			=	m_CloseLowLevel;
		para.CloseLowLevelSpanPlus	=	m_CloseLowLevelSpanPlus;
		para.CloseLowLevelSpanMinus	=	m_CloseLowLevelSpanMinus;
		para.CloseLowLevelDeviationPlus		=	m_CloseLowLevelDeviationPlus;
		para.CloseLowLevelDeviationMinus	=	m_CloseLowLevelDeviationMinus;

		para.CloseHighLevel			=	m_CloseHighLevel;
		para.CloseHighLevelSpanPlus	=	m_CloseHighLevelSpanPlus;
		para.CloseHighLevelSpanMinus=	m_CloseHighLevelSpanMinus;
		para.CloseHighLevelDeviationPlus	=	m_CloseHighLevelDeviationPlus;
		para.CloseHighLevelDeviationMinus	=	m_CloseHighLevelDeviationMinus;


		
		para.LowLevel			=	m_LowLevel;
		para.LowLevelSpanMinus	=	m_LowLevelSpanMinus;
		para.LowLevelSpanPlus	=	m_LowLevelSpanPlus;
		para.MainLevel			=	m_MainLevel;
		para.MainLevelSpanMinus	=	m_MainLevelSpanMinus;
		para.MainLevelSpanPlus	=	m_MainLevelSpanPlus;
		para.TakeoutTime		=	m_TakeoutTime;

		ParaFile.Write(&para,sizeof(para));
		
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

}

BOOL CCreateModifyParaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bSetFocusOnEdit = true;

	// TODO: Add extra initialization here
	this->m_ComboOpenDoorResetLevel.SetCurSel(3);  
	m_ComboParaType.SetCurSel(0);
	this->UpdateParaNameList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateModifyParaDlg::OnSelchangeListParaName() 
{
	// TODO: Add your control notification handler code here

	CFile ParaFile;
	CFileFind ff;
	SWITCH_CONFIG_PARA	para;
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
			
			memset(&para,0,sizeof(para));

			if(ParaFile.Read(&para,sizeof(SWITCH_CONFIG_PARA)) <= sizeof(SWITCH_CONFIG_PARA)) 
			{
				
				this->m_ComboParaType.SetCurSel(para.ParaType); 
				m_ComboOpenDoorResetLevel.SetCurSel(para.OpenDoorResetLevel); 
				m_HighLevelSpanMinus		= para.HighLevelSpanMinus;
				m_HighLevelSpanPlus			= para.HighLevelSpanPlus;
				m_HighLevel					= para.HighLevel;
				m_CloseMainLevel			= para.CloseLevel;
				m_CloseMainLevelSpanPlus	= para.CloseLevelSpanPlus;
				m_CloseMainLevelSpanMinus	= para.CloseLevelSpanMinus;
				m_CloseMainLevelDeviationPlus	= para.CloseLevelDeviationPlus;
				m_CloseMainLevelDeviationMinus	= para.CloseLevelDeviationMinus;

				m_LowLevel					= para.LowLevel;
				m_LowLevelSpanMinus			= para.LowLevelSpanMinus;
				m_LowLevelSpanPlus			= para.LowLevelSpanPlus;
				
				m_MainLevel					= para.MainLevel;
				m_MainLevelSpanMinus		= para.MainLevelSpanMinus;
				m_MainLevelSpanPlus			= para.MainLevelSpanPlus;
				m_TakeoutTime				= para.TakeoutTime;
				this->m_ParaName			= para_name; 

				m_CloseHighLevel			= para.CloseHighLevel;
				m_CloseHighLevelSpanPlus	= para.CloseHighLevelSpanPlus;
				m_CloseHighLevelSpanMinus	= para.CloseHighLevelSpanMinus;
				m_CloseHighLevelDeviationPlus	= para.CloseHighLevelDeviationPlus;
				m_CloseHighLevelDeviationMinus	= para.CloseHighLevelDeviationMinus;

				m_CloseLowLevel					= para.CloseLowLevel;
				m_CloseLowLevelSpanPlus			= para.CloseLowLevelSpanPlus;
				m_CloseLowLevelSpanMinus		= para.CloseLowLevelSpanMinus;
				m_CloseLowLevelDeviationPlus	= para.CloseLowLevelDeviationPlus;
				m_CloseLowLevelDeviationMinus	= para.CloseLowLevelDeviationMinus;
				
				this->UpdateData(false);
			
				ShowItems(para.ParaType);
				


			}
			else
			{
				memset(&para,0,sizeof(para));
				para.ParaType					=	0;
				para.OpenDoorResetLevel			=   0; 
				para.MainLevel					=	90;
				para.MainLevelSpanMinus			=	5;
				para.MainLevelSpanPlus			=	5;
				para.HighLevel					=	95;
				para.HighLevelSpanMinus			=	5;
				para.HighLevelSpanPlus			=	5;
				para.LowLevel					=	85;
				para.LowLevelSpanMinus			=	5;
				para.LowLevelSpanPlus			=	5;
				para.CloseLevel					=	50;
				para.CloseLevelSpanPlus			=	5;
				para.CloseLevelSpanMinus		=	5;
				para.CloseHighLevel				=	55;
				para.CloseHighLevelSpanPlus		=	5;
				para.CloseHighLevelSpanMinus	=	5;
				para.CloseLowLevel				=	45;
				para.CloseLowLevelSpanPlus		=	5;
				para.CloseLowLevelSpanMinus		=	5;
				
				para.OpenDoorResetLevel			=   0;
				para.TakeoutTime				=	0;

				ParaFile.SeekToBegin();
				ParaFile.Write(&para,sizeof(para)); 

				ShowItems(para.ParaType);
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

	WinExec("osk.exe",SW_NORMAL);
//	ShellExecute(GetSafeHwnd(), NULL, _T("osk.exe"),NULL,NULL,SW_NORMAL);
//	ShellExecute(GetSafeHwnd(), _T("open"), _T("osk.exe"), _T(""),_T(""),SW_SHOW);

}

void CCreateModifyParaDlg::OnEnKillfocusEditParaName()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hwnd = ::FindWindow(NULL, _T("屏幕键盘"));
	if (hwnd)
	{
		::SendMessage((HWND)hwnd, WM_CLOSE, 0, 0);
	}
}
