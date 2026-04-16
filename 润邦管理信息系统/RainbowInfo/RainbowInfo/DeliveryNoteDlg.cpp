// DeliveryNoteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RainbowInfo.h"
#include "DeliveryNoteDlg.h"

#include "atlstr.h"
#include "string.h"
#include <atlimage.h>
#include "listDlg.h"
#include "InputDlg.h"
#include <commctrl.h>
#include <afxwin.h>
#include <afxcmn.h>

// CDeliveryNoteDlg 对话框

IMPLEMENT_DYNAMIC(CDeliveryNoteDlg, CDialog)

CDeliveryNoteDlg::CDeliveryNoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeliveryNoteDlg::IDD, pParent)
	, m_Production(_T(""))
	, m_Counts(0)
	, m_Price(0)
	, m_TotalPrice(0)
	, m_Memo(_T(""))
	, m_Receiver(_T(""))
	, m_Type(_T(""))
	, m_SheetNr(_T(""))
	, m_Unit(_T(""))
{
	// 报表样式+网格+整行选中
	

}

CDeliveryNoteDlg::~CDeliveryNoteDlg()
{
}

void CDeliveryNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Production);
	DDX_Text(pDX, IDC_EDIT_NUM, m_Counts);
	DDX_Text(pDX, IDC_EDIT_PRICE, m_Price);
	DDX_Text(pDX, IDC_EDIT_AMOUNT, m_TotalPrice);
	DDX_Text(pDX, IDC_EDIT_MEMO, m_Memo);
	DDX_Text(pDX, IDC_EDIT_RECEIVER, m_Receiver);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_Type);

	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_SHEET_NR, m_SheetNr);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimeCtrl);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_Unit);
}


BEGIN_MESSAGE_MAP(CDeliveryNoteDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDeliveryNoteDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDeliveryNoteDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CDeliveryNoteDlg::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDOK, &CDeliveryNoteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RECEIVER, &CDeliveryNoteDlg::OnBnClickedButtonReceiver)
//	ON_EN_CHANGE(IDC_EDIT_NUM, &CDeliveryNoteDlg::OnEnChangeEditNum)
//	ON_EN_CHANGE(IDC_EDIT_PRICE, &CDeliveryNoteDlg::OnEnChangeEditPrice)
	ON_EN_SETFOCUS(IDC_EDIT_NUM, &CDeliveryNoteDlg::OnEnSetfocusEditNum)
	ON_EN_SETFOCUS(IDC_EDIT_PRICE, &CDeliveryNoteDlg::OnEnSetfocusEditPrice)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCTION, &CDeliveryNoteDlg::OnBnClickedButtonProduction)
	ON_BN_CLICKED(IDC_BUTTON_TYPE, &CDeliveryNoteDlg::OnBnClickedButtonType)
	ON_BN_CLICKED(IDC_BUTTON_UNIT, &CDeliveryNoteDlg::OnBnClickedButtonUnit)
END_MESSAGE_MAP()


// CDeliveryNoteDlg 消息处理程序


void ZoomAndSaveImage(CString szSrcPath, CString szSavePath, double dZoom)
{
    USES_CONVERSION;

    // 1. 加载图片
    CImage srcImg;
    srcImg.Load(szSrcPath);

    int nW = srcImg.GetWidth();
    int nH = srcImg.GetHeight();

    // 2. 计算放大尺寸
    int newW =(int)(nW * dZoom);
    int newH = (int)(nH * dZoom);

    // 3. 创建目标画布
    CImage dstImg;
    dstImg.Create(newW, newH, 24);

    CDC* pDC = CDC::FromHandle(dstImg.GetDC());
    pDC->SetStretchBltMode(COLORONCOLOR);

    // 4. 放大绘制
    srcImg.StretchBlt(pDC->m_hDC, 0, 0, newW, newH, SRCCOPY);

    dstImg.ReleaseDC();
    srcImg.Destroy();

    // 5. 保存
    dstImg.Save(szSavePath);
    dstImg.Destroy();
}

