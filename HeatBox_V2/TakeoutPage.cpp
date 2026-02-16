// TakeoutPage.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "TakeoutPage.h"
#include "TakeDlg.h"
#include "comm.h"
#include "common.h"
#include "RecordViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTakeoutPage dialog
	TCHAR level_str[18][10]=
	{
		_T(" 其  它 "),	    //NO_LEVEL		= 0,
		_T(" 主规格 "),		//MAIN_LEVEL	= 1,
		_T(" 高规格 "),		//TEST_RESULT_HIGH_LEVEL	= 2,
		_T(" 低规格 "),		//LOW_LEVEL		= 3,
		_T(" 不动作 "),		//OVER_HIGH_LEVEL	= 4,
		_T(" 低温品 "),		//OVER_LOW_LEVEL	= 5,
		_T(" 复小品 "),		//CLOSE_HIGH_LEVEL	= 6,
		_T(" 复大品 "),		//CLOSE_LOW_LEVEL	= 7,
		_T("动作闪动"),	//OPEN_FLASH_LEVEL	= 8,
		_T("复位闪动"),		//CLOSE_FLASH_LEVEL	= 9,
		_T("开门复位"),		//OPEN_DOOR_RESET_LEVEL	=	10,
		_T(" 高温品 "),		//HIGH_HIGH_LEVEL	=	11,
		_T(" 无产品 "),		//NOT_USED			=12
		_T(" 无PTC "),		//NO_PTC_LEVEL		=12
#ifdef _CHANGHONG_
		_T("高规复小"),		//HIGH_LEVEL_CLOSE_HIGH_LEVEL		= 13,
		_T("高规复大"),		//HIGH_LEVEL_CLOSE_LOW_LEVEL		= 14,
		_T("低规复小"),		//LOW_LEVEL_CLOSE_HIGH_LEVEL		= 15,
		_T("低规复大"),		//LOW_LEVEL_CLOSE_LOW_LEVEL		= 16,
#endif

	};



CTakeoutPage::CTakeoutPage(CWnd* pParent /*=NULL*/)
	: CDialog(CTakeoutPage::IDD, pParent)
	, m_bHighHighLevel(FALSE)
	, m_iHighHighLevel(0)
{
	//{{AFX_DATA_INIT(CTakeoutPage)
	m_iSelect = 0;
	m_bMainLevel = FALSE;
	m_bLowLevel = FALSE;
	m_bHighLevel = FALSE;
	m_bCloseHighLevel = FALSE;
	m_bCloseLowLevel = FALSE;
	m_bOverHighLevel = FALSE;
	m_bOverLowLevel = FALSE;
	m_bNotUsed = FALSE;
	m_bOpenFlashLevel = FALSE;
	m_bCloseFlashLevel = FALSE;
	m_iCloseHighLevel = 0;
	m_iCloseLowLevel = 0;
	m_iHighLevel = 0;
	m_iLowLevel = 0;
	m_iMainLevel = 0;
	m_iNotUsed = 0;
	m_iOpenFlashLevel = 0;
	m_iOverHighLevel = 0;
	m_iOverLowLevel = 0;
	m_bOpenDoorResetLevel = FALSE;
	m_iOpenDoorResetLevel = 0;
	m_iCloseFlashLevel = 0;
	m_bNoLevel = FALSE;
	m_iNoLevel = 0;
	//}}AFX_DATA_INIT
}


void CTakeoutPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTakeoutPage)
	DDX_Control(pDX, IDC_LIST_TEST_RESULT, m_ListTestResult);
	DDX_Control(pDX, IDC_SCROLLBAR_SWITCH_ID, m_ScrollBar_Switch_ID);
	DDX_Radio(pDX, IDC_RADIO_ONLY_ONE, m_iSelect);
	DDX_Check(pDX, IDC_CHECK_MAIN_LEVEL, m_bMainLevel);
	DDX_Check(pDX, IDC_CHECK_LOW_LEVEL, m_bLowLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_LEVEL, m_bHighLevel);
	DDX_Check(pDX, IDC_CHECK_CLOSE_HIGH_LEVEL, m_bCloseHighLevel);
	DDX_Check(pDX, IDC_CHECK_CLOSE_LOW_LEVEL, m_bCloseLowLevel);
	DDX_Check(pDX, IDC_CHECK_OVER_HIGH_LEVEL, m_bOverHighLevel);
	DDX_Check(pDX, IDC_CHECK_OVER_LOW_LEVEL, m_bOverLowLevel);
	DDX_Check(pDX, IDC_CHECK_NO_ACTION, m_bNotUsed);
	DDX_Check(pDX, IDC_CHECK_OPEN_FLASH, m_bOpenFlashLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL, m_iCloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL, m_iCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL, m_iHighLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL, m_iLowLevel);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL, m_iMainLevel);
	DDX_Text(pDX, IDC_EDIT_NO_ACTION_LEVEL, m_iNotUsed);
	DDX_Text(pDX, IDC_EDIT_OPEN_FLASH_LEVEL, m_iOpenFlashLevel);
	DDX_Text(pDX, IDC_EDIT_OVER_HIGH_LEVEL, m_iOverHighLevel);
	DDX_Text(pDX, IDC_EDIT_OVER_LOW_LEVEL, m_iOverLowLevel);
	DDX_Check(pDX, IDC_CHECK_OPEN_DOOR_RESET, m_bOpenDoorResetLevel);
	DDX_Text(pDX, IDC_EDIT_OPEN_DOOR_RESET, m_iOpenDoorResetLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_FLASH_LEVEL, m_iCloseFlashLevel);
	DDX_Check(pDX, IDC_CHECK_NO_LEVEL, m_bNoLevel);
	DDX_Check(pDX, IDC_CHECK_CLOSE_FLASH, m_bCloseFlashLevel);
	DDX_Text(pDX, IDC_EDIT_NO_LEVEL, m_iNoLevel);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_HIGH_HIGH_LEVEL, m_bHighHighLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_HIGH_LEVEL, m_iHighHighLevel);
}


BEGIN_MESSAGE_MAP(CTakeoutPage, CDialog)
	//{{AFX_MSG_MAP(CTakeoutPage)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_OVER_LOW_LEVEL, OnCheckOverLowLevel)
	ON_BN_CLICKED(IDC_CHECK_OVER_HIGH_LEVEL, OnCheckOverHighLevel)
	ON_BN_CLICKED(IDC_CHECK_LOW_LEVEL, OnCheckLowLevel)
	ON_BN_CLICKED(IDC_CHECK_HIGH_LEVEL, OnCheckHighLevel)
	ON_BN_CLICKED(IDC_CHECK_MAIN_LEVEL, OnCheckMainLevel)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_HIGH_LEVEL, OnCheckCloseHighLevel)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_LOW_LEVEL, OnCheckCloseLowLevel)
	ON_BN_CLICKED(IDC_CHECK_OPEN_FLASH, OnCheckOpenFlash)
	ON_BN_CLICKED(IDC_CHECK_OPEN_DOOR_RESET, OnCheckOpenDoorReset)
	ON_BN_CLICKED(IDC_CHECK_NO_ACTION, OnCheckNoAction)
	ON_BN_CLICKED(IDC_RADIO_ONLY_ONE, OnRadioOnlyOne)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_VIEW2, OnButtonRecordView2)
	ON_BN_CLICKED(IDC_BUTTON_TAKEOUT_DLG, OnButtonTakeoutDlg)
	ON_BN_CLICKED(IDC_CHECK_NO_LEVEL, OnCheckNoLevel)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_FLASH, OnCheckCloseFlash)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_HIGH_HIGH_LEVEL, &CTakeoutPage::OnBnClickedCheckHighHighLevel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTakeoutPage message handlers

void CTakeoutPage::DrawColorListItem(CDC MemDC, int switchNr, float openTemp, float closeTemp,TEST_RESULT_LEVEL level,bool bDraw)
{

}

void CTakeoutPage::DrawColorList()
{
	return;

	int i;
	bool bkColor = false;
	bool newbkColor = true;
	CRect w_rect;
	CRect ScrollBar_rect;
	this->GetWindowRect(w_rect); 
	m_ScrollBar_Switch_ID.GetWindowRect(ScrollBar_rect); 

	int nWidth = w_rect.Width() - ScrollBar_rect.Width();
    int nHeight = w_rect.Height();


	ScrollBar_rect.SetRect(nWidth,0, w_rect.Width(),nHeight);

	m_ScrollBar_Switch_ID.MoveWindow(&ScrollBar_rect);


	CDC MemDC,*dc;
	CBrush blackbrush(COLORREF(RGB(0,0,0)));
	CBrush rbrush(COLORREF(RGB(0xFF,0,0)));
	CBrush gbrush(COLORREF(RGB(0,0xFF,0)));
	CBrush bbrush(COLORREF(RGB(0,0,0xFF)));


	CPen	pen;
	CPen*	oldpen;
	CBitmap bitmap;
	CBitmap* pOldBitmap;
	
	if(!this->IsWindowVisible()) 
	{
		return;
	}

	COLORREF bk_color[2]=
	{
		COLORREF(RGB(0xFF,0x00,0x00)),
		COLORREF(RGB(0x00,0xFF,0x00))
	};

	//CBitmap所需资源
//	CBitmap bitmap;

	    dc=this->GetDC();
		
		if(dc != NULL)
		{
			if(MemDC.GetSafeHdc() != NULL)
			{
				MemDC.DeleteDC();
			}
			
			if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
			{
				ReleaseDC(dc);
				return;
			}
		}
		else
		{
			return;
		}
		CBitmap b;
		bitmap.DeleteObject();

		if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
		{
			ReleaseDC(dc);
			return;
		}

		pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境
		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,5, RGB(255,255,0)); 			//建立画笔
		oldpen=MemDC.SelectObject(&pen);					//选择画笔对象

		
