// SwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "SwitchDlg.h"
#include "comm.h"
#include "shlwapi.h"
#include "RecordViewDlg.h"
#include <stdio.h>
#include "takeDlg.h"
#include "typedefs.h"

#pragma  comment(lib,"shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "TakeDlg.h"
#include "HeatBoxDlg.h"
#include "CopyRight.h"
//#include "ChanliangRecordset.h"
#include "DlgInfomation.h"


SWITCH_TEST_RESULT	G_SwitchTestResult/*[6]*/[500];



BOOL WriteBmp(CString strFile,BYTE *vtData,DWORD dataLen,const SIZE &sizeImg);  
BOOL WriteBmp(CString strFile,HDC hdc);  
BOOL WriteBmp(CString strFile,HDC hdc,CRect rcDC); 

BOOL WriteBmp(CString strFile,BYTE *vtData,DWORD dataLen,const SIZE &sizeImg)   
   {     
     
       BITMAPINFOHEADER bmInfoHeader = {0};  
       bmInfoHeader.biSize		= sizeof(BITMAPINFOHEADER);  
       bmInfoHeader.biWidth		= sizeImg.cx;  
       bmInfoHeader.biHeight	= sizeImg.cy;  
       bmInfoHeader.biPlanes	= 1;  
       bmInfoHeader.biBitCount	= 24;  
     
       //Bimap file header in order to write bmp file  
       BITMAPFILEHEADER bmFileHeader = {0};  
       bmFileHeader.bfType = 0x4d42;  //bmp    
       bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
       bmFileHeader.bfSize = bmFileHeader.bfOffBits + ((bmInfoHeader.biWidth * bmInfoHeader.biHeight) * 3); ///3=(24 / 8)  
     
       HANDLE hFile = CreateFile(strFile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);  
       if(hFile == INVALID_HANDLE_VALUE)  
       {  
           return FALSE;  
       }  
     
       DWORD dwWrite = 0;  
       WriteFile(hFile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWrite,NULL);  
       WriteFile(hFile,&bmInfoHeader, sizeof(BITMAPINFOHEADER),&dwWrite,NULL);  
       WriteFile(hFile,&vtData[0], dataLen , &dwWrite,NULL);  
     
     
       CloseHandle(hFile);  
     
       return TRUE;  
   }   
     
     
   BOOL WriteBmp(CString strFile,HDC hdc)  
   {  
       int iWidth	=  GetDeviceCaps(hdc,HORZRES);  
       int iHeight	=  GetDeviceCaps(hdc,VERTRES);  
       RECT rcDC = {0,0,iWidth,iHeight};  
     
       return WriteBmp(strFile,hdc,rcDC);    
   }  
     
   BOOL WriteBmp(CString strFile,HDC hdc,CRect rcDC)  
   {  
       BOOL bRes			= FALSE;  
       BITMAPINFO bmpInfo	= {0};  
       BYTE *pData			= NULL;  
       SIZE sizeImg			= {0};  
       HBITMAP hBmp			= NULL;  
       BYTE * vtData;  
       HGDIOBJ hOldObj		= NULL;  
       HDC hdcMem			= NULL;  
     
       //Initilaize the bitmap information   
       bmpInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);  
       bmpInfo.bmiHeader.biWidth	= rcDC.right - rcDC.left;  
       bmpInfo.bmiHeader.biHeight	= rcDC.bottom - rcDC.top;  
       bmpInfo.bmiHeader.biPlanes	= 1;  
       bmpInfo.bmiHeader.biBitCount = 24;  
     
       //Create the compatible DC to get the data  
       hdcMem = CreateCompatibleDC(hdc);  
       if(hdcMem == NULL)  
       {  
           goto EXIT;  
       }  
     
       //Get the data from the memory DC     
       hBmp = CreateDIBSection(hdcMem,&bmpInfo,DIB_RGB_COLORS,reinterpret_cast<VOID **>(&pData),NULL,0);  
       if(hBmp == NULL)  
       {  
           goto EXIT;  
       }  
       hOldObj = SelectObject(hdcMem, hBmp);  
         
       //Draw to the memory DC  
       sizeImg.cx = bmpInfo.bmiHeader.biWidth;  
       sizeImg.cy = bmpInfo.bmiHeader.biHeight;  
       StretchBlt(hdcMem,  
                   0,  
                   0,  
                   sizeImg.cx,  
                   sizeImg.cy,  
                   hdc,  
                    rcDC.left,  
                    rcDC.top,  
                    rcDC.right - rcDC.left + 1,  
                    rcDC.bottom - rcDC.top + 1,  
                    SRCCOPY);  
          
      
	    vtData = (BYTE*)GlobalAlloc(GMEM_FIXED, sizeImg.cx * sizeImg.cy * 3);

        memcpy(&vtData[0],pData,sizeImg.cx * sizeImg.cy * 3);  
        
		bRes = WriteBmp(strFile,vtData,sizeImg.cx * sizeImg.cy * 3,sizeImg);  
      
        SelectObject(hdcMem, hOldObj);  
          
      
    EXIT:  
        if(hBmp != NULL)  
        {  
            DeleteObject(hBmp);  
        }  
      
        if(hdcMem != NULL)  
        {  
            DeleteDC(hdcMem);  
        }  
      
        return bRes;  
    }  
   

CTime  GetRecordTime(long seconds)
{
	CTimeSpan span(seconds);

	return STD_TIME + span;
	//return (std_time+span).Format(_T("%Y-%m-%d %H:%M:%S"));  
	
	
}

long  GetSpanSeconds(CTime t)
{
	
	CTimeSpan span = t - STD_TIME;
	return span.GetTotalSeconds(); 
}
/////////////////////////////////////////////////////////////////////////////
// CSwitchDlg dialog
 int SortSwitchGroup(int id)
 {
	 int ret = -1;
	 switch(id)
	 {
	 case 0: 
			ret = 0;
			break;
	 default:
		 ret = 16 - id;
		 break;
	 }
	 return ret;
 }

CSwitchDlg::CSwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSwitchDlg::IDD, pParent)
	, m_OpenTempLow(0)
	, m_OpenTempHigh(0)
	, m_CloseTempLow(0)
	, m_CloseTempHigh(0)
{
	memset(m_CurTemp,0,sizeof(m_CurTemp));
	m_CurTemp[0]				= 0.0f;
	m_CurTemp[1]				= 0.0f;
	m_CurTemp[2]				= 0.0f;
	m_CurTemp[3]				= 0.0f;
	m_SwitchNormalOpen			= 0;
	m_WorkStatus.AutoTestFlag	= false;
	m_WorkStatus.SubMode		= 0;
	m_WorkStatus.RemainTime		= 0;
	m_IsTaking					= false;
	m_iTestMode					= TEST_MODE_NORMAL;
	m_OpenDoor_Temp				= 0;
	m_OpenDoor_StartTime		= m_OpenDoor_StartTime.GetCurrentTime();
	m_OpenDoorIng				=	FALSE;
	//{{AFX_DATA_INIT(CSwitchDlg)
	m_TestMsg = _T("");
	m_PID_Temp = 0.0f;
	m_bHighVolt = TRUE;
	m_TestTimeMsg = _T("");
	//}}AFX_DATA_INIT

	m_bCoolRelay = false;

	m_Page = 0;
}


void CSwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT_1, m_CurTemp[0]);
	DDX_Text(pDX, IDC_EDIT_2, m_CurTemp[1]);
	DDX_Text(pDX, IDC_EDIT_3, m_CurTemp[2]);
	DDX_Text(pDX, IDC_EDIT_4, m_CurTemp[3]);

	//	DDX_Check(pDX, IDC_CHECK_AUTO_PROCESS, m_AutoTestFlag);

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSwitchDlg)
	DDX_Text(pDX, IDC_STATIC_TEST_MSG, m_TestMsg);
	DDX_Radio(pDX, IDC_RADIO_CLOSE, m_SwitchNormalOpen);
	DDX_Text(pDX, IDC_EDIT_5, m_PID_Temp);
	DDX_Check(pDX, IDC_CHECK_HIGH_VOLT, m_bHighVolt);
	DDX_Text(pDX, IDC_STATIC_TIME_MSG, m_TestTimeMsg);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CSwitchDlg, CDialog)
	//{{AFX_MSG_MAP(CSwitchDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_AUTO_PROCESS, OnStart)
	ON_BN_CLICKED(IDC_BUTTON_TAKEOUT, OnButtonTakeout)
	ON_BN_CLICKED(IDC_RADIO_CLOSE, OnRadioClose)
	ON_BN_CLICKED(IDC_RADIO_OPEN, OnRadioOpen)
	ON_BN_CLICKED(IDC_CHECK_HIGH_VOLT, OnCheckHighVolt)
	ON_EN_CHANGE(IDC_EDIT_1, OnChangeEdit1)
	ON_WM_LBUTTONDBLCLK()
	ON_CBN_SELCHANGE(IDC_COMBO_FLASH_GAP, OnSelchangeComboFlashGap)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_NEXT_PAGE, &CSwitchDlg::OnBnClickedCheckNextPage)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CSwitchDlg::OnBnClickedButtonPrint)
	ON_EN_CHANGE(IDC_EDIT_OPEN_TEMP_LOW, &CSwitchDlg::OnEnChangeEditOpenTempLow)
	ON_EN_CHANGE(IDC_EDIT_OPEN_TEMP_HIGH, &CSwitchDlg::OnEnChangeEditOpenTempHigh)
	ON_EN_CHANGE(IDC_EDIT_CLOSE_TEMP_LOW, &CSwitchDlg::OnEnChangeEditCloseTempLow)
	ON_EN_CHANGE(IDC_EDIT_CLOSE_TEMP_HIGH, &CSwitchDlg::OnEnChangeEditCloseTempHigh)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CSwitchDlg::OnBnClickedButtonRecord)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwitchDlg message handlers
	//CBitmap所需资源

void CSwitchDlg::DrawLED(void)
{
//#define TEMP_STRING_HEIGHT	160	//非图标区
//return;
	int i,j,k;
	CFile log_file;
	ShowTestResult_HeYang();
return;

	if(!this->IsWindowVisible()) 
	{
		return;
	}


	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=this->GetWindowDC();
		

	if(dc == NULL)
	{
		m_TestMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_TestMsg,m_TestMsg.GetLength()); 
			log_file.Close();
		}
		return;
	}
	
	CWnd *data_area = GetDlgItem(IDC_STATIC_DATA_AREA);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);
	nWidth	= (wndRect.right - wndRect.left);
	nHeight = wndRect.bottom -data_area_Rect.bottom;//wndRect.top -TEMP_STRING_HEIGHT;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		m_TestMsg.Format(_T("!MemDC.CreateCompatibleDC(dc)%s %d\r\n"),__FILE__,__LINE__);
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_TestMsg,m_TestMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			m_TestMsg.Format(_T("!bitmap.DeleteObject()%s %d\r\n"),__FILE__,__LINE__);
			this->UpdateData(false);
			if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
			{
				log_file.SeekToEnd();
				log_file.Write(m_TestMsg,m_TestMsg.GetLength()); 
				log_file.Close();
			}
			ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth * MAX_HEAT_BOX,nHeight))	//建立与显示设备兼容的位图
	{
		m_TestMsg.Format(_T("!bitmap.CreateCompatibleBitmap%s %d\r\n"),__FILE__,__LINE__);
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_TestMsg,m_TestMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth - PEN_WEIGHT  * 2) / G_iLedNrPerLine - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT * 2) / G_iLedLines     - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	/*
	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}
	*/

	int Ellipse_H_Space = nHeight / G_iLedLines - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	for(k = 0;k < G_CurMaxHeatBox; k++)
	{
		MemDC.RoundRect(PEN_WEIGHT + nWidth * k,PEN_WEIGHT,nWidth-PEN_WEIGHT*3 + nWidth * k + 4,nHeight-PEN_WEIGHT*5 ,10,10);
	}

	MemDC.SelectObject(&graybrush);				//选择画刷
{
		for(i=0; i< G_iLedLines ; i++)
		{
			for(j=0; j < G_iLedNrPerLine; j++)
			{
				if((i * G_iLedNrPerLine + j) >= G_iMaxLedNr)
				{
				}
				else
				{
#ifdef _HANJIE_192
					if(G_SwitchTestResult[m_BoxNr][11 - i + j * G_iLedNrPerLine ].IsClosing)
#else
					if(G_SwitchTestResult[(j % 5)  + i * 5 + 100 * (int)((j) / 5) ].IsClosing == false)
#endif

					{//闭合就是红色
						rect.left = 5 +  j * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2;
						rect.top =  5 +  i * (Ellipse_H     + Ellipse_H_Space) + Ellipse_H_Space / 2;
						rect.right  = rect.left + Ellipse_W;
						rect.bottom = rect.top  + Ellipse_H;
						MemDC.Ellipse(rect);		//填充背景
					}
				}
			}
		}
	}


	MemDC.SelectObject(&redbrush);				//选择画刷


	{
		for(i=0; i< G_iLedLines ; i++)
		{
			for(j=0; j < G_iLedNrPerLine; j++)
			{
				if((i * G_iLedNrPerLine + j) >= G_iMaxLedNr)
				{
				}
				else
				{
#ifdef _HANJIE_192
					if(G_SwitchTestResult[m_BoxNr][11 - i + j * G_iLedNrPerLine ].IsClosing)
#else
					if(G_SwitchTestResult[(j % 5)  + i * 5 + 100 * (int)((j) / 5) ].IsClosing)
#endif

					{//闭合就是红色
						rect.left = 5 +  j * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2;
						rect.top =  5 +  i * (Ellipse_H     + Ellipse_H_Space) + Ellipse_H_Space / 2;
						rect.right  = rect.left + Ellipse_W;
						rect.bottom = rect.top  + Ellipse_H;
						MemDC.Ellipse(rect);		//填充背景
					}
				}
			}
		}
	}


	if(G_iMaxLedNr == 500)
	{
			
					
			k = 0;
			{
				for(i=0; i < G_iLedLines; i++)
				{
					for(j = 0; j < G_iLedNrPerLine; j++)
					{
						CString nr;
						nr.Format(_T("%d"),(j % 5) + 1 + i* 5 + 100 * (int)((j) / 5));
						CSize size = MemDC.GetTextExtent(nr);
						MemDC.TextOut(5 + nWidth * k + (j) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
					}
				}
	
		}
	}
	else
	{
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr; i++)
			{
					CString nr;
					nr.Format(_T("%02d"),i+1);
					CSize size = MemDC.GetTextExtent(nr);
					MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
					
					
			}
		}	
	}

	
	{
		
		
		int nWidth = Ellipse_W_Space;
		int nHeight = Ellipse_H_Space;

		//dc->BitBlt((nWidth +PIC_GAP) * (switchNr % G_iLedNrPerLine),(nHeight+PIC_GAP) * (switchNr / G_iLedNrPerLine),nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
		CBrush tempBrush(RGB(255,255,0));


		MemDC.FillRect(CRect(Ellipse_W_Space*2 +(nWidth +Ellipse_W_GAP) * (05),0,(nWidth +Ellipse_W_GAP) * (05)+Ellipse_W_Space*3,(Ellipse_H + Ellipse_H_Space)*G_iLedLines), &tempBrush);
		MemDC.FillRect(CRect(Ellipse_W_Space*2 +(nWidth +Ellipse_W_GAP) * (10),0,(nWidth +Ellipse_W_GAP) * (10)+Ellipse_W_Space*3,(Ellipse_H + Ellipse_H_Space)*G_iLedLines), &tempBrush);
		MemDC.FillRect(CRect(Ellipse_W_Space*2 +(nWidth +Ellipse_W_GAP) * (15),0,(nWidth +Ellipse_W_GAP) * (15)+Ellipse_W_Space*3,(Ellipse_H + Ellipse_H_Space)*G_iLedLines), &tempBrush);
		MemDC.FillRect(CRect(Ellipse_W_Space*2 +(nWidth +Ellipse_W_GAP) * (20),0,(nWidth +Ellipse_W_GAP) * (20)+Ellipse_W_Space*3,(Ellipse_H + Ellipse_H_Space)*G_iLedLines), &tempBrush);
		MemDC.FillRect(CRect(Ellipse_W_Space*2 +(nWidth +Ellipse_W_GAP) * (25),0,(nWidth +Ellipse_W_GAP) * (25)+Ellipse_W_Space*3,(Ellipse_H + Ellipse_H_Space)*G_iLedLines), &tempBrush);
		MemDC.FillRect(CRect(0,(nHeight+Ellipse_W_GAP) * 10, (nWidth +Ellipse_W_GAP) * G_iLedNrPerLine-Ellipse_W_GAP,(nHeight+Ellipse_W_GAP) * 10-Ellipse_W_GAP), &tempBrush);
		

	}

	dc->BitBlt(0,data_area_Rect.bottom + PEN_WEIGHT * 5 ,nWidth * G_CurMaxHeatBox + 30 ,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
	this->UpdateData(false); 


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	ReleaseDC(dc); 
}

BOOL CSwitchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString s;	

	this->m_CurTemp[0] = 123.4;
	this->m_CurTemp[1] = 123.4;
	this->m_CurTemp[2] = 123.4;
	this->m_CurTemp[3] = 123.4;


	// TODO: Add extra initialization here
	this->SetTimer(NULL,500,NULL); 

