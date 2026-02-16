// ProductTempBar.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "ProductTempBar.h"
#include "SwitchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC)  
{  
    PBITMAPINFO pbi = CreateBitmapInfoStruct(hwnd, hBMP);  
  
    HANDLE hf;                 // file handle   
    BITMAPFILEHEADER hdr;       // bitmap file-header   
    PBITMAPINFOHEADER pbih;     // bitmap info-header   
    LPBYTE lpBits;              // memory pointer   
    DWORD dwTotal;              // total count of bytes   
    DWORD cb;                   // incremental count of bytes   
    BYTE *hp;                   // byte pointer   
    DWORD dwTmp;   
  
    pbih = (PBITMAPINFOHEADER) pbi;   
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);  
  
    if (!lpBits)   
	{
         AfxMessageBox(_T("GlobalAlloc Error"));   
	}
  
    // Retrieve the color table (RGBQUAD array) and the bits   
    // (array of palette indices) from the DIB.


	
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi,   
        DIB_RGB_COLORS))   
    {  
        AfxMessageBox(_T("GetDIBits Error"));   
    }
  
    // Create the .BMP file.   
    hf = CreateFile(pszFile,   
                   GENERIC_READ | GENERIC_WRITE,   
                   (DWORD) 0,   
                    NULL,   
                   CREATE_ALWAYS,   
                   FILE_ATTRIBUTE_NORMAL,   
                   (HANDLE) NULL);   
    if (hf == INVALID_HANDLE_VALUE)   
        AfxMessageBox(_T("CreateFile"));   
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"   
    // Compute the size of the entire file.   
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +   
                 pbih->biSize + pbih->biClrUsed   
                 * sizeof(RGBQUAD) + pbih->biSizeImage);   
    hdr.bfReserved1 = 0;   
    hdr.bfReserved2 = 0;   
  
    // Compute the offset to the array of color indices.   
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +   
                    pbih->biSize + pbih->biClrUsed   
                    * sizeof (RGBQUAD);   
  
    // Copy the BITMAPFILEHEADER into the .BMP file.   
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),   
        (LPDWORD) &dwTmp,  NULL))   
    {  
       AfxMessageBox(_T("WriteFile"));   
    }  
  
    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.   
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)   
                  + pbih->biClrUsed * sizeof (RGBQUAD),   
                  (LPDWORD) &dwTmp, ( NULL)))  
    {  
        AfxMessageBox(_T("WriteFile"));   
    }  
          
  
    // Copy the array of color indices into the .BMP file.   
    dwTotal = cb = pbih->biSizeImage;   
    hp = lpBits;   
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))   
    {  
		AfxMessageBox(_T("WriteFile"));   
    }  
  
    // Close the .BMP file.   
     if (!CloseHandle(hf))  
	 {
		AfxMessageBox(_T("CloseHandle"));   
	 }
  
    // Free memory.   
    GlobalFree((HGLOBAL)lpBits);  
  
    return TRUE;  
}  

//创建一个BMP文件头对象  
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)  
{  
    BITMAP bmp;   
    PBITMAPINFO pbmi;   
    WORD    cClrBits;   
  
    // Retrieve the bitmap color format, width, and height.   
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))   
	{
        AfxMessageBox(_T("GetObject"));   
	}
  
    // Convert the color format to a count of bits.   
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);   
    if (cClrBits == 1)   
        cClrBits = 1;   
    else if (cClrBits <= 4)   
        cClrBits = 4;   
    else if (cClrBits <= 8)   
        cClrBits = 8;   
    else if (cClrBits <= 16)   
        cClrBits = 16;   
    else if (cClrBits <= 24)   
        cClrBits = 24;   
    else cClrBits = 32;   
  
    // Allocate memory for the BITMAPINFO structure. (This structure   
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD   
    // data structures.)   
  
     if (cClrBits != 24)   
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,   
                    sizeof(BITMAPINFOHEADER) +   
                    sizeof(RGBQUAD) * (1<< cClrBits));   
  
     // There is no RGBQUAD array for the 24-bit-per-pixel format.   
  
     else   
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,   
                    sizeof(BITMAPINFOHEADER));   
  
    // Initialize the fields in the BITMAPINFO structure.   
  
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   
    pbmi->bmiHeader.biWidth = bmp.bmWidth;   
    pbmi->bmiHeader.biHeight = bmp.bmHeight;   
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;   
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;   
    if (cClrBits < 24)   
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits);   
  
    // If the bitmap is not compressed, set the BI_RGB flag.   
    pbmi->bmiHeader.biCompression = BI_RGB;   
  
    // Compute the number of bytes in the array of color   
    // indices and store the result in biSizeImage.   
    // For Windows NT, the width must be DWORD aligned unless   
    // the bitmap is RLE compressed. This example shows this.   
    // For Windows 95/98/Me, the width must be WORD aligned unless the   
    // bitmap is RLE compressed.  
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8  
                                  * pbmi->bmiHeader.biHeight;   
    // Set biClrImportant to 0, indicating that all of the   
    // device colors are important.   
     pbmi->bmiHeader.biClrImportant = 0;   
     return pbmi;   
} 