//	CBitmap* pOldBitmap;
//	CDC MemDC,*dc;
//	CBrush bbrush(COLORREF(RGB(0,0,0xFF)));

	
	//绘制工具资源
//	CPen	pen;
//	CPen*	oldpen;
//	int nWidth = SWITCH_PIC_WIDTH;
//    int nHeight = SWITCH_PIC_HIGH;

	//	CDC MemDC;
	CFont *font = MemDC.GetCurrentFont(); 
	LOGFONT  logfont;
	font->GetLogFont(&logfont);
	CString str,str1;
	str.Format(_T("编号    等级 　动作温度 复位温度"));     
	MemDC.TextOut(0,0,str);

	for(i = m_ScrollBar_Switch_ID.GetScrollPos(); i < G_iMaxLedNr[m_BoxNr]; i++)
	{
//		m_SwitchTestResult_Sorted[i].level = m_SwitchTestResult_Sorted[i].switch_nr % 11; 
		if(newbkColor)
		{
			if(bkColor == false)
			{
				MemDC.SetBkColor(COLORREF(RGB(0x80,0xFF,0)));//bk_color[0]);
				bkColor = true;
			}
			else
			{
				MemDC.SetBkColor(COLORREF(RGB(0xFF,0xFF,0)));//bk_color[1]);
				bkColor = false;
			}
		}
		if( i < G_iMaxLedNr[m_BoxNr] - 1)
		{
			if(m_SwitchTestResult_Sorted[i].level !=  m_SwitchTestResult_Sorted[i+1].level)
			{
				newbkColor = true;
			}
			else
			{
				newbkColor = false;
			}
		}

//
	if(G_iMaxLedNr[m_BoxNr] == 192)
	{
		CString nr;
		int item_count = m_SwitchTestResult_Sorted[i].switch_nr;
		if((item_count % 12) == 10)
		{
			nr.Format(_T("%dA"), item_count/12 + 1);
		}
		else if((item_count % 12) == 11)
		{
			nr.Format(_T("%dB"), item_count/12 + 1);
		}
		else if((item_count % 12) == 9)
		{
			nr.Format(_T("%d0"), item_count/12 + 1);
		}
		else
		{
			nr.Format(_T("%d%d"), (item_count/12) , (item_count%12) + 1);
		}
//		
		str.Format(_T("%s  　%s 　"), 
			nr,
			level_str[m_SwitchTestResult_Sorted[i].level]);
	}
	else
	{
		str.Format(_T("%d  　%s 　"), 
			m_SwitchTestResult_Sorted[i].switch_nr + 1,
			level_str[m_SwitchTestResult_Sorted[i].level]);
	}

		if(m_SwitchTestResult_Sorted[i].test_result.OpenTemp < 0.0)
		{
			str1.Format(_T("000.0 　"));
		}
		else
		{
			str1.Format(_T("%05.1f 　"),m_SwitchTestResult_Sorted[i].test_result.OpenTemp);
		}
		str +=str1;
		
		if(m_SwitchTestResult_Sorted[i].test_result.CloseTemp < 0.0)
		{
			str1.Format(_T("000.0"));
		}
		else
		{
			str1.Format(_T("%05.1f"),m_SwitchTestResult_Sorted[i].test_result.CloseTemp);
		}
		str +=str1;

		
		MemDC.TextOut(0,(logfont.lfHeight + 2) * (i + 1 - m_ScrollBar_Switch_ID.GetScrollPos()),str);
	}
	
	dc->BitBlt(0, 0,nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
	
	MemDC.DeleteDC();
	ReleaseDC(dc);
}

void CTakeoutPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	DrawColorList();

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTakeoutPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CStringArray sHeaderArray;
	CByteArray bArray;
	int WIDTH[10] = {16,30,27,27,30,30,30,30,30,30};

	for(int i = 0; i < 4; i++)
	{
		bArray.Add(WIDTH[i]);
	}
	
	sHeaderArray.Add(_T("编号"));
	sHeaderArray.Add(_T("规格"));
	sHeaderArray.Add(_T("动作"));
	sHeaderArray.Add(_T("复位"));

	m_ListTestResult.SetColumnSize(sHeaderArray.GetCount()); 
	m_ListTestResult.InitCtrl(&sHeaderArray,&bArray);
	m_ListTestResult.SetRowHeight(25);
	
	

	m_ScrollBar_Switch_ID.SetScrollRange(0, G_iMaxLedNr[m_BoxNr] - 1 , true);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTakeoutPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nSBCode)
	{
	case SB_BOTTOM:
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINEDOWN:
		pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 1) ;
		DrawColorList();
		break;
	case SB_LINEUP:
		pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 1) ;
		DrawColorList();
		break;
	case SB_PAGEDOWN:
		pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 5) ;
		DrawColorList();
		break;
	case SB_PAGEUP:
		pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 5) ;
		DrawColorList();
		break;
	case SB_THUMBPOSITION:
		break;
	case SB_THUMBTRACK:
		break;
	case SB_TOP:
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTakeoutPage::SortTestResult()
{
	int i,sort_id = 0;
	SWITCH_TEST_RESULT_AND_LEVEL temp[192];

	memset(&m_SwitchTestResult_Sorted, 0, sizeof(m_SwitchTestResult_Sorted));
	
	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		temp[i].test_result = m_SwitchTestResult[i];
		CalTestResultLevel(&temp[i]);
	}
	

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == MAIN_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == TEST_RESULT_HIGH_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == LOW_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

    for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == OVER_HIGH_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}
	
	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == OVER_LOW_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}
   
	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == CLOSE_HIGH_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == CLOSE_LOW_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == OPEN_FLASH_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}
	
	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == CLOSE_FLASH_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}
	
	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == OPEN_DOOR_RESET_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == NOT_USED)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == HIGH_HIGH_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		if(temp[i].level == NO_LEVEL)
		{
			m_SwitchTestResult_Sorted[sort_id].switch_nr  = i; 
			m_SwitchTestResult_Sorted[sort_id].level = temp[i].level;
			m_SwitchTestResult_Sorted[sort_id].test_result = temp[i].test_result;
			sort_id ++;
		}
	}

	if(sort_id < G_iMaxLedNr[m_BoxNr])
	{
		MessageBox(_T("请检查产品测试参数！测试参数没有覆盖全部温度！"));
	}
}

void CTakeoutPage::CalTestResultLevel(SWITCH_TEST_RESULT_AND_LEVEL *source)
{
	source->level = CheckTestLevel(m_SwitchConfigPara.para,source->test_result,m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType);
	return;
	
}

void CTakeoutPage::OnCheckOverLowLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bOverLowLevel = true;
	this->UpdateData(false);
	ShowTestResult();

}

void CTakeoutPage::ClearAllSelected()
{
	m_bMainLevel		= FALSE;
	m_bLowLevel			= FALSE;
	m_bHighLevel		= FALSE;
	m_bCloseHighLevel	= FALSE;
	m_bCloseLowLevel	= FALSE;
	m_bOverHighLevel	= FALSE;
	m_bOverLowLevel		= FALSE;
	m_bNotUsed			= FALSE;
	m_bOpenFlashLevel	= FALSE;
	m_bCloseFlashLevel	= FALSE;
	m_bOpenDoorResetLevel = FALSE;
	m_bNoLevel			= FALSE;
	m_bCloseFlashLevel  = FALSE;
	m_bHighHighLevel    = FALSE;
}


void CTakeoutPage::OnCheckOverHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bOverHighLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckLowLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bLowLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bHighLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckMainLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bMainLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckCloseHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bCloseHighLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckCloseLowLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bCloseLowLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckOpenFlash() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bOpenFlashLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckOpenDoorReset() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bOpenDoorResetLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckNoAction() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bNotUsed = true;
	this->UpdateData(false);
	ShowTestResult();
}