#ifdef MAX_LED_NR_256
	CWnd *w = GetDlgItem(IDC_CHECK_HIGH_VOLT);
	w->ShowWindow(SW_HIDE);
	w = GetDlgItem(IDC_COMBO_FLASH_GAP);
	w->ShowWindow(SW_HIDE);
	w = GetDlgItem(IDC_STATIC_FLASH);
	w->ShowWindow(SW_HIDE);
#endif
	//恢复生产记录
	CFile f;
	CString file_name;
	file_name.Format(_T("record%d.bin"),m_BoxNr);
	
#ifdef _DEBUG	
	if(f.Open(theAppDirectory+file_name, CFile::modeReadWrite))
	{
		f.SeekToBegin();
		if(f.Read(&G_SwitchTestResult[m_BoxNr],sizeof(G_SwitchTestResult[m_BoxNr])) != sizeof(G_SwitchTestResult[m_BoxNr]))
		{
			memset(&G_SwitchTestResult[m_BoxNr],0,sizeof(G_SwitchTestResult[m_BoxNr]));
		}
	
		f.Close(); 
	}


	memset(&G_SwitchTestResult[m_BoxNr],0,sizeof(G_SwitchTestResult[m_BoxNr]));
	for(int i =0 ; i < 100; i++)
		{
			G_SwitchTestResult[i].IsUsed		= true;
			G_SwitchTestResult[i].IsClosing	= (i % 2);
			G_SwitchTestResult[i].IsOpenned	= true;
			G_SwitchTestResult[i].OpenTemp		= 90+(i/5.0f);
			G_SwitchTestResult[i].IsClosed		= true;
			G_SwitchTestResult[i].CloseTemp	= 60+(i/2.0f);
		}
		G_SwitchTestResult[0].IsOpenned	= false;
		for(int i =100 ; i < 110; i++)
		{
			G_SwitchTestResult[i].IsUsed		= true;
			G_SwitchTestResult[i].IsClosing	= (i % 2);
			G_SwitchTestResult[i].IsOpenned	= false;
			G_SwitchTestResult[i].OpenTemp		= 0;
			G_SwitchTestResult[i].IsClosed		= false;
			G_SwitchTestResult[i].CloseTemp	= 0;
		}
		G_SwitchTestResult[8].OpenTemp = 88.8;
#endif

	m_BeginTestTime = m_BeginTestTime.GetCurrentTime(); 
	m_EndTestTime   = m_EndTestTime.GetCurrentTime(); 

	
	s.Format(_T("FLASH_GAP_%d"),m_BoxNr);
	


	CTimeSpan span(0,0,1,0);
	m_MeterAnswerTime = m_MeterAnswerTime.GetCurrentTime();
	m_MeterAnswerTime -= span;
	m_LedAnswerTime   = m_LedAnswerTime.GetCurrentTime();
	m_LedAnswerTime  -= span;

	CButton *b = (CButton*)GetDlgItem(IDC_BUTTON_TAKEOUT); 
	if(b != NULL)
	{
		if(b->GetBitmap() == NULL)
		{
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TAKE_BOARD)));
		}
	}
	

	s.Format(_T("TEST_MODE_%d"),m_BoxNr);
	m_iTestMode = AfxGetApp()->GetProfileInt(_T("SETTING"),s,0);


	s.Format(_T("STOP_TEST_TYPE_%d"),m_BoxNr);
	m_StopTestType = AfxGetApp()->GetProfileIntW(_T("SETTING"),s,0);
	

	

	CEdit *edit;

	m_OpenTempLow = AfxGetApp()->GetProfileIntW(_T("SET"),_T("OPEN TEMP LOW"),0) / 10.0f;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_OPEN_TEMP_LOW);
	s.Format(_T("%5.1f"), m_OpenTempLow);
	edit->SetWindowText(s); 

	m_OpenTempHigh = AfxGetApp()->GetProfileIntW(_T("SET"),_T("OPEN TEMP HIGH"),0) / 10.0f;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_OPEN_TEMP_HIGH);
	s.Format(_T("%5.1f"), m_OpenTempHigh);
	edit->SetWindowText(s); 

	m_CloseTempLow = AfxGetApp()->GetProfileIntW(_T("SET"),_T("CLOSE TEMP LOW"),0) / 10.0f;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_CLOSE_TEMP_LOW);
	s.Format(_T("%5.1f"), m_CloseTempLow);
	edit->SetWindowText(s); 

	m_CloseTempHigh = AfxGetApp()->GetProfileIntW(_T("SET"),_T("CLOSE TEMP HIGH"),0) / 10.0f;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_CLOSE_TEMP_HIGH);
	s.Format(_T("%5.1f"), m_CloseTempHigh);
	edit->SetWindowText(s); 


	s = _T("FirstCoolSencondHeat_CloseAC_Before_Heat"); 

	
	this->m_bFirstCoolSencondHeat_CloseAC_Before_Heat =	AfxGetApp()->GetProfileIntW(_T("SETTING"),s,0);  


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSwitchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
}

void CSwitchDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	DrawLED();

	// Do not call CDialog::OnPaint() for painting messages
}

void CSwitchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
}

BOOL CSwitchDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CSwitchDlg::AutoTestStep_DirectHeat(void)
{
	//如果现在的温度已经达到目标温度，就进入下一步
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();

	if((m_PID_Temp + 0.3f )>=  GetControlGoalTemp(SUB_MODE_DIRECT_HEAT))
	{
		m_WorkStatus.SubMode = SUB_MODE_NORMAL_PID;
		
		
		SendDirectHeatCmd(GetControlGoalTemp(SUB_MODE_NORMAL_PID));
	
		m_NormalPidBeginTime = m_NormalPidBeginTime.GetCurrentTime(); 

	}
	else
	{
		m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_DIRECT_HEAT) * (short)10 ; 
		m_WorkStatus.RemainTime = 0;
		this->m_TestMsg.Format(_T("全速升温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_DIRECT_HEAT));
		this->UpdateData(false);
	}
}

void CSwitchDlg::AuotTestStep_NormalPID(void)
{

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent()); 

	CTime cur_t;
	CTimeSpan span;
	//CTimeSpan span_close;
	
	cur_t	=	CTime::GetCurrentTime(); 
	span	=	cur_t - m_NormalPidBeginTime;
	
	//如果时间已到，就进入下一步
			
	if( span.GetMinutes() >= m_iNormalPidMinutes)
	{
		
		m_OpenDoorIng	= false;

		m_OpenDoor_Temp	= GetControlGoalTemp(SUB_MODE_NORMAL_PID) + m_OpenDoorResetHeatDegree; 

		m_WorkStatus.SubMode = SUB_MODE_STEP_PID;

		m_StepPidBeginTime = m_StepPidBeginTime.GetCurrentTime(); 
		
		
		this->m_LastStepPidTemp = this->m_CurTemp[0]; 
		
		if(m_iTestMode == TEST_MODE_OPEN_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH || m_iTestMode == TEST_MODE_CLOSE_FLASH )
		{
				maindlg->SetCmdHeat(true);

				maindlg->SetCmdAirControl(true);

				maindlg->SetCmdAirValve(true);

				SendDirectHeatCmd(m_OpenDoor_Temp);
		}
		else
		{
			SendStepPidHeatCmd();
		}

		
	}
	else
	{
	    m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_NORMAL_PID) * (short)10; 
		m_WorkStatus.RemainTime = (m_iNormalPidMinutes - span.GetMinutes()) * 60 - span.GetSeconds(); 
		this->UpdateData();
		span	=	cur_t - m_NormalPidBeginTime;
		this->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_NORMAL_PID) ,span.GetMinutes(),span.GetSeconds());
		this->UpdateData(false);
	}
}
void CSwitchDlg::AutoTestStep_StepPID(void)
{
	//如果现在的温度已经达到目标温度，就进入下一步
	// 或者时间已到

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());  
		


	if((m_PID_Temp  >=  m_ControlTempRange.open_temp_max + G_iHighHighLevelDelta)
		|| IsAllOpened())
	{
		if(m_ControlTempRange.close_temp_min == m_ControlTempRange.open_temp_max)
		{//不用测复位，直接结束测试
			this->ExitAutoTest(); 
		}
		else
		{
		
			
				
			maindlg->SetCmdAirControl(true); 
			maindlg->SetCmdAirValve(false); 

			if(m_iStepDownPidSpeed > 0)
			{//降温也要控制
				//SendDirectHeatCmd(m_ControlTempRange.close_temp_max + STEP_COOL_DELTA);
				//m_WorkStatus.SubMode = SUB_MODE_STEP_COOL;

				
				this->m_LastStepPidCoolTemp = this->m_CurTemp[0]; 

				SendDirectHeatCmd(m_LastStepPidCoolTemp);

				SendStepPIDCoolCmd();

				m_StepCoolPidBeginTime = CTime::GetCurrentTime();

				m_WorkStatus.SubMode = SUB_MODE_STEP_COOL;

				m_bIsClosingDoor = false;

				m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_STEP_COOL) * (short)10 ; 
				m_WorkStatus.RemainTime = 0;
				this->UpdateData();
				this->m_TestMsg.Format(_T("匀速降温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_STEP_COOL));
				this->UpdateData(false);

			}
			else
			{
				m_WorkStatus.SubMode = SUB_MODE_DIRECT_COOL;
				SendDirectHeatCmd(0);
			}
			 this->UpdateData();
			this->m_TestMsg.Format(_T("降温到 %5.1f度 "),m_SwitchConfigPara.CloseLowLevel - m_SwitchConfigPara.CloseLowLevelSpanMinus);
			this->UpdateData(false);

			
			
		}

	}
	else
	{
		
		CTime cur = cur.GetCurrentTime();
		CTimeSpan span = cur - m_StepPidBeginTime; 
		
		

		if(m_iTestMode == TEST_MODE_OPEN_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH)
		{
			OpenDoor_For_OpenFlashTest();
		}
		else
		{
			SendStepPidHeatCmd();

			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_STEP_PID) * (short)10; 
			
			int step_pid_minutes = (GetControlGoalTemp(SUB_MODE_STEP_PID)- GetControlGoalTemp(SUB_MODE_NORMAL_PID)) * 10 / m_iStepPidSpeed; 
			m_WorkStatus.RemainTime = step_pid_minutes  * 60 - span.GetTotalSeconds(); 
			this->UpdateData();
			this->m_TestMsg.Format(_T("线性升温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_PID),span.GetMinutes(),span.GetSeconds());
			this->UpdateData(false);
		}
		
	}
}

void CSwitchDlg::AutoTestStep_DircetCool(void)
{
	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;



	
	if(m_iStepDownPidSpeed > 0)
	{//降温也要控制
		if((m_PID_Temp < m_ControlTempRange.close_temp_max + STEP_COOL_DELTA  )
			|| IsAllClosed())
		{

			int minutes = (m_PID_Temp - m_ControlTempRange.close_temp_min) / m_iStepDownPidSpeed;
			SendDirectHeatCmd(m_ControlTempRange.close_temp_min);

			this->m_LastStepPidCoolTemp = this->m_CurTemp[0]; 

			SendStepPIDCoolCmd();

			m_WorkStatus.SubMode = SUB_MODE_STEP_COOL;

			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_STEP_COOL) * (short)10 ; 
			m_WorkStatus.RemainTime = 0;
			this->UpdateData();
			this->m_TestMsg.Format(_T("匀速降温到 %5.1f度"),GetControlGoalTemp(SUB_MODE_STEP_COOL));
			this->UpdateData(false);
		}
		else
		{//每分钟1度
			if(m_PID_Temp <= m_LastClosePIDTemp)
			{
				span = CTime::GetCurrentTime() - m_ClosePIDStartTime;
				if(span.GetMinutes() >= 1)
				{
					m_ClosePIDStartTime = m_ClosePIDStartTime.GetCurrentTime();
					m_LastClosePIDTemp = m_LastClosePIDTemp - 1;
					SendDirectHeatCmd(m_LastClosePIDTemp);
					m_TestMsg.Format(_T("恒温在%5.1f度1分钟"), m_LastClosePIDTemp);
				}
			}
		}
	}
	else
	{
		OpenDoor_For_CloseFlashTest();

		cur_t =  CTime::GetCurrentTime(); 
		span = cur_t - m_SendCommand.SendCmdTime;
		m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_DIRECT_COOL) * (short)10; 
		//if(m_iTestMode != TEST_MODE_CLOSE_DOOR_ONCE)
		{
			double stop_temp ;
			if(m_StopTestType == 0)
			{//降温到最低复位温度还是一直降温
				stop_temp = m_ControlTempRange.close_temp_min;
			}
			else
			{
				stop_temp = 0;
			}
			if((m_PID_Temp < stop_temp ) || (m_ControlTempRange.close_temp_max ==  m_ControlTempRange.close_temp_min)
				|| IsAllClosed())
			{
				
					ExitAutoTest();
				
			}
			else
			{
				this->UpdateData();
				if(m_StopTestType == 0)
				{
					this->m_TestMsg.Format(_T("降温到 %5.1f度 "),stop_temp);
				}
				else
				{
					this->m_TestMsg = _T("降温到所有产品都复位 ");
				}
				this->UpdateData(false);
			}
		}
	}

}
void CSwitchDlg::AutoTestStep_HeatAgain(void)
{
	if((GetLowestCloseTemp() < m_ControlTempRange.close_temp_max)
						&& (GetLowestCloseTemp() < 200))
	{//此时至少有一个复位的产品	
		this->m_TestMsg.Format(_T("升温到 %5.1f度 "),(float)GetHighestCloseTemp());
		if(m_PID_Temp > GetHighestCloseTemp())
		{
			SendDirectHeatCmd(0);

			ExitAutoTest();

		}
	}
	else
	{
			SendDirectHeatCmd(0);

			ExitAutoTest();
	
	}
}

void CSwitchDlg::AutoTestCoolStep_DriectCool(void)
{
	if(m_PID_Temp  <  this->GetControlGoalTemp(SUB_COOL_MODE_DIRECT_COOL))
	{
		m_WorkStatus.SubMode = SUB_COOL_MODE_PID_COOL;
		m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_PID_COOL) * (short)10; 
		SendDirectHeatCmd(GetControlGoalTemp(SUB_COOL_MODE_PID_COOL));
		m_StepPidBeginTime = CTime::GetCurrentTime();
	}
	else
	{
		
			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_DIRECT_COOL) * (short)10; 
			
			this->UpdateData();
			this->m_TestMsg.Format(_T("全速降温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_MODE_DIRECT_COOL));
			this->UpdateData(false);

		
	}

}