void OnExportChart(CString FileName,CDC *pChartDC,CTime BeginTestTime,CWnd *ParentWnd,CRect ChartRect) 
{ 
	// TODO: Add your control notification handler code here 
	CString strFilter= _T("BMP文件(*.bmp)|(*.bmp)|| "); 
	CString str;
	str.Format(_T("温度分布图[%04d%02d%02d%02d%02d%02d]"),
		BeginTestTime.GetYear(),
		BeginTestTime.GetMonth(), 
		BeginTestTime.GetDay(), 
		BeginTestTime.GetHour(), 
		BeginTestTime.GetMinute(), 
		BeginTestTime.GetSecond()); 

	



	//获取MSCHART的图片 
	//CDC* pChartDC; 

	// Get device context from MSChart 
	// pChartDC = this->m_Chart1.GetDC(); 

	// Get dimensions of MSChart 
	//RECT mschartRect; 
	//m_Chart1.GetClientRect( &mschartRect ); 
	int mschartWidth	= ChartRect.right - ChartRect.left; 
	int mschartHeight	= ChartRect.bottom - ChartRect.top; 

	// Create CBitmap 
	CBitmap myBitmap; 

	// Create Compatible Bitmap for MSChart 
	myBitmap.CreateCompatibleBitmap( pChartDC, mschartWidth, mschartHeight); 

	// Define device-context object 
	CDC myCopy; 
	myCopy.CreateCompatibleDC( pChartDC ); 

	// Get pointer to object being replaced 
	myCopy.SelectObject( myBitmap ); 

	myCopy.BitBlt( 0, 0, mschartWidth, mschartHeight, pChartDC, 0, 0, SRCCOPY ); 

	// Retrieve information about the CBitmap 
	BITMAP bits; 
	myBitmap.GetBitmap( &bits ); 

	// Open clipboard and empty its contents 
	::OpenClipboard(ParentWnd->GetSafeHwnd()); 
	
	if(!EmptyClipboard())
	{
		AfxMessageBox(_T("EmptyClipboard Error"));
	}

	// Copy our new MSChart bitmap to clipboard and close it 
	SetClipboardData( CF_BITMAP, myBitmap.GetSafeHandle() ); 
	CloseClipboard(); 
	//	m_cMsChart.EditCopy(); 
	// 
	if(FileName.GetLength() == 0) 
	{
		CFileDialog m_cFileDlg(FALSE, 
		_T("bmp "), 
		str, 
		OFN_OVERWRITEPROMPT, 
		_T("Save MSChart to BMP (*.bmp)|*.bmp|| "), ParentWnd); 
		if(m_cFileDlg.DoModal()==IDOK) 
		{ 
			FileName = m_cFileDlg.GetPathName(); 
		}
		else
		{
			return;
		}
	}

	if( !OpenClipboard(ParentWnd->GetSafeHwnd()) ) 
	{
		return; 
	}
			// Is the object stored in the clipboard of type CF_BITMAP? If not, something odd happened 
			// and perhaps some other application erased/replaced the contents of the clipboard? 
			if( !IsClipboardFormatAvailable( CF_BITMAP ) ) 
			{ 
				AfxMessageBox( _T("CF_BITMAP not available ") ); 
				CloseClipboard(); 
				return; 
			} 

			// Create and copy the handle to the BITMAP in the clipboard memory 
			HBITMAP hData = (HBITMAP)GetClipboardData(CF_BITMAP); 

			// Close the clipboard 
			CloseClipboard(); 
			// 
			PBITMAPINFO pbi = CreateBitmapInfoStruct(ParentWnd->GetSafeHwnd(), hData); 
			//save to file 
			CreateBMPFile( ParentWnd->GetSafeHwnd() , FileName.GetBuffer() , hData, ::GetDC(ParentWnd->GetSafeHwnd()) ); 

			FileName.ReleaseBuffer(); 

}


