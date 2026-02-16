// CurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "CurveDlg.h"
#include "HeatBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurveDlg dialog


CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	max=-9999.9f;
	min= 9999.9f;
}


void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurveDlg)
	//DDX_Control(pDX, IDC_MSCHART1, m_Chart);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_CURVE_TYPE, m_ComboCurveType);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialog)
	//{{AFX_MSG_MAP(CCurveDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCurveDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurveDlg message handlers

BOOL CCurveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CRect rect;
	
	GetClientRect(&rect);
	rect.top += rect.Height() / 20; 
	//m_Chart.MoveWindow(&rect);

	this->SetTimer(1,100,NULL); 



	this->m_ComboCurveType.SetCurSel(0);  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurveDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CRect rect;
	 
	GetClientRect(&rect);

	rect.top += rect.Height() / 20;
	rect.bottom -= rect.Height() / 20;
//	m_Chart.MoveWindow(&rect);
}

void CCurveDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CCurveDlg::UpdateTemp(double temp[4])
{
	if(this->m_TempArray[0].GetSize() > 10000)
	{
		m_TempArray[0].RemoveAt(0,100);
		m_TempArray[1].RemoveAt(0,100);
		m_TempArray[2].RemoveAt(0,100);
		m_TempArray[3].RemoveAt(0,100);
	}
	/*
	m_TempArray[0].Add(temp[0]);
	m_TempArray[1].Add(temp[1]);
	m_TempArray[2].Add(temp[2]);
	m_TempArray[3].Add(temp[3]);
*/
	this->DrawTempCurve();

	

}

void CCurveDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	static int i = 0;
	i++;
	//if(i < 50)
	{
		m_TempArray[0].Add(56.5 + i);  
		m_TempArray[1].Add(51.5 + i);  
		m_TempArray[2].Add(52.5 + i);  
		m_TempArray[3].Add(53.5 + i);  

		
	}*/

	//DrawTempCurve();
	//this->KillTimer(1); 
	CDialog::OnTimer(nIDEvent);
}

void CCurveDlg::OnExportChart(CString filePath) 
{ 
	// TODO: Add your control notification handler code here 
	
/*
	CString strFilter= _T("BMP文件(*.bmp)|(*.bmp)|| "); 
	CFileDialog m_cFileDlg(FALSE, 
	_T("bmp "), 
	_T("chart "), 
	OFN_OVERWRITEPROMPT, 
	_T("Save MSChart to BMP (*.bmp)|*.bmp|| "), this); 


	//获取MSCHART的图片 
	CDC* pChartDC; 

	// Get device context from MSChart 
	pChartDC = this->m_Chart.GetDC(); 

	
	// Get dimensions of MSChart 
	RECT mschartRect; 
	m_Chart.GetClientRect( &mschartRect ); 
	int mschartWidth	= mschartRect.right - mschartRect.left; 
	int mschartHeight	= mschartRect.bottom - mschartRect.top; 

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
	*/
}


BOOL CCurveDlg::CreateBMPFile(HWND hwnd, LPTSTR pszFile, HBITMAP hBMP, HDC hDC)  
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
PBITMAPINFO CCurveDlg::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)  
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
void CCurveDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnExportChart("");

	OnExportTempFlat("");
}


