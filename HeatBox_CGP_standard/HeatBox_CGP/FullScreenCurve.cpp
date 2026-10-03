// FullScreenCurve.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "HeatBox_CGPDlg.h"
#include "FullScreenCurve.h"



// CFullScreenCurve 对话框

IMPLEMENT_DYNAMIC(CFullScreenCurve, CDialog)

CFullScreenCurve::CFullScreenCurve(CWnd* pParent /*=NULL*/)
	: CDialog(CFullScreenCurve::IDD, pParent)
{
		m_BoxNr = -1;
}

CFullScreenCurve::~CFullScreenCurve()
{
}

void CFullScreenCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFullScreenCurve, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFullScreenCurve::OnBnClickedButtonSave)
	ON_WM_TIMER()
	ON_BN_CLICKED(1023, &CFullScreenCurve::OnBnClicked1023)
END_MESSAGE_MAP()


// CFullScreenCurve 消息处理程序

void CFullScreenCurve::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	

	this->ExportTempCurve(_T("")); 
}


BOOL CFullScreenCurve::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->ShowWindow(SW_SHOWMAXIMIZED); 

	this->SetTimer(1,1000,NULL); 
	return true;
}

void CFullScreenCurve::DrawTempCurve(void)
{
	CFile log_file;
	CString m_TestMsg;

	if(!this->IsWindowVisible()) 
	{
		return;
	}

	if(m_TempRecordArray.GetSize() <= 0)
	{
		return;
	}

	CString start_time;
	CTime t(m_TempRecordArray.GetAt(0).time);

	start_time.Format(_T("开始时间 %04d-%02d-%02d  %02d:%02d:%02d"),
		t.GetYear(),
		t.GetMonth(),
		t.GetDay(),
		t.GetHour(),
		t.GetMinute(),
		t.GetSecond());

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 
	 CBrush groundbrush(COLORREF(RGB(0,0,0)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush yellowbrush(COLORREF(RGB(0xFF,0xFF,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(192,192,192)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	
	//绘制准备
	dc=this->GetDC();
		

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
	
	//CWnd *data_area = GetDlgItem(IDC_MSCHART1);
	//CRect data_area_Rect;
	//data_area->GetWindowRect(data_area_Rect);

	CRect data_area_Rect;
	
	GetClientRect(&data_area_Rect);
	data_area_Rect.top += data_area_Rect.Height() / 20; 
	
	//data_area_Rect.bottom -= data_area_Rect.Height() / 10;

	//ScreenToClient(&data_area_Rect);


	
	
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
	if(!bitmap.CreateCompatibleBitmap(dc,data_area_Rect.Width(),data_area_Rect.Height()))	//建立与显示设备兼容的位图
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
		return;
	}

	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	1	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	



	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
	MemDC.Rectangle(0,0, data_area_Rect.Width(),data_area_Rect.Height());

	//if(m_bShowTempCurve)
	{
		double max_temp = -100.0f;
		double min_temp = 999.0f;
		for(int i = 0; i < m_TempRecordArray.GetSize(); i++)
		{
			TEMP_RECORD	tr =m_TempRecordArray.GetAt(i);
			if(tr.temp  < min_temp)
			{
				min_temp = tr.temp;
			}
			
			if(tr.temp > max_temp)
			{
				max_temp = tr.temp;
			}
			
		}
		
		

		CString s;
		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
		MemDC.SetBkMode(TRANSPARENT);
		
		s.Format(_T("       最高温度:%8.1f 最低温度: %8.1f"),max_temp,min_temp);
		s = start_time + s;
		if((max_temp - min_temp) < 50)
		{
			max_temp += 50.0f;
			min_temp -= 2.0f;
		}
		else
		{
			max_temp += 2.0f;
			min_temp -= 2.0f;
		}
		double y_gap = ((max_temp - min_temp) / 5 );
		
		pen.DeleteObject(); 
		pen.CreatePen(PS_DOT,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);

		MemDC.TextOutW(data_area_Rect.Width()/ 3,data_area_Rect.Height() /100 ,s);

		for(int i = 0; i <= 5; i++)
		{
			s.Format(_T("%  5.1f℃"),min_temp + y_gap * i );
			if(i < 5)
			{
				MemDC.TextOutW(0,data_area_Rect.Height() * 0.9 - data_area_Rect.Height() * 0.9 * i/ 5 - 20,s);
			}
			else
			{
				MemDC.TextOutW(0,data_area_Rect.Height() * 0.9  - data_area_Rect.Height() * 0.9 * i/ 5,s);
			}

			MemDC.MoveTo( 0, data_area_Rect.Height() * 0.9  - data_area_Rect.Height() * 0.9 * i/ 5);
			MemDC.LineTo(data_area_Rect.right, data_area_Rect.Height() * 0.9 - data_area_Rect.Height() * 0.9 * i/ 5);
		}

		CFont f;

		 CFont* pFont = MemDC.GetCurrentFont();
		 LOGFONT logFont ;
		 pFont->GetLogFont(&logFont);
		 logFont.lfEscapement = 900 ;//900/10 = 90
		 HFONT   hFont   =   CreateFontIndirect(&logFont);   
		 MemDC.SelectObject(hFont);
		// MemDC.DrawText(_T("VC中如何把一串文字旋转90度显示的？"),CRect(0,0,500,500),DT_CENTER);

		CFont *oldFont = MemDC.SelectObject(&f);
		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));


		double x_gap;


		if(m_TempRecordArray.GetSize() > 0)
		{
			x_gap = (double)(data_area_Rect.Width()) / (double)m_TempRecordArray.GetSize(); 
		}
		else
		{
			x_gap = data_area_Rect.Width() ; 
		}
	
		y_gap = (double)(data_area_Rect.Height() * 0.9 / (double)(max_temp - min_temp));


		for(int i = 0; i < 10; i++)
		{
			MemDC.MoveTo(data_area_Rect.Width() * i / 10, 0);
			MemDC.LineTo(data_area_Rect.Width() * i / 10, data_area_Rect.Height() * 0.9);
			
			int id = data_area_Rect.Width() * i / 10 / x_gap;
			if(id < m_TempRecordArray.GetSize())
			{
				CString s;
				CTime t(m_TempRecordArray.GetAt(id).time);
				s = t.Format(_T("%H:%M:%S")); 
				if((x_gap*(m_TempRecordArray.GetSize() - 1)) > (data_area_Rect.Width() * i / 10))
				{
					MemDC.TextOutW(data_area_Rect.Width() * i / 10, data_area_Rect.Height(),s); 
				}
			}
		}

		
		
		


		pen.DeleteObject(); 
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0,0));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		for(int i = 0; i < m_TempRecordArray.GetSize()-1; i++)
		{
			MemDC.MoveTo((int)(i*x_gap),    (data_area_Rect.Height() * 0.9 - (m_TempRecordArray.GetAt(i).temp    - min_temp) * y_gap));
		
			MemDC.LineTo((int)((i+1)*x_gap),(data_area_Rect.Height() * 0.9 - (m_TempRecordArray.GetAt(i+1).temp  - min_temp) *  y_gap));
			 
		}

		

		{//最后一个也标注时间
			CString s;
			CTime t(m_TempRecordArray.GetAt(m_TempRecordArray.GetSize() - 1).time);
			s = t.Format(_T("%H:%M:%S")); 
			MemDC.TextOutW(x_gap*(m_TempRecordArray.GetSize() - 1), data_area_Rect.Height(),s); 
		}
		
	}

	
	dc->BitBlt(data_area_Rect.left,data_area_Rect.top,data_area_Rect.Width(),data_area_Rect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形
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
void CFullScreenCurve::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(m_BoxNr < 0)
	{//历史曲线
		this->DrawTempCurve();
		return;
	}

	this->m_TempRecordArray.RemoveAll();
	this->m_TempRecordArray.Copy(::G_TempRecordArray); 
	this->DrawTempCurve(); 

	CDialog::OnTimer(nIDEvent);
}


void CFullScreenCurve::ExportTempCurve(CString filePath) 
{ 
	// TODO: Add your control notification handler code here 
	

	CString strFilter= _T("BMP文件(*.bmp)|(*.bmp)|| "); 
	CFileDialog m_cFileDlg(FALSE, 
	_T("bmp "), 
	_T("chart "), 
	OFN_OVERWRITEPROMPT, 
	_T("Save to BMP (*.bmp)|*.bmp|| "), this); 


	//获取MSCHART的图片 
	CDC* pChartDC; 

	// Get device context from MSChart 
	pChartDC = this->GetDC(); 

	
	CRect data_area_Rect;
	
	GetClientRect(&data_area_Rect);
	data_area_Rect.top += data_area_Rect.Height() / 20; 

	//CWnd *data_area = GetDlgItem(IDC_MSCHART1);
	//CRect data_area_Rect;
	//data_area->GetWindowRect(mschartRect);

	//ScreenToClient(&mschartRect);


	int mschartWidth	= data_area_Rect.Width(); 
	int mschartHeight	= data_area_Rect.Height(); 

	// Create CBitmap 
	CBitmap myBitmap; 

	// Create Compatible Bitmap for MSChart 
	myBitmap.CreateCompatibleBitmap( pChartDC, mschartWidth, mschartHeight); 

	// Define device-context object 
	CDC myCopy; 
	myCopy.CreateCompatibleDC( pChartDC ); 

	// Get pointer to object being replaced 
	myCopy.SelectObject( myBitmap ); 

	myCopy.BitBlt( 0, 0, mschartWidth, mschartHeight, pChartDC, data_area_Rect.left , data_area_Rect.top , SRCCOPY ); 

	// Retrieve information about the CBitmap 
	BITMAP bits; 
	myBitmap.GetBitmap( &bits ); 

	// Open clipboard and empty its contents 
	OpenClipboard(); 
	EmptyClipboard(); 

	// Copy our new MSChart bitmap to clipboard and close it 
	SetClipboardData( CF_BITMAP, myBitmap.GetSafeHandle() ); 
	CloseClipboard(); 
	//	m_cMsChart.EditCopy(); 
	if(filePath.GetLength() == 0)
	{
		if(m_cFileDlg.DoModal()!=IDOK)
		{
			return;
		}
		filePath = m_cFileDlg.GetPathName(); 
	}
	{ 
		
		if( !OpenClipboard() ) 
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
			PBITMAPINFO pbi = CreateBitmapInfoStruct( m_hWnd, hData); 
			//save to file 
			CreateBMPFile( m_hWnd, filePath.GetBuffer() , hData, ::GetDC( m_hWnd ) ); 

			filePath.ReleaseBuffer();

	}
}

PBITMAPINFO CFullScreenCurve::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)  
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

BOOL CFullScreenCurve::CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC)  
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
void CFullScreenCurve::OnBnClicked1023()
{
	// TODO: 在此添加控件通知处理程序代码

	this->ExportTempCurve(_T(""));

}