void DrawTempFlat(CDC *dc,SWITCH_TEST_RESULT  *pSwitchTestResult ,SWITCH_CONFIG_PARA *pSwitchConfigPara ,CRect ChartRect,CTime BeginTestTime, bool IsOpenTemp)
{
	//CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	//SWITCH_CONFIG_PARA	SwitchConfigPara = maindlg->m_SwitchDlg[m_BoxNr].m_SwitchConfigPara;
	int total[200],sum_total = 0;
	CONTROL_TEMP_RANGE temp_range;

	double max_temp = -999;
	double min_temp =  999;

	
	CheckHeatOrCoolMode(*pSwitchConfigPara,&temp_range);

	if((IsOpenTemp == false) && (temp_range.close_temp_min == temp_range.close_temp_max))
	{
	//	AfxMessageBox(_T("没有测试复位温度"));
		return;
	}
	

	int x_min,x_max;
	if(IsOpenTemp)
	{
		x_min = temp_range.open_temp_min - 1;
		x_max = temp_range.open_temp_max + 1;
	}
	else
	{
		x_min = temp_range.close_temp_min - 1;
		x_max = temp_range.close_temp_max;
	}

	memset(total,0,sizeof(total));

	for(int i = 0; i < G_iMaxLedNr; i++)
	{
		if(pSwitchTestResult[i].IsUsed)
		{
			sum_total ++;
			double temp;
			if(IsOpenTemp)
			{
				if(pSwitchTestResult[i].IsOpenned)
				{
					temp = (pSwitchTestResult[i].OpenTemp);

					if((temp >= temp_range.open_temp_min) && (temp <= temp_range.open_temp_max))
					{
						if(temp > max_temp)
						{
							max_temp = temp;
						}

						if(temp < min_temp)
						{
							min_temp = temp;
						}
					}
					
				}
				else
				{
					temp = x_max ;
				}
			}
			else
			{
				if(pSwitchTestResult[i].IsClosed)
				{
					temp = (pSwitchTestResult[i].CloseTemp );

					if((temp >= temp_range.close_temp_min) && (temp <= temp_range.close_temp_max))
					{
						if(temp > max_temp)
						{
							max_temp = temp;
						}

						if(temp < min_temp)
						{
							min_temp = temp;
						}
					}

				}
				else
				{
					temp = x_max;
				}
			}
			if(temp < 200)
			{
				total[(short)temp] ++;
			}
		}
	}

	x_min = min_temp - 1;
	x_max = max_temp + 1;


	CString start_time;
	start_time.Format(_T("测试时间 %04d-%02d-%02d  %02d:%02d:%02d  "),
		BeginTestTime.GetYear(),
		BeginTestTime.GetMonth(),
		BeginTestTime.GetDay(),
		BeginTestTime.GetHour(),
		BeginTestTime.GetMinute(),
		BeginTestTime.GetSecond());

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC;
	 
	 CFont font,*oldFont;
	 CBrush groundbrush(COLORREF(RGB(0xFF,0xFF,0xFF)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	CBrush bluebrush(COLORREF(RGB(0,0xFF,0)));
	 CBrush graybrush(COLORREF(RGB(192,192,192)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	
	//绘制准备
//	dc=this->GetDC();
		

	if(dc == NULL)
	{
		return;
	}

	
	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,ChartRect.Width(),ChartRect.Height()))	//建立与显示设备兼容的位图
	{
		bitmap.DeleteObject(); 
		return;
	}

	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0x00,0x00,0x00));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	

	

	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
	MemDC.Rectangle(0,0, ChartRect.Width(),ChartRect.Height());
	//MemDC.FillRect(CRect(0,0, ChartRect.Width(),ChartRect.Height()),
		
	//左边空1个FontSize.cx, 上面空 2 *FontSize.cy,	下面空 3 *FontSize.cy,
		
		
		CString s;
		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
		MemDC.SetBkMode(TRANSPARENT);
		
		CSize FontSize;

		s.Format(_T("最高温度:%5.1f℃ 最低温度:%5.1f℃"),max_temp,min_temp);
		s = start_time + s;

		int FontHeight = (ChartRect.Height()*10)/60;
		if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
		{
			oldFont = MemDC.SelectObject(&font);
		}
		else
		{
			return;
		}
		
		while(1)
		{
			 FontSize = MemDC.GetTextExtent(s);
			 if(FontSize.cx < ChartRect.Width() * 0.8)
			 {
				 break;
			 }
			 else
			 {
				MemDC.SelectObject(oldFont);
				font.DeleteObject(); 
				FontHeight -= 20;
				if(font.CreatePointFont(FontHeight,_T("黑体"),&MemDC))
				{
					oldFont = MemDC.SelectObject(&font);
				}
				else
				{
					return;
				}
			 }
			 
		}

		FontSize = MemDC.GetTextExtent(_T("88.8个"));

		
		int x_gap;
		

		x_gap = x_max - x_min + 1;
		


		pen.DeleteObject(); 
		pen.CreatePen(PS_DOT,PEN_WEIGHT, RGB(0x00,0x00,0x00));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		
		MemDC.SetTextColor(RGB(0,0,0));

		MemDC.DrawTextW(s,CRect(0,0,ChartRect.Width(),FontSize.cy * 2),DT_CENTER | DT_VCENTER);

		

		for(int i = 0; i <= 10; i++)
		{
			s.Format(_T("% 3d%%"),(10-i) * 10);
			CRect dt_rect;
			dt_rect.left   = 0;
			dt_rect.right  = FontSize.cx;
			dt_rect.top    = (ChartRect.Height() -  FontSize.cy * 5)* i / 10+ FontSize.cy * 2 -  FontSize.cy * 0.5;
			dt_rect.bottom = (ChartRect.Height() -  FontSize.cy * 5)* i / 10+ FontSize.cy * 2 +  FontSize.cy * 0.5;

			MemDC.DrawTextW(s,dt_rect,DT_CENTER);

			MemDC.MoveTo(FontSize.cx, (ChartRect.Height() -  FontSize.cy * 5)* i / 10 + FontSize.cy * 2); 	
			MemDC.LineTo(ChartRect.Width(), (ChartRect.Height() -  FontSize.cy * 5) * i / 10 + FontSize.cy * 2);
		}

		MemDC.MoveTo(FontSize.cx, FontSize.cy * 2); 	
		MemDC.LineTo(FontSize.cx, (ChartRect.Height() -  FontSize.cy * 3));
		
		for(int i = 0; i <=(int) x_gap; i++)
		{

			CRect rect;
			rect.left   = FontSize.cx + i * (ChartRect.Width() - FontSize.cx) / x_gap;
			rect.right  = rect.left + (ChartRect.Width() - FontSize.cx) / x_gap - FontSize.cx / 4;
			rect.top    = ChartRect.Height() - FontSize.cy * 3 - total[(int)x_min + i] * (ChartRect.Height() - FontSize.cy * 5) / sum_total;  
			rect.bottom = ChartRect.Height() - FontSize.cy * 3;

			CRect dt_rect;
			
			if(total[(int)x_min + i] > 0)
			{
				s.Format(_T("%d个"),total[(int)x_min + i]);
				dt_rect.left   = rect.left;
				dt_rect.right  = rect.right;
				dt_rect.top    = rect.top -  FontSize.cy;
				dt_rect.bottom = rect.top;
				MemDC.DrawTextW(s,dt_rect,DT_LEFT);

				s.Format(_T("%3.1f%%"),total[(int)x_min + i]*100.0f/sum_total) ;
				dt_rect.left   = rect.left;
				dt_rect.right  = rect.right;
				dt_rect.top    = rect.top -  2 * FontSize.cy;
				dt_rect.bottom = rect.top - FontSize.cy;
				MemDC.DrawTextW(s,dt_rect,DT_LEFT);
			}
			if(i == 0)
			{
				//s.Format(_T("<%d"),(int)(x_min+i)) ;
				s = _T("< <");
			}
			else if((x_min+i) == x_max)
			{
				//s.Format(_T(">%d"),(int)(x_min+i)) ;
				s = _T("> >");
			}
			else
			{
				s.Format(_T("%d"),(int)(x_min+i)) ;
			}
			dt_rect.left   = rect.left;
			dt_rect.right  = rect.right;
			dt_rect.top    = rect.bottom ;
			dt_rect.bottom = rect.bottom + FontSize.cy;
			MemDC.DrawTextW(s,dt_rect,DT_LEFT);

			if((x_min+i) == x_max)
			{
				MemDC.FillRect(rect,&redbrush);
			}
			else if( i == 0)
			{
				MemDC.FillRect(rect,&redbrush);
			}
			else
			{
				MemDC.FillRect(rect,&bluebrush);
			}
			
		}

	/*	
		 CFont* pFont = MemDC.GetCurrentFont();
		 LOGFONT logFont ;
		 pFont->GetLogFont(&logFont);
		 logFont.lfEscapement = 900 ;//900/10 = 90
		 HFONT   hFont   =   CreateFontIndirect(&logFont);   
		 MemDC.SelectObject(hFont);
		 MemDC.DrawText(_T("VC中如何把一串文字旋转90度显示的？"),CRect(0,0,500,500),DT_CENTER);
*/


	dc->BitBlt(ChartRect.left,ChartRect.top,ChartRect.Width(),ChartRect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形


	MemDC.SelectObject(oldbrush);

	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	if(bluebrush.GetSafeHandle() != NULL)
	{
		bluebrush.DeleteObject(); 
	}
	
	if(groundbrush.GetSafeHandle() != NULL)
	{
		groundbrush.DeleteObject(); 
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
	
	//ReleaseDC(dc); 
}


/////////////////////////////////////////////////////////////////////////////
// CProductTempBar dialog


CProductTempBar::CProductTempBar(CWnd* pParent /*=NULL*/)
	: CDialog(CProductTempBar::IDD, pParent)
{
	int i;
	for(i =0 ; i < G_iMaxLedNr; i++)
	{
		m_SwitchTestResult[i].IsUsed = false;
	}
	//{{AFX_DATA_INIT(CProductTempBar)

	//}}AFX_DATA_INIT

	m_bOpenTemp = true;
}


void CProductTempBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProductTempBar)
//	DDX_Control(pDX, IDC_MSCHART1, m_Chart1);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProductTempBar, CDialog)
	//{{AFX_MSG_MAP(CProductTempBar)
	ON_BN_CLICKED(IDC_RADIO_CLOSE_TEMP, OnRadioCloseTemp)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_OPEN_TEMP, OnRadioOpenTemp)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CProductTempBar::OnBnClickedButtonSave)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CProductTempBar::OnBnClickedButtonPrint)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProductTempBar message handlers