void CSwitchDlg::AutoTestCoolStep_PidCool(void)
{
	CTimeSpan span = CTime::GetCurrentTime() - m_StepPidBeginTime;
	if(span.GetTotalMinutes() >= m_iNormalPidMinutes)
	{
		m_WorkStatus.SubMode = SUB_COOL_MODE_STEP_COOL;
		m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_STEP_COOL) * (short)10; 
		
		if((m_iTestMode == TEST_MODE_NORMAL) && (m_iStepDownPidSpeed > 0))
		{
			m_StepCoolPidBeginTime = CTime::GetCurrentTime();
			this->m_LastStepPidCoolTemp = this->m_CurTemp[0]; 
			SendStepPIDCoolCmd();
			SendDirectHeatCmd(m_LastStepPidCoolTemp);
		}
		else
		{
			SendDirectHeatCmd(GetControlGoalTemp(SUB_COOL_MODE_STEP_COOL));
		}
	}
	else
	{
		
			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_PID_COOL) * (short)10; 
			
			this->UpdateData();
			this->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_COOL_MODE_PID_COOL),(int)(span.GetTotalSeconds()/60),(int)(span.GetTotalSeconds() % 60));
			this->UpdateData(false);

		
	}

}
void CSwitchDlg::AutoTestCoolStep_DirectHeat(void)
{
	if(m_PID_Temp  >=  GetControlGoalTemp(SUB_COOL_MODE_DIRECT_HEAT))
	{
		m_WorkStatus.SubMode = SUB_COOL_MODE_PID_HEAT;
		m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_PID_HEAT) * (short)10; 
		SendDirectHeatCmd(GetControlGoalTemp(SUB_COOL_MODE_PID_HEAT));
		this->UpdateData();
		this->m_TestMsg.Format(_T("恒温在 %5.1f度"),GetControlGoalTemp(SUB_COOL_MODE_PID_HEAT));
		m_NormalPidBeginTime	=	CTime::GetCurrentTime();
		this->UpdateData(false);
	}
	else
	{
		
			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_DIRECT_HEAT) * (short)10; 
			
			this->UpdateData();
			this->m_TestMsg.Format(_T("全速升温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_MODE_DIRECT_HEAT));
			this->UpdateData(false);
	}
}
void CSwitchDlg::AutoTestCoolStep_PidHeat(void)
{
	CTimeSpan span = CTime::GetCurrentTime() - m_NormalPidBeginTime;

	if(span.GetMinutes() >=  m_iNormalPidMinutes)
	{
		m_WorkStatus.SubMode = SUB_COOL_MODE_STEP_HEAT;
		m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_STEP_HEAT) * (short)10; 
		//SendDirectHeatCmd(GetControlGoalTemp(SUB_COOL_MODE_STEP_HEAT));
		m_StepPidBeginTime = m_StepPidBeginTime.GetCurrentTime(); 
		
		this->m_LastStepPidTemp = this->m_CurTemp[0]; 

		m_OpenDoor_Temp	= GetControlGoalTemp(SUB_COOL_MODE_PID_COOL) + m_OpenDoorResetHeatDegree; 

		if(m_iTestMode == TEST_MODE_OPEN_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH || m_iTestMode == TEST_MODE_CLOSE_FLASH )
		{
			SendDirectHeatCmd(m_OpenDoor_Temp);
		}
		else
		{
			SendStepPidHeatCmd();
		}

		this->UpdateData();
		this->m_TestMsg.Format(_T("线性升温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_MODE_STEP_HEAT));
		this->UpdateData(false);
	}
	else
	{
			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_PID_HEAT) * (short)10; 
			this->UpdateData();
			this->m_TestMsg.Format(_T("恒温在 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_COOL_MODE_PID_HEAT),(int)(span.GetTotalSeconds() / 60) ,(int)(span.GetTotalSeconds() % 60) );
			this->UpdateData(false);
	}
}
void CSwitchDlg::AutoTestCoolStep_StepHeat(void)
{
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());  
	
	if((m_PID_Temp  >=  m_ControlTempRange.close_temp_max + G_iHighHighLevelDelta)
		/*|| IsAllClosed()*/)
	{
		ExitAutoTest(); 
	}
	else
	{

		CTimeSpan span = CTime::GetCurrentTime() - m_StepPidBeginTime; 


		if(m_iTestMode == TEST_MODE_CLOSE_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH)
		{
			//OpenDoor_For_OpenFlashTest();
		}
		else
		{
			SendStepPidHeatCmd();

			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_STEP_HEAT) * (short)10; 
			
			int step_pid_minutes = (GetControlGoalTemp(SUB_COOL_MODE_STEP_HEAT)- GetControlGoalTemp(SUB_COOL_MODE_PID_HEAT)) * 10 / m_iStepPidSpeed; 
			m_WorkStatus.RemainTime = step_pid_minutes  * 60 - span.GetTotalSeconds(); 
			this->UpdateData();
			this->m_TestMsg.Format(_T("线性升温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_COOL_MODE_STEP_HEAT),(int)(span.GetTotalSeconds() / 60) ,(int)(span.GetTotalSeconds() % 60));
			this->UpdateData(false);
		}
		
	}
}


void CSwitchDlg::AutoTestCoolStep_StepCool(void)
{
	
	if(m_PID_Temp  <  m_ControlTempRange.open_temp_min)
	{
		m_WorkStatus.SubMode = SUB_COOL_MODE_DIRECT_HEAT;
		m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_DIRECT_HEAT) * (short)10; 
		SendDirectHeatCmd(GetControlGoalTemp(SUB_COOL_MODE_DIRECT_HEAT));
		this->UpdateData();
		this->m_TestMsg.Format(_T("全速升温到 %5.1f度"),GetControlGoalTemp(SUB_COOL_MODE_DIRECT_HEAT));
		this->UpdateData(false);

		if(m_bFirstCoolSencondHeat_CloseAC_Before_Heat)
		{

			CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();

			maindlg->SetCmdAirControl(false);

		}

	}
	else
	{
		CTime cur = cur.GetCurrentTime();
		CTimeSpan span = cur - m_StepCoolPidBeginTime; 
		
		if(m_iTestMode == TEST_MODE_CLOSE_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH)
		{
			//OpenDoor_For_CloseFlashTest();

		}
		else
		{
			SendStepPIDCoolCmd();

			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_STEP_COOL) * (short)10; 
			
			m_WorkStatus.RemainTime = span.GetTotalSeconds(); 
			this->UpdateData();
			this->m_TestMsg.Format(_T("线性降温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_COOL_MODE_STEP_COOL),span.GetTotalMinutes(),(span.GetTotalSeconds() % 60));
			this->UpdateData(false);
		}
		
	}
}

void CSwitchDlg::AutoTestStep_StepCool(void)
{
	
	//如果现在的温度已经达到目标温度，就进入下一步
	// 或者时间已到

	if((m_PID_Temp  <  m_ControlTempRange.close_temp_min)
		|| IsAllClosed())
	{
		this->ExitAutoTest(); 
	}
	else
	{
		CTime cur = cur.GetCurrentTime();
		CTimeSpan span = cur - m_StepCoolPidBeginTime; 
		int seconds = span.GetTotalSeconds(); 
		int minutes = seconds / 60;
		seconds = (seconds % 60);

		if(m_iTestMode == TEST_MODE_CLOSE_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH)
		{
			OpenDoor_For_CloseFlashTest();

		}
		else
		{
			SendStepPIDCoolCmd();

			m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_MODE_STEP_COOL) * (short)10; 
			
			m_WorkStatus.RemainTime = span.GetTotalSeconds(); 
			this->UpdateData();
			this->m_TestMsg.Format(_T("线性降温到 %5.1f度 %02d:%02d"),GetControlGoalTemp(SUB_MODE_STEP_COOL),minutes ,seconds);
			this->UpdateData(false);
		}
		
	}
}
void CSwitchDlg::OnTimer(UINT nIDEvent) 
{
	//ShowTestResult_HeYang();

	CString cmdStr;
	CTime cur_t;
	CTimeSpan span;
	CTimeSpan span_close;

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	


	cur_t = cur_t.GetCurrentTime();

	if(m_IsTaking == true)
	{
		m_MeterAnswerTime =  m_MeterAnswerTime.GetCurrentTime();
	}

	span = m_MeterAnswerTime.GetCurrentTime() - m_MeterAnswerTime;

	if( (span.GetTotalSeconds() % 20) == 19)
	{
		//WriteLogFile("温度仪表无响应!",m_BoxNr+1,true);
	}
	if( span.GetTotalSeconds() > 20)
	{this->UpdateData();
		m_TestTimeMsg = _T("温度仪表无响应!");
		
		span = m_LedAnswerTime.GetCurrentTime() - m_LedAnswerTime;
	
		if( (span.GetTotalSeconds() % 30) == 29)
		{
			//WriteLogFile("灯板无响应!",m_BoxNr+1,true);
		}
		if( span.GetTotalSeconds() > 20)
		{
			m_TestTimeMsg = _T("温度仪表、灯板无响应!");
		}
		this->UpdateData(false);

	}
	else
	{
	
		if(m_TestTimeMsg.Find(_T("无响应")) >= 0)
		{this->UpdateData();
			m_TestTimeMsg = _T("");
			this->UpdateData(false);
		}
	}

	m_WorkStatus.CurTemp [0] = (short)(this->m_CurTemp[0] *10); 
	m_WorkStatus.CurTemp [1] = (short)(this->m_CurTemp[1] *10); 
	m_WorkStatus.CurTemp [2] = (short)(this->m_CurTemp[2] *10); 
	m_WorkStatus.CurTemp [3] = (short)(this->m_CurTemp[3] *10); 


		
	if(m_WorkStatus.AutoTestFlag == false)
	{
		
		m_WorkStatus.GoalTemp = 0;
	}
	else
	{//自动测试状态
		m_EndTestTime = m_EndTestTime.GetCurrentTime();
		
		CTimeSpan ts = m_EndTestTime - m_BeginTestTime;
		
		m_TestTimeMsg.Format(_T("本次测试用时 %02d:%02d:%02d"),ts.GetHours() ,ts.GetMinutes() ,ts.GetSeconds() );
		
		this->UpdateData(false);

		switch(m_WorkStatus.SubMode)
		{//判断是否要进入下一个测试阶段
			case SUB_MODE_DIRECT_HEAT:
				AutoTestStep_DirectHeat();
				break;
			case SUB_MODE_NORMAL_PID: //恒温
				AuotTestStep_NormalPID();
				break;
			case SUB_MODE_STEP_PID://匀速加热
				AutoTestStep_StepPID();
				
				break;
			case SUB_MODE_DIRECT_COOL: //全速降温
				
				AutoTestStep_DircetCool();
				break;
			case SUB_MODE_STEP_COOL: //匀速降温
				
				AutoTestStep_StepCool();
				break;
			case SUB_MODE_HEAT_AGAIN:

				AutoTestStep_HeatAgain();
				
				break;

			case SUB_COOL_MODE_DIRECT_COOL:
				AutoTestCoolStep_DriectCool();
				break;

			case SUB_COOL_MODE_PID_COOL:
				CSwitchDlg::AutoTestCoolStep_PidCool();
				break;

			case SUB_COOL_MODE_STEP_COOL:
				AutoTestCoolStep_StepCool();
				break;

			case SUB_COOL_MODE_DIRECT_HEAT:
				AutoTestCoolStep_DirectHeat();
				break;

			case SUB_COOL_MODE_PID_HEAT:
				AutoTestCoolStep_PidHeat();
				break;

			case SUB_COOL_MODE_STEP_HEAT:
				AutoTestCoolStep_StepHeat();
				break;


		}

	
	}
//	DrawLED();
	CDialog::OnTimer(nIDEvent);
}

void CSwitchDlg::SaveCurve(void) 
{
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	CString s;
	s.Format(_T("d:\\温度曲线[%d]%04d%02d%02d%02d%02d%02d.bmp"),
		m_BoxNr+1,
				maindlg->m_SwitchDlg.m_BeginTestTime.GetYear(),
				maindlg->m_SwitchDlg.m_BeginTestTime.GetMonth(),
				maindlg->m_SwitchDlg.m_BeginTestTime.GetDay(),
				maindlg->m_SwitchDlg.m_BeginTestTime.GetHour(),
				maindlg->m_SwitchDlg.m_BeginTestTime.GetMinute(),
				maindlg->m_SwitchDlg.m_BeginTestTime.GetSecond());

	maindlg->m_Tab[m_BoxNr].SetCurSel(3); 
	maindlg->SelectTab(m_BoxNr);
	maindlg->m_CurveDlg.RedrawWindow(); 
	
	maindlg->m_CurveDlg.OnExportChart(s);  
	
	maindlg->m_Tab[m_BoxNr].SetCurSel(0); 
	maindlg->SelectTab(m_BoxNr);
}

void CSwitchDlg::OnStart(void) 
{
	
	memset(&m_SwitchConfigPara,0,sizeof(m_SwitchConfigPara));

	m_SwitchConfigPara.MainLevel = this->m_OpenTempLow;
	m_SwitchConfigPara.MainLevelSpanMinus = 0;
	m_SwitchConfigPara.MainLevelSpanPlus = m_OpenTempHigh - m_OpenTempLow;

	m_SwitchConfigPara.CloseLevel = m_CloseTempLow;
	m_SwitchConfigPara.CloseLevelSpanMinus = 0;
	m_SwitchConfigPara.CloseLevelSpanPlus = m_CloseTempHigh - m_CloseTempLow;


	// TODO: Add your control notification handler code here
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	

	m_HeatCoolOrder = CheckHeatOrCoolMode(m_SwitchConfigPara,&m_ControlTempRange);

	UpdateData();

	char para[32];
	memset(para,0,sizeof(para));

	CString s;
	s.Format(_T("TEST_MODE_%d"),m_BoxNr);
	m_iTestMode = AfxGetApp()->GetProfileInt(_T("SETTING"),s,0);
 

	m_OpenDoor_SwitchClosed[0] = 0;
	m_OpenDoor_SwitchClosed[1] = 0;
	m_OpenDoor_SwitchClosed[2] = 0;

	if(m_WorkStatus.AutoTestFlag == false)
	{

		this->UpdateData(true);

		if(m_ControlTempRange.open_temp_max == m_ControlTempRange.open_temp_min)
		{
			AfxMessageBox(_T("请选择正确的测试参数！"));
			return;
		}

#ifndef _DEBUG
		
		double min_temp = 999.9,max_temp = -999.9;
		
		for(int i = 0; i < 4; i++)
		{
			if(min_temp > m_CurTemp[i])
			{
				min_temp = m_CurTemp[i];
			}
			
			if(max_temp < m_CurTemp[i])
			{
				max_temp = m_CurTemp[i];
			}
		}


		if(max_temp - min_temp > 30.0f)
		{

			AfxMessageBox(_T("传感器温度不均匀，请检查传感器"));
			return;

		}
#endif

		if((m_CurTemp[0]<= -40.0f ||  m_CurTemp[0]> 200)
			&& (m_CurTemp[1]<= -40.0f ||  m_CurTemp[1]> 200)
			&& (m_CurTemp[2]<= -40.0f ||  m_CurTemp[2]> 200)
			&& (m_CurTemp[3]<= -40.0f ||  m_CurTemp[3]> 200))
		{
#ifndef _DEBUG
			AfxMessageBox(_T("传感器温度不正常，请检查传感器"));
			return;
#endif
		}

		
		

		
		if(m_iContinueTestCount <= 0)
		{
			
			CTimeSpan span = m_MeterAnswerTime.GetCurrentTime() - m_MeterAnswerTime;

			if( span.GetTotalSeconds() > 10)
			{
				
#ifndef _DEBUG
				AfxMessageBox(_T("温度仪表无响应!"),MB_ICONSTOP);
				return;
#endif
			}
			else
			{
				span = m_LedAnswerTime.GetCurrentTime() - m_LedAnswerTime;
			
				if( span.GetTotalSeconds() > 10)
				{
						
#ifndef _DEBUG
					AfxMessageBox(_T("灯板无响应!"),MB_ICONSTOP);
					//return;
#endif
				}
				
			}


			CString str;
			CString str2;
			if(m_HeatCoolOrder == HEAT_FIRST_COOL_SECOND)
			{
				str.Format(_T("全速加热到   % 5.1f度\r\n\r\n恒温在         % 8.1f度 %2d分钟\r\n\r\n线性加热到   % 5.1f度% 5.1f度/分钟\r\n\r\n"),
					GetControlGoalTemp(SUB_MODE_DIRECT_HEAT),
					GetControlGoalTemp(SUB_MODE_NORMAL_PID), this->m_iNormalPidMinutes,
					GetControlGoalTemp(SUB_MODE_STEP_PID),   this->m_iStepPidSpeed / 10.0f);
			}
			else
			{
				str.Format(_T("全速降温到   % 5.1f度\r\n\r\n恒温在         % 8.1f度 %2d分钟\r\n\r\n线性降温到   % 5.1f度% 5.1f度/分钟\r\n\r\n"),
					GetControlGoalTemp(SUB_COOL_MODE_DIRECT_COOL),
					GetControlGoalTemp(SUB_COOL_MODE_PID_COOL),		this->m_iNormalPidMinutes,
					GetControlGoalTemp(SUB_COOL_MODE_STEP_COOL),	this->m_iStepPidSpeed / 10.0f);
			}
			if(this->m_SwitchNormalOpen == 0)
			{
				str2 = _T("保护器类型： 常闭\r\n\r\n");
			}
			else
			{
				str2 = _T("保护器类型： 常开\r\n\r\n");
			}

			
			if(m_ControlTempRange.close_temp_min == m_ControlTempRange.close_temp_max)
			{
				str2 = _T("不测复位温度");
			}
			else
			{
				str2.Format(_T("复位温度下限 % 5.1f度\r\n复位温度上限 % 5.1f度\r\n"),m_ControlTempRange.close_temp_min,m_ControlTempRange.close_temp_max);
			}

			str += str2;

			CDlgInfomation dlg;

			s.Format(_T("TEST MEMO %d"), m_BoxNr);
			dlg.m_BoxNr = m_BoxNr;
		
			dlg.m_Memo = AfxGetApp()->GetProfileStringW(_T("SETTING"), s ,_T("请输入本次测试的说明"));
			dlg.m_Info = str;
			if(dlg.DoModal() != IDOK)
			{
				return ;
			}
			m_TestMemo = dlg.m_Memo;

			
			maindlg->m_TempParaDlg.m_ParaName = m_TestMemo;
			maindlg->m_TempParaDlg.UpdateData(false);
			
			maindlg->m_CurveDlg.m_TempArray[0].RemoveAll();
			maindlg->m_CurveDlg.m_TempArray[1].RemoveAll();
			maindlg->m_CurveDlg.m_TempArray[2].RemoveAll();
			maindlg->m_CurveDlg.m_TempArray[3].RemoveAll();
		}

	

		#define SET_FLASH_TEST_SPEED	0x16

		
		{
			if(m_SwitchNormalOpen == 0)
			{//常闭
				for(int i = 0; i < G_iMaxLedNr; i++)
				{
					if(G_SwitchTestResult[i].IsClosing)
					{
						G_SwitchTestResult[i].IsUsed		= true;	//产品是否插入
					}
					else
					{
						G_SwitchTestResult[i].IsUsed		= false;	//产品是否插入
					}
					G_SwitchTestResult[i].IsOpenned			= false;	//是否曾经打开过
					G_SwitchTestResult[i].IsClosed			= false;		//是否曾经复位过
					G_SwitchTestResult[i].IsOpenFlash		= false;		//是否动作时闪动
					G_SwitchTestResult[i].IsCloseFlash		= false;	//是否复位时闪动
					G_SwitchTestResult[i].IsCloseFlashTested = false;	//是否复位时闪动
					G_SwitchTestResult[i].IsOpenFlashTested =	false;
					G_SwitchTestResult[i].IsMoreThan1M		=	false;
					G_SwitchTestResult[i].OpenTemp			= 0.0f;		//动作温度
					G_SwitchTestResult[i].CloseTemp			= 0.0f;	//复位温度	
					G_SwitchTestResult[i].OpenFlashTemp		= 0.0f;		//动作温度
					G_SwitchTestResult[i].CloseFlashTemp	= 0.0f;	//复位温度	
					G_SwitchTestResult[i].IsOpenFlashTested = false;

				}
			}
			else
			{//常开
				for(int i = 0; i < G_iMaxLedNr; i++)
				{
					if(G_SwitchTestResult[i].IsClosing)
					{
						G_SwitchTestResult[i].IsUsed		= false;	//产品是否插入
					}
					else
					{
						G_SwitchTestResult[i].IsUsed		= true;	//产品是否插入
					}
					G_SwitchTestResult[i].IsOpenned			= false;	//是否曾经打开过
					G_SwitchTestResult[i].IsClosed			= false;		//是否曾经复位过
					G_SwitchTestResult[i].IsOpenFlash		= false;		//是否动作时闪动
					G_SwitchTestResult[i].IsCloseFlash		= false;	//是否复位时闪动
					G_SwitchTestResult[i].IsCloseFlashTested= false;	//是否复位时闪动
					G_SwitchTestResult[i].IsMoreThan1M		= false;
					G_SwitchTestResult[i].OpenTemp			= 0.0f;		//动作温度
					G_SwitchTestResult[i].CloseTemp			= 0.0f;	//复位温度	
					G_SwitchTestResult[i].OpenFlashTemp		= 0.0f;		//动作温度
					G_SwitchTestResult[i].CloseFlashTemp	= 0.0f;	//复位温度	
				}
			}
		}

		m_BeginTestTime = m_BeginTestTime.GetCurrentTime(); 
		m_EndTestTime   = m_EndTestTime.GetCurrentTime(); 
		if(m_HeatCoolOrder == HEAT_FIRST_COOL_SECOND)
		{
			EnterAutoTest();
		}
		else
		{
			EnterAutoTest_COOL_FIRST();
		}
	}
	else  
	{
		this->UpdateData(true);
		CString str;

		if( m_WorkStatus.SubMode == SUB_MODE_DIRECT_COOL)
		{//提前终止测试功能

			str.Format(_T("%d 号烘箱正在测试，需要停止吗?"),m_BoxNr+1);
		
			int ret = MessageBox(str, strMsgCaption,MB_OKCANCEL | MB_ICONQUESTION);
			if( ret != IDOK)
			{
				return;
			}

			m_iContinueTestCount = -1;

			SendDirectHeatCmd(GetHighestCloseTemp());

			m_WorkStatus.SubMode ++;
		}
		else if(m_iTestMode == TEST_MODE_NORMAL && m_WorkStatus.SubMode == SUB_MODE_STEP_PID)
		{
			str.Format(_T("%d 号烘箱正在升温，需要提前降温吗?"),m_BoxNr+1);
			
			int ret = MessageBox(str, strMsgCaption,MB_OKCANCEL | MB_ICONQUESTION);
			if( ret != IDOK)
			{
				return;
			}

			SendDirectHeatCmd(0);

			m_WorkStatus.SubMode ++;
		}
		else
		{

			str.Format(_T("%d 号烘箱正在测试，需要停止吗?"),m_BoxNr+1);
			
			int ret = MessageBox(str, strMsgCaption,MB_OKCANCEL | MB_ICONQUESTION);
			if( ret != IDOK)
			{
				return;
			}

			m_iContinueTestCount = -1;
			ExitAutoTest();

			CWnd *button;
			button = GetDlgItem(IDC_BUTTON_TAKEOUT);
			button->EnableWindow(true);
			
			m_EndTestTime = m_EndTestTime.GetCurrentTime(); 
		}
	}
	
}

void CSwitchDlg::OnButtonTakeout() 
{
	// TODO: Add your control notification handler code here

	
//	Dlg.DoModal();

	//初始化数据
	if(m_WorkStatus.AutoTestFlag == true)
	{
		m_TakeoutDlg.m_SwitchConfigPara = this->m_SwitchConfigPara;
		for(int j = 0; j < G_iMaxLedNr; j++)
		{
			memcpy(&G_TakeDlgSwitchTestResult[j], &G_SwitchTestResult[j], sizeof(G_SwitchTestResult[j]));
			memcpy(&G_TakeDlgSwitchTestResult_Bak[j], &G_SwitchTestResult[j], sizeof(G_SwitchTestResult[j]));
		}
		m_TakeoutDlg.m_iTakeBoardType = 0;
		
		m_TakeoutDlg.m_FlashGap		= 1;

		m_TakeoutDlg.m_WindowText		= _T("在线查看"); 
		m_TakeoutDlg.m_ParaName			=  m_ParaName;
		m_TakeoutDlg.m_SwitchNormalOpen = m_SwitchNormalOpen;
		m_TakeoutDlg.m_BeginTestTime	= m_BeginTestTime;
		m_TakeoutDlg.m_EndTestTime		= m_EndTestTime;
		
		m_TakeoutDlg.m_iDisplayTempType = this->m_SwitchConfigPara.ParaType;
		m_TakeoutDlg.m_BoxNr = m_BoxNr;
		m_TakeoutDlg.m_iSelect = 0;//2;
		m_TakeoutDlg.m_HeatBoxType	= (HEAT_BOX_TYPE)m_HeatBoxType;
		m_TakeoutDlg.SetAllSelected();
		m_IsTaking	=	false;
		m_TakeoutDlg.DoModal();
	}
	else
	{
		m_TakeoutDlg.m_SwitchConfigPara = this->m_SwitchConfigPara;
		for(int j = 0; j < G_iMaxLedNr; j++)
		{
			memcpy(&G_TakeDlgSwitchTestResult[0][j], &G_SwitchTestResult[j], sizeof(G_SwitchTestResult[j]));
			memcpy(&G_TakeDlgSwitchTestResult_Bak[0][j], &G_SwitchTestResult[j], sizeof(G_SwitchTestResult[j]));
		}
		m_TakeoutDlg.m_iTakeBoardType = 0;

		m_TakeoutDlg.m_WindowText	= _T("在线拔板"); 
		m_TakeoutDlg.m_ParaName		=  m_ParaName;
		m_TakeoutDlg.m_SwitchNormalOpen = m_SwitchNormalOpen;
		m_TakeoutDlg.m_BeginTestTime	= m_BeginTestTime;
		m_TakeoutDlg.m_EndTestTime		= m_EndTestTime;
		m_TakeoutDlg.m_BoxNr			= m_BoxNr;
		m_TakeoutDlg.m_iSelect			= 0;
		m_TakeoutDlg.ClearAllSelected(); 
		m_TakeoutDlg.m_iDisplayTempType = this->m_SwitchConfigPara.ParaType;
		m_TakeoutDlg.m_SwitchNormalOpen = m_SwitchNormalOpen; 
		m_TakeoutDlg.m_FlashGap			= 1;
		m_TakeoutDlg.m_HeatBoxType		= (HEAT_BOX_TYPE)m_HeatBoxType;


		m_IsTaking	=	true;
		m_TakeoutDlg.DoModal();
		m_IsTaking	=	false;
	}
	
	

}

SYSTEMTIME CloseTime,FlashTime;

unsigned char OIL_BIT[64] =
{ 
	 0, 2, 4, 6, 8,10,12,14,
	 1, 3, 5, 7, 9,11,13,15,
	16,18,20,22,24,26,28,30,
	17,19,21,23,25,27,29,31,
	32,34,36,38,40,42,44,46,
	33,35,37,39,41,43,45,47,
	48,50,52,54,56,58,60,62,
	49,51,53,55,57,59,61,63
};
unsigned char OIL_BIT_2[64] =
{
	10, 9,12,11,14,13,16,15, 2, 1,
	 4, 3, 6, 5, 8, 7,26,25,28,27,
	30,29,32,31,18,17,20,19,22,21,
	24,23,42,41,44,43,46,45,48,47,
	34,33,36,35,38,37,40,39,58,57,
	60,59,62,61,64,63,50,49,52,51
};

//用于处理发送测试命令后，判断仪表是否已经执行
void CSwitchDlg::DealCmdAnswer(char *para, unsigned char *flash,bool IsCmdAscii,char source_ID)
{

//	AfxMessageBox(_T("asa"));
	int i,size;
	CString s;
	CFile f;
	CString file_name;
	char crc[2];
	bool pcb_flash_status = false; 

	
	MODBUS_RS485_CMD_128					*modbus_cmd_128			= (MODBUS_RS485_CMD_128*) para;

	MODBUS_RS485_CMD_500					*modbus_cmd_192			= (MODBUS_RS485_CMD_500*) para;
	MODBUS_RS485_ANSWER_500					*answer_500	= (MODBUS_RS485_ANSWER_500*) para;


	unsigned char cmd;
	
	cmd  = modbus_cmd_128->cmd;
	


	switch(cmd)
	{
		case CMD_OMRON_READ:
			m_MeterAnswerTime	= m_MeterAnswerTime.GetCurrentTime();
			m_LedAnswerTime		= m_LedAnswerTime.GetCurrentTime();
			break;	

	
		case CMD_READ_STATUS:

			if(IsCmdAscii == false)
			{
				m_LedAnswerTime = m_LedAnswerTime.GetCurrentTime();
			}
		
			if(IsCmdAscii == false)
			{
				int size;
				
				size = sizeof(MODBUS_RS485_ANSWER_500);
				

				CComm::CRC16_AFF2((unsigned char *)para, size - 2,(unsigned char*)crc); 	
				
				if((crc[0] == para[size - 2])
					&& (crc[1] == para[size - 1]))
				{

				}
				else
				{
					break;
				}

				if(answer_500->para_len != 13 *2)
				{
					break;
				}
			}
			//for(int j = 0; j < 5 ; j++)
			//int j = 0;
			{
				for(i = 0; i < 100 ; i++)
				{
						if((answer_500->para[/*13*j +*/ (i)/8] & ( 0x01 << (i % 8))) != 0)
						{
							G_SwitchTestResult[i+100*source_ID].IsClosing = false;   
						}
						else
						{
							G_SwitchTestResult[i+100*source_ID].IsClosing = true;   
						}

						if(m_WorkStatus.AutoTestFlag)
						{
							
							if(m_SwitchNormalOpen == 0)
							{//常闭
								DealStatusNormalClose(&G_SwitchTestResult[i+100*source_ID],pcb_flash_status);
							}
							else
							{
								DealStatusNormalOpen(&G_SwitchTestResult[i+100*source_ID],pcb_flash_status);
							}
						}

				}
			}
			/*
			for(i = 400; i < 500 ; i++)
			{
					if((answer_500->para[13*4 + (i-400)/8] & ( 0x01 << (i % 8))) != 0)
					{
						G_SwitchTestResult[m_BoxNr][i].IsClosing = false;   
					}
					else
					{
						G_SwitchTestResult[m_BoxNr][i].IsClosing = true;   
					}

					if(m_WorkStatus.AutoTestFlag)
					{
						
						if(m_SwitchNormalOpen == 0)
						{//常闭
							DealStatusNormalClose(&G_SwitchTestResult[m_BoxNr][i],pcb_flash_status);
						}
						else
						{
							DealStatusNormalOpen(&G_SwitchTestResult[m_BoxNr][i],pcb_flash_status);
						}
					}

			}*/
		
					
					
				
				if(m_TakeoutDlg.GetSafeHwnd() != NULL)
				{
					if( m_TakeoutDlg.IsWindowVisible())
					{
						for(int i = 0; i < G_iMaxLedNr; i++)
						{
							memcpy(&G_TakeDlgSwitchTestResult[0][i], &G_SwitchTestResult[i], sizeof(G_TakeDlgSwitchTestResult[0][i]));
						}
						m_TakeoutDlg.ShowTestResult(true); 
					}
				}
				DrawLED();
				
		
		break;


	}

}


void CSwitchDlg::OnRadioClose() 
{
	// TODO: Add your control notification handler code here
	
	m_SwitchNormalOpen = 0;
	CString s;
	s.Format(_T("SwitchNormalOpen_%d"),m_BoxNr); 
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,m_SwitchNormalOpen); 
	
	this->UpdateData(false); 

}