void CCurveDlg::DrawTempCurve(void)
{
	CHeatBoxDlg* maindlg = (CHeatBoxDlg*)this->GetParent()->GetParent();
	CFile log_file;
	CString m_TestMsg;

	if(!this->IsWindowVisible()) 
	{
		return;
	}


	CString start_time;
	start_time.Format(_T("开始时间 %04d-%02d-%02d  %02d:%02d:%02d"),
		maindlg->m_SwitchDlg.m_BeginTestTime.GetYear(),
		maindlg->m_SwitchDlg.m_BeginTestTime.GetMonth(),
		maindlg->m_SwitchDlg.m_BeginTestTime.GetDay(),
		maindlg->m_SwitchDlg.m_BeginTestTime.GetHour(),
		maindlg->m_SwitchDlg.m_BeginTestTime.GetMinute(),
		maindlg->m_SwitchDlg.m_BeginTestTime.GetSecond());

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
		for(int i = 0; i < m_TempArray[0].GetSize(); i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(m_TempArray[j].GetAt(i) < min_temp)
				{
					min_temp = m_TempArray[j].GetAt(i);
				}
				
				if(m_TempArray[j].GetAt(i) > max_temp)
				{
					max_temp = m_TempArray[j].GetAt(i);
				}
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
				MemDC.TextOutW(0,data_area_Rect.Height() - data_area_Rect.Height() * i/ 5 - 20,s);
			}
			else
			{
				MemDC.TextOutW(0,data_area_Rect.Height()  - data_area_Rect.Height() * i/ 5,s);
			}

			MemDC.MoveTo( 0, data_area_Rect.Height()  - data_area_Rect.Height() * i/ 5);
			MemDC.LineTo(data_area_Rect.right, data_area_Rect.Height() - data_area_Rect.Height() * i/ 5);
		}

		for(int i = 1; i < 10; i++)
		{
			MemDC.MoveTo(data_area_Rect.Width() * i / 10, 0);
			MemDC.LineTo(data_area_Rect.Width() * i / 10, data_area_Rect.Height());
		}

		
		double x_gap;


		if(m_TempArray[0].GetSize() > 0)
		{
			x_gap = (double)(data_area_Rect.Width()) / (double)(m_TempArray[0].GetSize()); 
		}
		else
		{
			x_gap = data_area_Rect.Width() ; 
		}
	
		y_gap = (double)(data_area_Rect.Height() / (double)(max_temp - min_temp));
		


		pen.DeleteObject(); 
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0,0));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		for(int i = 0; i < m_TempArray[0].GetSize()-1; i++)
		{
			MemDC.MoveTo((int)(i*x_gap),    (data_area_Rect.Height() - (m_TempArray[0].GetAt(i)   - min_temp) * y_gap));
		
			MemDC.LineTo((int)((i+1)*x_gap),(data_area_Rect.Height() - (m_TempArray[0].GetAt(i+1) - min_temp) *  y_gap));
			 
		}

		
		pen.DeleteObject(); 
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0xFF,0));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		for(int i = 0; i < m_TempArray[1].GetSize()-1; i++)
		{
			MemDC.MoveTo((int)(i*x_gap),    (data_area_Rect.Height() - (m_TempArray[1].GetAt(i)   - min_temp) *  y_gap));
		
			MemDC.LineTo((int)((i+1)*x_gap),(data_area_Rect.Height() - (m_TempArray[1].GetAt(i+1) - min_temp) *  y_gap));
			 
		}

		pen.DeleteObject(); 
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0xFF));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		for(int i = 0; i < m_TempArray[2].GetSize()-1; i++)
		{
			MemDC.MoveTo((int)(i*x_gap),    (data_area_Rect.Height() - (m_TempArray[2].GetAt(i)   - min_temp) *  y_gap));
		
			MemDC.LineTo((int)((i+1)*x_gap),(data_area_Rect.Height() - (m_TempArray[2].GetAt(i+1) - min_temp) *  y_gap));
			 
		}

		pen.DeleteObject(); 
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0xFF,0));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		for(int i = 0; i < m_TempArray[3].GetSize()-1; i++)
		{
			MemDC.MoveTo((int)(i*x_gap),    (data_area_Rect.Height() - (m_TempArray[3].GetAt(i)   - min_temp) *  y_gap));
		
			MemDC.LineTo((int)((i+1)*x_gap),(data_area_Rect.Height() - (m_TempArray[3].GetAt(i+1) - min_temp) *  y_gap));
			 
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



void CCurveDlg::OnExportTempFlat(CString filePath) 
{ 
	// TODO: Add your control notification handler code here 
	

	CString strFilter= _T("BMP文件(*.bmp)|(*.bmp)|| "); 
	CFileDialog m_cFileDlg(FALSE, 
	_T("bmp "), 
	_T("chart "), 
	OFN_OVERWRITEPROMPT, 
	_T("Save MSChart to BMP (*.bmp)|*.bmp|| "), this); 


	//获取MSCHART的图片 
	CDC* pChartDC; 

	// Get device context from MSChart 
	pChartDC = this->GetDC(); 

	
	// Get dimensions of MSChart 
	CRect mschartRect; 

	CWnd *data_area = GetDlgItem(IDC_MSCHART1);
	CRect data_area_Rect;
	data_area->GetWindowRect(mschartRect);

	ScreenToClient(&mschartRect);


	int mschartWidth	= mschartRect.Width(); 
	int mschartHeight	= mschartRect.Height(); 

	// Create CBitmap 
	CBitmap myBitmap; 

	// Create Compatible Bitmap for MSChart 
	myBitmap.CreateCompatibleBitmap( pChartDC, mschartWidth, mschartHeight); 

	// Define device-context object 
	CDC myCopy; 
	myCopy.CreateCompatibleDC( pChartDC ); 

	// Get pointer to object being replaced 
	myCopy.SelectObject( myBitmap ); 

	myCopy.BitBlt( 0, 0, mschartWidth, mschartHeight, pChartDC, mschartRect.left , mschartRect.top , SRCCOPY ); 

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