// testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "testDlg.h"
#include "switchdlg.h"

// CtestDlg 对话框

IMPLEMENT_DYNAMIC(CtestDlg, CDialog)

CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestDlg::IDD, pParent)
{

}

CtestDlg::~CtestDlg()
{
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CtestDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CtestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CtestDlg 消息处理程序

void CtestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	DrawLED_JiaJu();
}

unsigned char m_JiaJuTested[32];

void CtestDlg::DrawLED_JiaJu(void)
{
	int i,j;//k;
	CFile log_file;
	
	
	if(!this->IsWindowVisible()) 
	{
		return;
	}


	
	for(i = 0; i < 32; i++)
	{
	//	CheckJiaJuPass(i);
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
		AfxMessageBox(_T("DrawLED_JiaJu 1"));
		return;
	}
	
	
	CRect wndRect;
	
	
	this->GetClientRect(wndRect);
	nWidth	= wndRect.Width();
	nHeight = wndRect.Height() * 0.8 ;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		ReleaseDC(dc);
		AfxMessageBox(_T("DrawLED_JiaJu 2"));
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			
			ReleaseDC(dc);
			AfxMessageBox(_T("DrawLED_JiaJu 3"));
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		
		ReleaseDC(dc);
		bitmap.DeleteObject(); 
		AfxMessageBox(_T("DrawLED_JiaJu 4"));
	}

	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth  - PEN_WEIGHT *2) / 16 - Ellipse_W_Space;

	int Ellipse_H = (nHeight - PEN_WEIGHT *2) /  8 - Ellipse_W_Space;
	
	int Ellipse_W_GAP = Ellipse_W;

	

	int Ellipse_H_Space = 3;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	int G_MAX_LED_NR = 128;
	
	{
		MemDC.RoundRect(PEN_WEIGHT,PEN_WEIGHT,nWidth-PEN_WEIGHT*3 + 4,nHeight-PEN_WEIGHT*5 ,10,10);
	}

	MemDC.SelectObject(&graybrush);				//选择画刷

	for(i=0; i < G_MAX_LED_NR ; i++)
	{//默认的全部是灰色
		rect.left	= 5 + ((int)(i % 16))*(Ellipse_W_GAP	+	Ellipse_W_Space)	+	Ellipse_W_Space / 2;
		rect.top	= 5 + ((int)(i / 16))*(Ellipse_H		+	Ellipse_H_Space)	+	Ellipse_H_Space / 2;
		rect.right	= rect.left + Ellipse_W;
		rect.bottom = rect.top  + Ellipse_H;
		if(G_SwitchTestResult[0][i].IsClosing == false)
		{
			MemDC.Ellipse(rect);		//填充背景
		}
	
	}
	
//#endif


	MemDC.SelectObject(&redbrush);				//选择画刷
	//k = 0;
	{
		for(i=0; i< 16; i++)
		{
			for(j=0; j < 8; j++)
			{
				if((j * 16 + i) >= G_MAX_LED_NR)
				{
				}
				else
				{
					if(G_SwitchTestResult[0][ i + j * 16 ].IsClosing == true)
					{//闭合就是红色
						rect.left	=  5 + i * (Ellipse_W + Ellipse_W_Space);
						rect.top	=  5 + j * (Ellipse_H + Ellipse_H_Space);
						rect.right  = rect.left + Ellipse_W;
						rect.bottom = rect.top  + Ellipse_H;
						MemDC.Ellipse(rect);		//填充背景
					}
				}
			}
		}
	}



	for(i = 0; i < G_MAX_LED_NR; i++)
	{
		int id;
		if(i < 64)
		{
			id = (i % 16);
		}
		else
		{
			id = 16 + (i % 16);
		}

		CString nr;
		//if(m_JiaJuTested[id] ==  0)
		{
			
			nr.Format(_T("%02d"),i+1);
			
			
		}
		//else
		{
		//	nr = _T("√");
		
		}
		CSize size = MemDC.GetTextExtent(nr);
		int x = 5 + ((int)(i % 16)) *(Ellipse_W     + Ellipse_W_Space);
		int y = 5 + ((int)(i / 16)) *(Ellipse_H     + Ellipse_H_Space);
		MemDC.DrawText(nr,CRect(x,y + (Ellipse_H - size.cy) / 2 ,x+Ellipse_W,y+Ellipse_H), DT_CENTER /*| DT_VCENTER*/);
			
	}
	

	//dc->BitBlt(nWidth,data_area_Rect.bottom + PEN_WEIGHT * 5 ,nWidth ,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
	dc->BitBlt(0,PEN_WEIGHT * 5 ,nWidth ,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
	


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

	return;
  
}