void CSwitchDlg::OnRadioOpen() 
{
	// TODO: Add your control notification handler code here
	//RS485_COMMAND cmd;
	char para[4] = {0,0,0,0};
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	m_SwitchNormalOpen = 1;
	CString s;
	s.Format(_T("SwitchNormalOpen_%d"),m_BoxNr); 
	AfxGetApp()->WriteProfileInt(_T("SETTING"),s,m_SwitchNormalOpen); 
	this->UpdateData(false); 

}



void CSwitchDlg::EnterAutoTest()
{
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();

	CWnd *button;

	m_WorkStatus.AutoTestFlag = true;
	button = GetDlgItem(IDC_CHECK_AUTO_PROCESS);
	button->SetWindowText(_T("停止生产")); 
	this->m_TestMsg = _T("自动测试开始");

	button = GetDlgItem(IDC_RADIO_CLOSE);
	button->EnableWindow(false);
	button = GetDlgItem(IDC_RADIO_OPEN);
	button->EnableWindow(false);

	

		
	
	maindlg->SetCmdFan(true);
	maindlg->SetCmdAirControl(false); 
	maindlg->SetCmdHeat(true); 

//	SendDirectHeatCmd(GetControlGoalTemp(SUB_MODE_DIRECT_HEAT));
//	m_WorkStatus.SubMode   = 	SUB_MODE_DIRECT_HEAT;
	

		m_OpenDoorIng	= false;

		m_OpenDoor_Temp	= GetControlGoalTemp(SUB_MODE_NORMAL_PID) + m_OpenDoorResetHeatDegree; 

		m_WorkStatus.SubMode = SUB_MODE_STEP_PID;

		m_StepPidBeginTime = m_StepPidBeginTime.GetCurrentTime(); 
		
		
		this->m_LastStepPidTemp = this->m_CurTemp[0]; 
		
		if(m_iTestMode == TEST_MODE_OPEN_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH || m_iTestMode == TEST_MODE_CLOSE_FLASH )
		{
				maindlg->SetCmdHeat(true);

				maindlg->SetCmdAirControl(true);

				maindlg->SetCmdAirValve(true);

				SendStepPidHeatCmd();

				SendDirectHeatCmd(m_LastStepPidTemp);
				//SendDirectHeatCmd(m_OpenDoor_Temp);
		}
		else
		{
			SendDirectHeatCmd(m_LastStepPidTemp);
			SendStepPidHeatCmd();
		}



	CButton *b = (CButton*)GetDlgItem(IDC_BUTTON_TAKEOUT); 

	if(b != NULL)
	{
		
		HBITMAP bitmap = b->GetBitmap();

		if(bitmap == NULL)
		{
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TESTING)));
		}
		else
		{
			DeleteObject(bitmap);
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TESTING)));
		}
	}
	
}

void CSwitchDlg::EnterAutoTest_COOL_FIRST()
{
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();

	CWnd *button;

	m_WorkStatus.AutoTestFlag = true;
	button = GetDlgItem(IDC_CHECK_AUTO_PROCESS);
	button->SetWindowText(_T("停止生产")); 
	this->m_TestMsg = _T("自动测试开始");

	button = GetDlgItem(IDC_RADIO_CLOSE);
	button->EnableWindow(false);
	button = GetDlgItem(IDC_RADIO_OPEN);
	button->EnableWindow(false);

	

		
	m_WorkStatus.SubMode   = 	SUB_COOL_MODE_STEP_COOL; //SUB_COOL_MODE_DIRECT_COOL;
	
	maindlg->SetCmdFan(true);
	maindlg->SetCmdAirControl(true); 
	maindlg->SetCmdHeat(true); 

	//SendDirectHeatCmd(GetControlGoalTemp(SUB_COOL_MODE_DIRECT_COOL));
	
	//降温全部匀速
	m_WorkStatus.SubMode = SUB_COOL_MODE_STEP_COOL;
	m_WorkStatus.GoalTemp = GetControlGoalTemp(SUB_COOL_MODE_STEP_COOL) * (short)10; 
	
	if((m_iTestMode == TEST_MODE_NORMAL) && (m_iStepDownPidSpeed > 0))
	{
		m_StepCoolPidBeginTime = CTime::GetCurrentTime();
		this->m_LastStepPidCoolTemp = this->m_CurTemp[0]; 
		SendStepPIDCoolCmd();
		SendDirectHeatCmd(m_LastStepPidCoolTemp);
	}
	else
	{
		SendDirectHeatCmd(GetControlGoalTemp(SUB_COOL_MODE_STEP_COOL));
	}
	


	CButton *b = (CButton*)GetDlgItem(IDC_BUTTON_TAKEOUT); 

	if(b != NULL)
	{
		
		HBITMAP bitmap = b->GetBitmap();

		if(bitmap == NULL)
		{
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TESTING)));
		}
		else
		{
			DeleteObject(bitmap);
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TESTING)));
		}
	}
	
}

