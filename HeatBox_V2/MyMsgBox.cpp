// MyMsgBox.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "MyMsgBox.h"


// CMyMsgBox 对话框

IMPLEMENT_DYNAMIC(CMyMsgBox, CDialog)

CMyMsgBox::CMyMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMyMsgBox::IDD, pParent)
	, m_Msg(_T(""))
{

}

CMyMsgBox::~CMyMsgBox()
{
}

void CMyMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_MSG, m_Msg);
}


BEGIN_MESSAGE_MAP(CMyMsgBox, CDialog)
	ON_BN_CLICKED(IDOK, &CMyMsgBox::OnBnClickedOk)
	
END_MESSAGE_MAP()


// CMyMsgBox 消息处理程序

void CMyMsgBox::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	SendMessage( WM_NCPAINT );
	//OnOK();
}


BOOL CMyMsgBox::OnInitDialog()
{

	CDialog::OnInitDialog();
	int i;
	
	
	this->SetWindowTextW(m_Title); 
	return true;
}

void CMyMsgBox::OnNcPaint(WPARAM,LPARAM)
{
// TODO: Add your message handler code here
CBitmap bmp;
bmp.LoadBitmap( IDB_BITMAP1 );

CWindowDC dc ( this );
CDC memDC;
memDC.CreateCompatibleDC( &dc );

CRect rect;
GetWindowRect( &rect );
memDC.SelectObject( &bmp );
dc.StretchBlt( 0, 0, rect.Width(), 25, &memDC, 0, 0, 10, 25, SRCCOPY );
// Do not call CDialog::OnNcPaint() for painting messages
}

/*
选择“WM_NCPAINT”消息，点击“添加处理程序 ”按钮，如下图所示：

确定后，在OnNcPaint()函数中添加如下代码：

// TODO: 在此处添加消息处理程序代码     CDC* pWinDC = GetWindowDC();     CRect rcTitle, rcWin;     GetWindowRect(rcWin);     rcTitle.left = 1;     rcTitle.top = 1;     rcTitle.right = rcWin.Width() - 2;     rcTitle.bottom = GetSystemMetrics(SM_CYSMCAPTION);     CBrush brbak = RGB(255, 0, 0);//将标题栏颜色设置为红色     pWinDC->SelectObject(&brbak);     pWinDC->FillRect(rcTitle, &brbak);     ReleaseDC(pWinDC);
*/