void CProductTempBar::OnRadioOpenTemp() 
{
	// TODO: Add your control notification handler code here



	m_bOpenTemp = true;
	

	CDC *dc = this->GetDC();
	CRect ChartRect;
//	this->m_Chart1.GetWindowRect(&ChartRect);
	this->GetWindowRect(&ChartRect); 
	ChartRect.top = ChartRect.Height() / 10; 
	ScreenToClient( &ChartRect);

	DrawTempFlat(dc,m_SwitchTestResult,&m_SwitchConfigPara, ChartRect,this->m_BeginTestTime,m_bOpenTemp);

	this->ReleaseDC(dc);


	return;
/*
	int count[201];
	float MinTemp = 200,MaxTemp = 0;
	int rowcount=m_Chart1.GetRowCount();
	float avr_temp = 0.0f;
	int	amount = 0;
	memset(count,0,sizeof(count));
	

	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);

	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(2);

	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetPercentFormat(_T("0.0%"));


	
	
	for(int i =0 ; i < G_iMaxLedNr; i++)
	{
		if(m_SwitchTestResult[i].IsUsed)
		{
			float t = (float)(m_SwitchTestResult[i].OpenTemp);
			
			amount ++;

			if(m_SwitchTestResult[i].IsOpenned)
			{
				avr_temp += m_SwitchTestResult[i].OpenTemp;
				

				if(t > MaxTemp)
				{
					MaxTemp = t;
				}
				if(t < MinTemp)
				{
					MinTemp = t;
				}

				if(t >0 && t <200)
				{
					count[(int)t] ++; 
				}
				else
				{
					count[0] ++;
				}
			}
			else
			{
				count[200]++;
			}
		}
	}



	CString str;

	VARIANT var; 
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(FALSE);		// 不自动标注Y轴刻度   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(amount);	// Y轴最大刻度   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(0);		// Y轴最小刻度   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(1);	// Y轴刻度5等分   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(1);	// 每刻度一个刻度线   

	if(amount > 0)
	{
		avr_temp /= amount;
		str.Format(_T("最低:%5.1f°,        最高%5.1f°,        平均:%5.1f°"),MinTemp, MaxTemp,avr_temp); 
	}
	else
	{
		str = _T("没有产品!  最低:0.0°,          最高0.0°,        平均:0.0°");
	}

	m_Chart1.SetTitleText(str);
	str.Format(_T("              测试时间: %04d-%02d-%02d %02d:%02d:%02d"),
		m_BeginTestTime.GetYear(),
		m_BeginTestTime.GetMonth(), 
		m_BeginTestTime.GetDay(), 
		m_BeginTestTime.GetHour(), 
		m_BeginTestTime.GetMinute(), 
		m_BeginTestTime.GetSecond()); 
	m_Chart1.SetFootnoteText(str); 


	if(MinTemp <= MaxTemp)
	{
		int RowCount = ((int)(MaxTemp) - (int)(MinTemp))/2 + 1 + 1;
		m_Chart1.SetRowCount(RowCount);

		
		for(int i = 0 ; i < RowCount - 1 ; i++)
		{
			this->m_Chart1.GetDataGrid().SetData(i+1,1,count[i*2+(int)(MinTemp)]+count[i*2+1+(int)(MinTemp)],0);
			str.Format(_T("%d-%d(%d)"),(int)(MinTemp)+2*i,(int)(MinTemp)+2+2*i,count[i*2+(int)(MinTemp)]+count[i*2+1+(int)(MinTemp)]);
			m_Chart1.GetDataGrid().SetRowLabel(  i + 1,1,str);
		}

			m_Chart1.GetDataGrid().SetData(RowCount,1,count[200],0);
			str.Format(_T("不动作(%d)"),count[200]);
			m_Chart1.GetDataGrid().SetRowLabel(  RowCount,1,str);
	}
	else
	{
	
		m_Chart1.SetRowCount(10);
		for(int i = (int)00 ; i < (int)200 ; i+=20)
		{
			this->m_Chart1.GetDataGrid().SetData(i/20+1,1,0,0);
			str.Format(_T("%d-%d"),i,i+19);
			m_Chart1.GetDataGrid().SetRowLabel(i/20 + 1,1,str);
		}
	}

*/

}