int AdjustID(int k)
{
#ifndef MAX_LED_NR_256
				return (k);
#else
				if((k % 16 ) >= 8)
				{
					return (128 + (k / 16) * 8 + (k % 16) - 8);
				}
				else
				{
					return ((k / 16) * 8 + (k % 16));
				}

#endif
}

void CTakeoutPage::ShowTestResult()
{
	CString str;
	int item_count;
	if(this->IsWindowVisible())
	{}
	else
	{
		return;
	}
	this->UpdateData();
//	bool hasShowed;
	
//	m_Msg = "";
	m_iNotUsed			= 0;
	m_iMainLevel		= 0;
	m_iLowLevel			= 0;
	m_iHighLevel		= 0;
	m_iHighHighLevel	= 0;
	
	
	m_iOverHighLevel	= 0;
	m_iOverLowLevel		= 0;
	
	m_iCloseHighLevel	= 0;
	m_iCloseLowLevel	= 0;
	
	m_iOpenFlashLevel	= 0;
	m_iCloseFlashLevel	= 0;
	
	m_iOpenDoorResetLevel = 0;

	m_iNoLevel			= 0;

	m_ListTestResult.DeleteAllItems();
	
	extern int AdjustProductID[];
	int i , k;
	for(k=0; k < G_iMaxLedNr[m_BoxNr] / 8; k++)
	{
		i = 0;
		for(int j = 0; j < 8; j++)
		{
			if(m_SwitchTestResult[k*8+j].IsOpenFlash)
			{
				i++;
			}
		}
		if(i >= 4)
		{
			for(int j = 0; j < 8; j++)
			{
				m_SwitchTestResult[k*8+j].IsOpenFlash = 0;
			}
		}

	}

	for(k=0; k < G_iMaxLedNr[m_BoxNr]; k++)
	{
		i = AdjustID(k);//AdjustProductID[k] - 1;

		//clear 
		//DrawOneSwitch(i,m_SwitchTestResult[i].OpenTemp,m_SwitchTestResult[i].CloseTemp,NO_LEVEL,false);
		//hasShowed = false;

	
//不合格的先选出来

		switch(	CheckTestLevel(m_SwitchConfigPara.para,m_SwitchTestResult[i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
		{

//不用的
		case NOT_USED:
			if(m_bNotUsed)
			{
				item_count = m_ListTestResult.GetItemCount(); 
				//str.Format("%d", k+1);
				str = GetSwitchNrString(k);
				CStringArray sArray;
				sArray.Add(str);
				sArray.Add(level_str[NOT_USED]);
				sArray.Add(_T(""));
				sArray.Add(_T(""));
				int index = m_ListTestResult.AddItem(&sArray);
				if(m_SwitchTestResult[i].IsClosing)
				{
					m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
					m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
					m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
					m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
					//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
				}
				else
				{
					m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
					m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
					m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
					m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
					//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
				}
				
			}
			m_iNotUsed ++;
		break;

		case HIGH_HIGH_LEVEL:
			if(m_bHighHighLevel)
			{
				item_count = m_ListTestResult.GetItemCount(); 
				//str.Format("%d", k+1);
				str = GetSwitchNrString(k);
				CStringArray sArray;
				sArray.Add(str);
				sArray.Add(level_str[HIGH_HIGH_LEVEL]);
				str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
				sArray.Add(str);
				str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
				sArray.Add(str);

				int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}

			}
			m_iHighHighLevel ++;
			break;
//动作闪烁		
		case OPEN_FLASH_LEVEL:
			if(m_bOpenFlashLevel)
			{
				item_count = m_ListTestResult.GetItemCount(); 
				//str.Format("%d", k+1);
				str = GetSwitchNrString(k);
				CStringArray sArray;
				sArray.Add(str);
				sArray.Add(level_str[OPEN_FLASH_LEVEL]);
				str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
				sArray.Add(str);
				str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
				sArray.Add(str);

				int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iOpenFlashLevel ++;
		break;
//复位闪烁		
		case CLOSE_FLASH_LEVEL:
			if(m_bCloseFlashLevel)
			{
				item_count = m_ListTestResult.GetItemCount(); 
				//str.Format("%d", k+1);
				str = GetSwitchNrString(k);
				CStringArray sArray;
				sArray.Add(str);
				sArray.Add(level_str[CLOSE_FLASH_LEVEL]);
				str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
				sArray.Add(str);
				str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
				sArray.Add(str);
				int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iCloseFlashLevel ++;
		break;
//高温品
		case OVER_HIGH_LEVEL:
				if(m_bOverHighLevel )
				{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[OVER_HIGH_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
				}
				m_iOverHighLevel++;
		break;
//复大品
		case CLOSE_HIGH_LEVEL:
			if(m_bCloseHighLevel)
			{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[CLOSE_HIGH_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iCloseHighLevel ++;
		break;
		
		
//复小品		
		case CLOSE_LOW_LEVEL:
			if(m_bCloseLowLevel)
			{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[CLOSE_LOW_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iCloseLowLevel ++;
		break;
//开门复位
		case OPEN_DOOR_RESET_LEVEL:
			if(m_bOpenDoorResetLevel)
			{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[OPEN_DOOR_RESET_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iOpenDoorResetLevel ++;
		break;
			
//有用产品			
			
				
//主规格
		case MAIN_LEVEL:
				if(m_bMainLevel)
				{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[MAIN_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);

					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
				}
				m_iMainLevel++;
			break;
			
//低规格
		case LOW_LEVEL:
				if(m_bLowLevel)
				{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[LOW_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
				}
				m_iLowLevel++;
		break;
			
//高规格
		case TEST_RESULT_HIGH_LEVEL:
			if(m_bHighLevel && IsValidTemp(m_SwitchTestResult[i].OpenTemp))
			{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[TEST_RESULT_HIGH_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iHighLevel ++;
		break;
			
				
//低温品			
		case OVER_LOW_LEVEL:
			if(m_bOverLowLevel && IsValidTemp(m_SwitchTestResult[i].OpenTemp))
			{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[OVER_LOW_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iOverLowLevel ++;
		break;
	
//高低复大小
		case NO_LEVEL:
			if(m_bNoLevel)
			{
					item_count = m_ListTestResult.GetItemCount(); 
					//str.Format("%d", k+1);
					str = GetSwitchNrString(k);
					CStringArray sArray;
					sArray.Add(str);
					sArray.Add(level_str[NO_LEVEL]);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].OpenTemp);
					sArray.Add(str);
					str.Format(_T("%5.1f"), m_SwitchTestResult[i].CloseTemp);
					sArray.Add(str);
					int index = m_ListTestResult.AddItem(&sArray);
					if(m_SwitchTestResult[i].IsClosing)
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0xFF,0,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
					else
					{
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,0);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,1);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,2);
						m_ListTestResult.SetItemBackgndColor(RGB(0,0xFF,0),index,3);
						//m_ListTestResult.SetItemTextColor(RGB(0,0,0),index,1);
					}
			}
			m_iNoLevel ++;
		break;
	
		
		}
	}
	this->UpdateData(false); 
}

void CTakeoutPage::OnRadioOnlyOne() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnButtonRecordView2() 
{
	// TODO: Add your control notification handler code here
	CRecordViewDlg dlg;
	dlg.m_BoxNr  = m_BoxNr;
	dlg.DoModal(); 
}

void CTakeoutPage::OnButtonTakeoutDlg() 
{
	// TODO: Add your control notification handler code here
	G_TakeoutDlg.m_SwitchConfigPara = this->m_SwitchConfigPara;
	for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		memcpy(&G_TakeDlgSwitchTestResult[0][i], &m_SwitchTestResult[i], sizeof(m_SwitchTestResult[i]));
	}
	G_TakeoutDlg.ShowTestResult(false); 
	G_TakeoutDlg.m_WindowText = "离线拔板"; 
	G_TakeoutDlg.m_BoxNr = m_BoxNr;
	G_TakeoutDlg.DoModal();
}

void CTakeoutPage::OnCheckNoLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bNoLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

void CTakeoutPage::OnCheckCloseFlash() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bCloseFlashLevel = true;
	this->UpdateData(false);
	ShowTestResult();
}

CString CTakeoutPage::GetSwitchNrString(int i)
{
	CString nr;

	if(G_iMaxLedNr[m_BoxNr] == 192)
	{
		if((i % 12) == 10)
		{
			nr.Format(_T("%dA"), i/12 + 1);
		}
		else if((i % 12) == 11)
		{
			nr.Format(_T("%dB"), i/12 + 1);
		}
		else if((i % 12) == 9)
		{
			nr.Format(_T("%d0"), i/12 + 1);
		}
		else
		{
			nr.Format(_T("%d%d"), (i/12) , (i%12) + 1);
		}
	}
	else
	{
		nr.Format(_T("%d"), i + 1);
	}
	return nr;
}


void CTakeoutPage::OnBnClickedCheckHighHighLevel()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	m_bHighHighLevel = true;
	this->UpdateData(false);
	ShowTestResult();

}