void CSwitchDlg::ExitAutoTest()
{
	CWnd *button;
	m_WorkStatus.AutoTestFlag = false;
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	char para[4]={0,0,0,0};
	
	button = GetDlgItem(IDC_CHECK_AUTO_PROCESS);
	button->SetWindowText(_T("开始生产")); 
	this->m_TestMsg = _T("自动测试停止");
	
	button = GetDlgItem(IDC_RADIO_CLOSE);
	button->EnableWindow(true);
	button = GetDlgItem(IDC_RADIO_OPEN);
	button->EnableWindow(true);

	m_WorkStatus.SubMode = 0;
	

	
	

	
	this->UpdateData(false);



	maindlg->SetCmdFan(false);
	maindlg->SetCmdHeat(false);
	maindlg->SetCmdAirControl(false);
	maindlg->SetCmdAirValve(false);


	SendDirectHeatCmd(0);
	

	CTimeSpan span = m_EndTestTime - m_BeginTestTime;
	//if(span.GetTotalSeconds() > 90)
	{//测试总时间必须大于90秒
		//cur_t = cur_t.GetCurrentTime();
		//span = cur_t - m_EndTestTime;
		//if((bSaveTestRecord==false) && (span.GetTotalSeconds() > 3))
		//{//测试结束30秒以后开始保存数据
		//AfxMessageBox(_T("开始保存数据"));
			this->SaveTestRecord(); 
			
		//	SaveChanliangTongji();
		//	bSaveTestRecord = true;
		//}
	}

	if(m_iContinueTestCount <= 0)
	{
		CString s;
		s.Format(_T("CONTINUE_TEST_COUNT_%d"),m_BoxNr);
		m_iContinueTestCount = AfxGetApp()->GetProfileIntW(_T("SETTING"),s,0);
	}
	else
	{
		m_iContinueTestCount --;
		OnStart(); 
	}

	CButton *b = (CButton*)GetDlgItem(IDC_BUTTON_TAKEOUT); 
	
	if(b != NULL)
	{
		
		HBITMAP bitmap = b->GetBitmap();

		if(bitmap == NULL)
		{
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TAKE_BOARD)));
		}
		else
		{
			DeleteObject(bitmap);
			b->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TAKE_BOARD)));
		}
	}

	if(AfxMessageBox(_T("是否打印测试报告?"),MB_YESNO) == IDYES)
	{
		OnBnClickedButtonPrint();
	}
}

void CSwitchDlg::OnCheckHighVolt() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CSwitchDlg::SendCmdSP(float temp)
{
}




LRESULT CSwitchDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

bool CSwitchDlg::IsAllOpened()
{
	int i;

	for(i = 0; i < G_iMaxLedNr; i++)
	{
		if((G_SwitchTestResult[i].IsUsed == true) &&  (G_SwitchTestResult[i].IsOpenned == false)) 
		{
			return false;
		}

	}
	return true;
}

bool CSwitchDlg::IsAllClosed()
{
	int i;

	for(i = 0; i < G_iMaxLedNr; i++)
	{
		if(m_iTestMode == TEST_MODE_CLOSE_FLASH || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH)
		{
			if((G_SwitchTestResult[i].IsUsed == true) 
			&& (G_SwitchTestResult[i].IsOpenned == true)
			&& (G_SwitchTestResult[i].IsClosed  == false)) 
			{
				return false;
			}

			if((G_SwitchTestResult[i].IsUsed == true) 
			&& (G_SwitchTestResult[i].IsOpenned == true)
			&& (G_SwitchTestResult[i].IsClosed  == true)
			&& (G_SwitchTestResult[i].IsCloseFlashTested  == false)) 
			{
				return false;
			}
		}
		else
		{
			if((G_SwitchTestResult[i].IsUsed == true) 
			&& (G_SwitchTestResult[i].IsOpenned == true)
			&& (G_SwitchTestResult[i].IsClosed  == false)
			) 
			{
				return false;
			}
		}

		


	}
	return true;
}


/*
void CSwitchDlg::SendControlCmd(COMMAND_ID cmd_id, float temp, int time)
{
		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		
		SHORT_BUF short_v;
		short_v.v = (short)(temp * 10);

		char para[32];
		memset(para,0,sizeof(para));
		
		para[1] = short_v.buf[1];
		para[0] = short_v.buf[0];

		short_v.v = (short)(time);

		para[3] = short_v.buf[1];
		para[2] = short_v.buf[0];


		maindlg->SetCmdString( m_BoxNr + '1',(COMMAND_ID)(cmd_id), para);

}*/


void	CSwitchDlg::SaveTestRecord_Hex(void)
{
	CFile RecFile;
	CString fName;
	RECORD	record;
	CTime cur;
	CTimeSpan span;
	cur = cur.GetCurrentTime();

	span = m_BeginTestTime - STD_TIME; 
	record.BeginStart_From201001010000Minutes = span.GetTotalMinutes();

	span = m_EndTestTime - STD_TIME; 
	record.EndStart_From201001010000Minutes = span.GetTotalMinutes();
	record.SwitchType = m_SwitchNormalOpen;
	record.ConfigPara = m_SwitchConfigPara;


	for(int i = 0; i < G_iMaxLedNr; i++)
	{
		record.TestResult[i] = G_SwitchTestResult[i];  
	}
	memset(record.ParaName,0,sizeof(record.ParaName));
	for(int i = 0; i < sizeof(record.ParaName)/sizeof(TCHAR),i < m_ParaName.GetLength() ; i++)
	{
		record.ParaName[i] = m_ParaName.GetAt(i); 
	}
	record.size = sizeof(record);
	
	//保存生产记录

	CString s;
	s.Format(_T("TEST MEMO %d"),m_BoxNr);
	CString memo = AfxGetApp()->GetProfileStringW(_T("SETTING"),s,_T(""));
	TCHAR *b = memo.GetBuffer(sizeof(record.Memo));
	memcpy(record.Memo,b,sizeof( record.Memo));
	memo.ReleaseBuffer(); 




	CFile f;
	CString file_name;

	CString sDir;
	
	sDir.Format(_T("log\\box%d"),m_BoxNr+1);

	sDir = theAppDirectory+ sDir;

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	file_name.Format(_T("\\%04d%02d%02d%02d%02d%02d-%04d%02d%02d%02d%02d%02d.record"),
		m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),m_BeginTestTime.GetDay(),m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(),
		m_EndTestTime.GetYear(),m_EndTestTime.GetMonth(),m_EndTestTime.GetDay(),m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());
	
	if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		f.Write(&record,sizeof(record)); 
		f.Close(); 
	}

	
	///////////////////////////////
	

	file_name.Format(_T("record%d.bin"),m_BoxNr);
	if(f.Open(theAppDirectory+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		f.Write(&G_SwitchTestResult[m_BoxNr],sizeof(G_SwitchTestResult[m_BoxNr])); 
		f.Close(); 
	}

	return;


//	CString sDir;
	sDir = theAppDirectory + _T("log");
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}
	fName.Format(_T("log\\RecFile%04d_%d.bin"),cur.GetYear(),m_BoxNr);    
	

	if(RecFile.Open(theAppDirectory+fName,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite))
	{

		if((RecFile.GetLength() % sizeof(RECORD)) != 0) 
		{//新记录格式
			RecFile.Close();
			RecFile.Remove( theAppDirectory+fName);
			if(RecFile.Open(theAppDirectory+fName,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite))
			{
				RecFile.SeekToEnd();
			
				RecFile.Write(&record,sizeof(record)); 
				RecFile.Flush();
				RecFile.Close();
			}
		}
		else
		{
			RecFile.SeekToEnd();
			
			RecFile.Write(&record,sizeof(record)); 
			RecFile.Flush();
			RecFile.Close();
		}
	}		

}

extern int AdjustID(int k);
extern TCHAR level_str[18][10];
extern TCHAR level_str_4_csv[18][10];

void CSwitchDlg::SaveExcel(void)
{



	int j;
	TCHAR str[1000];
	TCHAR str1[1000];
	CString  file_name;
	CTime cur = cur.GetCurrentTime();


	
	CString s ;
	s.Format(_T("生产记录%04d%02d%02d%02d%02d%02d"),
		m_BeginTestTime.GetYear(), 
		m_BeginTestTime.GetMonth(), 
		m_BeginTestTime.GetDay(), 
		m_BeginTestTime.GetHour(), 
		m_BeginTestTime.GetMinute(), 
		m_BeginTestTime.GetSecond()); 
	
	CString sDir;
	
	sDir = _T("d:\\测试记录");
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	sDir.Format(_T("d:\\测试记录\\烘箱%d"),m_BoxNr + 1);

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}
	
	sDir.Format(_T("d:\\测试记录\\烘箱%d\\%04d年%02d月"),m_BoxNr + 1,cur.GetYear(), cur.GetMonth());

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	CFile RecFile;

	{
		file_name.Format(_T("%02d日 %02d时 %02d分 %02d秒.csv"), cur.GetDay(), cur.GetHour(), cur.GetMinute(), cur.GetSecond());     
	
		file_name = sDir + _T("\\") + file_name;
		if(RecFile.Open(file_name,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{
			RecFile.Write(&UNICODE_FILE_HEAR,2);

			_stprintf_s(str,_T("测试参数%c\r\n"),CSV_COMMMA);
			RecFile.Write(str,wcslen(str) * sizeof(TCHAR));

			
			_stprintf_s(str,_T("主规格\r\n"));
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));

			_stprintf_s(str,_T("动作温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.MainLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.MainLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.MainLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			_stprintf_s(str,_T("复位温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.CloseLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.CloseLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.CloseLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));


			_stprintf_s(str,_T("高规格\r\n"));
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));

			_stprintf_s(str,_T("动作温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.HighLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.HighLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.HighLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			_stprintf_s(str,_T("复位温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.CloseHighLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.CloseHighLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.CloseHighLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));


			_stprintf_s(str,_T("低规格\r\n"));
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));

			_stprintf_s(str,_T("动作温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.LowLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.LowLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.LowLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			_stprintf_s(str,_T("复位温度%c%5.1f%c负偏差%c%5.1f%c正偏差%c%5.1f\r\n"),
				CSV_COMMMA,m_SwitchConfigPara.CloseLowLevel,CSV_COMMMA,
				CSV_COMMMA,-m_SwitchConfigPara.CloseLowLevelSpanMinus,CSV_COMMMA,
				CSV_COMMMA,m_SwitchConfigPara.CloseLowLevelSpanPlus);
			RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));

			//sprintf(str,"序号,动作温度,复位温度,规格,动作时间,复位时间\r\n");


			_stprintf_s(str,_T("序号%c动作温度%c复位温度%c规格\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
			RecFile.Write(str,wcslen(str)*  sizeof(TCHAR));
				
			for(j = 0; j < G_iMaxLedNr;j++)
			{
				if(G_SwitchTestResult[j].IsUsed)
				{
					if(G_iMaxLedNr == 500)
					{
						if((j % 12) < 9)
						{
							_stprintf_s(str,_T("%d%d%c"),j/12,(j%12)+1 ,CSV_COMMMA);				
						}
						else if((j % 12) == 9)
						{
							_stprintf_s(str,_T("%d0%c"),j/12 + 1,CSV_COMMMA);				
						}
						else if((j % 12) == 10)
						{
							_stprintf_s(str,_T("%dA%c"),j/12 + 1,CSV_COMMMA);				
						}
						else if((j % 12) == 11)
						{
							_stprintf_s(str,_T("%dB%c"),j/12 +1,CSV_COMMMA);				
						}
					}
					else
					{
						_stprintf_s(str,_T("%d%c"),j + 1,CSV_COMMMA);				
					}
					//CTime t  = GetRecordTime(G_SwitchTestResult[m_BoxNr][j].OpenTime); 
					//CTime t1 = GetRecordTime(G_SwitchTestResult[m_BoxNr][j].CloseTime); 
					//sprintf(str1,"%5.1f,%5.1f,%s,%04d/%02d/%02d %02d:%02d:%02d,%04d/%02d/%02d %02d:%02d:%02d\r\n",G_SwitchTestResult[m_BoxNr][j].OpenTemp,G_SwitchTestResult[m_BoxNr][j].CloseTemp,level_str_4_csv[CheckTestLevel(m_SwitchConfigPara,G_SwitchTestResult[m_BoxNr][j],HEAT_BOX_TYPE_NORMAL)],
					//	t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
					//	t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());   
					_stprintf_s(str1,_T("%5.1f%c%5.1f%c%s \r\n"),G_SwitchTestResult[j].OpenTemp,CSV_COMMMA,G_SwitchTestResult[j].CloseTemp,CSV_COMMMA,level_str_4_csv[CheckTestLevel(m_SwitchConfigPara,G_SwitchTestResult[j],HEAT_BOX_TYPE_NORMAL)]);
						
					wcscat_s(str,str1);
					RecFile.Write(str,wcslen(str) *  sizeof(TCHAR));
				}
				else
				{
					continue;

				}
			}

			RecFile.Close();
		}
	}
	
	
}
void CSwitchDlg::SaveTestRecord(void)
{
	//SaveCurve();
	SaveTestRecord_Hex();
	SaveExcel();
//	return;
/*
	CFile RecFile;
	CTime cur;
	cur = cur.GetCurrentTime();
	char str[100000],s[100];
	CString fName;
	CFileFind ff;

	CWnd *button;
	button = GetDlgItem(IDC_BUTTON_TAKEOUT);
	button->EnableWindow(true);
	
	//SendOpenDoorCmd(false); WriteLogFile("15",m_BoxNr+1,true);

	//保存生产记录
	CFile f;
	CString file_name;
	file_name.Format(_T("record%d.bin"),m_BoxNr);
	if(f.Open(theAppDirectory+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		f.SeekToBegin();
		f.Write(&m_SwitchTestResult,sizeof(m_SwitchTestResult)); 
		f.Close(); 
	}

	fName.Format(_T("log\\RecFile%04d_%d.bin"),cur.GetYear(),m_BoxNr);    
	
	str[0] = 0;
	sprintf(s,"%04d-%02d-%02d %02d:%02d:%02d,", m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),
		m_BeginTestTime.GetDay(),m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond());    
	strcat(str,s);
	sprintf(s,"%04d-%02d-%02d %02d:%02d:%02d,", m_EndTestTime.GetYear(),m_EndTestTime.GetMonth(),
		m_EndTestTime.GetDay(),m_EndTestTime.GetHour(),m_EndTestTime.GetMinute(),m_EndTestTime.GetSecond());    
	strcat(str,s); 
	//产品规格
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)(this->GetParent()->GetParent());
//	maindlg->m_ParaDlg[m_BoxNr].m_MainLevelList.GetWindowText(s,sizeof(s));   
	strcat(str,s);

	strcat(str,",");
	//产品类型
	if(this->m_SwitchNormalOpen == 0) 
	{
		strcat(str,"常闭,");
	}
	else
	{
		strcat(str,"常开,");
	}

	sprintf(s,"%5.1f,",m_SwitchConfigPara.MainLevel);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.MainLevelSpanMinus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.MainLevelSpanPlus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.HighLevel);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.HighLevelSpanMinus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.HighLevelSpanPlus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.LowLevel);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.LowLevelSpanMinus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.LowLevelSpanPlus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseLevel);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseLevelSpanMinus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseLevelSpanPlus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseHighLevel);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseHighLevelSpanMinus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseHighLevelSpanPlus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseLowLevel);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseLowLevelSpanMinus);
	strcat(str,s);
	sprintf(s,"%5.1f,",m_SwitchConfigPara.CloseLowLevelSpanPlus);
	strcat(str,s);

	sprintf(s,"%5.1f,\r\n",m_SwitchConfigPara.OpenDoorResetLevel);
	strcat(str,s);

	for(int k = 0; k < MAX_LED_NR; k++)
	{
		int i = AdjustID(k);
//		bool IsUsed;		//产品是否插入
		if(m_SwitchTestResult[i].IsUsed)
		{
			strcat(str,"Y,");
		}
		else
		{
			strcat(str,"N,");
		}
//		bool IsClosing;	    //现在是否闭合
		
//		bool IsOpenned;		//是否曾经打开过
		if(m_SwitchTestResult[i].IsOpenned)
		{
			strcat(str,"Y,");
		}
		else
		{
			strcat(str,"N,");
		}
//		bool IsClosed;		//是否曾经复位过
		if(m_SwitchTestResult[i].IsClosed)
		{
			strcat(str,"Y,");
		}
		else
		{
			strcat(str,"N,");
		}
//		bool IsFlash;	//是否动作时闪动
		if(m_SwitchTestResult[i].IsOpenFlash)
		{
			strcat(str,"Y,");
		}
		else
		{
			strcat(str,"N,");
		}
		if(m_SwitchTestResult[i].IsCloseFlash)
		{
			strcat(str,"Y,");
		}
		else
		{
			strcat(str,"N,");
		}
//		float OpenTemp;		//动作温度
		sprintf(s,"%5.1f,", m_SwitchTestResult[i].OpenTemp);
		strcat(str,s);
		
//		float CloseTemp;	//复位温度	
		sprintf(s,"%5.1f,", m_SwitchTestResult[i].CloseTemp);
		strcat(str,s);

		sprintf(s,"%s", level_str_4_csv[CheckTestLevel(m_SwitchConfigPara,m_SwitchTestResult[i],(HEAT_BOX_TYPE)m_HeatBoxType)]);
		/*switch(CheckTestLevel(m_SwitchConfigPara,m_SwitchTestResult[i]))
		{
		case 	NO_LEVEL:
			s = "高低复大小";
			break;
		
		case 	MAIN_LEVEL:
			s = "主规格";
			break;
		
		case 	HIGH_LEVEL:
			s = "高规格";
			break;
		case 	LOW_LEVEL:
			s = "低规格";
			break;
		case 	OVER_HIGH_LEVEL:
			s = "高温品";
			break;
		case 	OVER_LOW_LEVEL:
			s = "低温品";
			break;
		case 	CLOSE_HIGH_LEVEL:
			s = "复大品";
			break;
		case 	CLOSE_LOW_LEVEL:
			s = "复小品";
			break;
		case 	OPEN_FLASH_LEVEL:
			s = "动闪";
			break;
		case 	CLOSE_FLASH_LEVEL:
			s = "复闪";
			break;
		case 	OPEN_DOOR_RESET_LEVEL:
			s = "开门复位";
			break;
		case 	NOT_USED:
			s = "无产品";
			break;
		}
		
		strcat(str,s);
		strcat(str,",\r\n");
		
	}

//	if(NoTimeLimit || (((cur.GetMinute() % 10) == 0) && (cur.GetSecond() <= 1) ))  
	{
		if(!ff.FindFile(fName))
		{
			char str_title[10000];
			str_title[0] = 0;
			strcat(str_title,"开始时间,"); 
			strcat(str_title,"结束时间,");
			strcat(str_title,"规格,");
			strcat(str_title,"类型,");
			strcat(str_title,"主规格,"); 
			strcat(str_title,"主规负偏差,"); 
			strcat(str_title,"主规正偏差,"); 
			strcat(str_title,"高规格,"); 
			strcat(str_title,"高规负偏差,"); 
			strcat(str_title,"高规正偏差,"); 
			strcat(str_title,"低规格,"); 
			strcat(str_title,"低规负偏差,"); 
			strcat(str_title,"低规正偏差,"); 
			strcat(str_title,"复位温度,"); 
			strcat(str_title,"复位负偏差,"); 
			strcat(str_title,"复位正偏差,"); 
			strcat(str_title,"高规格复位温度,"); 
			strcat(str_title,"高规格复位负偏差,"); 
			strcat(str_title,"高规格复位正偏差,"); 
			strcat(str_title,"低规格复位温度,"); 
			strcat(str_title,"低规格复位负偏差,"); 
			strcat(str_title, "低规格复位正偏差,"); 
			strcat(str_title,"开门复位\r\n"); 
//		bool IsUsed;		//产品是否插入
			strcat(str_title,"是否有产品,");
//		bool IsClosing;	    //现在是否闭合
//				str_title += "当前状态,";
//		bool IsOpenned;		//是否曾经打开过
			strcat(str_title,"动作,");
//		bool IsClosed;		//是否曾经复位过
			strcat(str_title,"复位,");
//		bool IsFlash;	//是否动作时闪动
			strcat(str_title,"动闪,");

			strcat(str_title,"复闪,");
//		float OpenTemp;		//动作温度
			strcat(str_title,"动作温度,");
//		float CloseTemp;	//复位温度	
			strcat(str_title,"复位温度");
			strcat(str_title,"\r\n");

			if(RecFile.Open(theAppDirectory+fName,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite))
			{
				RecFile.SeekToEnd();
				
				RecFile.Write(str_title,strlen(str_title)); 
				RecFile.Flush();
				RecFile.Close();
			}			
		}
	
		if(RecFile.m_hFile == CFile::hFileNull)
		{
			if(RecFile.Open(theAppDirectory+fName,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite))
			{
				RecFile.SeekToEnd();
				
				RecFile.Write(str,strlen(str)); 
				RecFile.Flush();
				RecFile.Close();
			}
		}
		else
		{
		
		}
	}
	*/
}




