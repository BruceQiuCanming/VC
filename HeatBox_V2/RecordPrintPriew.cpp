// PrnPreview.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "RecordPrintPriew.h"
#include "RecordViewDlg.h"
#include "common.h"
#include "Bmp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrnPreview dialog


CPrnPreview::CPrnPreview(CWnd* pParent /*=NULL*/)
	: CDialog(CPrnPreview::IDD, pParent)
{
	m_PageNr = 0;
	m_CurGroupNr = 0;
	//{{AFX_DATA_INIT(CPrnPreview)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPrnPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrnPreview)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrnPreview, CDialog)
	//{{AFX_MSG_MAP(CPrnPreview)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrnPreview message handlers

void CPrnPreview::DisplayPrnView_ByGroup()
{
	CFile log_file;
	CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);

	 int back_color = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("BACK_COLOR"),RGB(128,128,128)); 

	 CBrush groundbrush(back_color);
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush graybrush(COLORREF(RGB(0xFF,0xFF,0xFF)));
	
	//绘制工具资源
	 CPen	pen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=this->GetWindowDC();
		

	if(dc == NULL)
	{
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Close();
		}
		return;
	}
	
	CRect wndRect;

	this->GetClientRect(wndRect);
	nWidth	= wndRect.right - wndRect.left;
	nHeight = wndRect.bottom - 30;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		this->UpdateData(false);
		CString fname = theAppDirectory;
		fname += "log.txt";
		if(log_file.Open(fname,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Close();
		}
		ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			this->UpdateData(false);
			CString fname = theAppDirectory;
			fname += "log.txt";
			if(log_file.Open(	fname		,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
			{
				log_file.SeekToEnd();
				log_file.Close();
			}
			ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nWidth * 7008 /4950/*nHeight*/))	//建立与显示设备兼容的位图
	{
		this->UpdateData(false);
		CString fname = theAppDirectory;
			fname += "log.txt";
		if(log_file.Open(fname,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Close();
		}
		ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境


	if(m_PageNr == 0)
	{
		PrintReport_ByGroup(&MemDC,this->m_Title,m_BoxNr,m_BeginTestTime,m_EndTestTime,
			m_TestParaName,this->m_TestProductType,m_TestMemo, m_SwitchConfigPara,m_SwitchTestResult_Ex,m_bTestCloseLevel,m_HeatBoxType,0);
		
	}
	else
	{
		PrintReport_ByGroup_Ticket(&MemDC,this->m_Title,m_BoxNr,m_BeginTestTime,m_EndTestTime,
			m_TestParaName,this->m_TestProductType,m_TestMemo, m_SwitchConfigPara,m_SwitchTestResult_Ex,m_bTestCloseLevel,m_HeatBoxType,0);
	}

	dc->BitBlt(5, 60,nWidth,nHeight,&MemDC,0,20 * GetScrollPos(SB_VERT),SRCCOPY); 	//显示原图形


	MemDC.DeleteDC();
	ReleaseDC(dc);

	CWnd *w = this->GetDlgItem(IDC_BUTTON_NEXT);
	w->ShowWindow(SW_SHOW);

}

void CPrnPreview::DisplayPrnView()
{
	//CDC *dc = GetWindowDC();

	

	if(!this->IsWindowVisible()) 
	{
		return;
	}

	
	DisplayPrnView_ByGroup();
	return;

	CFile log_file;

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);

	 int back_color = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("BACK_COLOR"),RGB(128,128,128)); 

	 CBrush groundbrush(back_color);
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush graybrush(COLORREF(RGB(0xFF,0xFF,0xFF)));
	
	//绘制工具资源
	 CPen	pen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=this->GetWindowDC();
		

	if(dc == NULL)
	{
		CString fname = theAppDirectory;
			fname += "log.txt";
		if(log_file.Open(fname,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Close();
		}
		return;
	}
	
	CRect wndRect;

	this->GetClientRect(wndRect);
	nWidth	= wndRect.right - wndRect.left;
	nHeight = wndRect.bottom - 30;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		this->UpdateData(false);
		CString fname = theAppDirectory;
			fname += "log.txt";
		if(log_file.Open(fname,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Close();
		}
		ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			this->UpdateData(false);
			CString fname = theAppDirectory;
			fname += "log.txt";
			if(log_file.Open(fname,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
			{
				log_file.SeekToEnd();
				log_file.Close();
			}
			ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nWidth * 7008 /4950/*nHeight*/))	//建立与显示设备兼容的位图
	{
		this->UpdateData(false);
		CString fname = theAppDirectory;
			fname += "log.txt";
		if(log_file.Open(fname,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Close();
		}
		ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境


	PrintReport_ByGroup(&MemDC,this->m_Title,m_BoxNr,m_BeginTestTime,m_EndTestTime,
		m_TestParaName,this->m_TestProductType,m_TestMemo, m_SwitchConfigPara,m_SwitchTestResult_Ex,m_bTestCloseLevel,m_HeatBoxType,0);
	
	
	dc->BitBlt(5, 60,nWidth,nHeight,&MemDC,0,20 * GetScrollPos(SB_VERT),SRCCOPY); 	//显示原图形


	MemDC.DeleteDC();
	ReleaseDC(dc);

	CWnd *w = this->GetDlgItem(IDC_BUTTON_NEXT);
	w->ShowWindow(SW_SHOW);
}

void CPrnPreview::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	DisplayPrnView();
	// Do not call CDialog::OnPaint() for painting messages
}

void CPrnPreview::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int pos = GetScrollPos(SB_VERT);
	switch(nSBCode)
	{
	case SB_LINEDOWN:
		if(pos <99 )
		{
			pos ++;
		}
		break;
	case SB_LINEUP:
		if(pos >0 )
		{
			pos --;
		}
		break;
    case SB_PAGEDOWN:
		if(pos < 99 )
		{
			pos += 5;
		}
		break;
	case SB_PAGEUP:
		if(pos >0 )
		{
			pos -= 5;
		}
		break;

	}
	SetScrollPos(SB_VERT,pos);
    DisplayPrnView();


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CPrnPreview::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetScrollRange(SB_VERT, 0 , 99);
	this->ShowWindow(SW_SHOWMAXIMIZED); 
	this->SetWindowTextW(_T("打印预览"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrnPreview::OnButtonNext()
{

	m_PageNr++;
	if(m_PageNr > 1)
	{
		m_PageNr = 0;
	}
	DisplayPrnView();
}