void CProductTempBar::OnRadioCloseTemp() 
{
	// TODO: Add your control notification handler code here

	m_bOpenTemp = false;
	
	CDC *dc = this->GetDC();
	CRect ChartRect;
//	this->m_Chart1.GetWindowRect(&ChartRect);
	this->GetWindowRect(&ChartRect); 
	ChartRect.top = ChartRect.Height() / 10; 
	ScreenToClient( &ChartRect);

	DrawTempFlat(dc,m_SwitchTestResult,&m_SwitchConfigPara, ChartRect,this->m_BeginTestTime,m_bOpenTemp);

	this->ReleaseDC(dc);

	return;
/*

	int count[201];
	float MinTemp = 200,MaxTemp = 0;
	int rowcount=m_Chart1.GetRowCount();
	
	float avr_temp = 0.0f;
	int	amount = 0;
	memset(count,0,sizeof(count));

	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetLocationType(1);

	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetComponent(2);

	m_Chart1.GetPlot().GetSeriesCollection().GetItem(1).GetDataPoints().GetItem(-1).GetDataPointLabel().SetPercentFormat(_T("0.0%"));

	for(int i =0 ; i < G_iMaxLedNr; i++)
	{
		if(m_SwitchTestResult[i].IsUsed)
		{
			float t = (m_SwitchTestResult[i].CloseTemp);
			amount ++;
			if(m_SwitchTestResult[i].IsClosed)
			{
				avr_temp += m_SwitchTestResult[i].CloseTemp ;
				
				if(t > MaxTemp)
				{
					MaxTemp = t;
				}
				if(t < MinTemp)
				{
					MinTemp = t;
				}

				if(t >0 && t <200)
				{
					count[(int)t] ++; 
				}
				else
				{
					count[0] ++;
				}
			}
			else
			{
				count[200] ++;
			}
		}
	}

	CString str;

	if(amount > 0)
	{
		avr_temp /= amount;
		str.Format(_T("最低:%5.1f°,        最高%5.1f°,        平均:%5.1f°"),MinTemp, MaxTemp,avr_temp); 
	}
	else
	{
		str = _T("没有产品!  最低:0.0°,          最高0.0°,        平均:0.0°");
	}

	m_Chart1.SetTitleText(str);

	VARIANT var; 
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(FALSE); // 不自动标注Y轴刻度   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(amount); // Y轴最大刻度   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(0); // Y轴最小刻度   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(1); // Y轴刻度5等分   
	m_Chart1.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(1); // 每刻度一个刻度线   
	//m_Chart1.GetPlot().GetAxis(1,var).GetAxisTitle().SetText(_T("小时")); // Y轴名称   


	if(MinTemp < MaxTemp)
	{
		int RowCount = ((int)(MaxTemp) - (int)(MinTemp))/2 + 1 + 1;
		m_Chart1.SetRowCount(RowCount);

		m_Chart1.GetDataGrid().SetData(1,1,count[200],0);
		str.Format(_T("不复位(%d)"),count[200]);
		m_Chart1.GetDataGrid().SetRowLabel( 1,1,str);

		for(int i = 1 ; i < RowCount ; i++)
		{
			this->m_Chart1.GetDataGrid().SetData(i+1,1,count[i*2+(int)(MinTemp)]+count[i*2+1+(int)(MinTemp)],0);
			str.Format(_T("%d-%d(%d)"),(int)(MinTemp)+2*i,(int)(MinTemp)+2+2*i,count[i*2+(int)(MinTemp)]+count[i*2+1+(int)(MinTemp)]);
			m_Chart1.GetDataGrid().SetRowLabel(  i + 1,1,str);
		}
	}
	else
	{
		m_Chart1.SetRowCount(10);
		for(int i = (int)00 ; i < (int)200 ; i+=20)
		{
			this->m_Chart1.GetDataGrid().SetData(i/20+1,1,0,0);
			str.Format(_T("%d-%d"),i,i+19);
			m_Chart1.GetDataGrid().SetRowLabel(i/20 + 1,1,str);
		}
	}
	*/
}