void CSwitchDlg::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}







void CSwitchDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//初始化数据

	CDialog::OnLButtonDblClk(nFlags, point);
	
	return;

	m_TakeoutDlg.m_SwitchConfigPara = this->m_SwitchConfigPara;
	for(int i = 0; i < G_iMaxLedNr; i++)
	{
		memcpy(&G_TakeDlgSwitchTestResult[0][i], &G_SwitchTestResult[i], sizeof(G_SwitchTestResult[i]));
		memcpy(&G_TakeDlgSwitchTestResult_Bak[0][i], &G_SwitchTestResult[i], sizeof(G_SwitchTestResult[i]));
	}

	m_TakeoutDlg.m_iTakeBoardType = 0;
	
	m_TakeoutDlg.m_FlashGap		= 1;

	m_TakeoutDlg.m_WindowText		= _T("在线查看"); 
	m_TakeoutDlg.m_ParaName			=  m_ParaName;
	m_TakeoutDlg.m_SwitchNormalOpen = m_SwitchNormalOpen;
	m_TakeoutDlg.m_BeginTestTime	= m_BeginTestTime;
	m_TakeoutDlg.m_EndTestTime		= m_EndTestTime;
	
	m_TakeoutDlg.m_iDisplayTempType = this->m_SwitchConfigPara.ParaType;
	m_TakeoutDlg.m_BoxNr = m_BoxNr;
	m_TakeoutDlg.m_iSelect = 0;//2;
	m_TakeoutDlg.m_HeatBoxType	= (HEAT_BOX_TYPE)m_HeatBoxType;
	m_TakeoutDlg.SetAllSelected();
	
	m_TakeoutDlg.DoModal();

}




double CSwitchDlg::GetLowestCloseTemp()
{
	int i;
	double temp = 10000;

	for(i = 0; i < G_iMaxLedNr; i ++)
	{
		if(G_SwitchTestResult[i].IsClosed && G_SwitchTestResult[i].IsUsed)
		{
			if((G_SwitchTestResult[i].CloseTemp < temp) && (G_SwitchTestResult[i].CloseTemp>0))
			{
				temp = G_SwitchTestResult[i].CloseTemp;
			}
		}
	}
	return temp;
}

int CSwitchDlg::GetHighestCloseTemp()
{
	int i;
	int temp = 0;

	for(i = 0; i < G_iMaxLedNr; i ++)
	{
		if(G_SwitchTestResult[i].IsClosed && G_SwitchTestResult[i].IsUsed)
		{
			if(G_SwitchTestResult[i].CloseTemp > temp)
			{
				temp = G_SwitchTestResult[i].CloseTemp;
			}
		}
	}

	if(temp > m_SwitchConfigPara.CloseHighLevel + m_SwitchConfigPara.CloseHighLevelSpanPlus)
	{
		temp = m_SwitchConfigPara.CloseHighLevel + m_SwitchConfigPara.CloseHighLevelSpanPlus;
	}
	temp += 5;

	return temp;
}

void CSwitchDlg::OnSelchangeComboFlashGap() 
{
	// TODO: Add your control notification handler code here


}

void CSwitchDlg::SendDirectHeatCmd(float temp)
{
		CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		
		char para[100];
		memset(para,0,sizeof(para));

		*((short * )&para[0]) = CDebugDlg::PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = CDebugDlg::PcShort2OmronShort((short)(temp *10));
		
		maindlg->SetCmdString_OMRON( m_BoxNr + 1, CMD_OMRON_WRITE, para);

}

void CSwitchDlg::SendStepPidHeatCmd(void)
{
	if(m_iStepPidSpeed <= 0)
	{
		return;
	}
	static DWORD tick = ::GetTickCount();
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();

	if((::GetTickCount() - tick) > 6000)//每6秒发一次命令 
	{
		char para[100];

		tick = ::GetTickCount();

		memset(para,0,sizeof(para));

		m_LastStepPidTemp += m_iStepPidSpeed / 100.0f; 

		*((short * )&para[0]) = CDebugDlg::PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = CDebugDlg::PcShort2OmronShort((short)(m_LastStepPidTemp *10));
		
		maindlg->SetCmdString_OMRON( m_BoxNr + 1, CMD_OMRON_WRITE, para);
	}

}



void CSwitchDlg::SendStepPIDCmd(float TopTemp)
{
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
		
		char para[100];
		memset(para,0,sizeof(para));

		*((short * )&para[0]) = CDebugDlg::PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = CDebugDlg::PcShort2OmronShort((short)(TopTemp *10));
		
		maindlg->SetCmdString_OMRON( m_BoxNr + 1, CMD_OMRON_WRITE, para);

}

void CSwitchDlg::SendStepPIDCoolCmd(void)
{
	static DWORD tick = ::GetTickCount();
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();

	if((::GetTickCount() - tick) > 6000)//每6秒发一次命令 
	{
		char para[100];

		tick = ::GetTickCount();

		memset(para,0,sizeof(para));

		m_LastStepPidCoolTemp -= m_iStepDownPidSpeed / 100.0f; 

		*((short * )&para[0]) = CDebugDlg::PcShort2OmronShort(OMRON_SP);

		*((short * )&para[2]) = CDebugDlg::PcShort2OmronShort((short)(m_LastStepPidCoolTemp *10));
		
		maindlg->SetCmdString_OMRON( m_BoxNr + 1, CMD_OMRON_WRITE, para);

		//根据降温速度,自动调节空调电磁筏
		/*

		if( SUB_MODE_STEP_COOL == m_WorkStatus.SubMode)
		{
			if(m_LastStepPidCoolTemp > (this->m_PID_Temp + 1.0f))
			{
				maindlg->SetCmdAirValve(true); 
			}
			else if(m_LastStepPidCoolTemp < (this->m_PID_Temp - 2.0f))
			{
				maindlg->SetCmdAirValve(false); 
			}
		}

		*/

	}
}


void CSwitchDlg::OpenDoor_For_OpenFlashTest()
{
#define HEAT_DEGREE	m_OpenDoorResetHeatDegree
#define COOL_DEGREE	m_OpenDoorResetCoolDegree

	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());

	if(m_WorkStatus.SubMode != SUB_MODE_STEP_PID)
	{
		return;
	}

	if(m_OpenDoorIng == false)
	{
		CTime cur = CTime::GetCurrentTime(); 
		CTimeSpan span = cur - m_OpenDoor_StartTime;
		
		if(((m_OpenDoor_Temp <= m_PID_Temp) /*&& OpennedAndNotFlashTested()*/)
			|| (m_PID_Temp >=  m_ControlTempRange.open_temp_max)
			|| (span.GetTotalMinutes() >= 5)
			)
		{
			m_OpenDoor_StartTime = CTime::GetCurrentTime(); 
			this->m_TestMsg.Format(_T("反复开关门降温到%5.1f"),m_OpenDoor_Temp - COOL_DEGREE);
			this->UpdateData(false);
			maindlg->SetCmdAirValve(false);
			//maindlg->SetCmdHeat(false);
			this->SendDirectHeatCmd(m_OpenDoor_Temp - COOL_DEGREE - 1.0f); 
		
			m_OpenDoorIng = true;
		}
	}
	else
	{
		CTime cur = CTime::GetCurrentTime(); 
		CTimeSpan span = cur - m_OpenDoor_StartTime;
			

		if(((m_OpenDoor_Temp	- COOL_DEGREE ) >= m_PID_Temp) 
			|| (span.GetTotalMinutes() >= 5))
		{
			m_OpenDoor_StartTime = CTime::GetCurrentTime(); 
			
			m_OpenDoorIng = false;

			
	

			m_OpenDoor_Temp += HEAT_DEGREE;
			
			this->m_TestMsg.Format(_T("反复开关门升温到%5.1f"),m_OpenDoor_Temp);
			this->UpdateData(false);
			


			
				maindlg->SetCmdHeat(true);
				maindlg->SetCmdAirValve(true);
				SendDirectHeatCmd(m_OpenDoor_Temp + 1.0);
	
			

		}
	}

}

BOOL CSwitchDlg::OpennedAndNotFlashTested()
{
	int i;
	for(i = 0; i < G_iMaxLedNr; i++)
	{
		if(G_SwitchTestResult[i].IsOpenFlashTested == false
			&& G_SwitchTestResult[i].IsOpenned == true)
		{
			return true;
		}
	}
	return false;
}

void CSwitchDlg::ClearPidStepCount(bool bClear)
{
	
		// TODO: Add your control notification handler code here
	CHeatBoxDlg *maindlg = (CHeatBoxDlg *)(this->GetParent()->GetParent());
	

	if(G_iMaxLedNr == 500)
	{
		MODBUS_RS485_CMD_500 cmd;
		
		cmd.RS485_Addr = '1' + this->m_BoxNr;
		if(bClear)
			cmd.cmd = CMD_WRITE_STEP_PID_COUNT;//CMD_SET_HIGH_VOLT;
		else
			cmd.cmd = CMD_READ_STEP_PID_COUNT;//CMD_SET_HIGH_VOLT;

		//cmd.para_len = sizeof(cmd.para);

		memset(cmd.para, 0, sizeof(cmd.para));


		CComm::CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc); 	

		maindlg->SetCmdString( cmd.RS485_Addr,(COMMAND_ID)cmd.cmd, (char*)cmd.para );

	}
	else
	{
		MODBUS_RS485_CMD_128 cmd;
		
		cmd.RS485_Addr = '1' + this->m_BoxNr;
		if(bClear)
			cmd.cmd = CMD_WRITE_STEP_PID_COUNT;//CMD_SET_HIGH_VOLT;
		else
			cmd.cmd = CMD_READ_STEP_PID_COUNT;//CMD_SET_HIGH_VOLT;
		cmd.para_len = sizeof(cmd.para);

		memset(cmd.para, 0, sizeof(cmd.para));

		

		CComm::CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc); 	

		maindlg->SetCmdString( cmd.RS485_Addr,(COMMAND_ID)cmd.cmd, (char*)cmd.para);

	}

}