void CDeliveryNoteDlg::DrawRect(CDC *dc,CRect rect)
{
	CPen pen(PS_SOLID,2,RGB(0,0,0));
	CPen *oldpen = dc->SelectObject(&pen);
	dc->MoveTo(rect.left,rect.top);
	dc->LineTo(rect.right,rect.top);
	dc->LineTo(rect.right,rect.bottom);
	dc->LineTo(rect.left,rect.bottom);
	dc->LineTo(rect.left,rect.top);
	dc->SelectObject(&oldpen);
}

void CDeliveryNoteDlg::DrawText(CDC *dc,CRect rect,CString text,UINT format)
{
	rect.left += 3;
	rect.right-= 3;
	rect.top  += 3;
	rect.right-= 3;
	dc->DrawText(text,rect,format);

}

extern CString theAppDirectory;

BOOL IsFileExist(LPCTSTR szPath)
{
    CFile file;
    // 只读打开、不创建
    if(file.Open(szPath, CFile::modeRead))
    {
        file.Close();
        return TRUE;
    }
    return FALSE;
}

void CDeliveryNoteDlg::PrintDeliveryNote(void)
{

    CPrintDialog prnDlg(true);//false);
	CString printer;
	CDC dc;
	
	
//	ZoomAndSaveImage(_T("d:\\1.bmp"),_T("d:\\2.bmp"),2.0f);

	
	//prnDlg.SetWindowTextW(_T("选择PDF打印机"));

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
	strTitle = _T("送货单");
	//this->GetWindowText(strTitle); 
	CString app_name = _T("热保护器测试记录");
	//AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += "[";
	app_name += strTitle;
	app_name += "]";

	CTime cur;
	this->m_DateTimeCtrl.GetTime(cur); // CTime::GetCurrentTime();
	CString s,s1;

	


	CString sDir;
	
	
	sDir = theAppDirectory;// +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	
	sDir += this->m_Receiver;

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	sDir += "\\";
	s.Format("%04d",cur.GetYear());
	sDir += s;
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	sDir += "\\";
	s.Format("%02d",cur.GetMonth());
	sDir += s;
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	sDir += "\\";
	s.Format("%02d",cur.GetDay());
	sDir += s;
	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	sDir += "\\";
	app_name = sDir + _T("送货单_");
	app_name += this->m_Receiver;
	app_name += "_";
	s.Format("%04d%02d%02d_SN_%s",cur.GetYear(),cur.GetMonth(),cur.GetDay(),this->m_SheetNr);
	app_name += s;
	app_name += ".pdf";

	if(IsFileExist(app_name))
	{
		if(AfxMessageBox("文件已经存在，需要覆盖码？",MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			return;
		}

	}
	docInfo.lpszDocName =   app_name;

	docInfo.lpszOutput  =   app_name;

	

	int index =dc.StartDoc(&docInfo);  

	int pagecx=dc.GetDeviceCaps(HORZRES);
    int pagecy=dc.GetDeviceCaps(VERTRES);


	{
		dc.StartPage();
		CRect temp_rect;

		

		CImage srcImg;
		srcImg.Load(_T("d:\\2.bmp"));
		
		int fontHeight;
		int x,y = 0;;
		CFont TempFont,*oldfont;
		int lines = 40;

		int per_line_height = pagecy / lines;

		fontHeight =  per_line_height * 2;

		TempFont.CreatePointFont(fontHeight,_T("黑体"),&dc);

		oldfont = dc.SelectObject(&TempFont); 
		CSize font_size = dc.GetTextExtent(m_Receiver);
		
		

	//	CBrush b(RGB(0,0x40,0x40));
	//	dc.FillRect(CRect(0,0,pagecx,pagecy),&b);
	//	b.DeleteObject();

		y += per_line_height * 2;
		temp_rect.left	= 0;
		temp_rect.right = pagecx;
		temp_rect.top	=	y;
		temp_rect.bottom=	temp_rect.top + per_line_height * 2;


		dc.DrawText("送 货 单",temp_rect, DT_CENTER | DT_VCENTER);


		TempFont.DeleteObject();
		fontHeight /= 2.5;
		TempFont.CreatePointFont(fontHeight - 10,_T("宋体"),&dc);
		oldfont = dc.SelectObject(&TempFont); 
		font_size = dc.GetTextExtent("A");

		
		
		per_line_height = font_size.cy + 6;
		
		int left_space = font_size.cx * 5;
		int line_chars = (pagecx - left_space)/ font_size.cx;
		int product_x  = left_space;

		int product_width = font_size.cx * (line_chars - 6)* 0.25;

		int type_x = product_x + product_width;
		int type_width = font_size.cx *  (line_chars - 6) * 0.15;

		int unit_x = type_x + type_width;
		int unit_width = font_size.cx * (line_chars - 6) * 0.05;

		int counts_x = unit_x + unit_width;
		int counts_width = font_size.cx * (line_chars - 6) * 0.05;

		int price_x = counts_x + counts_width;
		int price_width = font_size.cx * (line_chars - 6) * 0.15;

		int totalprice_x = price_x + price_width;
		int totalprice_width = font_size.cx * (line_chars - 6) * 0.15;

		int mem_x = totalprice_x + totalprice_width;
		int mem_width = font_size.cx * (line_chars - 6) * 0.20;



		y+= per_line_height * 4;
		s = "收货单位： " + this->m_Receiver;
		temp_rect.left	= left_space;
		temp_rect.right	= pagecx - left_space;
		temp_rect.top	=	y;
		temp_rect.bottom=	temp_rect.top +  per_line_height;
		dc.DrawText(s,temp_rect, DT_LEFT | DT_VCENTER);

		
		s.Format("发货时间： %d年%d月%d日  SN: %s",cur.GetYear(),cur.GetMonth(),cur.GetDay(),this->m_SheetNr);
		temp_rect.left	= left_space;
		temp_rect.right	= pagecx - left_space;
		temp_rect.top	=	y;
		temp_rect.bottom=	temp_rect.top +  per_line_height;
		dc.DrawText(s,temp_rect, DT_RIGHT | DT_VCENTER);
		
		y+= per_line_height * 2;
			temp_rect.left	= product_x;
			temp_rect.right = temp_rect.left + product_width;
			temp_rect.top	=	y + 2;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawRect(&dc,temp_rect);
			DrawText(&dc,temp_rect,"品名", DT_CENTER | DT_VCENTER);

			temp_rect.left	= type_x;
			temp_rect.right = temp_rect.left + type_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawText(&dc,temp_rect,"型号", DT_CENTER | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= unit_x;
			temp_rect.right = temp_rect.left + unit_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawText(&dc,temp_rect,"单位", DT_CENTER | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= counts_x;
			temp_rect.right = temp_rect.left + counts_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawText(&dc,temp_rect,"数量", DT_CENTER | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= price_x;
			temp_rect.right = temp_rect.left + price_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawText(&dc,temp_rect,"单价", DT_CENTER | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= totalprice_x;
			temp_rect.right = temp_rect.left + totalprice_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawText(&dc,temp_rect,"总价", DT_CENTER | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= mem_x;
			temp_rect.right = temp_rect.left + mem_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawText(&dc,temp_rect,"备注", DT_CENTER | DT_VCENTER);
			DrawRect(&dc,temp_rect);

		//内容
		for(int i = 0; i < this->m_ListCtrl.GetItemCount(); i++)
		{
			y+= per_line_height ;
			temp_rect.left	= product_x;
			temp_rect.right = temp_rect.left + product_width;
			temp_rect.top	=	y + 2;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			DrawRect(&dc,temp_rect);
			s = m_ListCtrl.GetItemText(i,0);
			DrawText(&dc,temp_rect,s, DT_LEFT | DT_VCENTER);

			temp_rect.left	= type_x;
			temp_rect.right = temp_rect.left + type_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			s = m_ListCtrl.GetItemText(i,1);
			DrawText(&dc,temp_rect,s, DT_LEFT | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= unit_x;
			temp_rect.right = temp_rect.left + unit_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			s = m_ListCtrl.GetItemText(i,2);
			DrawText(&dc,temp_rect,s, DT_LEFT | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= counts_x;
			temp_rect.right = temp_rect.left + counts_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			s = m_ListCtrl.GetItemText(i,3);
			DrawText(&dc,temp_rect,s, DT_RIGHT | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= price_x;
			temp_rect.right = temp_rect.left + price_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			s = m_ListCtrl.GetItemText(i,4);
			DrawText(&dc,temp_rect,s, DT_RIGHT | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= totalprice_x;
			temp_rect.right = temp_rect.left + totalprice_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			s = m_ListCtrl.GetItemText(i,5);
			DrawText(&dc,temp_rect,s, DT_RIGHT | DT_VCENTER);
			DrawRect(&dc,temp_rect);

			temp_rect.left	= mem_x;
			temp_rect.right = temp_rect.left + mem_width;
			temp_rect.top	=	y;
			temp_rect.bottom=	temp_rect.top + per_line_height ;
			s = m_ListCtrl.GetItemText(i,6);
			DrawText(&dc,temp_rect, s , DT_LEFT | DT_VCENTER);
			DrawRect(&dc,temp_rect);


			
		}
		

		y+= per_line_height ;
		y+= per_line_height ;
		temp_rect.left	= left_space;
		temp_rect.right = pagecx - left_space;
		temp_rect.top	=	y + 2;
		temp_rect.bottom=	temp_rect.top + per_line_height ;
		
		DrawText(&dc,temp_rect,"发货单位：常州市润邦电子科技有限公司  经手人：仇灿明 ", DT_LEFT | DT_VCENTER);

		srcImg.StretchBlt(dc.m_hDC, pagecx/2, y - per_line_height * 3, srcImg.GetWidth(), srcImg.GetHeight(), SRCAND/*SRCCOPY*/);

		
		dc.EndPage(); 
	}

	dc.EndDoc();

}


void CDeliveryNoteDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();

	if(this->m_Counts <= 0 || this->m_Price <= 0)
	{
		AfxMessageBox("数量，价格不能为 0");
		return;
	}
	GetUser();
	CString s,s1,s2;

	this->m_Receiver.Trim();
	bool same = false;

	if(m_Receiver.GetLength() > 0)
	{
		
		for(int i = 0; i < m_ReceiverArray.GetSize(); i++)
		{
			
			s = m_ReceiverArray.GetAt(i);
			s1 = m_Receiver + "\r\n";
			
			if(s.Compare(s1) == 0)
			{
				same = true;
				break;
			}
		}

		if(same == false)
		{
			CFile f;
			f.Open( "客户.txt",CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);

			f.SeekToEnd();

			f.Write(m_Receiver,m_Receiver.GetLength());
			f.Write("\r\n",2);
			f.Close();
		}
	}

	
	GetProduction();
	
	this->m_Production.Trim();
	if(m_Production.GetLength() > 0)
	{
		same = false;
		for(int i = 0; i < m_ProductionArray.GetSize(); i++)
		{
			
			s = m_ProductionArray.GetAt(i);
			s1 = m_Production + "\r\n";
			
			if(s.Compare(s1) == 0)
			{
				same = true;
				break;
			}
		}

		if(same == false)
		{
			CFile f;
			f.Open( "品名.txt",CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);

			f.SeekToEnd();

			f.Write(m_Production,m_Production.GetLength());
			f.Write("\r\n",2);
			f.Close();
		}
	}


	GetType();
	
	this->m_Type.Trim();
	if(m_Type.GetLength() > 0)
	{
		same = false;
		for(int i = 0; i < m_TypeArray.GetSize(); i++)
		{
			
			s = m_TypeArray.GetAt(i);
			s1 = m_Type + "\r\n";
			
			if(s.Compare(s1) == 0)
			{
				same = true;
				break;
			}
		}

		if(same == false)
		{
			CFile f;
			f.Open( "型号.txt",CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);

			f.SeekToEnd();

			f.Write(m_Type,m_Type.GetLength());
			f.Write("\r\n",2);
			f.Close();
		}
	}

	GetUnit();
	
	this->m_Unit.Trim();
	if(m_Unit.GetLength() > 0)
	{
		same = false;
		for(int i = 0; i < m_UnitArray.GetSize(); i++)
		{
			
			s = m_UnitArray.GetAt(i);
			s1 = m_Unit + "\r\n";
			
			if(s.Compare(s1) == 0)
			{
				same = true;
				break;
			}
		}

		if(same == false)
		{
			CFile f;
			f.Open( "单位.txt",CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);

			f.SeekToEnd();

			f.Write(m_Unit,m_Unit.GetLength());
			f.Write("\r\n",2);
			f.Close();
		}
	}

	
	// 插入行
	int i = m_ListCtrl.InsertItem(0, this->m_Production);
	m_ListCtrl.SetItemText(i,1,this->m_Type);
	m_ListCtrl.SetItemText(i,2,this->m_Unit);

	s.Format("%5d",this->m_Counts);
	m_ListCtrl.SetItemText(i,3,s);

	s.Format("%10.2f",this->m_Price);
	m_ListCtrl.SetItemText(i,4,s);

	s.Format("%10.2f",this->m_TotalPrice);
	m_ListCtrl.SetItemText(i,5,s);

	m_ListCtrl.SetItemText(i,6,this->m_Memo);



}

void CDeliveryNoteDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos = this->m_ListCtrl.GetFirstSelectedItemPosition();
	if(pos >= 0)
	{
		int nSelRow = m_ListCtrl.GetNextSelectedItem(pos);
		m_ListCtrl.DeleteItem(nSelRow);
	}

}

void CDeliveryNoteDlg::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	if(this->m_SheetNr.GetLength() == 0)
	{
		AfxMessageBox("请输入单号");
		return;
	}
	PrintDeliveryNote();
}

void CDeliveryNoteDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDeliveryNoteDlg::GetUser(void)
{
	m_ReceiverArray.RemoveAll();
	CFile f( _T("客户.txt"),CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	f.SeekToBegin();
	char buf[10];
	CString s_line;
	while(1)
	{
		if(f.Read(buf,sizeof(buf[0])) == 0)
		{
			break;
		}
		s_line += CString(buf[0]);
		if(buf[0] == '\n')
		{
			m_ReceiverArray.Add(s_line);
			s_line = "";
		}
	}

}

void CDeliveryNoteDlg::GetProduction(void)
{
	m_ProductionArray.RemoveAll();
	CFile f( _T("品名.txt"),CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	f.SeekToBegin();
	char buf[10];
	CString s_line;
	while(1)
	{
		if(f.Read(buf,sizeof(buf[0])) == 0)
		{
			break;
		}
		s_line += CString(buf[0]);
		if(buf[0] == '\n')
		{
			m_ProductionArray.Add(s_line);
			s_line = "";
		}
	}

}

void CDeliveryNoteDlg::GetType(void)
{
	m_TypeArray.RemoveAll();
	CFile f( _T("型号.txt"),CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	f.SeekToBegin();
	char buf[10];
	CString s_line;
	while(1)
	{
		if(f.Read(buf,sizeof(buf[0])) == 0)
		{
			break;
		}
		s_line += CString(buf[0]);
		if(buf[0] == '\n')
		{
			m_TypeArray.Add(s_line);
			s_line = "";
		}
	}

}

void CDeliveryNoteDlg::GetUnit(void)
{
	m_UnitArray.RemoveAll();
	CFile f( _T("单位.txt"),CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	f.SeekToBegin();
	char buf[10];
	CString s_line;
	while(1)
	{
		if(f.Read(buf,sizeof(buf[0])) == 0)
		{
			break;
		}
		s_line += CString(buf[0]);
		if(buf[0] == '\n')
		{
			m_UnitArray.Add(s_line);
			s_line = "";
		}
	}

}

void CDeliveryNoteDlg::OnBnClickedButtonReceiver()
{
	// TODO: 在此添加控件通知处理程序代码
	CListDlg dlg_list;

	dlg_list.m_Tiltle = "选择 客户";
	
	GetUser();

	dlg_list.m_ListItem.RemoveAll();
	dlg_list.m_ListItem.Append(this->m_ReceiverArray);
	if(dlg_list.DoModal() != IDOK)
	{
		return;
	}

	
	dlg_list.m_SelectString.Trim(); 
	if(dlg_list.m_SelectString.GetLength() > 0)
	{
		this->m_Receiver = dlg_list.m_SelectString;
		this->UpdateData(false);
		return;
	}

	
}

//void CDeliveryNoteDlg::OnEnChangeEditNum()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
//	// 发送该通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//
//	this->UpdateData();
//
//	this->m_TotalPrice = this->m_Price * this->m_Counts;
//
//	this->UpdateData(false);
//}

//void CDeliveryNoteDlg::OnEnChangeEditPrice()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
//	// 发送该通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//
//	
//}

void CDeliveryNoteDlg::OnEnSetfocusEditNum()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData();

	CWnd * w = this->GetDlgItem(IDC_EDIT_AMOUNT);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		this->m_Counts = _tcstod(dlg.m_Input,NULL);
		
		this->m_TotalPrice = this->m_Price * this->m_Counts;

		this->UpdateData(false);

	}
}

void CDeliveryNoteDlg::OnEnSetfocusEditPrice()
{
	// TODO: 在此添加控件通知处理程序代码

	CWnd * w = this->GetDlgItem(IDC_EDIT_AMOUNT);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		this->m_Price = _tcstod(dlg.m_Input,NULL);
		
		this->m_TotalPrice = this->m_Price * this->m_Counts;

		this->UpdateData(false);

	}
}

BOOL CDeliveryNoteDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	// 报表样式+网格+整行选中
	m_ListCtrl.ModifyStyle(0, LVS_REPORT|LVS_SHOWSELALWAYS);

	// 插入列
	m_ListCtrl.InsertColumn(0, _T("品名"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("型号"), 0, 100);
	m_ListCtrl.InsertColumn(2, _T("单位"), 0, 100);
	m_ListCtrl.InsertColumn(3, _T("数量"), 0, 60);
	m_ListCtrl.InsertColumn(4, _T("单价"), 0, 60);
	m_ListCtrl.InsertColumn(5, _T("总价"), 0, 60);
	m_ListCtrl.InsertColumn(6, _T("备注"), 0, 100);


	
	return true;
}
void CDeliveryNoteDlg::OnBnClickedButtonProduction()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData();

	CListDlg dlg_list;

	dlg_list.m_Tiltle = "选择 品名";
	
	GetProduction();

	dlg_list.m_ListItem.RemoveAll();
	dlg_list.m_ListItem.Append(this->m_ProductionArray);
	if(dlg_list.DoModal() != IDOK)
	{
		return;
	}

	
	dlg_list.m_SelectString.Trim(); 
	if(dlg_list.m_SelectString.GetLength() > 0)
	{
		this->m_Production = dlg_list.m_SelectString;
		this->UpdateData(false);
		return;
	}
}

void CDeliveryNoteDlg::OnBnClickedButtonType()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData();


	CListDlg dlg_list;

	dlg_list.m_Tiltle = "选择 型号";
	
	GetType();

	dlg_list.m_ListItem.RemoveAll();
	dlg_list.m_ListItem.Append(this->m_TypeArray);
	if(dlg_list.DoModal() != IDOK)
	{
		return;
	}

	
	dlg_list.m_SelectString.Trim(); 
	if(dlg_list.m_SelectString.GetLength() > 0)
	{
		this->m_Type = dlg_list.m_SelectString;
		this->UpdateData(false);
		return;
	}
}

void CDeliveryNoteDlg::OnBnClickedButtonUnit()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();


	CListDlg dlg_list;

	dlg_list.m_Tiltle = "选择 单位";
	
	GetUnit();

	dlg_list.m_ListItem.RemoveAll();
	dlg_list.m_ListItem.Append(this->m_UnitArray);
	if(dlg_list.DoModal() != IDOK)
	{
		return;
	}

	
	dlg_list.m_SelectString.Trim(); 
	if(dlg_list.m_SelectString.GetLength() > 0)
	{
		this->m_Unit = dlg_list.m_SelectString;
		this->UpdateData(false);
		return;
	}
}