void CProductTempBar::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	OnRadioOpenTemp();


}

void CProductTempBar::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	CDC *dc = m_Chart1.GetDC();
	CRect ChartRect;
	this->m_Chart1.GetWindowRect(&ChartRect); 
	this->ScreenToClient(&ChartRect);

	OnExportChart(_T("C:\\a.bmp"),dc,this->m_BeginTestTime,this,ChartRect);

	this->ReleaseDC(dc);
	*/

}


void CProductTempBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CProductTempBar::OnBnClickedButtonPrint()
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
	
	app_name += "[";
	app_name += strTitle;
	app_name += "]";
	docInfo.lpszDocName =   app_name;


	i =	dc.StartDoc(&docInfo);  
	i =	dc.StartPage();

	CString BoxNr;


	CRect ChartRect;
	ChartRect.left = 0;
	ChartRect.right = dc.GetDeviceCaps(HORZRES);
	ChartRect.top = 0; 
	ChartRect.bottom = dc.GetDeviceCaps(VERTRES);
	
	DrawTempFlat(&dc,m_SwitchTestResult,&m_SwitchConfigPara, ChartRect,this->m_BeginTestTime,m_bOpenTemp);


	i =	dc.EndPage(); 
	
	

	i =	dc.EndDoc(); 
	
	dc.DeleteDC();


	
}

void CProductTempBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDblClk(nFlags, point);

	CPaintDC dc(this);
	
	CRect ChartRect;
	this->GetClientRect(&ChartRect); 
	
	//this->ClientToScreen(&ChartRect);

	OnExportChart(_T("C:\\a.bmp"),&dc,this->m_BeginTestTime,this,ChartRect);

	this->ReleaseDC(&dc);
}

BOOL CProductTempBar::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->ShowWindow( SW_SHOWMAXIMIZED);
	return true;
}