HBRUSH CSwitchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		hbr = G_bkBrush;
		break;

	case CTLCOLOR_BTN:	//    Button control
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		break;
	case CTLCOLOR_EDIT:	//    Edit control
		//hbr = G_bkBrush;
		 
		break;
	case CTLCOLOR_LISTBOX:	//    List-box control
		break;
	case CTLCOLOR_MSGBOX:	//    Message box
		hbr = G_bkBrush;
		break;
	case CTLCOLOR_SCROLLBAR:	//    Scroll-bar control
		break;
	case CTLCOLOR_STATIC:	//    Static control
		hbr = G_bkBrush;

		if(pWnd->GetDlgCtrlID() == IDC_STATIC_TEST_MSG)
		{
			pDC->SetBkColor(RGB(255, 255, 255)); 
		}
		else
		{
			pDC->SetBkColor(BK_COLOR); 
			
			pDC->SetTextColor(RGB(255, 255, 60));
		}
		break;
	}

	switch(pWnd->GetDlgCtrlID())
	{
	
	case IDC_EDIT_5:
		pDC->SetBkColor(RGB(255,255,255)); 
		pDC->SetTextColor(RGB(255,0,0));
		break;
	case IDC_CHECK_AUTO_PROCESS:
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		break;

	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSwitchDlg::OpenDoor_For_CloseFlashTest(void)
{
	int	i;

	if(m_iTestMode == TEST_MODE_CLOSE_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH)
	{
		if((m_WorkStatus.SubMode == SUB_MODE_DIRECT_COOL) || (m_WorkStatus.SubMode == SUB_MODE_STEP_COOL))
		{//cool 

			m_CloseDoorTopTemp = -999;
			
			//只有有产品复位了,才进行反复开关门测试
			for(i = 0; i < G_iMaxLedNr; i++)
			{
				if((G_SwitchTestResult[i].IsClosed) && (G_SwitchTestResult[i].IsCloseFlashTested == false))
				{
					if(m_CloseDoorTopTemp < (int)(G_SwitchTestResult[i].CloseTemp + 5.0f))
					{
						m_CloseDoorTopTemp =(int)(G_SwitchTestResult[i].CloseTemp + 5.0f);
					}
				}
			}
			
			CTimeSpan span = CTime::GetCurrentTime() - m_CloseDoorStartTime;

			if(m_bIsClosingDoor == false)
			{
				if(m_CloseDoorTopTemp > -100)
				{
					//降温的过程中有保护器复位
					SendDirectHeatCmd(m_CloseDoorTopTemp + 1);

					m_CloseDoorStartTemp = m_PID_Temp;
					
					m_bIsClosingDoor = true;
					m_TestMsg.Format(_T("关门升温 %5.1f度"), m_CloseDoorTopTemp);

					m_CloseDoorStartTime = m_CloseDoorStartTime.GetCurrentTime();
					UpdateData(false);
				
				}
				else
				{
					SendStepPIDCoolCmd();
					m_TestMsg.Format(_T("开门降温 %5.1f度"),m_LastStepPidCoolTemp);
					UpdateData(false);
				}
			}
			else if(m_bIsClosingDoor  && ((m_PID_Temp > (int)m_CloseDoorTopTemp) || ((span.GetTotalSeconds() > 120))))
			{
				
				
				m_bIsClosingDoor = false;
				for(i = 0; i < G_iMaxLedNr; i++)
				{
					if((G_SwitchTestResult[i].IsClosed) && (G_SwitchTestResult[i].IsCloseFlashTested == false))
					{
						G_SwitchTestResult[i].IsCloseFlashTested = true;
					}
				}
				m_TestMsg.Format(_T("开门降温"));
				if(m_CloseDoorTopTemp != 0)
				{
					m_LastStepPidCoolTemp = m_CloseDoorTopTemp - 5.0f;
				}
				UpdateData(false);
			}
		}
	}
}


HEAT_COOL_ORDER	CheckHeatOrCoolMode(SWITCH_CONFIG_PARA	SwitchConfigPara,CONTROL_TEMP_RANGE *temp_range)
{
	HEAT_COOL_ORDER order = HEAT_COOL_UNKNOW;

	if(SwitchConfigPara.MainLevel < -50.0f || SwitchConfigPara.MainLevel > 200.0f)
	{
		temp_range->open_temp_max = INVALID_TEMP; 
		temp_range->open_temp_min = INVALID_TEMP; 
		temp_range->close_temp_max = INVALID_TEMP; 
		temp_range->close_temp_min = INVALID_TEMP; 
		return order;
	}

	if((SwitchConfigPara.LowLevelSpanMinus != 0) ||  (SwitchConfigPara.LowLevelSpanPlus != 0))
	{//测低规格
		if((SwitchConfigPara.HighLevelSpanMinus != 0) ||  (SwitchConfigPara.HighLevelSpanPlus != 0))
		{//测低规格,测高规格
			temp_range->open_temp_min  = SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus;
			temp_range->open_temp_max  = SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus; 
			if((SwitchConfigPara.CloseLevelSpanMinus != 0) || (SwitchConfigPara.CloseLevelSpanPlus != 0))
			{//测复位温度
				temp_range->close_temp_min = SwitchConfigPara.CloseLowLevel - SwitchConfigPara.CloseLowLevelSpanMinus;
				temp_range->close_temp_max = SwitchConfigPara.CloseHighLevel+ SwitchConfigPara.CloseHighLevelSpanPlus;
			//	AfxMessageBox(_T("测低规格,测主规格,测高规格"));
			}
			else
			{//不测复位温度
				temp_range->close_temp_min = 0;
				temp_range->close_temp_max = 0;
			//	AfxMessageBox(_T("测低规格,测主规格,测高规格，不测复位温度"));
			}
			//AfxMessageBox(_T("1"));
		}
		else
		{//测低规格,测主规格
			temp_range->open_temp_min  = SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus;
			temp_range->open_temp_max  = SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus; 
			if((SwitchConfigPara.CloseLevelSpanMinus != 0) || (SwitchConfigPara.CloseLevelSpanPlus != 0))
			{//测复位温度
				temp_range->close_temp_min = SwitchConfigPara.CloseLowLevel - SwitchConfigPara.CloseLowLevelSpanMinus;
				temp_range->close_temp_max = SwitchConfigPara.CloseLevel+ SwitchConfigPara.CloseLevelSpanPlus;
			//	AfxMessageBox(_T("测低规格,测主规格"));
			}
			else
			{//不测复位温度
				temp_range->close_temp_min = 0;
				temp_range->close_temp_max = 0;
			//	AfxMessageBox(_T("测低规格,测主规格，不测复位温度"));
			}
			//AfxMessageBox(_T("2"));
		}
	}
	else if((SwitchConfigPara.HighLevelSpanMinus != 0) ||  (SwitchConfigPara.HighLevelSpanPlus != 0))
	{//测高规格
		//测高规格,测主规格
		temp_range->open_temp_min  = SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus;
		temp_range->open_temp_max  = SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus; 

		if((SwitchConfigPara.CloseLevelSpanMinus != 0) || (SwitchConfigPara.CloseLevelSpanPlus != 0))
		{//测复位温度
			temp_range->close_temp_min = SwitchConfigPara.CloseLevel		- SwitchConfigPara.CloseLevelSpanMinus;
			temp_range->close_temp_max = SwitchConfigPara.CloseHighLevel	+ SwitchConfigPara.CloseHighLevelSpanPlus;
		//	AfxMessageBox(_T("测主规格,测高规格"));
		}
		else
		{//不测复位温度
			temp_range->close_temp_min = 0;
			temp_range->close_temp_max = 0;
		//	AfxMessageBox(_T("测主规格,测高规格，不测复位温度"));
		}
		//AfxMessageBox(_T("3"));
	}
	else
	{//只测主规格、
		temp_range->open_temp_min  = SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus;
		temp_range->open_temp_max  = SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus; 
		if(SwitchConfigPara.CloseLevelSpanMinus != 0 || SwitchConfigPara.CloseLevelSpanPlus != 0)
		{//测复位温度
			temp_range->close_temp_min = SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus;
			temp_range->close_temp_max = SwitchConfigPara.CloseLevel + SwitchConfigPara.CloseLevelSpanPlus;
		//	AfxMessageBox(_T("测主规格"));
		}
		else
		{//不测复位温度
			temp_range->close_temp_min = 0;
			temp_range->close_temp_max = 0;
		//	AfxMessageBox(_T("测主规格，不测复位温度"));
		}
		//AfxMessageBox(_T("4"));
	}
	
	if(temp_range->close_temp_min != temp_range->close_temp_max )
	{//测复位
		if(temp_range->open_temp_max > temp_range->close_temp_max)
		{//先制热，再制冷
			order = HEAT_FIRST_COOL_SECOND;
		}
		else
		{//先制冷，再制热

			order = COOL_FIRST_HEAT_SECOND;
		}
	}
	else
	{//不测复位、
		order = HEAT_FIRST_COOL_SECOND;
		/*
		if(temp_range->open_temp_min > 25)
		{
			order = HEAT_FIRST_COOL_SECOND;
		}
		else if(temp_range->open_temp_max < 25)
		{
			order = COOL_FIRST_HEAT_SECOND;
		}
		else
		{
			AfxMessageBox(_T("系统无法确定 加热 还是 制冷方式\r\n请与设备供应商联系！"));
			order = HEAT_COOL_UNKNOW;
		}
		*/
	}

	//CString s;
	//s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f,%d"),temp_range->open_temp_min,temp_range->open_temp_max,temp_range->close_temp_min,temp_range->close_temp_max,order);
	//AfxMessageBox(s);

	
	return order;
}

double CSwitchDlg::GetControlGoalTemp(AUTO_TEST_STEP step)
{
	double goal_temp = INVALID_TEMP;
	//CONTROL_TEMP_RANGE	range;
	CheckHeatOrCoolMode(m_SwitchConfigPara , &m_ControlTempRange);

	if(m_HeatCoolOrder == HEAT_FIRST_COOL_SECOND)
	{
		switch(step)
		{
		case SUB_MODE_DIRECT_HEAT:
			goal_temp = m_ControlTempRange.open_temp_min - DIRECT_HEAT_DELTA;
			
			break;
		case SUB_MODE_NORMAL_PID:
			goal_temp = m_ControlTempRange.open_temp_min - PID_HEAT_DELTA;
			break;
		case SUB_MODE_STEP_PID:
			goal_temp = m_ControlTempRange.open_temp_max + G_iHighHighLevelDelta + 2.0f;
			break;
		case SUB_MODE_DIRECT_COOL:
			goal_temp = m_ControlTempRange.close_temp_min;
			break;
		case SUB_MODE_STEP_COOL:
			goal_temp = m_ControlTempRange.close_temp_min;
			break;
		case SUB_MODE_HEAT_AGAIN:
			goal_temp = GetLowestCloseTemp();
			break;
		}
	}
	else if(m_HeatCoolOrder == COOL_FIRST_HEAT_SECOND)
	{//制冷
		switch(step)
		{
		case SUB_MODE_READY:
			break;
		case SUB_COOL_MODE_DIRECT_COOL:
			goal_temp = m_ControlTempRange.open_temp_max + DIRECT_HEAT_DELTA;
			
			break;
		case SUB_COOL_MODE_PID_COOL:
			goal_temp = m_ControlTempRange.open_temp_max + PID_HEAT_DELTA;
			break;
		case SUB_COOL_MODE_STEP_COOL:
			goal_temp = m_ControlTempRange.open_temp_min - G_iHighHighLevelDelta - 2.0f;
			break;
		case SUB_COOL_MODE_DIRECT_HEAT:
			goal_temp = m_ControlTempRange.close_temp_min - STEP_COOL_DELTA;
			break;
		case SUB_COOL_MODE_PID_HEAT:
			goal_temp = m_ControlTempRange.close_temp_min - STEP_COOL_DELTA / 2 ;
			break;
		case SUB_COOL_MODE_STEP_HEAT:
			goal_temp = m_ControlTempRange.close_temp_max + 2.0f;
			break;
		}
		//AfxMessageBox(_T("制冷 没有处理此功能\r\n请选择测试参数"));
	}
	return goal_temp;
}


void CSwitchDlg::PrintTestResult(void) 
{
	// TODO: Add your control notification handler code here
	
	CDC dc;
	int i;
	
	CPrintDialog prnDlg(false);
	TCHAR PrinterName[100];
	if(prnDlg.GetDefaults())
	{
		if(prnDlg.m_pd.hDC )
		{
			DeleteDC(prnDlg.m_pd.hDC);
		}

		if(prnDlg.m_pd.hDevNames)
		{
			LPDEVNAMES pdn = (LPDEVNAMES)GlobalLock(prnDlg.m_pd.hDevNames);  
			memset(PrinterName,0,sizeof(PrinterName));
			_tcsncpy_s(PrinterName,(LPTSTR)pdn + pdn->wDeviceOffset,99);
			m_Printer = PrinterName;
			::GlobalUnlock(pdn);
			::GlobalFree(pdn); 
		}
	}

	
	if(!dc.CreateDC(_T(""),m_Printer,_T(""),NULL))
	{
		AfxMessageBox(_T("请设置打印机"));
		return;
	}	
	
	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	CString strTitle;
	this->GetWindowText(strTitle); 
	CString app_name;
	AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += "[";
	app_name += strTitle;
	app_name += "]";
	docInfo.lpszDocName =   app_name;


	i=dc.StartDoc(&docInfo);  
	i=	dc.StartPage();

	CString BoxNr;


	BoxNr.Format(_T("%d"), m_BoxNr + 3-2);

	//AfxMessageBox("打印1");
	
	CString switchtype;
	if(m_SwitchNormalOpen)
	{
		switchtype = _T("常开");
	}
	else
	{
		switchtype = _T("常闭");
	}
	
	SWITCH_TEST_RESULT_EX SwitchTestResult_Ex[500];

	for(i = 0; i < G_iMaxLedNr; i++)
	{
		SwitchTestResult_Ex[i].ID = i;
		SwitchTestResult_Ex[i].result = G_SwitchTestResult[i];
	}

	PrintReport(&dc,app_name, m_BoxNr,
				m_BeginTestTime,m_EndTestTime,
				m_ParaName, m_TestMemo,
				switchtype,
				m_SwitchConfigPara, SwitchTestResult_Ex,
				TRUE,0,127,(HEAT_BOX_TYPE)m_HeatBoxType);


	i =	dc.EndPage(); 
	
	i =	dc.EndDoc(); 
	
	dc.DeleteDC();

}




void CSwitchDlg::DealStatusNormalClose(SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status)
{
	if(m_WorkStatus.SubMode == SUB_MODE_DIRECT_HEAT
		|| m_WorkStatus.SubMode == SUB_MODE_NORMAL_PID
		|| m_WorkStatus.SubMode == SUB_MODE_STEP_PID
		|| m_WorkStatus.SubMode == SUB_COOL_MODE_DIRECT_COOL
		|| m_WorkStatus.SubMode == SUB_COOL_MODE_PID_COOL
		|| m_WorkStatus.SubMode == SUB_COOL_MODE_STEP_COOL)
	{//升温
			//if(m_SwitchType == 0)
			{//常闭
				//假如是闪动产品，就将最后一次动作时的温度计作动作温度
				if(m_iTestMode == TEST_MODE_OPEN_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH)
				{//否则就要对同一个产品反复开关门了
					if(SwitchTestResult->IsClosing == true
						&& SwitchTestResult->IsUsed == true
						&& SwitchTestResult->IsOpenned == true
						)
					{
						SwitchTestResult->IsOpenFlash = true;
						//SwitchTestResult->IsOpenned = false; 
						//SwitchTestResult->OpenTemp = 0.0f;
					}
				}
				else
				{
					if(SwitchTestResult->IsClosing == true
						&& SwitchTestResult->IsUsed == true
						&& SwitchTestResult->IsOpenned == true
						)
					{
						SwitchTestResult->IsOpenFlash = true;
						SwitchTestResult->IsOpenned = false; 
						SwitchTestResult->OpenTemp = 0.0f;
					}

					
					if(pcb_flash_status)
					{
						if(m_HeatBoxType == HEAT_BOX_TYPE_PTC)
						{
							SwitchTestResult->IsMoreThan1M = true;
						}
						else
						{
							SwitchTestResult->IsOpenFlash = true;
						}
					}

				}
				

				//判断是否是刚打开
				if(SwitchTestResult->IsOpenned == false
					&& SwitchTestResult->IsClosing == false
					&& SwitchTestResult->IsUsed == true
					)
				{
					SwitchTestResult->IsOpenned = true; 
					SwitchTestResult->OpenTemp =  m_PID_Temp;
					SwitchTestResult->OpenTime= GetSpanSeconds(CTime::GetCurrentTime());
					this->UpdateData(false);
				}
			}
			
	}//升温



	else if(m_WorkStatus.SubMode  == SUB_MODE_DIRECT_COOL
		  || m_WorkStatus.SubMode == SUB_MODE_STEP_COOL
		  || m_WorkStatus.SubMode == SUB_MODE_HEAT_AGAIN
		  || m_WorkStatus.SubMode == SUB_COOL_MODE_DIRECT_HEAT
		  || m_WorkStatus.SubMode == SUB_COOL_MODE_PID_COOL
		  || m_WorkStatus.SubMode == SUB_COOL_MODE_STEP_HEAT)
	{//降温
		//if(m_SwitchType == 0)
		{//常闭
			//判断是否是刚复位过
			{
				if(SwitchTestResult->IsClosing		== false
					&& SwitchTestResult->IsUsed		== true
					&& SwitchTestResult->IsClosed	== true
					&& SwitchTestResult->IsCloseFlash == false
					)
				{
					SwitchTestResult->IsCloseFlash		= true;
					SwitchTestResult->CloseFlashTemp	= m_PID_Temp;
				}

				if(pcb_flash_status)
				{
					if(m_iTestMode != TEST_MODE_OPEN_FLASH &&  m_iTestMode != TEST_MODE_OPEN_AND_CLOSE_FLASH)
					{
						if(SwitchTestResult->IsCloseFlash == false)
						{
							SwitchTestResult->IsCloseFlash		= true;
							SwitchTestResult->CloseFlashTemp	= m_PID_Temp;
						}
					}
				}
			
			}

				
			if(SwitchTestResult->IsOpenned == true
				&& SwitchTestResult->IsClosing == true
				&& SwitchTestResult->IsClosed == false
				&& SwitchTestResult->IsUsed == true)
			{//记录第一次复位时的温度

				SwitchTestResult->IsClosed	= true;

				SwitchTestResult->CloseTemp = m_PID_Temp;
				SwitchTestResult->CloseTime = GetSpanSeconds(CTime::GetCurrentTime());
				this->UpdateData(false);
			}
		}
		
	}//降温
}
void CSwitchDlg::DealStatusNormalOpen(SWITCH_TEST_RESULT	*SwitchTestResult,bool pcb_flash_status)
{
	if(m_WorkStatus.SubMode == SUB_MODE_DIRECT_HEAT
		|| m_WorkStatus.SubMode == SUB_MODE_NORMAL_PID
		|| m_WorkStatus.SubMode == SUB_MODE_STEP_PID
		|| m_WorkStatus.SubMode == SUB_COOL_MODE_DIRECT_COOL
		|| m_WorkStatus.SubMode == SUB_COOL_MODE_PID_COOL
		|| m_WorkStatus.SubMode == SUB_COOL_MODE_STEP_COOL
		)
	{//升温
			
			{//常开
				//假如是闪动产品，就将最后一次动作时的温度计作动作温度
				if(SwitchTestResult->IsClosing == false
					&& SwitchTestResult->IsUsed == true
					&& SwitchTestResult->IsOpenned == true
					)
				{
					SwitchTestResult->IsOpenFlash = true;
					SwitchTestResult->IsOpenned = false; 
					SwitchTestResult->OpenTemp = 0.0f;
				}

				//判断是否是刚打开
				if(SwitchTestResult->IsOpenned == false
					&& SwitchTestResult->IsClosing == true
					&& SwitchTestResult->IsUsed == true
					)
				{
					SwitchTestResult->IsOpenned = true; 
					SwitchTestResult->OpenTemp = m_PID_Temp;
					SwitchTestResult->OpenTime = GetSpanSeconds(CTime::GetCurrentTime());
					this->UpdateData(false);
				}
			}
	}//升温
	else if(m_WorkStatus.SubMode  == SUB_MODE_DIRECT_COOL
		  || m_WorkStatus.SubMode == SUB_MODE_DIRECT_COOL
		  || m_WorkStatus.SubMode == SUB_MODE_STEP_COOL
		  || m_WorkStatus.SubMode == SUB_MODE_HEAT_AGAIN
		  || m_WorkStatus.SubMode == SUB_COOL_MODE_DIRECT_HEAT
		  || m_WorkStatus.SubMode == SUB_COOL_MODE_PID_HEAT
		  || m_WorkStatus.SubMode == SUB_COOL_MODE_STEP_HEAT)
	{//降温
		
		{//常开
			//判断是否是刚复位过
			if(SwitchTestResult->IsClosing		== true
				&& SwitchTestResult->IsUsed		== true
				&& SwitchTestResult->IsClosed	== true
				)
			{
				SwitchTestResult->IsClosed		= false; 
				SwitchTestResult->CloseTemp		= 0.0f;
			}
#ifndef	_PTC_
			if(pcb_flash_status)
			{
				//SwitchTestResult->IsCloseFlash = true;
			}
		
#endif									
			if(SwitchTestResult->IsUsed == true
				&& SwitchTestResult->IsOpenned == true
				&& SwitchTestResult->IsClosed == false
				&& SwitchTestResult->IsClosing == false)
			{//记录第一次复位时的温度
				SwitchTestResult->IsClosed = true;

				if(m_bIsClosingDoor == true && (m_iTestMode == TEST_MODE_CLOSE_FLASH  || m_iTestMode == TEST_MODE_OPEN_AND_CLOSE_FLASH))
				{
					SwitchTestResult->CloseTemp = m_CloseDoorStartTemp;
				}
				else
				{
					SwitchTestResult->CloseTemp = m_PID_Temp;
					SwitchTestResult->CloseTime= GetSpanSeconds(CTime::GetCurrentTime());
				}
				
				this->UpdateData(false);
			}//记录第一次复位时的温度
		}//常开
	}//降温
}

static CDC MemDC;
static CBitmap bitmap;
static CBitmap* pOldBitmap;

void CSwitchDlg::ShowTestResult_HeYang(void)
{

	CRect w_rect;
	if(!this->IsWindowVisible()) 
	{
		return;
	}
	
	//G_SwitchTestResult[0][0].IsUsed  = true;
	//G_SwitchTestResult[0][0].IsOpenned = true;
	//G_SwitchTestResult[0][0].OpenTemp += 0.1f;

	this->GetClientRect(w_rect); 
	#define TOP_GAP 100
	#define LEFT_GAP 20

	int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top  - TOP_GAP     ) / 35		- PIC_GAP;
	int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - LEFT_GAP * 2) / 9		- PIC_GAP;


	this->GetClientRect( w_rect);


	int nWidth	= SWITCH_PIC_WIDTH/3;
    int nHeight = SWITCH_PIC_HIGH ;

	CDC *dc = NULL;
	
	CFont font;
	CString msg;
	



	    dc=this->GetDC();
		
		if(dc != NULL)
		{
			if(MemDC.GetSafeHdc() != NULL)
			{
				MemDC.DeleteDC();
			}
			
			if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
			{
				if(dc->GetSafeHdc() != NULL)
				{
					ReleaseDC(dc);
				}
				return;
			}
		}
		else
		{
			return;
		}
	
		bitmap.DeleteObject();

		if(!bitmap.CreateCompatibleBitmap(dc,w_rect.Width() ,w_rect.Height()))	//建立与显示设备兼容的位图
		{
			if(dc->GetSafeHdc() != NULL)
			{
				ReleaseDC(dc);
			}
			return;
		}

		pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

		CBrush bbrush(COLORREF(RGB(255,255,255)));

		MemDC.FillRect(w_rect,&bbrush);

	int i,j,id;
	if(font.CreatePointFont(SWITCH_PIC_HIGH * 6 * 3,_T("黑体"),&MemDC))
	{
		CFont *oldfont = MemDC.SelectObject(&font); 
		MemDC.SetTextColor(RGB(0,0,0)); 

		CRect rect;
		rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * 0 + nWidth*0;
		rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-4);
		rect.right  = rect.left + nWidth * 24;
		rect.bottom = rect.top + nHeight * 3;
		MemDC.DrawText(_T("JUC温控器自动温度特性机 III"), rect  ,DT_CENTER | DT_VCENTER);

		MemDC.SelectObject(oldfont); 
		font.DeleteObject(); 
	}
	if(font.CreatePointFont(SWITCH_PIC_HIGH * 6,_T("黑体"),&MemDC))
	{
		CFont *oldfont = MemDC.SelectObject(&font); 
		MemDC.SetTextColor(RGB(0,0,0)); 
		for(j = 0; j < 9; j++)
		{
			CRect rect;
			rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*0;
			rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-1);
			rect.right  = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
			MemDC.DrawText(_T("工位"), rect  ,DT_RIGHT | DT_VCENTER);
			rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*1;
			rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-1);
			rect.right  = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
			MemDC.DrawText(_T("动作"), rect  ,DT_CENTER | DT_VCENTER);
			rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*2;
			rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-1);
			rect.right  = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
			MemDC.DrawText(_T("复位"), rect  ,DT_CENTER | DT_VCENTER);

		}

		for(j = 0; j < 9; j++)
		{
			for(i = 0; i < 30; i++)
			{
				MemDC.SetTextColor(RGB(0,0,0)); 
				id  = i  + j * 30 + 250 * m_Page;
				if(m_Page == 0)
				{
					if(id >= 250)
					{
						continue;
					}
				}
				else
				{
					if(id >= 500)
					{
						continue;
					}
				}
				msg.Format(_T("%5d"), id + 1); 
				CRect rect;
				rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*0;
				rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP) * i;
				rect.right  = rect.left + nWidth;
				rect.bottom = rect.top + nHeight;

				MemDC.DrawText(msg, rect  ,DT_LEFT | DT_VCENTER);

				MemDC.SetTextColor(RGB(255,0,0));
				
					rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*1;
					rect.top  = TOP_GAP + (SWITCH_PIC_HIGH + PIC_GAP)* i;
					rect.right  = rect.left + nWidth;
					rect.bottom = rect.top + nHeight;
				if(G_SwitchTestResult[id].IsOpenned)
				{
					if(G_SwitchTestResult[id].IsOpenFlash)
					{
						msg.Format(_T("X%5.1f"), G_SwitchTestResult[id].OpenTemp); 
					}
					else
					{
						msg.Format(_T("%5.1f"), G_SwitchTestResult[id].OpenTemp);
					}
					MemDC.DrawText(msg, rect  ,DT_LEFT | DT_VCENTER);
				}
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.right,rect.top); 
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.left,rect.bottom); 
				MemDC.MoveTo(rect.left,rect.bottom -1);
				MemDC.LineTo(rect.right ,rect.bottom- 1); 
				
				rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*2;
				rect.top  = TOP_GAP + (SWITCH_PIC_HIGH + PIC_GAP)* i;
				rect.right  = rect.left + nWidth;
				rect.bottom = rect.top + nHeight;
				if(G_SwitchTestResult[id].IsClosed )
				{
					if(G_SwitchTestResult[id].IsCloseFlash)
					{
						msg.Format(_T("%5.1fX"), ::G_SwitchTestResult[id].CloseTemp); 
					}
					else
					{
						msg.Format(_T("%5.1f"), ::G_SwitchTestResult[id].CloseTemp); 
					}
					MemDC.DrawText(msg, rect  ,DT_LEFT | DT_VCENTER);
				}
				
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.right,rect.top); 
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.left,rect.bottom); 
				MemDC.MoveTo(rect.right,rect.top);
				MemDC.LineTo(rect.right,rect.bottom); 
				MemDC.MoveTo(rect.left,rect.bottom -1);
				MemDC.LineTo(rect.right ,rect.bottom-1); 


			}
		}
		
		MemDC.SelectObject(oldfont); 
		font.DeleteObject(); 
	}
	else
	{
		CSize size = MemDC.GetTextExtent(msg);
		MemDC.TextOut(0  , 0,msg);
	}
		


	
	dc->BitBlt(0,0,w_rect.Width(),w_rect.Height() - 6 * nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形


	


#ifndef _DEBUG
	if(dc->GetSafeHdc())
	{
		dc->DeleteDC(); 
	}
#endif

	CWnd *w;
	w = this->GetDlgItem(IDC_EDIT_1);
	w->MoveWindow(LEFT_GAP+nWidth*0,w_rect.Height() - 5 * nHeight,nWidth,nHeight);
	w = this->GetDlgItem(IDC_EDIT_2);
	w->MoveWindow(LEFT_GAP+nWidth*1,w_rect.Height() - 5 * nHeight,nWidth,nHeight);
	w = this->GetDlgItem(IDC_EDIT_3);
	w->MoveWindow(LEFT_GAP+nWidth*2,w_rect.Height() - 5 * nHeight,nWidth,nHeight);
	w = this->GetDlgItem(IDC_EDIT_4);
	w->MoveWindow(LEFT_GAP+nWidth*3,w_rect.Height() - 5 * nHeight,nWidth,nHeight);

	w = this->GetDlgItem(IDC_EDIT_5);
	w->MoveWindow(LEFT_GAP+nWidth*4,w_rect.Height() - 5 * nHeight,nWidth,nHeight);
	w->ShowWindow(SW_SHOW); 

	w = this->GetDlgItem(IDC_RADIO_CLOSE);
	w->MoveWindow(LEFT_GAP+nWidth*0,w_rect.Height() - 3 * nHeight,nWidth,nHeight);
	w = this->GetDlgItem(IDC_RADIO_OPEN);
	w->MoveWindow(LEFT_GAP+nWidth*2,w_rect.Height() - 3 * nHeight,nWidth,nHeight);

	w = this->GetDlgItem(IDC_CHECK_AUTO_PROCESS);
	w->MoveWindow(LEFT_GAP+nWidth*6,w_rect.Height() - 5 * nHeight,nWidth*2,nHeight*1.5);
	
	w = this->GetDlgItem(IDC_STATIC_STATUS);
	w->MoveWindow(LEFT_GAP+nWidth*6,w_rect.Height() - 3 * nHeight,nWidth,nHeight);

	w = this->GetDlgItem(IDC_STATIC_TEST_MSG);
	w->MoveWindow(LEFT_GAP+nWidth*7,w_rect.Height() - 3 * nHeight,nWidth*5,nHeight*1.5);
	
	w = this->GetDlgItem(IDC_STATIC_TIME_MSG);
	w->MoveWindow(LEFT_GAP+nWidth*6,w_rect.Height() - 1 * nHeight,nWidth*6,nHeight*1.5);

	w = this->GetDlgItem(IDC_CHECK_NEXT_PAGE);
	w->MoveWindow(LEFT_GAP+nWidth*10,w_rect.Height() - 5 * nHeight,nWidth*2,nHeight*1.5);

	w = this->GetDlgItem(IDC_STATIC_OPEN_TEMP_LOW);
	w->MoveWindow(LEFT_GAP+nWidth*13,w_rect.Height() - 5 * nHeight,nWidth*3,nHeight*1.5);
	w = this->GetDlgItem(IDC_STATIC_OPEN_TEMP_HIGH);
	w->MoveWindow(LEFT_GAP+nWidth*15,w_rect.Height() - 5 * nHeight,nWidth*3,nHeight*1.5);
	w = this->GetDlgItem(IDC_STATIC_CLOSE_TEMP_LOW);
	w->MoveWindow(LEFT_GAP+nWidth*17,w_rect.Height() - 5 * nHeight,nWidth*3,nHeight*1.5);
	w = this->GetDlgItem(IDC_STATIC_CLOSE_TEMP_HIGH);
	w->MoveWindow(LEFT_GAP+nWidth*19,w_rect.Height() - 5 * nHeight,nWidth*3,nHeight*1.5);

	w = this->GetDlgItem(IDC_EDIT_OPEN_TEMP_LOW);
	w->MoveWindow(LEFT_GAP+nWidth*13,w_rect.Height() - 3 * nHeight,nWidth*1.5,nHeight*1.5);
	w = this->GetDlgItem(IDC_EDIT_OPEN_TEMP_HIGH);
	w->MoveWindow(LEFT_GAP+nWidth*15,w_rect.Height() - 3 * nHeight,nWidth*1.5,nHeight*1.5);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_TEMP_LOW);
	w->MoveWindow(LEFT_GAP+nWidth*17,w_rect.Height() - 3 * nHeight,nWidth*1.5,nHeight*1.5);
	w = this->GetDlgItem(IDC_EDIT_CLOSE_TEMP_HIGH);
	w->MoveWindow(LEFT_GAP+nWidth*19,w_rect.Height() - 3 * nHeight,nWidth*1.5,nHeight*1.5);


	w = this->GetDlgItem(IDC_BUTTON_PRINT);
	w->MoveWindow(LEFT_GAP+nWidth*22,w_rect.Height() - 5 * nHeight,nWidth*1.5,nHeight*1.5);
	
	w = this->GetDlgItem(IDC_BUTTON_RECORD);
	w->MoveWindow(LEFT_GAP+nWidth*22,w_rect.Height() - 3 * nHeight,nWidth*1.5,nHeight*1.5);
	
}

void CSwitchDlg::OnBnClickedCheckNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Page == 0)
	{
		m_Page = 1;
	}
	else
	{
		m_Page = 0;
	}

	this->RedrawWindow(); 
}

void CSwitchDlg::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CPrintDialog prnDlg(true);//false);
	CString printer;
	CDC dc;
	int i;
	
	

	if(prnDlg.DoModal() != IDOK)
	{
		AfxMessageBox(_T("打印取消!"));
		return;
	}
	printer  = prnDlg.GetDeviceName();
//		AfxMessageBox(printer);

	if(!dc.CreateDC(_T(""),printer,_T(""),NULL))
	{
		AfxMessageBox(_T("请设置打印机"));
		return;
	}	
	
	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	CString strTitle;
	strTitle.Format(_T("%04d%02d%02d%02d%02d"),
		m_BeginTestTime.GetYear(),
		m_BeginTestTime.GetMonth(), 
		m_BeginTestTime.GetDay(), 
		m_BeginTestTime.GetHour(), 
		m_BeginTestTime.GetMinute()); 
	//this->GetWindowText(strTitle); 
	CString app_name = _T("热保护器测试记录");
	//AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += "[";
	app_name += strTitle;
	app_name += "]";
	docInfo.lpszDocName =   app_name;


	i=dc.StartDoc(&docInfo);  
	i=	dc.StartPage();

	CString BoxNr;
	SWITCH_TEST_RESULT_EX SwitchTestResult_Ex[500];

	

	BoxNr.Format(_T("%d"), m_BoxNr + 1);

	//AfxMessageBox("打印1");
	CString ProductType;
	if(m_SwitchNormalOpen == 0)
	{
		ProductType = _T("常闭");
	}
	else
	{
		ProductType = _T("常开");
	}

	for(int i = 0; i <  G_iMaxLedNr; i++)
	{
		SwitchTestResult_Ex[i].ID = i;
		SwitchTestResult_Ex[i].result = G_SwitchTestResult[i];
	}


	memset(&m_SwitchConfigPara,0,sizeof(m_SwitchConfigPara));

	m_SwitchConfigPara.MainLevel = this->m_OpenTempLow;
	m_SwitchConfigPara.MainLevelSpanMinus = 0;
	m_SwitchConfigPara.MainLevelSpanPlus = m_OpenTempHigh - m_OpenTempLow;

	m_SwitchConfigPara.CloseLevel = m_CloseTempLow;
	m_SwitchConfigPara.CloseLevelSpanMinus = 0;
	m_SwitchConfigPara.CloseLevelSpanPlus = m_CloseTempHigh - m_CloseTempLow;

	PrintReport(&dc,app_name, m_BoxNr,
				m_BeginTestTime,m_EndTestTime,
				m_ParaName,ProductType, _T(""),
				m_SwitchConfigPara,SwitchTestResult_Ex,
				TRUE,0,249,(HEAT_BOX_TYPE)m_HeatBoxType);

	


	i =	dc.EndPage(); 
	
	if(G_iMaxLedNr > 250)
	{
		i = dc.StartPage();
		PrintReport(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_ParaName,ProductType,_T(""), 
					m_SwitchConfigPara,SwitchTestResult_Ex,
					TRUE,250,G_iMaxLedNr - 1,(HEAT_BOX_TYPE)m_HeatBoxType);

		i =	dc.EndPage(); 
	}

	i =	dc.EndDoc(); 
	
	dc.DeleteDC();
}

void CSwitchDlg::OnEnChangeEditOpenTempLow()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CEdit *edit;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_OPEN_TEMP_LOW);
	TCHAR buf[10];
	edit->GetWindowText(buf,10); 
	m_OpenTempLow = _tcstod(buf,NULL);

	AfxGetApp()->WriteProfileInt(_T("SET"),_T("OPEN TEMP LOW"),this->m_OpenTempLow * 10);

}

void CSwitchDlg::OnEnChangeEditOpenTempHigh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CEdit *edit;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_OPEN_TEMP_HIGH);
	TCHAR buf[10];
	edit->GetWindowText(buf,10); 
	m_OpenTempHigh = _tcstod(buf,NULL);

	AfxGetApp()->WriteProfileInt(_T("SET"),_T("OPEN TEMP HIGH"),this->m_OpenTempHigh * 10);

}

void CSwitchDlg::OnEnChangeEditCloseTempLow()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CEdit *edit;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_CLOSE_TEMP_LOW);
	TCHAR buf[10];
	edit->GetWindowText(buf,10); 
	m_CloseTempLow = _tcstod(buf,NULL);

	AfxGetApp()->WriteProfileInt(_T("SET"),_T("CLOSE TEMP LOW"),this->m_CloseTempLow * 10);

}

void CSwitchDlg::OnEnChangeEditCloseTempHigh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CEdit *edit;
	edit=(CEdit*)this->GetDlgItem(IDC_EDIT_CLOSE_TEMP_HIGH);
	TCHAR buf[10];
	edit->GetWindowText(buf,10); 
	m_CloseTempHigh = _tcstod(buf,NULL);

	AfxGetApp()->WriteProfileInt(_T("SET"),_T("CLOSE TEMP HIGH"),this->m_CloseTempHigh * 10);

}

void CSwitchDlg::OnBnClickedButtonRecord()
{
	// TODO: 在此添加控件通知处理程序代码

	CRecordViewDlg dlg;
	dlg.m_BoxNr  = 0;
	dlg.DoModal(); 
}
