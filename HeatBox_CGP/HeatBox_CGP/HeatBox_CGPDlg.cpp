// HeatBox_CGPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "HeatBox_CGPDlg.h"
#include "TestParaDlg.h"
#include "SysConfigDlg.h"
#include "typedefs.h"
#include "comm.h"
#include "ViewTestRecordDlg.h"
#include "NumInputDlg.h"
#include <math.h>
#include "shlwapi.h"
#include "psapi.h"
#include "windows.h"
#include "ListDlg.h"
#include "scannerdlg.h"

#pragma  comment(lib,"shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib,"Psapi.lib")

int m_OnCtlColor = 0;
CHeatBox_CGPDlg		*G_MainDlg;
NORMAL_CONFIG_PARA	G_NormalConfigPara;
CTempRecordArray	G_TempRecordArray;

CString				m_Barcode[MAX_LED_NR];		//2023/5/15增加二维条码

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CScannerDlg G_ScannerDlg;

// CHeatBox_CGPDlg 对话框
unsigned char	HexBuf[64*1024*10];
int				HexBufLen;


BOOL FreeMyData(LPCTSTR ResName, LPCTSTR ResType, /*LPCTSTR FileName,*/ int Type,unsigned char *HexBuf,int *HexBufLen) 
{ 
	BOOL bRet = TRUE; 
	HINSTANCE hInst = AfxGetInstanceHandle();
	//先判断我们指定的资源是否存在 
	HRSRC hResInfo = FindResource(hInst,ResName,ResType);
	if(hResInfo == NULL) 
		return FALSE;
	HANDLE hFile = NULL; 
	DWORD dwWritten = 0;
	//开始调入指定的资源到内存 
	HGLOBAL hResData = LoadResource(hInst,hResInfo); 
	LPVOID pvResData = LockResource(hResData); 
	DWORD dwResSize = SizeofResource(hInst,hResInfo);

	*HexBufLen = dwResSize;
	memcpy(HexBuf,pvResData,dwResSize);
	/*
	//再创建目标文件并保存数据至文件 
	if(Type == 1)//系统加隐藏属性 如果指定类型为1,那么加上系统和隐藏属性,防止普通用户对文件进行破坏 
	{ 
		hFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM, NULL); 
		WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL); 
	} 
	else 
	{ 
		hFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL); 
		if(WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL)	== TRUE) 
		{
		//	AfxMessageBox(FileName);
		}
	}

	if(hFile == INVALID_HANDLE_VALUE)//创建文件失败 
	{ 
		FreeResource(hResData); 
		return FALSE; 
	}
	//释放有关内存资源 
	CloseHandle(hFile); 
	*/
	FreeResource(hResData);
	return TRUE; 
}

CString theAppDirectory;

int CheckLevelItem(TEST_PARA	TestPara, SWITCH_TEST_RESULT SwitchStatus, int TestPro)//int ID)
{
	if(SwitchStatus.IsUsed  == false)
		return 0;

	
	if(SwitchStatus.IsOpenFlash)
		return 0;

	if(SwitchStatus.IsCloseFlash)
		return 0;
	if(TestPara.HeatCoolType[TestPro] == 0)
	{//Heat  -->  Cool
		if(SwitchStatus.IsOpenned == true)
		{
			if(SwitchStatus.OpenTemp < TestPara.OpenTemp[TestPro]  - TestPara.OpenTempMinus[TestPro])
				return 0;


			if(SwitchStatus.OpenTemp > TestPara.OpenTemp[TestPro]  + TestPara.OpenTempPlus[TestPro])
				return 0;
		}
		

		if(SwitchStatus.OpenTemp  == INVALID_OPEN_TEMP)
		{
			return 0;
		}

		

		if((TestPara.OpenCloseGapLow[TestPro] != 0)  || (TestPara.OpenCloseGapHigh[TestPro] != 0))
		{

			float delta = SwitchStatus.OpenTemp - SwitchStatus.CloseTemp;
						
			if(SwitchStatus.IsClosed == true)
			{
				if(delta < TestPara.OpenCloseGapLow[TestPro])
					return 0;

				if(delta > TestPara.OpenCloseGapHigh[TestPro])
					return 0;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if((TestPara.CloseTempMinus[TestPro] == 0) && (TestPara.CloseTempPlus[TestPro] == 0))
			{
				return 1;
			}
		}
	}
	else
	{//Cool --> Heat
		if(SwitchStatus.IsOpenned == true)
		{
			
			if(SwitchStatus.OpenTemp < TestPara.OpenTemp[TestPro]  - TestPara.OpenTempMinus[TestPro])
				return 0;


			if(SwitchStatus.OpenTemp > TestPara.OpenTemp[TestPro]  + TestPara.OpenTempPlus[TestPro])
				return 0;
		}

		if(SwitchStatus.OpenTemp  == INVALID_OPEN_TEMP)
		{
			return 0;
		}

		if(SwitchStatus.IsClosed == true)
		{
			
			if( TestPara.OpenCloseGapLow[TestPro] != 0 && TestPara.OpenCloseGapHigh[TestPro] != 0)
			{

				float delta =  SwitchStatus.CloseTemp - SwitchStatus.OpenTemp;

			
				if(delta < TestPara.OpenCloseGapLow[TestPro])
					return 0;


				if(delta >  TestPara.OpenCloseGapHigh[TestPro])
					return 0;

			}
			else
			{
				if(SwitchStatus.CloseTemp < TestPara.CloseTemp[TestPro] - TestPara.CloseTempMinus[TestPro])
					return 0;

				if(SwitchStatus.CloseTemp > TestPara.CloseTemp[TestPro] + TestPara.CloseTempPlus[TestPro])
					return 0;

				
			}
		}
		else
		{
			return 0;
		}
	}
	return 1;
}
CBrush blackbrush(COLORREF(RGB(0,0,0)));
CBrush groundbrush(COLORREF(RGB(64,128,128)));
CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
CBrush graybrush(COLORREF(RGB(0,0,0)));
CBrush yellowbrush(COLORREF(RGB(0xFF,0xFF,0)));
CBrush greenbrush(COLORREF(RGB(0x0,0xFF,0)));
CBrush NotUsed_groundbrush(COLORREF(RGB(60,60,60)));

//CFont tempFont;
CFont DrawTempCurve_Font1,DrawTempCurve_Font2,DrawTempCurve_Font3,DrawTempCurve_Font4;
CFont DrawTempControlInfo_Font1,DrawTempControlInfo_Font2,DrawTempControlInfo_Font3;
CFont DrawAllItem_Font1;

CSize chnSize(0,0);
CDC * dc = NULL;
CDC		MemDC;
CPen	pen;
CBitmap	bitmap;

CHeatBox_CGPDlg::CHeatBox_CGPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeatBox_CGPDlg::IDD, pParent)
	, m_TestMsg(_T(""))
	
	, m_TestParaName(_T(""))
	, m_Productor(_T(""))
	, m_SwitchNr(0)
	, m_ProductNo(_T(""))
	, m_TestSpeed(0)
	, m_FlashGap(0)
	, m_EnterTestMode(_T(""))
	, m_KeepWarmMode(_T(""))
	, m_HeatOutPercent(0)
	, m_CoolOutPercent(0)
	, m_CurSv(0)
	, m_bShowTempCurve(false)
	
{
	m_DebugDlg = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_SwitchStatus,0,sizeof(m_SwitchStatus));
	m_TestMode = TEST_MODE_IDLE;
	m_TopTemp = 0;
	m_bFlashShow = false;

	m_CurTestPro	= 0;
	m_CurTestCycle	= 0;
	m_CurTemp		= 0.0f;
	m_Takeboard[0] = false;
	m_Takeboard[1] = false;
	m_Takeboard[2] = false;
	m_Takeboard[3] = false;
	m_Amp[0] = m_Amp[1] =m_Amp[2]  = 0.0f;
	m_Volt[0] = m_Volt[1] = m_Volt[2] = 0.0f;

	m_LastTestPro	= 0;
	m_ClickedButtonTestStart = false;

	m_CurScannerID = 0;
}


void CHeatBox_CGPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_BARCODE, m_ListBarcode);
}

BEGIN_MESSAGE_MAP(CHeatBox_CGPDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TEST_PARA, &CHeatBox_CGPDlg::OnBnClickedButtonTestPara)
	ON_BN_CLICKED(IDC_BUTTON_SYS_CONFIG, &CHeatBox_CGPDlg::OnBnClickedButtonSysConfig)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()

	ON_MESSAGE(COMM_MSG, OnComm)
	ON_MESSAGE(COMM_MSG+1, OnCommScanner)
	ON_BN_CLICKED(IDOK, &CHeatBox_CGPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TEST_READY, &CHeatBox_CGPDlg::OnBnClickedButtonTestReady)
	ON_BN_CLICKED(IDC_BUTTON_TEST_STOP, &CHeatBox_CGPDlg::OnBnClickedButtonTestStop)
	ON_BN_CLICKED(IDC_BUTTON_TEST_START, &CHeatBox_CGPDlg::OnBnClickedButtonTestStart)
	ON_BN_CLICKED(IDC_BUTTON_PRE_TEMP, &CHeatBox_CGPDlg::OnBnClickedButtonPreTemp)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CHeatBox_CGPDlg::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG, &CHeatBox_CGPDlg::OnBnClickedButtonDebug)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_ADJUST, &CHeatBox_CGPDlg::OnBnClickedButtonAdjust)
	ON_BN_CLICKED(IDC_BUTTON_TEST_NEXT, &CHeatBox_CGPDlg::OnBnClickedButtonTestNext)
	ON_BN_CLICKED(IDC_BUTTON_MY_LANGUAGE, &CHeatBox_CGPDlg::OnBnClickedButtonMyLanguage)
	ON_BN_CLICKED(IDC_BUTTON_DEL_BARCODE, &CHeatBox_CGPDlg::OnBnClickedButtonDelBarcode)
	ON_LBN_DBLCLK(IDC_LIST_BARCODE, &CHeatBox_CGPDlg::OnLbnDblclkListBarcode)
	ON_LBN_SELCHANGE(IDC_LIST_BARCODE, &CHeatBox_CGPDlg::OnLbnSelchangeListBarcode)
END_MESSAGE_MAP()


// CHeatBox_CGPDlg 消息处理程序

BOOL CHeatBox_CGPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	


	G_MainDlg = this;

	WriteLogFileEx(_T("开始") , __LINE__);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	
	

    this->ReadConfig();
	
	
	G_CommPara.comm = &m_Comm;
	G_CommPara.MessageID	=	COMM_MSG;
	G_CommPara.m_hWnd = this->GetSafeHwnd(); 
	
	int comm_port = G_NormalConfigPara.CommPort;	// AfxGetApp()->GetProfileInt(_T("SETTING"),_T("COMM PORT"),0); 
	m_Comm.OpenComm(comm_port+1,&G_CommPara);

	G_CommParaScanner.comm = &m_CommScanner;
	G_CommParaScanner.MessageID	=	COMM_MSG + 1;
	G_CommParaScanner.m_hWnd = this->GetSafeHwnd(); 
	
	comm_port = G_NormalConfigPara.CommPort;	// AfxGetApp()->GetProfileInt(_T("SETTING"),_T("COMM PORT"),0); 
	m_CommScanner.OpenCommScanner(comm_port+2,&G_CommParaScanner);

	SelectLanguage();

	

	this->SetTimer(1,(m_TestPara.FlashGap  + 1) *100 ,NULL);

	//this->SetTimer(1,G_NormalConfigPara.CommSpeedTimer ,NULL); 

	this->ShowWindow(SW_SHOWMAXIMIZED); 

	this->PrepareDraw(); 

#ifdef _DEBUG 	
	this->m_SwitchStatus[0][0].IsUsed = true;
	this->m_SwitchStatus[0][0].IsOpenned = true;
	this->m_SwitchStatus[0][0].IsClosed  = true;
	this->m_SwitchStatus[0][0].CloseTemp = 67.67f;
	this->m_SwitchStatus[0][0].OpenTemp  = 74.0f;
	this->m_SwitchStatus[0][0].IsClosing = true;
	this->m_SwitchStatus[0][0].IsOpenFlash = false;
	
	this->m_SwitchStatus[0][1].IsUsed = true;
	this->m_SwitchStatus[0][1].IsOpenned = false;
	this->m_SwitchStatus[0][1].IsClosed  = true;
	this->m_SwitchStatus[0][1].CloseTemp = 70.0f;
	this->m_SwitchStatus[0][1].OpenTemp  = INVALID_OPEN_TEMP;//79.0f;
	this->m_SwitchStatus[0][1].IsOpenFlash =false;

	this->m_SwitchStatus[0][2].IsUsed = true;
	this->m_SwitchStatus[0][2].IsOpenned = true;
	this->m_SwitchStatus[0][2].IsClosed  = true;
	this->m_SwitchStatus[0][2].CloseTemp = 69.0f;
	this->m_SwitchStatus[0][2].OpenTemp  = 80.0f;
	this->m_SwitchStatus[0][2].IsOpenFlash =false;
	

	this->m_SwitchStatus[0][10].IsUsed = true;
	this->m_SwitchStatus[0][10].IsOpenned = false;
	this->m_SwitchStatus[0][10].IsClosed  = false;
	this->m_SwitchStatus[0][10].CloseTemp = 70.67f;
	this->m_SwitchStatus[0][10].OpenTemp  = 101.0f;
	this->m_SwitchStatus[0][10].IsClosing = true;
	this->m_SwitchStatus[0][10].IsOpenFlash = false;


	this->m_SwitchStatus[0][74].IsUsed = true;
	this->m_SwitchStatus[0][74].IsOpenned = true;
	this->m_SwitchStatus[0][74].IsClosed  = true;
	this->m_SwitchStatus[0][74].CloseTemp = 70.67f;
	this->m_SwitchStatus[0][74].OpenTemp  = 91.0f;
	this->m_SwitchStatus[0][74].IsClosing = true;
	this->m_SwitchStatus[0][74].IsOpenFlash = true;
	
	this->m_SwitchStatus[0][75].IsUsed = true;
	this->m_SwitchStatus[0][75].IsOpenned = true;
	this->m_SwitchStatus[0][75].IsClosed  = true;
	this->m_SwitchStatus[0][75].CloseTemp = 70.0f;
	this->m_SwitchStatus[0][75].OpenTemp  = 92.0f;
	this->m_SwitchStatus[0][75].IsOpenFlash =false;

	this->m_SwitchStatus[0][76].IsUsed = true;
	this->m_SwitchStatus[0][76].IsOpenned = true;
	this->m_SwitchStatus[0][76].IsClosed  = true;
	this->m_SwitchStatus[0][76].CloseTemp = 69.0f;
	this->m_SwitchStatus[0][76].OpenTemp  = 93.0f;
	this->m_SwitchStatus[0][76].IsOpenFlash =false;
#endif

	this->SendCmdReadAdjustAll(); 

	
	m_background_hbr = CreateSolidBrush(RGB(80,180,0));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHeatBox_CGPDlg::OnPaint()
{
	static int i = 0;
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	if(i == 0)
	{
#ifdef _VoltAmp_
	for(int i = 0; i < 48; i++)
#else

	int draw_items;
	if(m_SwitchNr > 64)
	{
		draw_items = 64;
	}
	else
	{
		draw_items = m_SwitchNr;
	}
	for(int i = 0; i < draw_items; i++)
#endif
	{
//		DrawItem(i);
	}

	for(int i = 0; i < 6; i++)
	{
		DrawVoltAmp(i);
	}

	DrawTempCurve();

	DrawTempControlInfo();
	}

	this->DrawAllItem(64);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHeatBox_CGPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHeatBox_CGPDlg::OnBnClickedButtonTestPara()
{
	// TODO: 在此添加控件通知处理程序代码
/*
	m_SwitchStatus[0][0].IsUsed = true;
	m_SwitchStatus[0][0].IsOpenned = true;
	m_SwitchStatus[0][0].IsClosed = true;
	m_SwitchStatus[0][0].OpenTemp = -20.0f;
	m_SwitchStatus[0][0].CloseTemp = -13.0f;
	this->SaveTestResult();
	return;
*/
	CTestParaDlg dlg;
	dlg.DoModal();
	
	this->ReadConfig();

	DrawTempCurve();

}

 

void CHeatBox_CGPDlg::DrawAllItem(int TotalCount)
{

	CFile log_file;



	if(!this->IsWindowVisible()) 
	{
		return;
	}
	


	 CBitmap	* pOldBitmap	= NULL;
	 CBrush		* oldbrush		= NULL;
	 CPen		* oldpen		= NULL;
	 CFont		*oldFont		= NULL;

	 CRect rect(0,00,70,52);
	 
	int nWidth;
    int nHeight;

	
	CWnd *data_area = GetDlgItem(IDC_LIST_BARCODE);//IDOK);
	CRect wndRect,data_area_Rect;
	data_area->GetWindowRect(data_area_Rect);

	ScreenToClient(&data_area_Rect); 

	this->GetClientRect(wndRect);

	nWidth	= (wndRect.right - wndRect.left);
	nHeight = wndRect.bottom - data_area_Rect.bottom;
	
	

	
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境
	
	

#define PEN_WEIGHT	2
	pen.DeleteObject(); 
	if(!pen.GetSafeHandle())
	{
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	}
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	

	
	int item_Space			=	2;
	int item_NR_PER_LINE	=	8;
	int	item_LINES			=	8;

	int item_W = nWidth  / item_NR_PER_LINE - item_Space;
	int item_H = nHeight / item_LINES       - item_Space;//Ellipse_W;
	

	oldFont = MemDC.SelectObject(&DrawAllItem_Font1);

	for(int ID = 0; ID < 64; ID++)
	{

	//background
	if(m_TestPara.IsDoubleSwitch)
	{
		if(CheckLevelItem(this->m_TestPara,this->m_SwitchStatus[m_LastTestPro][ID],m_LastTestPro ) == 0
			|| CheckLevelItem(this->m_TestPara,this->m_SwitchStatus[m_LastTestPro][ID + 64],m_LastTestPro ) == 0)
		{
			oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
		}
		else 
		{
			oldbrush=MemDC.SelectObject(&NotUsed_groundbrush);			//选择画刷
		}
	}
	else
	{
		if(CheckLevelItem(this->m_TestPara,this->m_SwitchStatus[m_LastTestPro][ID],m_LastTestPro ) == 0)
		{
			oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
		}
		else 
		{
			oldbrush=MemDC.SelectObject(&NotUsed_groundbrush);			//选择画刷
		}
	}

	


	MemDC.Rectangle(0,0, item_W,item_H);

	MemDC.SetBkColor(RGB(0,0,0));
	MemDC.SetTextColor(RGB(255,255,255));
	CString s;
	s.Format(_T("%02d "),ID+1);
	
	MemDC.TextOut(0,0,s);
	if(m_SwitchStatus[m_LastTestPro][ID].IsUsed)
	{
		//CFont tempFont;
		
		CString s;
		
		

		

		MemDC.SetBkMode(TRANSPARENT);
		if(((m_SwitchStatus[m_LastTestPro][ID].IsOpenFlash || m_SwitchStatus[m_LastTestPro][ID].IsCloseFlash) && m_bFlashShow)
			 || ((m_TestPara.IsDoubleSwitch) && (m_SwitchStatus[m_LastTestPro][ID + 64].IsOpenFlash || m_SwitchStatus[m_LastTestPro][ID + 64].IsCloseFlash) && m_bFlashShow))
		{
		}
		else
		{
			//动作温度
			if(m_TestPara.IsDoubleSwitch)
			{
					if(m_SwitchStatus[m_LastTestPro][ID].IsOpenned && m_SwitchStatus[m_LastTestPro][ID + 64].IsOpenned )
					{
						if(m_SwitchStatus[m_LastTestPro][ID].OpenTemp == m_SwitchStatus[m_LastTestPro][ID + 64].OpenTemp)
						{
							s.Format(_T("% 5.2f"),m_SwitchStatus[m_LastTestPro][ID].OpenTemp);  
							if(m_SwitchStatus[m_LastTestPro][ID].OpenTemp >= m_TestPara.OpenTemp[m_LastTestPro]  - m_TestPara.OpenTempMinus[m_LastTestPro]
								&& m_SwitchStatus[m_LastTestPro][ID].OpenTemp <= m_TestPara.OpenTemp[m_LastTestPro] + m_TestPara.OpenTempPlus[m_LastTestPro])
							{
								MemDC.SetTextColor(RGB(0,255,0));
							}
							else
							{
								MemDC.SetTextColor(RGB(255,0,0));
							}
						}
						else
						{
							float OpenTemp = m_SwitchStatus[m_LastTestPro][ID].OpenTemp;

							if(OpenTemp > m_SwitchStatus[m_LastTestPro][ID+64].OpenTemp)
							{
								OpenTemp = m_SwitchStatus[m_LastTestPro][ID+64].OpenTemp;
							}

							s.Format(_T("% 5.2f"),OpenTemp);  
							if(OpenTemp >= m_TestPara.OpenTemp[m_LastTestPro]  - m_TestPara.OpenTempMinus[m_LastTestPro]
								&& OpenTemp <= m_TestPara.OpenTemp[m_LastTestPro] + m_TestPara.OpenTempPlus[m_LastTestPro])
							{
								MemDC.SetTextColor(RGB(0,255,0));
							}
							else
							{
								MemDC.SetTextColor(RGB(255,0,0));
							}
						}
						
						
						chnSize = MemDC.GetTextExtent(s);

						MemDC.TextOut(item_W * 0.6 - chnSize.cx  ,(item_H /2 - chnSize.cy)  / 2 ,s);
					}
					else if(m_SwitchStatus[m_LastTestPro][ID].IsOpenned)
					{
						s.Format(_T("% 5.2f"),m_SwitchStatus[m_LastTestPro][ID].OpenTemp);  
						if(m_SwitchStatus[m_LastTestPro][ID].OpenTemp >= m_TestPara.OpenTemp[m_LastTestPro]  - m_TestPara.OpenTempMinus[m_LastTestPro]
							&& m_SwitchStatus[m_LastTestPro][ID].OpenTemp <= m_TestPara.OpenTemp[m_LastTestPro] + m_TestPara.OpenTempPlus[m_LastTestPro])
						{
							MemDC.SetTextColor(RGB(0,255,0));
						}
						else
						{
							MemDC.SetTextColor(RGB(255,0,0));
						}
						chnSize = MemDC.GetTextExtent(s);

						MemDC.TextOut(item_W * 0.6 - chnSize.cx  ,(item_H /2 - chnSize.cy)  / 2 ,s);
					}
					else if(m_SwitchStatus[m_LastTestPro][ID + 64].IsOpenned)
					{
						s.Format(_T("% 5.2f"),m_SwitchStatus[m_LastTestPro][ID + 64].OpenTemp);  
						if(m_SwitchStatus[m_LastTestPro][ID + 64].OpenTemp >= m_TestPara.OpenTemp[m_LastTestPro]  - m_TestPara.OpenTempMinus[m_LastTestPro]
							&& m_SwitchStatus[m_LastTestPro][ID + 64].OpenTemp <= m_TestPara.OpenTemp[m_LastTestPro] + m_TestPara.OpenTempPlus[m_LastTestPro])
						{
							MemDC.SetTextColor(RGB(0,255,0));
						}
						else
						{
							MemDC.SetTextColor(RGB(255,0,0));
						}
						chnSize = MemDC.GetTextExtent(s);

						MemDC.TextOut(item_W * 0.6 - chnSize.cx  ,(item_H /2 - chnSize.cy)  / 2 ,s);
					}
			}
			else
			{
			
				if(m_SwitchStatus[m_LastTestPro][ID].IsOpenned)
				{

					s.Format(_T("% 5.2f"),m_SwitchStatus[m_LastTestPro][ID].OpenTemp);  
					
					if(m_SwitchStatus[m_LastTestPro][ID].OpenTemp >= m_TestPara.OpenTemp[m_LastTestPro]  - m_TestPara.OpenTempMinus[m_LastTestPro]
						&& m_SwitchStatus[m_LastTestPro][ID].OpenTemp <= m_TestPara.OpenTemp[m_LastTestPro] + m_TestPara.OpenTempPlus[m_LastTestPro])
					{
						MemDC.SetTextColor(RGB(0,255,0));
					}
					else
					{
						MemDC.SetTextColor(RGB(255,0,0));
					}
					chnSize = MemDC.GetTextExtent(s);
					//MemDC.TextOut(item_W/ 8,0,s);
					MemDC.TextOut(item_W * 0.6 - chnSize.cx  ,(item_H /2 - chnSize.cy)  / 2 ,s);
				}
			}


			//复位温度
			if(m_SwitchStatus[m_LastTestPro][ID].IsClosed)
			{
				s.Format(_T("% 5.2f"),m_SwitchStatus[m_LastTestPro][ID].CloseTemp );  
				MemDC.SetTextColor(RGB(0,255,0));
				chnSize = MemDC.GetTextExtent(s);
				MemDC.TextOut(item_W * 0.6 - chnSize.cx,item_H * 5 / 10,s);
				
			}
			
			//if(this->m_SwitchNr <= 64)
			{
				//动作复位温度差
				if(m_SwitchStatus[m_LastTestPro][ID].IsOpenned && m_SwitchStatus[m_LastTestPro][ID].IsClosed)
				{
					float delta = m_SwitchStatus[m_LastTestPro][ID].OpenTemp - m_SwitchStatus[m_LastTestPro][ID].CloseTemp;
					s.Format(_T("% 5.2f"),delta);  
					if(delta >= m_TestPara.OpenCloseGapLow[m_LastTestPro] && delta <= m_TestPara.OpenCloseGapHigh[m_LastTestPro])
					{
						MemDC.SetTextColor(RGB(0,255,0));
					}
					else
					{
						MemDC.SetTextColor(RGB(255,0,0));
					}
					chnSize = MemDC.GetTextExtent(s);
					MemDC.TextOut((item_W - 2 - chnSize.cx ),item_H/2 - chnSize.cy / 2 ,s);
				}
			}
		}
	}
 
	if(this->m_SwitchStatus[m_LastTestPro][ID].IsClosing)
	{
		oldbrush=MemDC.SelectObject(&redbrush);
	}
	else
	{
		oldbrush=MemDC.SelectObject(&graybrush);
	}
	
	CRect led_rect;
	led_rect.left= item_Space;
	led_rect.right = item_W / 8;
	led_rect.top   = item_H / 3;
	led_rect.bottom = led_rect.top + (led_rect.right - led_rect.left);
	MemDC.Ellipse(led_rect);

	if(this->m_TestPara.IsDoubleSwitch)
	{
		if(this->m_SwitchStatus[m_LastTestPro][ID + 64].IsClosing)
		{
			oldbrush=MemDC.SelectObject(&greenbrush);
		}
		else
		{
			oldbrush=MemDC.SelectObject(&graybrush);
		}
	
		led_rect.left= item_Space;
		led_rect.right = item_W / 8;
		led_rect.top = item_H * 2/ 3;
		led_rect.bottom = led_rect.top + (led_rect.right - led_rect.left);
		MemDC.Ellipse(led_rect);
    }

	dc->BitBlt((item_W + item_Space)*(ID % item_NR_PER_LINE),data_area_Rect.bottom + item_Space +(item_H + item_Space) * (ID / item_LINES) ,item_W ,item_H,&MemDC,0,0,SRCCOPY); 	//显示原图形
	
	}


	if(oldbrush != NULL)
	{
		MemDC.SelectObject(oldbrush);
	}
	if(pOldBitmap != NULL)
	{
		MemDC.SelectObject(pOldBitmap);
	}
	if(oldpen != NULL)
	{
		MemDC.SelectObject(oldpen);
	}
	if(oldFont != NULL)
	{
		MemDC.SelectObject(oldFont);
	}
	
	
}
void CHeatBox_CGPDlg::OnBnClickedButtonSysConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonLanguage();
	CSysConfigDlg dlg;
	dlg.DoModal();
}

HBRUSH CHeatBox_CGPDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	m_OnCtlColor ++;

	return m_background_hbr;

	/*static HBRUSH new_hbr = CreateSolidBrush(RGB(80,180,0));
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	

	m_OnCtlColor ++;

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_BUTTON_TEST_PARA:
		//return ::CreateSolidBrush(RGB(0xFF,0,0));
		break;

		case IDC_BUTTON_TEST_READY:
		//	pDC->SetTextColor(RGB(255,0,0));
		//	pDC->SetBkMode(TRANSPARENT);
		//	pDC->SetBkColor( RGB(255,0,0));
		break;
	}
	return hbr;//::CreateSolidBrush(RGB(80,180,0));
	*/
}

UINT CHeatBox_CGPDlg::GetUsedMemory(void)
{
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	
	SYSTEM_INFO si;
    GetSystemInfo(&si);

	PROCESS_MEMORY_COUNTERS pmc;
   	DWORD pid		= GetCurrentProcessId();
	HANDLE handle	= ::GetCurrentProcess(); 
	GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));

	int usedMemory = 0;

    PSAPI_WORKING_SET_INFORMATION workSet;
    memset(&workSet, 0, sizeof(workSet));
    BOOL bOk = QueryWorkingSet(handle, &workSet, sizeof(workSet));
    if (bOk || (!bOk && GetLastError() == ERROR_BAD_LENGTH))
    {
        int nSize = sizeof(workSet.NumberOfEntries) + workSet.NumberOfEntries*sizeof(workSet.WorkingSetInfo);
        char* pBuf = new char[nSize];
        if (pBuf)
        {
            QueryWorkingSet(handle, pBuf, nSize);
            PSAPI_WORKING_SET_BLOCK* pFirst = (PSAPI_WORKING_SET_BLOCK*)(pBuf + sizeof(workSet.NumberOfEntries));
            DWORD dwMem = 0;
            for (ULONG_PTR nMemEntryCnt = 0; nMemEntryCnt < workSet.NumberOfEntries; nMemEntryCnt++, pFirst++)
            {
                if (pFirst->Shared == 0)
                {
                    dwMem += si.dwPageSize;
                }
            }
            delete pBuf;
            pBuf = NULL;
            if (workSet.NumberOfEntries > 0)
            {
                usedMemory = dwMem / (1024);
            }
        }
    }

	return usedMemory;
}

void CHeatBox_CGPDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if(this->m_NewBarcode.GetLength() >= 42)
	{
		m_NewBarcode = m_NewBarcode.Right(42);

		m_Barcode[m_CurScannerID] = m_NewBarcode;
		m_NewBarcode = _T("");
		
		

			m_CurScannerID ++;
			if(m_CurScannerID >= 60)
			{
				m_CurScannerID = 0;
			}
		

		ShowBarcode();

		m_ListBarcode.SetCurSel(m_CurScannerID);
	}

	
	if(this->m_SendmCmdProgram.GetCount() > 0)
	{
		MODBUS_RS485_CMD_PROGRAM cmd = m_SendmCmdProgram.GetAt(0); 
		this->m_Comm.WriteComm((char*)&cmd,sizeof(cmd));
		//this->m_DebugDlg[m_MeterID % G_CurMaxHeatBox].m_CalMsg.Format(_T("发送命令 %X"),cmd.cmd);
		CString s = _T("");
		for(int i = 0; i < sizeof(cmd); i++)
		{
			CString s1;
			s1.Format(_T("%02X "),((unsigned char*)&cmd)[i]);
			s+=s1;
			
		}
		if(m_DebugDlg != NULL)
		{
			if(m_DebugDlg->IsWindowVisible ())
			{
				//m_DebugDlg.m_CommSendString = s;
				CWnd * w = m_DebugDlg->GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE);
				if(m_SendmCmdProgram.GetCount() == 0)
				{
					w->SetWindowTextW(_T("更新线路板程序"));
				}
				else
				{
					s.Format(_T("更新线路板程序...%d"), m_SendmCmdProgram.GetCount());
					w->SetWindowTextW(s);
				}
				
			}
		}
		return;
	}

	static int m_MeterID = 0;
	MODBUS_RS485_CMD cmd;



	CString s;
	CTime cur = CTime::GetCurrentTime();


	if(m_MeterID == 0)
	{
#ifdef _VoltAmp_
		m_MeterID = 1;
#endif
		
		m_bFlashShow = !m_bFlashShow;

		cmd.RS485_Addr =  '1';
		cmd.cmd = CMD_READ_TEMP;
		cmd.para_len = sizeof(cmd.para);
		
		memset(cmd.para,0,sizeof(cmd.para));
	//	WorkStatus = (WORK_STATUS *)cmd.para;		
	//	WorkStatus->TestMode = this->m_TestMode;
	//	WorkStatus->TestSpeed = this->m_TestSpeed;

		CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,cmd.crc); 
	}
	else
	{
		


		m_MeterID = 0;
		cmd.RS485_Addr =  'A';
		cmd.cmd = CMD_READ_TEMP;
		cmd.para_len = sizeof(cmd.para);
		
		memset(cmd.para,0,sizeof(cmd.para));
		
		 CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,cmd.crc ); 
	}

	if(m_SendCmdArray.GetSize() == 0)
	{	
		m_CommTimeOut = 0;
		
		if(m_Comm.IsOpened())
		{
			if(m_DebugDlg != NULL)
			{
				if(m_DebugDlg->IsWindowVisible ())
				{
					CWnd *w = m_DebugDlg->GetDlgItem(IDC_EDIT_SEND_CMD);
					CString s;
					s.Format(_T("发送：%X"),cmd.cmd);
					w->SetWindowTextW(s);
				}
			}
			//m_SendCmdArray.Add(cmd); 
			this->m_Comm.WriteComm((char*)&cmd,sizeof(cmd));
		}
	}
	else
	{
		if(m_Comm.IsOpened())
		{
			
			if(m_SendCmdArray.GetSize() > 0) 
			{
				m_CommTimeOut++;

				cmd = m_SendCmdArray.GetAt(0);
				if(m_DebugDlg != NULL)
				{
					if(m_DebugDlg->IsWindowVisible ())
					{
						CWnd *w = m_DebugDlg->GetDlgItem(IDC_EDIT_SEND_CMD);
						CString s;
						s.Format(_T("发送：%X"),cmd.cmd);
						w->SetWindowTextW(s);
					}
				}
				
				this->m_Comm.WriteComm((char*)&cmd,sizeof(cmd));
			}
		}
	}

	

	m_UsedMem = GetUsedMemory();
		
	CString m_CompileTime;
	m_CompileTime = __DATE__;
	m_CompileTime += _T(" ");
	m_CompileTime += __TIME__;

	s.Format(_T("CGP 温度开关测试系统                       %04d/%02d/%02d  %02d:%02d:%02d  发送：%6d 命令：0x%X  接收：%6d , 丢失:%d        内存%dKB,  等待命令数%d"),
		cur.GetYear(), cur.GetMonth(),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond() , m_Comm.m_WriteCommCount , cmd.cmd , m_Comm.m_ReadCommCount , m_Comm.m_WriteCommCount - m_Comm.m_ReadCommCount, m_UsedMem,m_SendCmdArray.GetCount()); 
	this->SetWindowTextW(_T("软件编译时间:") + m_CompileTime + _T(" ") + s);  

	CDialog::OnTimer(nIDEvent);
}

CString GetPcbModeText(int mode)
{
	CString s = _T("无定义");
	switch(mode)
	{
	case SUB_MODE_STOP:
		s = _T("停止");
		break;
	case SUB_MODE_DIRECT_HEAT:
		s = _T("全速加热");
		break;
	case SUB_MODE_NORMAL_PID:
		s = _T("恒温");
		break;
	case SUB_MODE_INCREASE_PID:
		s = _T("线性升温");
		break;
	case SUB_MODE_DIRECT_COOL:
		s = _T("全速降温");
		break;
	case SUB_MODE_PID_COOL:
		s = _T("全速制冷");
		break;
	case SUB_MODE_STEP_PID_COOL:
		s = _T("线性降温");
		break;
	case SUB_MODE_AT:
		s = _T("自整定");
		break;
	case SUB_MODE_COOL_AT:
		s = _T("冷自整定");
		break;
	case SUB_MODE_OUTSIDE_AIR_HEAT:
		s = _T("外部空气升温");
		break;
	case SUB_MODE_PERCENT_HEAT_WITH_AIR:
		s = _T("外部空气比例升温");
		break;
	};
	return s;
};


LRESULT CHeatBox_CGPDlg::OnCommScanner(WPARAM wParam,LPARAM lParam)
{
	int i,source_id,data_len;
	
	char rec_cmd[1024];

	
	//WriteLogFileEx(__FILE__ , __LINE__);

	data_len = lParam;
	
	memset(rec_cmd,0,sizeof(rec_cmd));
	memcpy(rec_cmd,(char*)wParam,lParam);

	CString s,s1;

	
		s.Format(_T("len = %d,"),data_len);
	//	AfxMessageBox(s);

	for(int i = 0; i < 42;i++)
	{
		s1.Format(_T("%c"),rec_cmd[i]);
		s+=s1;
	}
	//AfxMessageBox(s);

	if(rec_cmd[0] != 'A'
		|| rec_cmd[1] != '0'
		|| rec_cmd[2] != '0'
		|| rec_cmd[3] != '0'
		|| rec_cmd[4] != '0'
		|| rec_cmd[5] != '1'
		|| rec_cmd[6] != '0'
		|| rec_cmd[7] != '7'

		|| rec_cmd[39] != 'A'
		|| rec_cmd[40] != '0'
		|| rec_cmd[41] != '1'
		)
	{
		return 0;
	}
	

	if(rec_cmd[8] != 0x7C
		|| rec_cmd[23] != 0x7C
		|| rec_cmd[38] != 0x7C)
	{
		
		return 0;
	}

	

	s= _T("");
	if(data_len == 42)
	{
		for(int i = 0; i < data_len; i ++)
		{
			if(rec_cmd[i] >= '0' && rec_cmd[i]<='9')
			{
				s1.Format(_T("%c"),rec_cmd[i]);
				s+=s1;
			}
			else if(rec_cmd[i] >= 'A' && rec_cmd[i]<='Z')
			{
				s1.Format(_T("%c"),rec_cmd[i]);
				s+=s1;
			}
			else if(rec_cmd[i] == 0x7C )
			{
				s1.Format(_T("%c"),rec_cmd[i]);
				s+=s1;
			}
			
			
		}
		if(s.GetLength() == 42)
		{
			m_NewBarcode = s;
		}
		else
		{
		//	AfxMessageBox(s);
		//	s.Format(_T("%d"),s.GetLength());
		//	AfxMessageBox(s);
		}
	}
	else
	{
		//AfxMessageBox(_T("OnCommScanner return 2"));
	}
	
	//AfxMessageBox(m_NewBarcode);

	return 0;
}
LRESULT CHeatBox_CGPDlg::OnComm(WPARAM wParam,LPARAM lParam)
{
	
	this->m_Comm.m_ReadCommCount ++;



	int i,source_id,data_len;
	CTime t;
	t=t.GetCurrentTime(); 
	char rec_cmd[1024];

	//WriteLogFileEx(__FILE__ , __LINE__);

	data_len = lParam;
	
	memset(rec_cmd,0,sizeof(rec_cmd));
	memcpy(rec_cmd,(char*)wParam,lParam);
	


	
	MODBUS_RS485_CMD_ANSWER *answer;
	
	answer = (MODBUS_RS485_CMD_ANSWER*)rec_cmd;
			

	//箱子的编号只能是1到9
	switch(answer->RS485_Addr)
	{
	case '1':
    case 'A':
		source_id = 0;
		break;

	
	default:
		return 0;
	}
		CString s;
//分析具体的命令
	if(answer->cmd != CMD_READ_STATUS
		&&  answer->cmd != CMD_READ_TEMP)
	{
		if(m_SendCmdArray.GetSize() > 0)
		{
			m_SendCmdArray.RemoveAt(0);
			this->m_CommTimeOut = 0; 
		}
	}
	if(answer->cmd == CMD_READ_TEMP)
	{
		if(m_SendCmdArray.GetSize() > 0)
		{
			MODBUS_RS485_CMD cmd0 = m_SendCmdArray.GetAt(0);
			if(cmd0.cmd == CMD_READ_TEMP)
			{
				m_SendCmdArray.RemoveAt(0);
				this->m_CommTimeOut = 0; 
			}
		}
	}
	switch((unsigned char)(answer->cmd))
	{
	case CMD_AT:
		AfxMessageBox(_T("开始PID自整定"));
		
		break;
	case CMD_COOL_FULL_OUT:
		//AfxMessageBox(_T("CMD_COOL_FULL_OUT"));
		
		break;
	case CMD_PROGRAM_START:
		if(this->m_SendmCmdProgram.GetSize() > 0) 
		{
			m_SendmCmdProgram.RemoveAt(0); 
		}
		//AfxMessageBox(_T("CMD_PROGRAM_START"));
		
		break;
	case CMD_PROGRAM_DATA:
		if(this->m_SendmCmdProgram.GetSize() > 0) 
		{
			m_SendmCmdProgram.RemoveAt(0); 
		}
		if(this->m_SendmCmdProgram.GetSize() == 0) 
		{
			AfxMessageBox(_T("程序更新结束"));
		//	this->SetTimer(1,100,NULL);
		}
		//AfxMessageBox(_T("CMD_PROGRAM_DATA"));
		
		break;
	case CMD_PROGRAM_END:
		if(this->m_SendmCmdProgram.GetSize() > 0) 
		{
			m_SendmCmdProgram.RemoveAt(0); 
		}
		//AfxMessageBox(_T("程序更新结束"));
		
		break;
	case CMD_AIR_VALVE:
		if(this->m_SendmCmdProgram.GetSize() > 0) 
		{
			m_SendmCmdProgram.RemoveAt(0); 
		}
		break;
	case CMD_WRITE_STEP_PID_COUNT:
		{
			int *count =  (int*)&rec_cmd[3];
			char s[1024];
			sprintf(s,"CMD_WRITE_STEP_PID_COUNT 使用 = %d", *count);
			WriteLogFile(s,source_id+1);
		
			
		}
	
		break;
	case CMD_READ_STEP_PID_COUNT:
		
		{
			int *count =  (int*)&rec_cmd[3];
			char s[1024];
			sprintf(s,"CMD_READ_STEP_PID_COUNT = %d", *count);
			WriteLogFile(s,source_id+1);
		}
		
		break;
    case CMD_AUTO_TEST:
		WriteLogFile("CMD_AUTO_TEST",source_id+1);
		break;
	case CMD_SET_RELAY:
		WriteLogFile("CMD_SET_RELAY",source_id+1);
	
		break;
	case CMD_CPLD_RESET:
		WriteLogFile("CMD_CPLD_RESET",source_id+1);
		
		break;
	case CMD_READ_ADJUST_ALL:
		WriteLogFile("CMD_READ_ADJUST_ALL",source_id+1);
		memcpy(&G_NormalConfigPara.AdjustTemp,answer->para,sizeof(G_NormalConfigPara.AdjustTemp));  
		this->SaveConfig(); 
		break;
	case CMD_SET_FLASH_TEST_SPEED:
		WriteLogFile("CMD_SET_FLASH_TEST_SPEED",source_id+1);
		
		break;
	case CMD_SET_HIGH_VOLT:
		WriteLogFile("CMD_SET_HIGH_VOLT",source_id+1);
		
		break;
	case CMD_READ_STATUS:
		
//		WriteLogFile("CMD_READ_STATUS",false);
		if(data_len == (2 + 1 + MAX_LED_NR/8 * 2 + 2))
		{
			for(i=0; i < (MAX_LED_NR +7)/8;i++)
			{
//				m_SwitchDlg[source_id].m_SwitchStatus[i] = rec_cmd[3+i];
			}
		}
		else
		{
			char s[1024];
			sprintf(s,"CMD_READ_FPGA_STATUS datalen = %d\r\n", data_len);
			WriteLogFile(s,source_id+1);
		}

		break;
	case CMD_READ_OPEN_FLASH:
		
		WriteLogFile("CMD_READ_OPEN_FLASH",source_id+1);
		break;
	case CMD_READ_CLOSE_FLASH:
		
		WriteLogFile("CMD_READ_CLOSE_FLASH",source_id);
		if(data_len == (2 + 1 + MAX_LED_NR/8 + 2))
		{
		}
		else
		{
			char s[1024];
			sprintf(s,"CMD_READ_CLOSE_FLASH datalen = %d\r\n", data_len);
			WriteLogFile(s,source_id+1);
		}
		break;

	case CMD_READ_TEMP:
		//保存当前温度
	//	WriteLogFileEx(__FILE__ , __LINE__);

		

		if(answer->RS485_Addr == '1')
		{
			
			{
				memcpy(&m_ReadTempAnswer,answer->para,sizeof(m_ReadTempAnswer));
				
				m_CurTemp   = m_ReadTempAnswer.CurTemp;
				
				
				m_CurSv			 = m_ReadTempAnswer.CurSv; 
				m_HeatOutPercent = m_ReadTempAnswer.HeatPercent;
				m_CoolOutPercent = m_ReadTempAnswer.CoolPercent;

			
				{
					static CTime last_time;
					CTime cur = CTime::GetCurrentTime();
					if(cur.GetSecond() !=  last_time.GetSecond())
					{
						TEMP_RECORD tr;
						tr.temp		=	m_CurTemp;
						tr.time     =   cur.GetTime();
						last_time	=	cur;

						::G_TempRecordArray.Add(tr);  
					}
				}
				

				

#ifndef _VoltAmp_
				for(i = 0; i < MAX_LED_NR /*, i < m_SwitchNr*/; i++)
				{
					if(m_ReadTempAnswer.SwitchStatus[i / 8] & (0x80 >> (i % 8)))
					{
						m_SwitchStatus[0][i].IsClosing =  true;
					}
					else
					{
						m_SwitchStatus[0][i].IsClosing =  false;
					}
					m_SwitchStatus[1][i].IsClosing = m_SwitchStatus[0][i].IsClosing;
					m_SwitchStatus[2][i].IsClosing = m_SwitchStatus[0][i].IsClosing;
					m_SwitchStatus[3][i].IsClosing = m_SwitchStatus[0][i].IsClosing;

				}
#else



				for(i = 0; i < 3; i++)
				{
					if(m_Amp[i] > 0.5)
					{
						m_SwitchStatus[0][i].IsUsed		= true;
						m_SwitchStatus[0][i].IsClosing = true;
						m_SwitchStatus[1][i].IsClosing = true;
						m_SwitchStatus[2][i].IsClosing = true;
						m_SwitchStatus[3][i].IsClosing = true;
					}
					else
					{
						m_SwitchStatus[0][i].IsClosing = false;
						m_SwitchStatus[1][i].IsClosing = false;
						m_SwitchStatus[2][i].IsClosing = false;
						m_SwitchStatus[3][i].IsClosing = false;
					}
				}

					
#endif	
				int draw_items;
				if(m_SwitchNr > 64)
				{
					draw_items = 64;
				}
				else
				{
					draw_items = m_SwitchNr;
				}


				
				this->DrawAllItem(draw_items);

				if(m_DebugDlg != NULL)
				{
					if(m_DebugDlg->IsWindowVisible())
					{
						m_DebugDlg->DrawLED(); 
					}
				}

				DrawTempControlInfo();
				DrawTempCurve();
				DealSwitchTest();
				

				for(int i = 0; i < 6; i++)
				{
					DrawVoltAmp(i);
				}
			}
		
		//更新有关窗口
			if(m_DebugDlg != NULL)
			{
				if(m_DebugDlg->m_bShowed && m_DebugDlg->IsWindowVisible()) 
				{
					m_DebugDlg->m_CurTemp = m_CurTemp;
					CWnd * wnd = m_DebugDlg->GetDlgItem(IDC_EDIT_CUR_TEMP);
					CString s;
					s.Format(_T("%6.2f"), m_DebugDlg->m_CurTemp);
					if(wnd != NULL)
					{
						wnd->SetWindowTextW(s);
					}

					wnd = m_DebugDlg->GetDlgItem(IDC_EDIT_MSG);
					if(m_ReadTempAnswer.AirControlIsOn)
					{
						s = _T(":空调开");
					}
					else
					{
						s = _T(":空调关");
					}

					if(m_ReadTempAnswer.ValveIsOn)
					{
						s += _T(":阀开");
					}
					else
					{
						s += _T(":阀关");
					}

					wnd->SetWindowTextW(::GetPcbModeText(m_ReadTempAnswer.TestMode) + s);
					
				}
			}
		}
		else
		{
			VOLT_AMP_AD * volt_amp= (VOLT_AMP_AD *)answer->para;
#define MAX_AD	(4096 * 10)
#define MA_4_AD (MAX_AD * 0.004 * 120 / 2.500)
#define MAX_AMP 20.0f
			if(volt_amp->VoltAmp[0] < MA_4_AD)
			{
				this->m_Amp[0] =  0;
			}
			else
			{
				this->m_Amp[0] = MAX_AMP * (volt_amp->VoltAmp[0] - MA_4_AD) / (MAX_AD - MA_4_AD ) - 0.1;
			}
			if(volt_amp->VoltAmp[1] < MA_4_AD)
			{
				this->m_Amp[1] =  0;
			}
			else
			{
				this->m_Amp[1] = MAX_AMP * (volt_amp->VoltAmp[1] - MA_4_AD) / (MAX_AD - MA_4_AD) - 0.1;
			}
			if(volt_amp->VoltAmp[2] < MA_4_AD)
			{
				this->m_Amp[2] =  0;
			}
			else
			{
				this->m_Amp[2] = MAX_AMP * (volt_amp->VoltAmp[2] - MA_4_AD) / (MAX_AD - MA_4_AD) -0.1;
			}

#define MAX_VOLT 500.0f		
			if(volt_amp->VoltAmp[4] < MA_4_AD)
			{
				this->m_Volt[0] =  0;
			}
			else
			{
				this->m_Volt[0] = MAX_VOLT * (volt_amp->VoltAmp[4] - MA_4_AD) / (MAX_AD - MA_4_AD);
				if(this->m_Volt[0] < 10)
				{
					this->m_Volt[0]= 0;
				}
			}
			
			if(volt_amp->VoltAmp[5] < MA_4_AD)
			{
				this->m_Volt[1] =  0;
			}
			else
			{
				this->m_Volt[1] = MAX_VOLT * (volt_amp->VoltAmp[5] - MA_4_AD) / (MAX_AD - MA_4_AD);
				if(this->m_Volt[1] < 10)
				{
					this->m_Volt[1]= 0;
				}
			}

			if(volt_amp->VoltAmp[6] < MA_4_AD)
			{
				this->m_Volt[2] =  0;
			}
			else
			{
				this->m_Volt[2] = MAX_VOLT * (volt_amp->VoltAmp[6] - MA_4_AD) / (MAX_AD - MA_4_AD);
				if(this->m_Volt[2] < 10)
				{
					this->m_Volt[2]= 0;

				}
			}

			for(int i = 0; i < 6; i++)
			{
				DrawVoltAmp(i);
			}
		}
		break;


	case CMD_READ_PID_T:
	
		WriteLogFile("read pid t",source_id+1);
		if(m_DebugDlg != NULL)
		{
			if(m_DebugDlg->IsWindowVisible())
			{
				m_DebugDlg->m_Read_P		= *((short*)(&answer->para[0]));
				m_DebugDlg->m_Read_I		= *((short*)(&answer->para[2]));
				m_DebugDlg->m_Read_D		= *((short*)(&answer->para[4]));
				m_DebugDlg->m_Read_P_Cool	= *((short*)(&answer->para[8]));
				m_DebugDlg->m_Read_I_Cool	= *((short*)(&answer->para[10]));
				m_DebugDlg->m_Read_D_Cool	= *((short*)(&answer->para[12]));

				
				m_DebugDlg->UpdateData(false); 
			}
		}
		break;
	case CMD_SET_PID_T:
	
		KillTimer(1);

		SetTimer(1,G_NormalConfigPara.CommSpeedTimer ,NULL); 
		
		WriteLogFile("set pid t",source_id+1);
		break;
	case CMD_SET_ADJUST_LOW:
	
		WriteLogFile("set adjust low",source_id+1);
		break;
	case CMD_SET_ADJUST_HIGH:
		
		WriteLogFile("set adjust high",source_id+1);
		break;
	case CMD_READ_ADJUST_LOW:
	
		WriteLogFile("read adjust low",source_id+1);
		if(m_DebugDlg != NULL)
		{
			if(m_DebugDlg->IsWindowVisible()) 
			{
				memcpy(&m_DebugDlg->m_ReadAdjustLow_1,answer->para,sizeof(m_DebugDlg->m_ReadAdjustLow_1));
				/*char *buf= (char*)&short_a;
				buf [0] = answer->para[0];
				buf [1] = answer->para[1];
				m_DebugDlg->m_ReadAdjustLow_1 = short_a / 10.0f;
				
				buf [0] = answer->para[2];
				buf [1] = answer->para[3];
				m_DebugDlg->m_ReadAdjustLow_2 = short_a / 10.0f;
				*/
				m_DebugDlg->UpdateData(false); 

			}
		}
		break;
	case CMD_READ_ADJUST_HIGH:
	
		WriteLogFile("read adjust high",source_id+1);
		if(m_DebugDlg != NULL)
		{
			if(m_DebugDlg->IsWindowVisible()) 
			{
				memcpy(&m_DebugDlg->m_ReadAdjustHigh_1,answer->para,sizeof(m_DebugDlg->m_ReadAdjustHigh_1));
				/*
				char *buf= (char*)&short_a;
				buf [0] = answer->para[0];
				buf [1] = answer->para[1];
				m_DebugDlg->m_ReadAdjustHigh_1 = short_a / 10.0f;
				
				buf [0] = answer->para[2];
				buf [1] = answer->para[3];
				m_DebugDlg->m_ReadAdjustHigh_2 = short_a / 10.0f;
				*/

				m_DebugDlg->UpdateData(false); 

			}
		}
		break;

	case CMD_READ_AUTHOR:
	
		WriteLogFile("CMD_READ_AUTHOR",source_id+1);
//		memcpy(&G_CopyRight.m_Author[source_id],&rec_cmd[3],sizeof(G_CopyRight.m_Author[0]));
		break;
	case CMD_READ_BUILDDATE:

		WriteLogFile("CMD_READ_BUILDDATE",source_id+1);
//		memcpy(&G_CopyRight.m_BuildDate[source_id],&rec_cmd[3],sizeof(G_CopyRight.m_BuildDate[0]));
		break;
	
	case CMD_READ_PRODUCEDATE:

		WriteLogFile("CMD_READ_PRODUCEDATE",source_id+1);
//		memcpy(&G_CopyRight.m_ProductDate[source_id][0],&rec_cmd[3],sizeof(G_CopyRight.m_ProductDate[0]));
		for(i=0; i < 8; i++)
		{
//			G_CopyRight.m_ProductDate[source_id][i] +='0';
		}
		break;

	case CMD_READ_SN:

		WriteLogFile("CMD_READ_SN",source_id+1);
//		memcpy(&G_CopyRight.m_SN[source_id],&rec_cmd[3],sizeof(unsigned int));
		break;
	case CMD_SET_DISPLAY_TYPE:

		WriteLogFile("CMD_SET_DISPLAY_TYPE",source_id+1);
		AfxMessageBox(_T("设置成功"));
		break;
    case CMD_NORMAL_PID_OUT:

		WriteLogFile("CMD_NORMAL_PID_OUT",source_id+1);
//		m_DebugDlg[source_id].DealCmdAnswer( CMD_NORMAL_PID_OUT,rec_cmd,0);
		break;
    case CMD_STEP_PID_OUT:

		WriteLogFile("CMD_STEP_PID_OUT",source_id+1);
//		m_DebugDlg[source_id].DealCmdAnswer( CMD_STEP_PID_OUT,rec_cmd,0);
		
		break;
	case CMD_FULL_OUT:

		WriteLogFile("CMD_FULL_OUT",source_id+1);
		break;
	case CMD_STOP_OUT:

		WriteLogFile("CMD_STOP_OUT",source_id+1);
		break;
	
    default:
		break;
	
	}
	//WriteLogFileEx(__FILE__ , __LINE__);
	return 0;
}

MODBUS_RS485_CMD CHeatBox_CGPDlg::SetCmd(int equip, COMMAND_ID cmdId, char *para)
{

		int i;
		MODBUS_RS485_CMD cmd;

		cmd.RS485_Addr	= equip;
		cmd.cmd			= cmdId;
		cmd.para_len	= sizeof(cmd.para); 
		for(i = 0; i < sizeof(cmd.para); i++)
		{
			cmd.para[i] = para[i];
		}
		
		CRC16((unsigned char *)&cmd,sizeof(cmd)-2,cmd.crc);
		
		return cmd;

}


void CHeatBox_CGPDlg::SaveConfig()
{
	CString sDir;
	
	
	sDir = theAppDirectory;// +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	//memset(&G_NormalConfigPara,0,sizeof(G_NormalConfigPara));

	CFile para_file;

	if(para_file.Open(sDir + _T("HeatBox_CGP_ConfigPara.INI"),CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		if(G_NormalConfigPara.CommSpeedTimer == 0)
		{
			G_NormalConfigPara.CommSpeedTimer	= 350;
		}
		

		if(G_NormalConfigPara.SwitchCount == 0)
		{
			G_NormalConfigPara.SwitchCount		= 64;
		}

		para_file.SeekToBegin();
		para_file.Write(&G_NormalConfigPara,sizeof(G_NormalConfigPara));
		para_file.Close(); 
		
	}
}

void CHeatBox_CGPDlg::ReadConfig()
{
	CString sDir;
	
	
	sDir = theAppDirectory;// +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	memset(&G_NormalConfigPara,0,sizeof(G_NormalConfigPara));

	CFile para_file;

	if(para_file.Open(sDir + _T("HeatBox_CGP_ConfigPara.INI"),CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		para_file.SeekToBegin();
		memset(&G_NormalConfigPara,0,sizeof(G_NormalConfigPara));
		UINT len = para_file.Read(&G_NormalConfigPara,sizeof(G_NormalConfigPara));
		
		if(G_NormalConfigPara.CommSpeedTimer == 0)
		{
			G_NormalConfigPara.CommSpeedTimer	= 150;
		}
		if(G_NormalConfigPara.SwitchCount == 0)
		{
			G_NormalConfigPara.SwitchCount		= 64;
		}
		if(_tcslen(G_NormalConfigPara.ParaName)  == 0)
		{
			memcpy(G_NormalConfigPara.ParaName,_T("测试参数.para"),sizeof(_T("测试参数.para")));
		}
		if(len < sizeof(G_NormalConfigPara))
		{
			para_file.SeekToBegin();
			para_file.Write(&G_NormalConfigPara,sizeof(G_NormalConfigPara));
		}
		para_file.Close(); 
	}
	else
	{
		memset(&G_NormalConfigPara,0,sizeof(G_NormalConfigPara));

		G_NormalConfigPara.CommSpeedTimer	= 150;
		G_NormalConfigPara.SwitchCount		= 64;

	}


	m_SwitchNr = G_NormalConfigPara.SwitchCount;	//AfxGetApp()->GetProfileInt("SET","SWITCH_NR",64);

	CString szFilePath = ::theAppDirectory;

	szFilePath += "para";
	if(!PathIsDirectory(szFilePath))
	{
		::CreateDirectory(szFilePath,NULL); 
	}
	
	szFilePath +="\\";

	szFilePath += G_NormalConfigPara.ParaName; 
	
	CFile test_para_file;
	if(test_para_file.Open(szFilePath, CFile::modeReadWrite))
	{

		memset(&m_TestPara,0,sizeof(m_TestPara)); 
		
		int i = test_para_file.Read(&m_TestPara,sizeof(m_TestPara));

		//if(i >= sizeof(m_TestPara))
		{
			//m_IsDoubleSwitch	= m_TestPara.IsDoubleSwitch;
			
			m_FlashGap			= m_TestPara.FlashGap; 

			
			switch(m_TestPara.KeepWarmMode)
			{
			case 0:
				this->m_KeepWarmMode = "通常保温点进入";
				break;
			case 1:
				this->m_KeepWarmMode = "接近动作点进入";
				break;

			}
		}
			
		
	}
	else
	{
		memset(&m_TestPara,0,sizeof(m_TestPara)); 
	}

}

void CHeatBox_CGPDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CHeatBox_CGPDlg::EndDialog(0); 
}

void CHeatBox_CGPDlg::OnBnClickedButtonTestReady()
{
	// TODO: 在此添加控件通知处理程序代码

	
	this->ShowBarcode();


	TEST_MODE_READY_cmd_sended = false;


	//进入准备温度区

	if(m_TestPara.HeatCoolType[m_CurTestPro] == 0)
	{//Heat-->Cool
		m_TopTemp = HEAT_COOL_READY_TEMP(); 

		if(this->m_CurTemp <  (m_TopTemp - TEST_MODE_READY_GAP_TEMP))
		{

			//if(m_ReadTempAnswer.AirControlIsOn)
			{
				SendCmdStop();
			}
			this->SendCmdHeatFullOut(m_TopTemp - TEST_MODE_READY_GAP_TEMP);
			//this->SendCmdHeatNormalPid( m_TopTemp);

		}
		else if(this->m_CurTemp >  (m_TopTemp + TEST_MODE_READY_GAP_TEMP))
		{
			SendCmdCoolStepPid(m_TopTemp + TEST_MODE_READY_GAP_TEMP);
			//this->SendCmdCoolFullOut( m_TopTemp);
			
		}
	}
	else
	{//Cool--Heat
		

		m_TopTemp = COOL_HEAT_READY_TEMP ;
		
		if(this->m_CurTemp < ( m_TopTemp))
		{
			this->SendCmdHeatNormalPid( m_TopTemp);
		}
		else
		{
			this->SendCmdCoolFullOut( m_TopTemp);
		}
	}
	
	m_TestMode = TEST_MODE::TEST_MODE_READY; 
}

void CHeatBox_CGPDlg::OnBnClickedButtonTestStop()
{
	// TODO: 在此添加控件通知处理程序代码


	CString cmd;

	this->SendCmdStop(); 

	//SendCmdRelayOut(false);

	//m_SendCmdArray.Add(SetCmd('1', CMD_STOP_OUT,para));
	
	m_TestMode = TEST_MODE::TEST_MODE_IDLE; 
	CButton *button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
	if(button != NULL)
	{
		button->EnableWindow(true); 
	}
	button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
	if(button != NULL)
	{
		button->EnableWindow(true); 
	}
	m_TopTemp = 0;
	this->m_CurTestPro = 0; 


}


void CHeatBox_CGPDlg::ClearStatus(void)
{
	if(m_CurTestPro == 0)
	{
		for(int pro = 0; pro < 4; pro++)
		{
			for(int i = 0; i < MAX_LED_NR; i++)
			{
				m_SwitchStatus[pro][i].IsUsed = false;
				m_SwitchStatus[pro][i].CloseTemp = INVALID_TEMP;
					m_SwitchStatus[pro][i].OpenTemp  = INVALID_TEMP;
					m_SwitchStatus[pro][i].IsClosed  = false;
					m_SwitchStatus[pro][i].IsOpenned = false;
					m_SwitchStatus[pro][i].IsOpenFlash = false;
	//				m_SwitchStatus[pro][i].OpenFlashTemp = INVALID_TEMP;
					m_SwitchStatus[pro][i].IsCloseFlash = false;
	//				m_SwitchStatus[pro][i].CloseFlashTemp = INVALID_TEMP;
				//}

			}
		}
	}
}
void CHeatBox_CGPDlg::CheckIsUsed(void)
{
	int count		= 0;
	int MaxCount	= 0;

	switch(m_TestPara.IsDoubleSwitch) 
	{
			case 1:
				MaxCount = 128;
				break;
			case 0:
			default:
				MaxCount =  64;
	}

	if(m_CurTestPro == 0)
	{
		for(int pro = 0; pro < 4; pro++)
		{
			for(int i = 0; i < MaxCount; i++)
			{
				if(m_TestPara.IsNormalOpen  == 0)
				{
					if(this->m_SwitchStatus[pro][i].IsClosing)
					{
						m_SwitchStatus[pro][i].IsUsed = true;
						count++;
					}
					else
					{
					//	m_SwitchStatus[pro][i].IsUsed = false;
					}
				}
				else
				{
					if(this->m_SwitchStatus[pro][i].IsClosing)
					{
					//	m_SwitchStatus[pro][i].IsUsed = false;
					}
					else
					{
						m_SwitchStatus[pro][i].IsUsed = true;
						count++;
					}
				}
					
			}
		}

		m_Takeboard[0] = false;
		m_Takeboard[1] = false;
		m_Takeboard[2] = false;
		m_Takeboard[3] = false;
	}

	//CString s;
	//s.Format(_T("IsUsed = %d"),count);
	//AfxMessageBox(s);
}


void CHeatBox_CGPDlg::OnBnClickedButtonTestStart()
{
	SendCmdStop();
	ClearStatus();
	EnterTestStart();

	

}

void CHeatBox_CGPDlg::EnterTestStart()
{
	// TODO: 在此添加控件通知处理程序代码
 
	m_ClickedButtonTestStart = true;

	if(m_TestPara.HeatCoolType[m_CurTestPro] == 0)
	{//Heat-->Cool

		/*if(m_CurTemp < (HEAT_COOL_READY_TEMP() - TEST_MODE_READY_GAP_TEMP))
		{//当前温度比等待温度低，先加热
			//WriteLogFile(__FILE__ , __LINE__);
			
			m_TestMode = TEST_MODE::TEST_MODE_READY;
			m_TopTemp = (m_TestPara.OpenTemp[m_CurTestPro] - m_TestPara.OpenTempMinus[m_CurTestPro] - 5.0f);
			this->SendCmdHeatNormalPid(m_TopTemp);	
			CButton *button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			TEST_MODE_READY_cmd_sended = false;


		}
		else*/ if(m_CurTemp >= (HEAT_COOL_READY_TEMP() + TEST_MODE_READY_GAP_TEMP))
		{//当前温度比等待温度高，先降温

			m_TestMode = TEST_MODE_READY; 
			m_TopTemp = HEAT_COOL_READY_TEMP();
			//SendCmdCoolFullOut(m_TopTemp);
			SendCmdCoolStepPid(m_TopTemp);
			CButton *button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}

			TEST_MODE_READY_cmd_sended = false;

		}
		else
		{//在温度区，就开始升温

			m_TestMode			= TEST_MODE::TEST_MODE_LINE_UP;
			
			this->m_BeginTime   = CTime::GetCurrentTime(); 

#ifdef _VoltAmp_
			SendCmdRelayOut(true);
#endif
			m_LastTestPro		=  m_CurTestPro;
			m_TopTemp			= (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro] + 2.0f);
			CButton *button	= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
			if(button != NULL)
			{
				button->EnableWindow(false); 
			}
			SendCmdStepPid(m_TopTemp);
			TEST_MODE_READY_cmd_sended =  true;

		}
	}
	else
	{//Cool->Heat
		if(m_CurTemp < (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempMinus[m_CurTestPro] + 2.0f - 0.3f))
		{//当前温度比等待温度低，先加热
			m_TestMode = TEST_MODE::TEST_MODE_READY;
			m_TopTemp = (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro] + 2.0f);
			this->SendCmdHeatFullOut(m_TopTemp);	
			CButton *button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
			if(button != NULL)
			{
				button->EnableWindow(false); 
			}
			button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			TEST_MODE_READY_cmd_sended = false;
		}
		else if(m_CurTemp >= (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro] + 2.0f + 0.3f))
		{//当前温度比等待温度高，先降温
			m_TestMode = TEST_MODE_READY; 
			m_TopTemp = (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro] + 2.0f);
			SendCmdCoolFullOut(m_TopTemp);
			CButton *button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}

			TEST_MODE_READY_cmd_sended = false;

		}
		else
		{//在温度区，就开始降温
 			m_TestMode = TEST_MODE::TEST_MODE_LINE_DOWN;
			m_LastTestPro	=  m_CurTestPro;

			m_TopTemp = (m_TestPara.OpenTemp[m_CurTestPro] - m_TestPara.OpenTempMinus[m_CurTestPro] - 1.0f);
			CButton *button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			button= (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
			button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
			if(button != NULL)
			{
				button->EnableWindow(false); 
			}
			this->SendCmdCoolStepPid(m_TopTemp);
			TEST_MODE_READY_cmd_sended =  true;

		}
	}

	CheckIsUsed();
	
	if(G_TempRecordArray.GetSize() > 0)  
	{
		G_TempRecordArray.RemoveAll();
	}


}

void CHeatBox_CGPDlg::OnBnClickedButtonPreTemp()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];
	CString cmd;
	para[0] = 860 % 256;		
	para[1] = 860 / 256;

	para[2] = 10 % 256;
	para[3] = 10 / 256;;
	m_SendCmdArray.Add(SetCmd('1', CMD_NORMAL_PID_OUT,para));
}


void CHeatBox_CGPDlg::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveTestResult();
return;
	CViewTestRecordDlg dlg;
	dlg.DoModal();
}

void CHeatBox_CGPDlg::OnBnClickedButtonDebug()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SendCmdReadAdjustLow(); 
	this->SendCmdReadAdjustHigh();
	this->SendCmdReadPidT();
	CDebugDlg dlg;
	m_DebugDlg = &dlg;
	dlg.m_bShowed = true;
	dlg.DoModal(); 
	m_DebugDlg = NULL;
}

void CHeatBox_CGPDlg::DrawTempControlInfo(void)
{

//	AfxMessageBox(_T("6"));

	if(!this->IsWindowVisible()) 
	{
		return;
	}

	CBitmap		* pOldBitmap	= NULL;

	CBrush		*oldbrush		= NULL;
	CPen		*oldpen			= NULL;
	CFont		*oldFont		= NULL;
	//绘制准备

	
	
	CWnd *data_area = GetDlgItem(IDOK);
	CRect IDOK_rect,wndRect,data_area_Rect;
	data_area->GetWindowRect(IDOK_rect);
	ScreenToClient(&IDOK_rect);

	this->GetClientRect(wndRect);
	
	data_area_Rect.left		= wndRect.right * 5 / 6;
	data_area_Rect.bottom	= IDOK_rect.bottom;
	data_area_Rect.right	= wndRect.right;
	data_area_Rect.top		= wndRect.top;

	

	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2

	pen.DeleteObject(); 
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	



	//background
	oldbrush=MemDC.SelectObject(&blackbrush);				//选择画刷
	MemDC.Rectangle(0,0, data_area_Rect.Width(),data_area_Rect.Height());

	
	CString s;
		
	//当前温度
	{
		//tempFont.DeleteObject(); 
		//tempFont.CreatePointFont(300,_T("宋体"),&MemDC);
		oldFont = MemDC.SelectObject(&DrawTempControlInfo_Font3);
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.SetTextColor(RGB(0xFF,0,0)); 
		s.Format(_T("PV %6.2f"),this->m_CurTemp);  
		MemDC.TextOutW(0,0, s);
		MemDC.MoveTo(0,38);
		MemDC.LineTo(data_area_Rect.Width() ,38);
	
	//目标温度
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.SetTextColor(RGB(0,0xFF,0));
		s.Format(_T("SV %6.2f"),this->m_CurSv);
		MemDC.TextOutW(0,40,s);
		MemDC.MoveTo(0,88);
		MemDC.LineTo(data_area_Rect.Width() ,88);

		//tempFont.DeleteObject(); 
		//tempFont.CreatePointFont(200,_T("宋体"),&MemDC);
		oldFont = MemDC.SelectObject(&DrawTempControlInfo_Font1);

		MemDC.SetTextColor(RGB(0xFF,0,0));
		switch(::G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("制热 % 6d%%"),this->m_HeatOutPercent); 
			break;
		case 1:
			s.Format(_T("Heat % 6d%%"),this->m_HeatOutPercent); 
			break;
		}
		MemDC.TextOutW(0,90,s);
		MemDC.MoveTo(0,118);
		MemDC.LineTo(data_area_Rect.Width() ,118);

		switch(::G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("制冷 % 6d%%"),this->m_CoolOutPercent); 
			break;
		case 1:
			s.Format(_T("Cool % 6d%%"),this->m_CoolOutPercent); 
			break;
		}
		MemDC.SetTextColor(RGB(0,0xFF,0));
		MemDC.TextOutW(0,120,s);
		MemDC.MoveTo(0,158);
		MemDC.LineTo(data_area_Rect.Width() ,158);

		//tempFont.DeleteObject(); 
		//tempFont.CreatePointFont(150,_T("宋体"),&MemDC);
		oldFont = MemDC.SelectObject(&DrawTempControlInfo_Font2);

		float speed = 0;
		if(G_TempRecordArray.GetSize() > 30)
		{
			speed  = G_TempRecordArray.GetAt(G_TempRecordArray.GetSize() - 1).temp  - G_TempRecordArray.GetAt(G_TempRecordArray.GetSize() - 28).temp;
			
			speed  /= G_TempRecordArray.GetAt(G_TempRecordArray.GetSize() - 1).time  - G_TempRecordArray.GetAt(G_TempRecordArray.GetSize() - 28).time;

			speed  *= 60.0f;
		}
		
		switch(::G_NormalConfigPara.Language)
		{
		case 0:
#ifdef _VoltAmp_
			s.Format("温速  %6.2f度/分",speed / 2); 
#else
			s.Format(_T("温速  %6.2f度/分"),speed);
#endif
			break;
		case 1:
#ifdef _VoltAmp_
			s.Format(_T("Speed %6.2f℃/m"),speed / 2); 
#else
			s.Format(_T("Speed %6.2f℃/min"),speed); 
#endif
			break;
		}
		MemDC.SetTextColor(RGB(0xFF,0xFF,0));
		MemDC.TextOutW(0,170,s);
		MemDC.MoveTo(0,198);
		MemDC.LineTo(data_area_Rect.Width() ,198);
		//当前工作状态
		
		s.Format(_T("%5.1f"),m_TopTemp);
		switch(::G_NormalConfigPara.Language)
		{
		case 0:
			switch(m_TestMode)
			{
			case TEST_MODE::TEST_MODE_IDLE:
				s = _T("系统空闲...") + s; 
				break;
			case TEST_MODE::TEST_MODE_FULL_UP:
				s = _T("全速加热...")+ s;
				break;
			case TEST_MODE::TEST_MODE_LINE_UP:
				s = _T("匀速加热...")+ s;
				break;
			case TEST_MODE::TEST_MODE_LINE_DOWN:
				s = _T("匀速降温...")+ s;
				break;

			case TEST_MODE::TEST_MODE_READY:
				s.Format(_T("%5.1f--%5.1f"),m_TopTemp - TEST_MODE_READY_GAP_TEMP,m_TopTemp + TEST_MODE_READY_GAP_TEMP);
				s = _T("测试准备...")+ s;
				break;
			case TEST_MODE_FULL_DOWN:
				s = _T("全速降温...")+ s;
				break;
			default:
				s = _T("错误的状态...");
				break;
			}
			break;

		case 1:
			switch(m_TestMode)
			{
			case TEST_MODE::TEST_MODE_IDLE:
				s = _T("Idle...") + s; 
				break;
			case TEST_MODE::TEST_MODE_FULL_UP:
				s = _T("Full Heat...") + s;
				break;
			case TEST_MODE::TEST_MODE_LINE_UP:
				s = _T("Line Heat...") + s;
				break;
			case TEST_MODE::TEST_MODE_LINE_DOWN:
				s = _T("Line Cool...") + s;
				break;

			case TEST_MODE::TEST_MODE_READY:
				s = _T("Ready...") + s;
				break;
			case TEST_MODE_FULL_DOWN:
				s = _T("Full Cool...")+ s;
				break;
			default:
				s = _T("Error ...");
				break;
			}
			break;
		}
		MemDC.SetTextColor(RGB(0xFF,0xFF,0));
		MemDC.TextOutW(0,210,s);
		
	}

	dc->BitBlt(data_area_Rect.left,data_area_Rect.top,data_area_Rect.Width(),data_area_Rect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形
	this->UpdateData(false); 


	
	if(oldbrush != NULL)
	{
		MemDC.SelectObject(oldbrush);
	}
	if(pOldBitmap != NULL)
	{
		MemDC.SelectObject(pOldBitmap);
	}
	if(oldpen != NULL)
	{
		MemDC.SelectObject(oldpen);
	}
	if(oldFont != NULL)
	{
		MemDC.SelectObject(oldFont);
	}
	
	

}

void CHeatBox_CGPDlg::PrepareDraw(void)
{
	//AfxMessageBox(_T("1"));
	CFile log_file;
		//绘制准备
	dc = this->GetDC();
		

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

//	ReleaseDC(dc); 
//return;
	CWnd *data_area = GetDlgItem(IDC_BUTTON_PRINT);
	CRect IDC_BUTTON_PRINT_rect,wndRect,data_area_Rect;
	data_area->GetWindowRect(IDC_BUTTON_PRINT_rect);
	ScreenToClient(&IDC_BUTTON_PRINT_rect);

	this->GetClientRect(wndRect);

	data_area_Rect.left		= IDC_BUTTON_PRINT_rect.right + 10 ;
	data_area_Rect.bottom	= IDC_BUTTON_PRINT_rect.bottom;
	data_area_Rect.right	= wndRect.right * 5 / 6 -10;
	data_area_Rect.top		= wndRect.top;

	MemDC.DeleteDC();
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
//ReleaseDC(dc); 
//return;		

	//bitmap.DeleteObject(); 
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
	}

	DrawTempCurve_Font1.CreatePointFont(data_area_Rect.Height() / 16 * 10 ,_T("宋体"),&MemDC);
	DrawTempCurve_Font2.CreatePointFont(data_area_Rect.Height() / 20 * 10 ,_T("宋体"),&MemDC);
	DrawTempCurve_Font3.CreatePointFont(data_area_Rect.Height() / 10 * 10 ,_T("宋体"),&MemDC);
	DrawTempCurve_Font4.CreatePointFont(data_area_Rect.Height() / 30 * 10 ,_T("宋体"),&MemDC);
	DrawTempControlInfo_Font1.CreatePointFont(200 ,_T("宋体"),&MemDC);
	DrawTempControlInfo_Font2.CreatePointFont(150 ,_T("宋体"),&MemDC);
	DrawTempControlInfo_Font3.CreatePointFont(300 ,_T("宋体"),&MemDC);
	
	
	int nWidth;
    int nHeight;

	
	data_area = GetDlgItem(IDC_LIST_BARCODE);//IDOK);
	data_area->GetWindowRect(data_area_Rect);

	

	this->GetClientRect(wndRect);

	data_area_Rect.right = wndRect.right - 30;

	ScreenToClient(&data_area_Rect);

	data_area->MoveWindow(data_area_Rect);

	

	 

	nWidth	= (wndRect.right - wndRect.left);
	nHeight = wndRect.bottom - data_area_Rect.bottom;

	int item_Space			=	2;
	int item_NR_PER_LINE	=	8;
	int	item_LINES			=	8;

	int item_W = nWidth  / item_NR_PER_LINE - item_Space;
	int item_H = nHeight / item_LINES       - item_Space;//Ellipse_W;
	CFont * oldFont;

	if(DrawAllItem_Font1.GetSafeHandle() == NULL) 
	{
		int FontPoint = item_W * 10;
		

		while(1)
		{
			DrawAllItem_Font1.DeleteObject(); 
			DrawAllItem_Font1.CreatePointFont(FontPoint,_T("黑体"),&MemDC);
			oldFont = MemDC.SelectObject(&DrawAllItem_Font1);
			chnSize = MemDC.GetTextExtent(_T("123.45"));
			if(item_W * 0.45  >= chnSize.cx)
			{
				break;
			}
			FontPoint -= 1;
			DrawAllItem_Font1.DeleteObject();
			MemDC.SelectObject(oldFont);
		}

	}
	
}
void CHeatBox_CGPDlg::DrawTempCurve(void)
{
	//	AfxMessageBox(_T("5"));
	if(!this->IsWindowVisible()) 
	{
		return;
	}


	
	 CBitmap	*pOldBitmap = NULL;
	 CBrush		*oldbrush	= NULL;
	 CPen		*oldpen		= NULL;
	 CFont		*oldFont	= NULL;


	 if(bitmap.GetSafeHandle() == NULL)
	 {
		 this->PrepareDraw();
	 }
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境
//ReleaseDC(dc); 
//return;	

#define PEN_WEIGHT	2	
	pen.DeleteObject(); 
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);								//选择画笔对象


	CWnd *data_area = GetDlgItem(IDC_BUTTON_PRINT);
	CRect IDC_BUTTON_PRINT_rect,wndRect,data_area_Rect;
	data_area->GetWindowRect(IDC_BUTTON_PRINT_rect);
	ScreenToClient(&IDC_BUTTON_PRINT_rect);

	this->GetClientRect(wndRect);

	data_area_Rect.left		= IDC_BUTTON_PRINT_rect.right + 10 ;
	data_area_Rect.bottom	= IDC_BUTTON_PRINT_rect.bottom;
	data_area_Rect.right	= wndRect.right * 5 / 6 -10;
	data_area_Rect.top		= wndRect.top;

	//background
	oldbrush=MemDC.SelectObject(&blackbrush);				//选择画刷
	MemDC.Rectangle(0,0, data_area_Rect.Width(),data_area_Rect.Height());

	if(m_bShowTempCurve)
	{
		MemDC.SelectObject(&DrawTempCurve_Font1); 
		float max_temp = -100.0f;
		float min_temp = 999.0f;
		for(int i = 0; i < G_TempRecordArray.GetSize(); i++)
		{
			if(G_TempRecordArray.GetAt(i).temp  < min_temp)
			{
				min_temp = G_TempRecordArray.GetAt(i).temp ;
			}
			
			if(G_TempRecordArray.GetAt(i).temp  > max_temp)
			{
				max_temp = G_TempRecordArray.GetAt(i).temp;
			}
		}
		max_temp += 5.0f;
		min_temp -= 2.0f;
		
		max_temp = (int)(max_temp);
		min_temp = (int)(min_temp);

		CString s;
		MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
		MemDC.SetBkMode(TRANSPARENT);
		float y_gap = ((max_temp - min_temp) / 4.0f );
		
		pen.DeleteObject(); 
		pen.CreatePen(PS_DOT,PEN_WEIGHT, RGB(0xFF,0xFF,0xFF));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		for(int i = 0; i <= 4; i++)
		{
			s.Format(_T("%.1f"),min_temp + y_gap * i );
			MemDC.TextOutW(0,data_area_Rect.bottom - data_area_Rect.Height() * 0.8 * i / 4 - 20,s);

			MemDC.MoveTo( 0,					data_area_Rect.bottom - data_area_Rect.Height() * 0.8 * i/ 4);
			MemDC.LineTo(data_area_Rect.right,	data_area_Rect.bottom - data_area_Rect.Height() * 0.8 * i/ 4);
		}

		MemDC.SelectObject(&DrawTempCurve_Font2);

		for(int i = 1; i < 10; i++)
		{
	
			MemDC.MoveTo(data_area_Rect.Width() * i / 10, data_area_Rect.Height() * 0.2);
			MemDC.LineTo(data_area_Rect.Width() * i / 10, data_area_Rect.Height());
		}

		pen.DeleteObject(); 
		pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0xFF,0,0));  		//建立画笔
		oldpen=MemDC.SelectObject(&pen);
		int start_id =  - data_area_Rect.Width();
		int x_gap;

	
		if(data_area_Rect.Width() >= G_TempRecordArray.GetSize())
		{
			if(G_TempRecordArray.GetSize() > 0)
			{
				x_gap = data_area_Rect.Width() / G_TempRecordArray.GetSize(); 
			}
			else
			{
				x_gap = data_area_Rect.Width() ; 
			}

			if(G_TempRecordArray.GetSize() > 0)
			{
				MemDC.MoveTo(0,data_area_Rect.bottom - (G_TempRecordArray.GetAt(0).temp  - min_temp) * data_area_Rect.Height() * 0.8 /(max_temp - min_temp));
				for(int i = 0; i < G_TempRecordArray.GetSize(); i++)
				{
					MemDC.LineTo(i*x_gap,data_area_Rect.bottom  - (G_TempRecordArray.GetAt(i).temp  - min_temp) * data_area_Rect.Height() * 0.8 /(max_temp - min_temp));
					 
				}
			}

		}
		else
		{
			
			x_gap = G_TempRecordArray.GetSize() / data_area_Rect.Width() + 1 ; 
			if(G_TempRecordArray.GetSize() > 0)
			{
				MemDC.MoveTo(0,data_area_Rect.Height() - (G_TempRecordArray.GetAt(0).temp  - min_temp) * data_area_Rect.Height() * 0.8 /(max_temp - min_temp));
				for(int i = 0; i < G_TempRecordArray.GetSize(); i+=x_gap)
				{
					MemDC.LineTo(i/x_gap,data_area_Rect.Height() - (G_TempRecordArray.GetAt(i).temp  - min_temp) * data_area_Rect.Height() * 0.8 /(max_temp -min_temp));
					 
				}
			}

		}

		int hh,mm,ss;
		CTime cur = CTime::GetCurrentTime();
		hh = G_TempRecordArray.GetSize() / 3600 / 2;
		mm = (G_TempRecordArray.GetSize() / 2 - hh * 3600 ) / 60;
		ss = (G_TempRecordArray.GetSize() / 2) % 60; 
		
		switch(::G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("%s     持续时间:%02d:%02d:%02d    发送：%d 接收：%d ，命令数%d"),
				cur.Format(_T("%Y/%m/%d %H:%M:%S")) , hh,mm,ss, m_Comm.m_WriteCommCount,m_Comm.m_ReadCommCount,m_SendCmdArray.GetCount()); 
			
			break;
		case 1:
			s.Format(_T("%s      Total Time:%02d:%02d:%02d  发送：%d 接收：%d"),
				cur.Format(_T("%Y/%m/%d %H:%M:%S")), hh,mm,ss , m_Comm.m_WriteCommCount,m_Comm.m_ReadCommCount); 
			break;
		}
		MemDC.TextOutW(data_area_Rect.Width() / 3, 10,s);  

	}
	else  //
    {

		CString s,s1;
		CSize chnSize;
		CRect rect; 

	//	tempFont.DeleteObject(); 
	//	tempFont.CreatePointFont(data_area_Rect.Height() / 16 * 10 ,_T("宋体"),&MemDC);
		oldFont = MemDC.SelectObject(&DrawTempCurve_Font1);

		MemDC.SetBkMode(TRANSPARENT);

		chnSize = dc->GetTextExtent(_T("国"));
		MemDC.SetTextColor(RGB(0xFF,0xFF,0)); 
		switch(G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("厂家:%s"),this->m_TestPara.Productor);
			break;
		case 1:
			s.Format(_T("Pro.:%s"),this->m_TestPara.Productor);
			break;
		}
		rect.left  = 0;
		rect.right =  data_area_Rect.Width() * 35 / 100;
		rect.top   = 0;
		rect.bottom = data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);
		switch(G_NormalConfigPara.Language)
		{
		case 0:
			s1 = G_NormalConfigPara.ParaName;
			s1 = s1.Left(s1.GetLength() - 5); //del .para  
			s.Format(_T("型号:%s"),s1);
			break;
		case 1:
			s1 = G_NormalConfigPara.ParaName;
			s1 = s1.Left(s1.GetLength() - 5); //del .para  
			s.Format(_T("Para:%s"),s1);
			break;
		}
		
		//*
		rect.left  = 0;
		rect.right =  data_area_Rect.Width() * 35 / 100;
		rect.top   = data_area_Rect.Height() / 6;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);

		//tempFont.DeleteObject(); 
		//tempFont.CreatePointFont(data_area_Rect.Height() / 20 * 10 ,_T("宋体"),&MemDC);
		oldFont = MemDC.SelectObject(&DrawTempCurve_Font2);

		chnSize = dc->GetTextExtent(_T("国"));

		switch(G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("工位:%d"),this->m_SwitchNr);
			break;
		case 1:
			s.Format(_T("Count:%d"),this->m_SwitchNr);
			break;
		}
		rect.left  = 0;
		rect.right =  data_area_Rect.Width() * 15 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 2;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);
		
		switch(G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("校温:"));//%4.1f:%4.1f",this->m_AdjustTempHeat,m_AdjustTempCool);
			break;
		case 1:
			s.Format(_T("Temp Adj.:"));//%4.1f:%4.1f",this->m_AdjustTempHeat,m_AdjustTempCool);
			break;
		}
		rect.left  = 0;
		rect.right =  data_area_Rect.Width() * 15 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 3;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);

		MemDC.SetTextColor(RGB(0xFF,0,0)); 
		s.Format(_T("%4.1f"),::G_NormalConfigPara.AdjustTemp);
		rect.left  = data_area_Rect.Width() * 8 / 100;
		rect.right =  data_area_Rect.Width() * 15 / 100;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);

		MemDC.SetTextColor(RGB(0xFF,0xFF,0)); 
		switch(G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("闪动:%dms"),this->m_TestPara.FlashGap * 100);
			break;
		case 1:
			s.Format(_T("Flash:%dms"),this->m_TestPara.FlashGap * 100);
			break;
		}
		rect.left  = 0;
		rect.right =  data_area_Rect.Width() * 15 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 4;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);
		switch(G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("准备:"),"");//this->m_TestPara.KeepWarmMode);
			break;
		case 1:
			s.Format(_T("Ready:"),"");//this->m_TestPara.KeepWarmMode);
			break;
		}
		rect.left  = 0;
		rect.right =  data_area_Rect.Width() * 15 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 5;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);

		



		switch(G_NormalConfigPara.Language)
		{
		case 0:
			s.Format(_T("批次:%s"),this->m_TestPara.ProductorNo);//m_ProductNo);
			break;
		case 1:
			s.Format(_T("Pro.No:%s"),this->m_ProductNo);
			break;
		}
		rect.left  = data_area_Rect.Width() * 15 / 100;
		rect.right =  data_area_Rect.Width() * 35 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 2;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);
		
		switch(G_NormalConfigPara.Language)
		{
		case 0:
			if(this->m_TestPara.HeatSpeed == 0  ) 
			{
				s.Format(_T("加热: 比例(%d%%)"),m_TestPara.HeatPercent);  
			}
			else
			{
				s.Format(_T("加热: %3.1f度/分钟"),m_TestPara.HeatSpeed / 10.0f); 
			}
		break;
		case 1:
			if(this->m_TestPara.HeatSpeed == 0  ) 
			{
				s.Format(_T("RATIO:%d%%"),m_TestPara.HeatPercent);  
			}
			else
			{
				s.Format(_T("%3.1f°C/m"),m_TestPara.HeatSpeed / 10.0f); 
			}
			break;
		}
		rect.left  = data_area_Rect.Width() * 15 / 100;
		rect.right =  data_area_Rect.Width() * 35 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 3;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);
		
		switch(G_NormalConfigPara.Language)
		{
		case 0:
			switch(m_TestPara.IsDoubleSwitch) 
			{
			case 1:
				s = _T("类型:双路");
				break;
			case 0:
			default:
				s = _T("类型:单路");
			}

			switch(m_TestPara.IsNormalOpen) 
			{
			case 1:
				s += _T("常开");
				break;
			case 0:
			default:
				s += _T("常闭");
			}
			break;
		case 1:
			switch(m_TestPara.IsDoubleSwitch) 
			{
			case 1:
				s = _T("PROTECTOR:Double");
				break;
			case 0:
			default:
				s = _T("PROTECTOR:Single");
			}
			switch(m_TestPara.IsNormalOpen) 
			{
			case 1:
				s += _T(",Open");
				break;
			case 0:
			default:
				s += _T(",Close");
			}
			break;

		}
		rect.left  = data_area_Rect.Width() * 15 / 100;
		rect.right =  data_area_Rect.Width() * 35 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 4;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);

		switch(G_NormalConfigPara.Language)
		{
		case 0:
			switch(m_TestPara.KeepWarmMode)
			{
			case 1:
				s = _T("进入:接近动作点");
				break;
			case 0:
			default:
				s = _T("进入:通常保温点");
				break;
			}
			break;
		case 1:
			switch(m_TestPara.KeepWarmMode)
			{
			case 1:
				s = _T("Entry:Open Temp");
				break;
			case 0:
			default:
				s = _T("Entry:Keep Warm");
				break;
			}
			break;
		}
		rect.left  = data_area_Rect.Width() * 15 / 100;
		rect.right =  data_area_Rect.Width() * 35 / 100;
		rect.top   = data_area_Rect.Height() / 6 * 5;
		rect.bottom = rect.top + data_area_Rect.Height() / 6;
		MemDC.Rectangle(&rect);
		rect.top += (rect.Height() - chnSize.cy)/2;
		MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);
		//
		CString testPointStr[6][2] =
		{
			{_T("无     "),_T("N/C,   ")},
			{_T("热点   "),_T("HIGH POINT, ")},
			{_T("中一点 "),_T("MID 1, ")},
			{_T("中二点 "),_T("MID 2, ")},
			{_T("低点   "),_T("LOW,   ")},
			{_T("PU/PD点"),_T("PU/PD, ")}
		};

		MemDC.SetTextColor(RGB(0,0,0)); 
		rect.left  = data_area_Rect.Width() * 38 / 100;
		rect.right = data_area_Rect.Width();


		for(int i = 0; i < 4; i++)
		{
			rect.top   = data_area_Rect.Height() / 4 * i;
			rect.bottom = rect.top + data_area_Rect.Height() / 4 - 2;
			if(m_CurTestPro == i )
			{
				oldbrush = MemDC.SelectObject(&yellowbrush);
			}
			else if((m_TestPara.TestPoint[i] != 0) && (i != 0))
			{
				oldbrush = MemDC.SelectObject(&graybrush);
			}
			else if((i == 0))
			{
				oldbrush = MemDC.SelectObject(&graybrush);
			}
			else
			{
				oldbrush = MemDC.SelectObject(&blackbrush);
			}
			MemDC.Rectangle(&rect);

			

			if((m_TestPara.TestPoint[i] == 0) && (i != 0))
			{
				s = testPointStr[0][G_NormalConfigPara.Language];
				rect.top += 5;
				MemDC.DrawText(s,&rect,DT_LEFT);
			}
			else
			{
				int  TestPoint = m_TestPara.TestPoint[i];
				if(TestPoint < 0)
				{
					break;
				}
				if(i == 0)
				{
					TestPoint++;
				}
				if(m_TestPara.OpenCloseGapLow[i] != 0 && m_TestPara.OpenCloseGapHigh[i] != 0)
				{
					switch(G_NormalConfigPara.Language)
					{
					case 0:
						s.Format(_T("%s 断开:%5.1f(%3.1f ~ %3.1f) 差动:%3.1f ~ %3.1f"),
						testPointStr[TestPoint][G_NormalConfigPara.Language],
						m_TestPara.OpenTemp[i],
						m_TestPara.OpenTempPlus[i],
						-m_TestPara.OpenTempMinus[i],
						m_TestPara.OpenCloseGapLow[i],
						m_TestPara.OpenCloseGapHigh[i]);  
						break;
					case 1:
						s.Format(_T("%s:Open:%5.1f(%3.1f ~ %3.1f) Gap:%3.1f ~ %3.1f"),
						testPointStr[TestPoint][G_NormalConfigPara.Language],
						m_TestPara.OpenTemp[i],
						m_TestPara.OpenTempPlus[i],
						-m_TestPara.OpenTempMinus[i],
						m_TestPara.OpenCloseGapLow[i],
						m_TestPara.OpenCloseGapHigh[i]);  
						break;
					}
				}
				else
				{//Cool -> Heat
					switch(G_NormalConfigPara.Language)
					{
					case 0:
						s.Format(_T("%s 接通:%5.1f(%3.1f ~ %3.1f) 差动:"),
						testPointStr[TestPoint][0],
						m_TestPara.OpenTemp[i],
						m_TestPara.OpenTempPlus[i],
						-m_TestPara.OpenTempMinus[i]);
						break;
					case 1:
						s.Format(_T("%s Close:%5.1f(%3.1f ~ %3.1f) Gap:"),
							testPointStr[TestPoint][1],
							m_TestPara.OpenTemp[i],
							m_TestPara.OpenTempPlus[i],
							-m_TestPara.OpenTempMinus[i]);
						break;
					}
					
				}
				

				rect.top += 5;
				MemDC.DrawText(s,&rect,DT_LEFT);

				rect.top += (rect.Height())/2;

				CRect tempRect;
				tempRect.left = rect.left + 3;
				tempRect.right= tempRect.left + 20;
				tempRect.top  = rect.top;
				tempRect.bottom = rect.bottom - 5;
				DrawUN(&MemDC,tempRect,m_TestPara.HeatCoolType[i]);

				if(m_TestPara.CloseTempMinus[i] != 0 || m_TestPara.CloseTempPlus[i] != 0 )
				{
					switch(G_NormalConfigPara.Language)
					{
					case 0:
						s.Format(_T("        断开:%5.1f(%3.1f ~ %3.1f) 循环: %d"),
							m_TestPara.CloseTemp[i],
							m_TestPara.CloseTempPlus[i],
							-m_TestPara.CloseTempMinus[i],
							m_TestPara.Cycle[i]);
						break;
					case 1:
						s.Format(_T("      Open:%5.1f(%3.1f ~ %3.1f) Cycle: %d"),
							m_TestPara.CloseTemp[i],
							m_TestPara.CloseTempPlus[i],
							-m_TestPara.CloseTempMinus[i],
							m_TestPara.Cycle[i]);
						break;
					}

				}
				else
				{//Cool -> Heat
					switch(G_NormalConfigPara.Language)
					{
					case 0:
						s.Format(_T("        接通:                  循环: %d"),
							m_TestPara.Cycle[i]);
						break;
					case 1:
						s.Format(_T("        Close:                 Cycle: %d"),
							m_TestPara.Cycle[i]);
						break;
					}
				}
				MemDC.DrawText(s,&rect,DT_VCENTER | DT_LEFT);

				MemDC.SetTextColor(RGB(0xFF,0,0));
				if(this->m_Takeboard[i])
				{
					CRect rect_1 = rect;
					rect_1.left += rect_1.Width() * 5 /6; 
					switch(G_NormalConfigPara.Language)
					{
					case 0:
						MemDC.DrawText(_T("请拔板!"),&rect_1,DT_VCENTER | DT_LEFT);
						break;
					case 1:
						MemDC.DrawText(_T("Please Pullout!"),&rect_1,DT_VCENTER | DT_LEFT);
						break;
					}
				}
			}
			
		}


		MemDC.SelectObject(&blackbrush);
		
	//	tempFont.DeleteObject(); 
	//	tempFont.CreatePointFont(data_area_Rect.Height() / 10 * 10 ,_T("宋体"),&MemDC);
		oldFont = MemDC.SelectObject(&DrawTempCurve_Font3);
		MemDC.SetTextColor(RGB(0xFF,0,0)); 
		for(int i = 0; i < 4; i++)
		{
			CString s;
			if((m_CurTestPro == i) && (m_bFlashShow == false))
			{
				s = "";
			}
			else
			{
				s.Format(_T("%d"),i+1);
			}
			rect.left  = data_area_Rect.Width() * 35 / 100;
			rect.right = data_area_Rect.Width() * 38 / 100;
			rect.top   = data_area_Rect.Height() / 4 * i;
			rect.bottom = rect.top + data_area_Rect.Height() / 4;
			MemDC.Rectangle(&rect);
			rect.top += 10;
			MemDC.DrawText(s,rect, DT_VCENTER | DT_LEFT);
			
		}
		//*/
	
	}

 
	dc->BitBlt(data_area_Rect.left,data_area_Rect.top,data_area_Rect.Width(),data_area_Rect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形



	if(oldbrush != NULL)
	{
		MemDC.SelectObject(oldbrush);
	}
	if(pOldBitmap != NULL)
	{
		MemDC.SelectObject(pOldBitmap);
	}
	if(oldpen != NULL)
	{
		MemDC.SelectObject(oldpen);
	}
	if(oldFont != NULL)
	{
		MemDC.SelectObject(oldFont);
	}
	/*
	MemDC.DeleteDC(); 
	bitmap.DeleteObject();	
	pen.DeleteObject(); 
	tempFont.DeleteObject(); 
	ReleaseDC(dc); 
	*/
}

void CHeatBox_CGPDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd *data_area = GetDlgItem(IDC_BUTTON_PRINT);
	CRect IDC_BUTTON_PRINT_rect,wndRect,data_area_Rect;
	data_area->GetWindowRect(IDC_BUTTON_PRINT_rect);
	
	this->GetWindowRect(wndRect);

	data_area_Rect.left		= IDC_BUTTON_PRINT_rect.right + 10 ;
	data_area_Rect.bottom	= IDC_BUTTON_PRINT_rect.bottom;
	data_area_Rect.right	= wndRect.right * 5 / 6 -10;
	data_area_Rect.top		= wndRect.top + 30;
	
	if(point.x > data_area_Rect.left && point.x < data_area_Rect.right
		&& point.y > data_area_Rect.top && point.y < data_area_Rect.bottom)
	{
		this->m_bShowTempCurve = !m_bShowTempCurve;
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

bool CHeatBox_CGPDlg::IsAllOpend(void)
{
	for(int i = 0; i < MAX_LED_NR; i++)
	{
		if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
		{
			if(m_SwitchStatus[m_CurTestPro][i].IsOpenned  == false)
			{
				return false;
			}
		}
	}

	return true;
}


float CHeatBox_CGPDlg:: HEAT_COOL_READY_TEMP (void) 
{
	if((m_TestPara.OpenCloseGapLow[m_CurTestPro] == 0) && (m_TestPara.OpenCloseGapHigh[m_CurTestPro] == 0))
	{
		return m_TestPara.OpenTemp[m_CurTestPro] -  m_TestPara.OpenTempMinus[m_CurTestPro] - 5.0f;
	}
	else
	{
		return m_TestPara.OpenTemp[m_CurTestPro] -  m_TestPara.OpenTempMinus[m_CurTestPro] - 5.0f;
	}
}

void CHeatBox_CGPDlg::DealSwitchTest_HeatCool(void)
{
	
	CButton *button;
	int i;
	//先升后降模式


		switch(m_TestMode)
		{
		case TEST_MODE_IDLE:
			
			break;
		case TEST_MODE_FULL_UP:

#ifndef _VoltAmp_

			SaveOpenStatus();
#else

			for(i = 0; i < 3; i++)
			{
				if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
				{
					if(m_SwitchStatus[m_CurTestPro][i].IsClosing ==  false  && m_SwitchStatus[m_CurTestPro][i].IsOpenned == false)
					{
						m_SwitchStatus[m_CurTestPro][i].IsOpenned = true;
						m_SwitchStatus[m_CurTestPro][i].OpenTemp  = m_CurTemp; 
					}
					
					if(m_SwitchStatus[m_CurTestPro][i].IsClosing ==  true  && m_SwitchStatus[m_CurTestPro][i].IsOpenned == true)
					{
						m_SwitchStatus[m_CurTestPro][i].IsOpenFlash = true;
					}

				}
			}
#endif
	

			if(m_CurTemp >= (m_TestPara.OpenTemp[m_CurTestPro]  - m_TestPara.OpenTempMinus[m_CurTestPro] - 3.0f))
			{//进入线性区
				this->m_TestMode  = TEST_MODE_LINE_UP;
				m_TopTemp = m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro] + 2.0f;
				
				this->SendCmdStepPid(m_TopTemp);   

				CheckIsUsed();

			}


			break;
		case TEST_MODE_READY:
	
			//AfxMessageBox(_T("1"));
			button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
			if(button != NULL)
			{
				button->EnableWindow(false); 
			}

			//if(fabs(m_CurTemp - m_TopTemp) < TEST_MODE_READY_GAP_TEMP) //PID CONTROL
			if(m_CurTemp < HEAT_COOL_READY_TEMP() + TEST_MODE_READY_GAP_TEMP)
			{
				//AfxMessageBox(_T("2"));
				

				if(m_ClickedButtonTestStart)
				{//先按了 “开始”按钮，准备结束后就要主动进入测试状态
					//SelectStartOrNextButton(); 
					SendCmdStop();
					EnterTestStart();
					CheckIsUsed();
					break;
				}
				
				

				
				button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
				if(button != NULL)
				{
					button->EnableWindow(true); 
				}

			}
			else
			{//AfxMessageBox(_T("3"));
				button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
				if(button != NULL)
				{
					button->EnableWindow(false); 
				}

			}
		
			    if(m_ReadTempAnswer.CurTemp <( HEAT_COOL_READY_TEMP() - TEST_MODE_READY_GAP_TEMP))
				{
					if( m_ReadTempAnswer.HeatPercent == 0)
					{
						//this->SendCmdAirValvePercent(0); 
						//this->SendCmdStop();
						//this->SendCmdHeatNormalPid(HEAT_COOL_READY_TEMP()); 
						this->SendCmdHeatFullOut(m_TopTemp - TEST_MODE_READY_GAP_TEMP);
						SendCmdReadTemp();
					}
				}
				
				else if(m_ReadTempAnswer.CurTemp > (HEAT_COOL_READY_TEMP() + TEST_MODE_READY_GAP_TEMP))
				{
					if(m_ReadTempAnswer.CoolPercent == 0)
					{
						this->SendCmdAirValvePercent(50); 
					}
					
					//SendCmdCoolFullOut(this->m_TopTemp);
					SendCmdCoolStepPid(HEAT_COOL_READY_TEMP() + TEST_MODE_READY_GAP_TEMP);
					this->SendCmdReadTemp(); 
				}
				else
				{
					if(m_ReadTempAnswer.CoolPercent > 0 || m_ReadTempAnswer.HeatPercent > 0)
					{
						this->SendCmdStop();
						SendCmdReadTemp();
					
					}
					
				}

			break;
		case TEST_MODE_LINE_UP:

			button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
			if(button != NULL)
			{
				button->EnableWindow(false);
			}
#ifdef _VoltAmp_

			for(i = 0; i < 3; i++)
			{
				if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
				{
					if(m_SwitchStatus[m_CurTestPro][i].IsClosing ==  false  && m_SwitchStatus[m_CurTestPro][i].IsOpenned == false)
					{
						m_SwitchStatus[m_CurTestPro][i].IsOpenned = true;
						m_SwitchStatus[m_CurTestPro][i].OpenTemp  = m_CurTemp; 
					}
					
					if(m_SwitchStatus[m_CurTestPro][i].IsClosing ==  true  && m_SwitchStatus[m_CurTestPro][i].IsOpenned == true)
					{
						m_SwitchStatus[m_CurTestPro][i].IsOpenFlash = true;
					}

				}
			}
#else
			SaveOpenStatus();
#endif
	
			if((m_CurTemp >= (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro]) + 2.0f)
				|| IsAllOpend())
			{//进入全速降温区

				for(i = 0; i < MAX_LED_NR; i++)
				{
					if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
					{
						if((m_SwitchStatus[m_CurTestPro][i].IsClosing ==  true) && (m_SwitchStatus[m_CurTestPro][i].IsOpenned ==  false))
						{
							m_SwitchStatus[m_CurTestPro][i].OpenTemp  = INVALID_OPEN_TEMP; 
						}
						
					}
				}

				if((m_TestPara.OpenCloseGapLow[m_CurTestPro] == 0) && (m_TestPara.OpenCloseGapHigh[m_CurTestPro] == 0)
					&& (m_TestPara.CloseTempMinus[m_CurTestPro] == 0) && (m_TestPara.CloseTempPlus[m_CurTestPro] == 0))
				{//本次测试结束,进入准备区

					this->m_TestMode  = TEST_MODE::TEST_MODE_READY;
					m_Takeboard[m_CurTestPro] = true;
			
					m_ClickedButtonTestStart = false;
					switch(m_CurTestPro)
					{
					case 0:

						if(m_TestPara.TestPoint[1] == 0)
						{
							m_CurTestPro = 0;
							m_TopTemp = HEAT_COOL_READY_TEMP();
							 
							SendCmdCoolPidOut(m_TopTemp);
		
							SaveTestResult();
						
						}
						else
						{
							m_CurTestPro = 1;

							m_TopTemp = HEAT_COOL_READY_TEMP();
							this->SendCmdHeatNormalPid(m_TopTemp);  
							
						}
						break;
					case 1:

						if(m_TestPara.TestPoint[2] == 0)
						{
							m_CurTestPro = 0;
							m_TopTemp = HEAT_COOL_READY_TEMP();
							this->SendCmdHeatNormalPid(m_TopTemp);  
							
							SaveTestResult();
							
						}
						else
						{
							m_CurTestPro = 2;
							m_TopTemp = HEAT_COOL_READY_TEMP();
							this->SendCmdHeatNormalPid(m_TopTemp);  
							
						}

						break;
					case 2:

						if(m_TestPara.TestPoint[3] == 0)
						{
							m_CurTestPro = 0;
							m_TopTemp = HEAT_COOL_READY_TEMP();
							this->SendCmdHeatNormalPid(m_TopTemp);  
							
							SaveTestResult();
							
						}
						else
						{
							m_CurTestPro = 3;
							m_TopTemp = HEAT_COOL_READY_TEMP();
							this->SendCmdHeatNormalPid(m_TopTemp);  
							
						}

						break;
					case 3:

						m_CurTestPro = 0;
						m_TopTemp = HEAT_COOL_READY_TEMP();
						this->SendCmdHeatNormalPid(m_TopTemp);  
						
						SaveTestResult();

						break;	
					}

				}
				else
				{

					this->m_TestMode	= TEST_MODE_FULL_DOWN;
					m_TopTemp			= m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro]- m_TestPara.OpenCloseGapLow[m_CurTestPro] + 2.0f;
					this->SendCmdCoolFullOut(m_TopTemp);

				}

			}
			break;
		case TEST_MODE_FULL_DOWN:

#ifdef _VoltAmp_
			for(i = 0; i < 3; i++)
			{
				if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
				{
					if(m_SwitchStatus[m_CurTestPro][i].IsClosing == true && m_SwitchStatus[m_CurTestPro][i].IsClosed  == false)
					{
						m_SwitchStatus[m_CurTestPro][i].IsClosed  = true;
						m_SwitchStatus[m_CurTestPro][i].CloseTemp = m_CurTemp; 
					}

					if(m_SwitchStatus[m_CurTestPro][i].IsClosing == false && m_SwitchStatus[m_CurTestPro][i].IsClosed  == true)
					{
						m_SwitchStatus[m_CurTestPro][i].IsCloseFlash = true;
					}

					
				}
			}

#else
			SaveCloseStatus();
#endif
			if(m_CurTemp <= (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro] - m_TestPara.OpenCloseGapLow[m_CurTestPro] + 3.5f))
			{//进入线性降温区
				this->m_TestMode  = TEST_MODE_LINE_DOWN;
				m_TopTemp = m_TestPara.OpenTemp[m_CurTestPro] - m_TestPara.OpenTempMinus[m_CurTestPro] - m_TestPara.OpenCloseGapHigh[m_CurTestPro] - 2.0f;
				this->SendCmdCoolStepPid(m_TopTemp);   
			}
			break;
		case TEST_MODE_LINE_DOWN:

#ifdef _VoltAmp_
			for(i = 0; i < 3; i++)
			{
				if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
				{
					if(m_SwitchStatus[m_CurTestPro][i].IsClosing == true && m_SwitchStatus[m_CurTestPro][i].IsClosed  == false)
					{
						m_SwitchStatus[m_CurTestPro][i].IsClosed  = true;
						m_SwitchStatus[m_CurTestPro][i].CloseTemp = m_CurTemp; 
					}

					if(m_SwitchStatus[m_CurTestPro][i].IsClosing == false && m_SwitchStatus[m_CurTestPro][i].IsClosed  == true)
					{
						m_SwitchStatus[m_CurTestPro][i].IsCloseFlash = true;
					}

					
				}
			}

#else
			SaveCloseStatus();
			
#endif
			

			if(m_CurTemp < (m_TestPara.OpenTemp[m_CurTestPro] - m_TestPara.OpenTempMinus[m_CurTestPro] - m_TestPara.OpenCloseGapHigh[m_CurTestPro]))
			{//本次测试结束,进入准备区
				this->SendCmdStop(); 
				this->m_TestMode  = TEST_MODE::TEST_MODE_READY;
				m_Takeboard[m_CurTestPro] = true;

				m_ClickedButtonTestStart = false;
				switch(m_CurTestPro)
				{
				case 0:
						
					if(m_TestPara.TestPoint[1] == 0)
					{
						m_CurTestPro = 0;
						m_TopTemp = HEAT_COOL_READY_TEMP();
						this->SendCmdHeatNormalPid(m_TopTemp);  
					
						SaveTestResult();
					
					}
					else
					{
						m_CurTestPro = 1;

						m_TopTemp = HEAT_COOL_READY_TEMP();
						this->SendCmdHeatNormalPid(m_TopTemp);  
						
					}
					break;
				case 1:
					if(m_TestPara.TestPoint[2] == 0)
					{
						m_CurTestPro = 0;
						m_TopTemp = HEAT_COOL_READY_TEMP();
						this->SendCmdHeatNormalPid(m_TopTemp);  
				
						SaveTestResult();
						
					}
					else
					{
						m_CurTestPro = 2;
						m_TopTemp = HEAT_COOL_READY_TEMP();
						this->SendCmdHeatNormalPid(m_TopTemp);  
						
					}
					break;
				case 2:
					if(m_TestPara.TestPoint[3] == 0)
					{
						m_CurTestPro = 0;
						m_TopTemp = HEAT_COOL_READY_TEMP();
						this->SendCmdHeatNormalPid(m_TopTemp);  
				
						SaveTestResult();
						
					}
					else
					{
						m_CurTestPro = 3;
						m_TopTemp = HEAT_COOL_READY_TEMP();
						this->SendCmdHeatNormalPid(m_TopTemp);  
						
					}
					break;
				case 3:
					m_CurTestPro = 0;
					m_TopTemp = HEAT_COOL_READY_TEMP();
					this->SendCmdHeatNormalPid(m_TopTemp); 
				
					SaveTestResult();
					
					break;	
				}
			}
			break;
		
		}

}
void CHeatBox_CGPDlg::DealSwitchTest_CoolHeat(void)
{
	CButton *button;
	int i;
	switch(m_TestMode)
		{
		case TEST_MODE_IDLE:
			
			break;
		case TEST_MODE_FULL_UP:

			SaveCloseStatus();
			if((m_TestPara.OpenCloseGapLow[m_CurTestPro] == 0) && (m_TestPara.OpenCloseGapHigh[m_CurTestPro] == 0))
			{//使用复位温度区间进行控制
				if(m_CurTemp >= m_TopTemp)
				{//进入线性区
					this->m_TestMode  = TEST_MODE_LINE_UP;
					m_TopTemp = m_TestPara.CloseTemp[m_CurTestPro] + m_TestPara.CloseTempPlus[m_CurTestPro] +  1.0f;
					this->SendCmdStepPid(m_TopTemp);   
				}
			}
			else
			{//使用开停差控制
				if(m_CurTemp >= m_TopTemp)
				{//进入线性区
					this->m_TestMode  = TEST_MODE_LINE_UP;
					m_TopTemp = m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro] + m_TestPara.OpenCloseGapHigh[m_CurTestPro] +  2.0f;
					this->SendCmdStepPid(m_TopTemp);   
				}

			}
			break;
		case TEST_MODE_READY:

			

			//温度控制
			if(m_CurTemp <  (m_TopTemp - 0.3f))
			{
				if(m_HeatOutPercent == 0)
				{
					//this->SendCmdHeatNormalPid(m_TopTemp);
				}
			}
			else if(m_CurTemp > ( m_TopTemp + 0.2f))
			{
				if(m_CoolOutPercent != 100)
				{
						// TODO: 在此添加控件通知处理程序代码
						this->SendCmdCoolFullOut(m_TopTemp);
						
				}
			}
			else
			{
				button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
				if(button != NULL)
				{
					button->EnableWindow();
				}
				button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
				if(button != NULL)
				{
					button->EnableWindow(false);
				}
				if(m_CoolOutPercent != 0  || this->m_HeatOutPercent > 0)
				{
						char para[16];
						CString cmd;
						m_SendCmdArray.Add(SetCmd('1', CMD_STOP_OUT,para));
						
				}
				//自动进入测试状态
				if(m_ClickedButtonTestStart)
				{//先按了 “开始”按钮，准备结束后就要主动进入测试状态
					//SelectStartOrNextButton(); 
					SendCmdStop();
					EnterTestStart();
				}
				else
				{//仅按了“准备”按钮
					button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_READY);
					if(button != NULL)
					{
						button->EnableWindow(false);
					}
				}
			}
			break;
		case TEST_MODE_LINE_UP:
			//复位
			SaveCloseStatus();

			if(m_CurTemp >= (m_TestPara.CloseTemp[m_CurTestPro] + m_TestPara.CloseTempPlus[m_CurTestPro]))
			{//测试结束
				this->m_TestMode  = TEST_MODE_READY;
				
				this->m_Takeboard[ m_CurTestPro ] =true;

				m_ClickedButtonTestStart= false;

				switch(m_CurTestPro)
				{
				case 0:
					if(m_TestPara.TestPoint[1] == 0)
					{
						m_CurTestPro = 0;
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
						if(button != NULL)
						{
							button->EnableWindow(false);
						}
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
						if(button != NULL)
						{
							button->EnableWindow(true);
						}
					
						SaveTestResult();
					}
					else
					{
						m_LastTestPro = 0;
						m_CurTestPro = 1;
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
						if(button != NULL)
						{
							button->EnableWindow(true);
						}
					}
					OnBnClickedButtonTestReady();
						
					break;
				case 1:
					if(m_TestPara.TestPoint[2] == 0)
					{
						m_CurTestPro = 0;
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
						if(button != NULL)
						{
							button->EnableWindow(false);
						}
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
						if(button != NULL)
						{
							button->EnableWindow(true);
						}
					
						SaveTestResult();
						
					}
					else
					{
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
						if(button != NULL)
						{
							button->EnableWindow(true);
						}
						m_LastTestPro = 1;
						m_CurTestPro = 2;
					}

					OnBnClickedButtonTestReady();
						
					break;
				case 2:
					if(m_TestPara.TestPoint[3] == 0)
					{
						m_CurTestPro = 0;
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
						if(button != NULL)
						{
							button->EnableWindow(false);
						}
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
						if(button != NULL)
						{
							button->EnableWindow(true);
						}
						
						SaveTestResult();
					}
					else
					{
						m_LastTestPro = 2;
						m_CurTestPro = 3;
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
						if(button != NULL)
						{
							button->EnableWindow(false);
						}
						
					}
					OnBnClickedButtonTestReady();
						
					break;
				case 3:
					m_LastTestPro = 3;
					m_CurTestPro = 0;
					button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
					if(button != NULL)
					{
						button->EnableWindow(false);
					}
					button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
					if(button != NULL)
					{
						button->EnableWindow(true);
					}
			
					SaveTestResult();
					OnBnClickedButtonTestReady();
						
					break;	
				}
				//m_TopTemp = COOL_HEAT_READY_TEMP;
				//this->SendCmdCoolPidOut(m_TopTemp);   
			}
			break;
		case TEST_MODE_FULL_DOWN:
			//记录保护器状态，
			SaveOpenStatus();
			
			if(m_CurTemp <= (m_TestPara.OpenTemp[m_CurTestPro] + m_TestPara.OpenTempPlus[m_CurTestPro]  + 2.0f))
			{//进入线性降温区
				this->m_TestMode  = TEST_MODE_LINE_DOWN;
				m_TopTemp = m_TestPara.OpenTemp[m_CurTestPro] - m_TestPara.OpenTempMinus[m_CurTestPro]  - 2.0f;
				this->SendCmdCoolStepPid(m_TopTemp);   
			}
			break;
		case TEST_MODE_LINE_DOWN:
			//线性降温
			SaveOpenStatus();

			if(m_CurTemp < (m_TestPara.OpenTemp[m_CurTestPro] - m_TestPara.OpenTempMinus[m_CurTestPro] - 1.0f))
			{//降温结束,进入升温区
				for(i = 0; i < MAX_LED_NR; i++)
				{
					if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
					{
						if(m_SwitchStatus[m_CurTestPro][i].IsOpenned ==  false)
						{
							m_SwitchStatus[m_CurTestPro][i].OpenTemp  = INVALID_OPEN_TEMP; 
						}
						
					}
				}
				m_TestMode  = TEST_MODE::TEST_MODE_FULL_UP;
				if((m_TestPara.OpenCloseGapLow[m_CurTestPro] == 0) && (m_TestPara.OpenCloseGapHigh[m_CurTestPro] == 0))
				{//使用复位温度区间进行控制
					m_TopTemp	= m_TestPara.CloseTemp[m_CurTestPro] + m_TestPara.CloseTempPlus[m_CurTestPro] + 1.0f;
				}
				else
				{
					m_TopTemp	= m_TestPara.OpenTemp[m_CurTestPro] - m_TestPara.CloseTempMinus[m_CurTestPro] + m_TestPara.OpenCloseGapLow[m_CurTestPro];
				}
				SendCmdHeatFullOut(m_TopTemp);

			}
			break;
		
		}
}
void CHeatBox_CGPDlg::DealSwitchTest(void)
{
	
	if(m_TestPara.HeatCoolType[m_CurTestPro] == 0)
	{	
		DealSwitchTest_HeatCool();
	}
	else
	{//先降后升模式
		
		DealSwitchTest_CoolHeat();
	}
}

int CHeatBox_CGPDlg::SendCmdStepPid(float TopTemp)
{
	char para[16];
	CString cmd;
	
	memset(para,0,sizeof(para));

	if(this->m_TestPara.HeatSpeed == 0)
	{
		memcpy(para,&TopTemp,sizeof(TopTemp));
		para[4] = 0;
		para[6] = m_TestPara.HeatPercent;
	}
	else
	{
		short seconds = (TopTemp - m_CurTemp) * 60;
		
		seconds = seconds * 10 / m_TestPara.HeatSpeed; 
		

		memcpy(para,&TopTemp,sizeof(TopTemp));

		memcpy(&para[sizeof(TopTemp)],&seconds,sizeof(seconds));


	}


	m_SendCmdArray.Add(SetCmd('1', CMD_STEP_PID_OUT,para));
	return 0;
}

int CHeatBox_CGPDlg::SendCmdStop(void)
{
	char para[16];
	CString cmd;
	

	memset(para,0,sizeof(para));
	
	m_SendCmdArray.Add(SetCmd('1', CMD_SET_RELAY,para));

	m_SendCmdArray.Add(SetCmd('1', CMD_STOP_OUT,para));

	m_SendCmdArray.Add(SetCmd('1', CMD_AIR_VALVE,para));

	return 0;
}

int CHeatBox_CGPDlg::SendCmdReadTemp(void)
{
	char para[16];

	memset(para,0,sizeof(para));
	
	m_SendCmdArray.Add(SetCmd('1', CMD_READ_TEMP,para));

	return 0;
}

int CHeatBox_CGPDlg::SendCmdReadAdjustAll(void)
{
	char para[16];
	CString cmd;
	

	memset(para,0,sizeof(para));
	
	m_SendCmdArray.Add(SetCmd('1', CMD_READ_ADJUST_ALL,para));

	return 0;
}

int CHeatBox_CGPDlg::SendCmdSetAdjustAll(float temp)
{
	char para[16];
	CString cmd;

	memset(para,0,sizeof(para));
	memcpy(para,&temp,sizeof(temp));
	m_SendCmdArray.Add(SetCmd('1', CMD_SET_ADJUST_ALL,para));

	return 0;
}

int CHeatBox_CGPDlg::SendCmdHeatFullOut(float TopTemp)
{
	char para[16];
	CString cmd;
	
	memcpy(para,&TopTemp,sizeof(TopTemp));

	m_SendCmdArray.Add(SetCmd('1', CMD_FULL_OUT,para));

	return 0;
}

int CHeatBox_CGPDlg::SendCmdHeatNormalPid(float TopTemp)
{
	char para[16];

	memcpy(para,&TopTemp,sizeof(TopTemp));

	m_SendCmdArray.Add(SetCmd('1', CMD_NORMAL_PID_OUT,para));
	return 0;
}


int CHeatBox_CGPDlg::SendCmdCoolPidOut(float BottomTemp)
{
	char para[16];
	CString cmd;
	
	memcpy(para,&BottomTemp,sizeof(BottomTemp));

	m_SendCmdArray.Add(SetCmd('1', CMD_COOL_PID_OUT,para));
	return 0;
}

int CHeatBox_CGPDlg::SendCmdAirValvePercent(char  percent)
{
	char para[16];
	CString cmd;
	
	memset(para,0,sizeof(para));
	para[0] = percent;

	m_SendCmdArray.Add(SetCmd('1', CMD_AIR_VALVE,para));
	return 0;
}


int CHeatBox_CGPDlg::SendCmdCoolFullOut(float BottomTemp)
{
	char para[16];
	CString cmd;
	
	para[0] = 1;
	
	m_SendCmdArray.Add(SetCmd('1', CMD_SET_RELAY,para));

	memcpy(para,&BottomTemp,sizeof(BottomTemp));

	m_SendCmdArray.Add(SetCmd('1', CMD_COOL_FULL_OUT,para));

	this->SendCmdAirValvePercent(50); 
	return 0;
}
int CHeatBox_CGPDlg::SendCmdCoolStepPid(float BottomTemp)
{
	char para[16];
	CString cmd;

	memset(para,0,sizeof(para)); 
	
	BottomTemp -= 1.0f;

	
	
	if(this->m_TestPara.CoolSpeed == 0)
	{
		
		memcpy(para,&BottomTemp,sizeof(BottomTemp));	
		memcpy(&para[sizeof(BottomTemp) + sizeof(short)],&m_TestPara.CoolPercent,sizeof(m_TestPara.CoolPercent));
	}
	else
	{
		short seconds = fabs((BottomTemp - m_CurTemp)  * 60) ;
		
		seconds = seconds * 10 / m_TestPara.CoolSpeed;
		memcpy(para,&BottomTemp,sizeof(BottomTemp));	
		memcpy(&para[sizeof(BottomTemp)],&seconds,sizeof(seconds));

	}
	
		

	m_SendCmdArray.Add(SetCmd('1', CMD_COOL_STEP_PID_OUT,para));

	return 0;
}
void CHeatBox_CGPDlg::SaveTestResult(void)
{
	m_ClickedButtonTestStart = false;

	SaveTestRecord_Hex();

	
	SaveExcel();


	for(int i = 0; i < sizeof(m_Barcode) / sizeof(m_Barcode[0]); i++)
	{
		m_Barcode[i] = _T("");
	}

	m_CurScannerID = 0;

	this->ShowBarcode();
}


void CHeatBox_CGPDlg::SaveExcel(void)
{
	CString  file_name;
	CTime cur = cur.GetCurrentTime();
	bool bDeltaType = false;


	CFile RecFile;

	{
		CString dir;
		dir =  _T("D:\\测试记录");

			if(!PathIsDirectory(dir))
			{
				if(::CreateDirectory(dir,NULL) == false)
				{
					AfxMessageBox(_T("建立目录失败!"));
				}
			}

		dir.Format( _T("D:\\测试记录\\%04d"),cur.GetYear());
		if(!PathIsDirectory(dir))
			{
				if(::CreateDirectory(dir,NULL) == false)
				{
					AfxMessageBox(_T("建立目录失败!"));
				}
			}

		dir.Format( _T("D:\\测试记录\\%04d\\%02d"),cur.GetYear(),cur.GetMonth());
		if(!PathIsDirectory(dir))
			{
				if(::CreateDirectory(dir,NULL) == false)
				{
					AfxMessageBox(_T("建立目录失败!"));
				}
			}

		dir.Format( _T("D:\\测试记录\\%04d\\%02d\\%02d"),cur.GetYear(),cur.GetMonth(),cur.GetDay());
		if(!PathIsDirectory(dir))
		{
			if(::CreateDirectory(dir,NULL) == false)
			{
				AfxMessageBox(_T("建立目录失败!"));
			}
		}

		file_name.Format(_T("\\%02d日%02d时%02d分%02d秒.csv"),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond());	
		file_name = dir + file_name; 
		if(RecFile.Open(file_name,CFile::modeReadWrite | CFile::modeCreate))
		{
unsigned short UNICODE_FILE_HEAR = 0xFEFF;
unsigned short CSV_COMMMA		 = 0x0009;
			RecFile.SeekToBegin();
			RecFile.Write(&UNICODE_FILE_HEAR,2);

			TCHAR buf[1024];
			TCHAR buf1[1024];

			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("公司名称:%c %s\r\n") ,CSV_COMMMA, m_TestPara.Productor);
			}
			else
			{
				_stprintf(buf,_T("Productor:%c %s\r\n") ,CSV_COMMMA, m_TestPara.Productor);
			}
			RecFile.Write((char*)buf,_tcslen(buf)*sizeof(TCHAR)); 

			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("批号:%c %s\r\n") ,CSV_COMMMA, m_TestPara.ProductorNo);
			}
			else
			{
				_stprintf(buf,_T("Product No.:%c %s\r\n") ,CSV_COMMMA, m_TestPara.ProductorNo);
			}

			RecFile.Write((char*)buf,_tcslen(buf)*sizeof(TCHAR)); 



			for(int i = 0; i < sizeof(G_NormalConfigPara.ParaName); i++)
			{
				if(G_NormalConfigPara.ParaName[i] == '.')
				{
					G_NormalConfigPara.ParaName[i] = 0;
					break;
				}
			}
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("产品规格:%c %s\r\n"),CSV_COMMMA,G_NormalConfigPara.ParaName);
			}
			else
			{
				_stprintf(buf,_T("ParaName:%c %s\r\n"),CSV_COMMMA,G_NormalConfigPara.ParaName);
			}

			RecFile.Write(buf,_tcslen(buf)*sizeof(TCHAR)); 

			_stprintf(buf,_T("测试速度:%c %5.1f度/分钟\r\n"),CSV_COMMMA,(m_TestPara.TestSpeed + 1)*0.5);
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR) );

			if(m_TestPara.OpenCloseGapHigh[0] != 0 || m_TestPara.OpenCloseGapLow != 0)
			{
				bDeltaType = true;
			}
			else
			{
				bDeltaType = false;
			}
			if(m_TestPara.IsDoubleSwitch == 0)
			{
				if(G_NormalConfigPara.Language == 0)
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("产品类型:%c 单路 回差温度\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("产品类型:%c 单路 动作复位温度\r\n"),CSV_COMMMA);
					}
				}
				else
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("Switch Type:%c Single Delta\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("Switch Type::%c Single Reset\r\n"),CSV_COMMMA);
					}
					
				}
			}
			else
			{
				if(G_NormalConfigPara.Language == 0)
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("产品类型:%c 双路 回差温度\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("产品类型:%c 双路 动作复位温度\r\n"),CSV_COMMMA);
					}
					
				}
				else
				{
					if(bDeltaType)
					{
						_stprintf(buf,_T("Switch Type:%c Double Delta\r\n"),CSV_COMMMA);
					}
					else
					{
						_stprintf(buf,_T("Switch Type:%c Double Reset\r\n"),CSV_COMMMA);
					}
					
				}
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR)); 
	
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("测试时间:%c %04d/%02d/%02d %02d:%02d:%02d\r\n"),CSV_COMMMA, cur.GetYear(),cur.GetMonth(),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond());	
			}
			else
			{
				_stprintf(buf,_T("Test Time:%c %04d/%02d/%02d %02d:%02d:%02d\r\n"),CSV_COMMMA, cur.GetYear(),cur.GetMonth(),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond());	
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));


			
			
	
			float open_max_temp = -999.0f;
			float open_min_temp = 999.0f;
			float close_max_temp = -999.0f;
			float close_min_temp = 999.0f;
			float delata_max_temp = -999.0f;
			float delata_min_temp = 999.0f;

			float open_avr_temp = 0,close_avr_temp = 0;
			int total_product = 0,total_main_level = 0;
			int total_product_Openned = 0;
			int total_product_Closed = 0;

			for(int i = 0; i < MAX_LED_NR; i++)
			{
				if(this->m_SwitchStatus[0][i].IsUsed)
				{
					total_product ++;
					if(m_SwitchStatus[0][i].IsOpenned)
					{
						open_avr_temp += m_SwitchStatus[0][i].OpenTemp;
						total_product_Openned ++;
					}
					if(m_SwitchStatus[0][i].IsClosed)
					{
						close_avr_temp+= m_SwitchStatus[0][i].CloseTemp;
						total_product_Closed ++;
					}
					
					if(CheckLevelItem(m_TestPara,m_SwitchStatus[0][i],0 ) != 0)
					{
						total_main_level++;
					}
					if(m_SwitchStatus[0][i].IsOpenned)
					{
						if(m_SwitchStatus[0][i].OpenTemp < open_min_temp)
						{
							open_min_temp = m_SwitchStatus[0][i].OpenTemp;
						}
						if(m_SwitchStatus[0][i].OpenTemp > open_max_temp)
						{
							open_max_temp = m_SwitchStatus[0][i].OpenTemp;
						}

					}
					
					if(m_SwitchStatus[0][i].IsClosed)
					{
						if(m_SwitchStatus[0][i].CloseTemp < close_min_temp)
						{
							close_min_temp = m_SwitchStatus[0][i].CloseTemp;
						}
						if(m_SwitchStatus[0][i].CloseTemp > close_max_temp)
						{
							close_max_temp = m_SwitchStatus[0][i].CloseTemp;
						}
					}
					if(m_SwitchStatus[0][i].IsOpenned && m_SwitchStatus[0][i].IsClosed )
					{
						if(fabs(m_SwitchStatus[0][i].OpenTemp - m_SwitchStatus[0][i].CloseTemp) > delata_max_temp)
						{
							delata_max_temp = fabs(m_SwitchStatus[0][i].OpenTemp - m_SwitchStatus[0][i].CloseTemp);
						}

						if(fabs(m_SwitchStatus[0][i].OpenTemp - m_SwitchStatus[0][i].CloseTemp) < delata_min_temp)
						{
							delata_min_temp = fabs(m_SwitchStatus[0][i].OpenTemp - m_SwitchStatus[0][i].CloseTemp);
						}
					}

				}
			}
			
			if(total_product > 0)
			{
				if(bDeltaType)
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f%c回差最低温度:%c%5.1f\r\n"),CSV_COMMMA,open_min_temp,CSV_COMMMA,CSV_COMMMA,delata_min_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c回差最高温度:%c%5.1f\r\n"),CSV_COMMMA,open_max_temp,CSV_COMMMA,CSV_COMMMA,delata_max_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
				else
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f%c复位最低温度:%c%5.1f,\r\n"),CSV_COMMMA,open_min_temp,CSV_COMMMA,CSV_COMMMA,close_min_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c复位最高温度:%c%5.1f,\r\n"),CSV_COMMMA,open_max_temp,CSV_COMMMA,CSV_COMMMA,close_max_temp);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
			}
			else
			{
				if(bDeltaType)
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f%c回差最低温度:%c%5.1f \r\n"),CSV_COMMMA,0.0f,CSV_COMMMA,CSV_COMMMA,0.0f);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					RecFile.Write(_T("\r\n"),2 * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c回差最高温度:%c%5.1f \r\n"),CSV_COMMMA,0.0f,CSV_COMMMA,CSV_COMMMA,0.0f);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
				else
				{
					_stprintf(buf,_T("动作最低温度:%c%5.1f%c复位最低温度:%c%5.1f\r\n"),CSV_COMMMA,0,CSV_COMMMA,CSV_COMMMA,0);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
					_stprintf(buf,_T("动作最高温度:%c%5.1f%c复位最高温度:%c%5.1f\r\n"),CSV_COMMMA,0,CSV_COMMMA,CSV_COMMMA,0);
					RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				}
			}


			if(total_product > 0 )
			{
				open_avr_temp /= total_product_Openned;
				close_avr_temp/= total_product_Closed;
			}

			
			if(G_NormalConfigPara.Language == 0)
			{
				if(total_product > 0)
				{
					_stprintf(buf,_T("产品数:%c %d%c合格品:%c %d%c合格率:%c %5.1f%%\r\n"),CSV_COMMMA,
						total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,CSV_COMMMA,(float)(total_main_level*100.0f/total_product));
				}
				else
				{
					_stprintf(buf,_T("产品数:%c %d%c合格品:%c %d%c合格率:%c %5.1f%%\r\n"),CSV_COMMMA,
						total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,CSV_COMMMA,0.00);
				}
			}
			else
			{
				if(total_product > 0)
				{
					_stprintf(buf,_T("Total:%c %d,Pass:%c %d%c %5.1f%%\r\n"),CSV_COMMMA,
						total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,(float)(total_main_level*100.0f/total_product));
				}
				else
				{
					_stprintf(buf,_T("Total:%c %d%cPass:%c %d%c %5.1f%%\r\n"),CSV_COMMMA,
						total_product,CSV_COMMMA,CSV_COMMMA,total_main_level,CSV_COMMMA,0.00);
				}
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
			
			/*
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,"断开最高温度:, %5.1f,最低温度:, %5.1f,平均温度:, %5.1f\r\n",
					open_max_temp,open_min_temp,open_avr_temp);
			}
			else
			{
				_stprintf(buf,"Open Max Temp:, %5.1f,Open Min Temp:, %5.1f,Avr:, %5.1f\r\n",
					open_max_temp,open_min_temp,open_avr_temp);
			}
			RecFile.Write(buf,strlen(buf));
			
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,"复位最高温度:, %5.1f,最低温度:, %5.1f,平均温度:, %5.1f\r\n",
					close_max_temp,close_min_temp,close_avr_temp);
			}
			else
			{
				_stprintf(buf,"Close Max Temp:,%5.1f,Close Min Temp:,%5.1f,Avr,: %5.1f\r\n",
					close_max_temp,close_min_temp,close_avr_temp);
			
			}
			RecFile.Write(buf,strlen(buf));
			*/
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("测试参数\r\n"));
			}
			else
			{
				_stprintf(buf,_T("Test Para\r\n"));
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
			
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("断开温度:%c %5.1f%c负偏差:%c %5.1f%c正偏差:%c %5.1f\r\n"),CSV_COMMMA,
					m_TestPara.OpenTemp[0],CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenTempMinus[0],CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenTempPlus[0]);
			}
			else
			{
				_stprintf(buf,_T("Open Temp:%c %5.1f%cGap -:%c %5.1f%cGap +:%c %5.1f\r\n"),CSV_COMMMA,
					m_TestPara.OpenTemp[0],CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenTempMinus[0],CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenTempPlus[0]);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
			
			if(G_NormalConfigPara.Language == 0)
			{
				_stprintf(buf,_T("%c%c回差下限:%c %5.1f%c回差上限:%c %5.1f\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenCloseGapLow[0],CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenCloseGapHigh[0]);
			}
			else
			{
				_stprintf(buf,_T("%c%cClose Gap Min:%c %5.1f,Max:%c %5.1f\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenCloseGapLow[0],CSV_COMMMA,CSV_COMMMA,m_TestPara.OpenCloseGapHigh[0]);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
	
			
			if(G_NormalConfigPara.Language == 0)
			{
				if(bDeltaType)
				{
					_stprintf(buf,_T("编号 %c动作温度%c回差温度%c闪动%c判定结果\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
				}
				else
				{
					_stprintf(buf,_T("编号 %c动作温度%c复位温度%c闪动%c判定结果\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
				}
			}
			else
			{
				_stprintf(buf,_T("Nr. %cOpen,Close,Level\r\n"),CSV_COMMMA);
			}
			RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));

			for(int i = 0; i < MAX_LED_NR; i++)
			{
				if(m_SwitchStatus[0][i].IsUsed == false)
				{
					continue;
				}

				if(MAX_LED_NR == 192)
				{
					if((i % 12) < 9)
					{
						_stprintf(buf,_T("%02d%d,"),i/12,(i%12)+1 );				
					}
					else if((i % 12) == 9)
					{
						_stprintf(buf,_T("%02d0,"),i/12 + 1);				
					}
					else if((i % 12) == 10)
					{
						_stprintf(buf,_T("%02dA,"),i/12 + 1);				
					}
					else if((i % 12) == 11)
					{
						_stprintf(buf,_T("%02dB,"),i/12 +1);				
					}
				}
				else
				{
					_stprintf(buf,_T("%3d%c"),i+1,CSV_COMMMA); 
				}
				
				if(bDeltaType)
				{
					if(m_SwitchStatus[0][i].OpenTemp >= m_SwitchStatus[0][i].CloseTemp)
					{
						TCHAR buf2[100];
						memset(buf1,0,sizeof(buf1));
						if(m_SwitchStatus[0][i].IsOpenned)
						{
							_stprintf(buf2,_T("%5.1f%c"),m_SwitchStatus[0][i].OpenTemp,CSV_COMMMA);
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没断开"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);

						if(m_SwitchStatus[0][i].IsClosed)
						{
							_stprintf(buf2,_T("%5.1f%c"),m_SwitchStatus[0][i].OpenTemp - m_SwitchStatus[0][i].CloseTemp,CSV_COMMMA );
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没复位"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);

						
					}
					else
					{
						TCHAR buf2[100];
						memset(buf1,0,sizeof(buf1));
						if(m_SwitchStatus[0][i].IsOpenned)
						{
							_stprintf(buf2,_T("%5.1f%c"),m_SwitchStatus[0][i].OpenTemp,CSV_COMMMA);
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没断开"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);

						if(m_SwitchStatus[0][i].IsClosed)
						{
							_stprintf(buf2,_T("%5.1f%c"),m_SwitchStatus[0][i].CloseTemp - m_SwitchStatus[0][i].OpenTemp,CSV_COMMMA  );
						}
						else
						{
							_stprintf(buf2,_T("%s%c"),_T("没复位"),CSV_COMMMA);
						}
						_tcscat(buf1,buf2);
					}
			
				}
				else
				{
					_stprintf(buf1,_T("%5.1f%c%5.1f"),
						m_SwitchStatus[0][i].OpenTemp,CSV_COMMMA,
						m_SwitchStatus[0][i].CloseTemp);
				}
				_tcscat(buf,buf1);
				
				if(m_SwitchStatus[0][i].IsOpenFlash || m_SwitchStatus[0][i].IsCloseFlash)
				{
					_stprintf(buf1,_T("%cF"),CSV_COMMMA);
					_tcscat(buf,buf1);
				}
				else
				{
					_stprintf(buf1,_T("%c "),CSV_COMMMA);
					_tcscat(buf,buf1);
				}

				if(m_SwitchStatus[0][i].IsUsed)
				{
					_stprintf(buf1,_T("%c%s"),CSV_COMMMA,m_Barcode[i]);
					_tcscat(buf,buf1);
				}
				
				if(m_SwitchStatus[0][i].IsUsed)
				{
					if(CheckLevelItem(m_TestPara,m_SwitchStatus[0][i],0 ) != 0)
					{
						_stprintf(buf1,_T("%c合格 \r\n"),CSV_COMMMA);
						_tcscat(buf,buf1);
					}
					else
					{
						_stprintf(buf1,_T("%c不合格 \r\n"),CSV_COMMMA);
						_tcscat(buf,buf1);
					}


				}
				else
				{
					_stprintf(buf1,_T("%c --- \r\n"),CSV_COMMMA);
					_tcscat(buf,buf1);
				}

				
				RecFile.Write(buf,_tcslen(buf) * sizeof(TCHAR));
				
			}
		}
	}

	}
void CHeatBox_CGPDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	
}

void CHeatBox_CGPDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	
}

void CHeatBox_CGPDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	
}

void CHeatBox_CGPDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	
}

void CHeatBox_CGPDlg::OnBnClickedButtonDataManage()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CHeatBox_CGPDlg::OnBnClickedButtonAdjust()
{
	// TODO: 在此添加控件通知处理程序代码

	

	CNumInputDlg dlg;
	switch(::G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = _T("温度修正");
		dlg.m_Msg[0] =_T("温度偏差"); 
		dlg.m_Msg[1] =_T("");//"降温偏差"; 
		dlg.m_Msg[2] =_T(""); 
		break;
	case 1:
		dlg.m_Title = _T("Temp Adjust");
		dlg.m_Msg[0] =_T("Temp Adjust"); 
		dlg.m_Msg[1] =_T("");//"Cool Adjust"; 
		dlg.m_Msg[2] =_T(""); 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		::G_NormalConfigPara.AdjustTemp  = _wtof(buf);

		this->SaveConfig();

		this->SendCmdSetAdjustAll( G_NormalConfigPara.AdjustTemp );

		this->UpdateData(false); 
	}
}

void CHeatBox_CGPDlg::OnBnClickedButtonTestNext()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonTestStart();

	
}

void CHeatBox_CGPDlg::SelectStartOrNextButton(void)
{
	CButton *button;
	switch(m_CurTestPro)
				{
				case 0:
					if(this->m_Takeboard[0] == false)
					{
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
						if(button != NULL)
						{
							button->EnableWindow();
						}
						m_CurTestPro = 0;
					}
					else
					{
						if(m_TestPara.TestPoint[1] == 0)
						{
							button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
							if(button != NULL)
							{
								button->EnableWindow();
							}
							m_CurTestPro = 0;
						}
						else
						{
							button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
							if(button != NULL)
							{
								button->EnableWindow();
							}
						}
					}
					break;
				case 1:
						if(m_TestPara.TestPoint[2] == 0)
						{
							button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
							if(button != NULL)
							{
								button->EnableWindow();
							}
							m_CurTestPro = 0;
						}
						else
						{
							button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
							if(button != NULL)
							{
								button->EnableWindow(); 
							}
						}
					
					break;
				case 2:
						if(m_TestPara.TestPoint[3] == 0)
						{
							button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
							if(button != NULL)
							{
								button->EnableWindow();
							}
							m_CurTestPro = 0;
						}
						else
						{
							button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_NEXT);
							if(button != NULL)
							{
								button->EnableWindow();
							}
							m_CurTestPro = 0;
						}
					
					break;
				case 3:
						button = (CButton *)this->GetDlgItem(IDC_BUTTON_TEST_START);
						if(button != NULL)
						{
							button->EnableWindow();
						}
					
					break;


				}
}


void CHeatBox_CGPDlg::SendCmdReadAdjustLow(void)
{
	char para[16];
	CString cmd;
	

	memset(para,0,sizeof(para));

	m_SendCmdArray.Add(SetCmd('1', CMD_READ_ADJUST_LOW,para));


}

void CHeatBox_CGPDlg::SendCmdReadAdjustHigh(void)
{
	char para[16];
	CString cmd;
	

	memset(para,0,sizeof(para));

	m_SendCmdArray.Add(SetCmd('1', CMD_READ_ADJUST_HIGH,para));

}

void CHeatBox_CGPDlg::SendCmdReadPidT(void)
{
	char para[16];
	CString cmd;
	

	memset(para,0,sizeof(para));

	m_SendCmdArray.Add(SetCmd('1', CMD_READ_PID_T,para));

}
void CHeatBox_CGPDlg::OnBnClickedButtonLanguage()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void CHeatBox_CGPDlg::OnBnClickedButtonMyLanguage()
{
	// TODO: 在此添加控件通知处理程序代码

	G_NormalConfigPara.Language ++;
	if(G_NormalConfigPara.Language > 1)
	{
		G_NormalConfigPara.Language = 0;
	}
	::G_NormalConfigPara.Language = G_NormalConfigPara.Language;
	::G_MainDlg->SaveConfig();  
	
	SelectLanguage();

}

void CHeatBox_CGPDlg::SelectLanguage()
{
	CWnd *wnd;
	switch(G_NormalConfigPara.Language)
	{
	case 0://Chinese
		wnd = GetDlgItem(IDC_BUTTON_TEST_PARA);
		wnd->SetWindowTextW(_T("测试参数")); 

		wnd = GetDlgItem(IDC_BUTTON_SYS_CONFIG);
		wnd->SetWindowTextW(_T("系统配置"));

		wnd = GetDlgItem(IDC_BUTTON_DEBUG);
		wnd->SetWindowTextW(_T("调试维护")); 

		wnd = GetDlgItem(IDC_BUTTON_ADJUST);
		wnd->SetWindowTextW(_T("温度修正"));
		
		wnd = GetDlgItem(IDOK);
		wnd->SetWindowTextW(_T("退出系统"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_READY);
		wnd->SetWindowTextW(_T("测试准备"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_START);
		wnd->SetWindowTextW(_T("测试开始"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_STOP);
		wnd->SetWindowTextW(_T("中止"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_NEXT);
		wnd->SetWindowTextW(_T("下一步"));

		wnd = GetDlgItem(IDC_BUTTON_PRINT);
		wnd->SetWindowTextW(_T("分检打印"));

		wnd = GetDlgItem(IDC_BUTTON_DEL_BARCODE);
		wnd->SetWindowTextW(_T("删除选中的条形码"));
		break;

	case 1://English
		wnd = GetDlgItem(IDC_BUTTON_TEST_PARA);
		wnd->SetWindowTextW(_T("TEST PARA")); 

		wnd = GetDlgItem(IDC_BUTTON_SYS_CONFIG);
		wnd->SetWindowTextW(_T("SYS CONF.")); 
		
		wnd = GetDlgItem(IDC_BUTTON_DEBUG);
		wnd->SetWindowTextW(_T("DEBUG")); 

		wnd = GetDlgItem(IDC_BUTTON_ADJUST);
		wnd->SetWindowTextW(_T("TEMP ADJ."));

		wnd = GetDlgItem(IDOK);
		wnd->SetWindowTextW(_T("EXIT"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_READY);
		wnd->SetWindowTextW(_T("READY"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_START);
		wnd->SetWindowTextW(_T("START"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_STOP);
		wnd->SetWindowTextW(_T("STOP"));

		wnd = GetDlgItem(IDC_BUTTON_TEST_NEXT);
		wnd->SetWindowTextW(_T("NEXT"));

		wnd = GetDlgItem(IDC_BUTTON_PRINT);
		wnd->SetWindowTextW(_T("PRINT"));

		wnd = GetDlgItem(IDC_BUTTON_DEL_BARCODE);
		wnd->SetWindowTextW(_T("Del selected barcode"));
		break;
	}

	for(int i = 0; i < 6; i++)
	{
		DrawVoltAmp(i);
	}
	DrawTempCurve();
	DrawTempControlInfo();
}
void CHeatBox_CGPDlg::DrawVoltAmp(int ID)
{
#ifndef _VoltAmp_
	return;
#endif
	CFile log_file;



	if(!this->IsWindowVisible()) 
	{
		return;
	}
	

	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 CBrush NotUsed_groundbrush(COLORREF(RGB(192,192,192)));
	 
	 CBrush groundbrush(COLORREF(RGB(0,0,0)));
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
	
	CWnd *data_area = GetDlgItem(IDOK);
	CRect wndRect,data_area_Rect;
	data_area->GetWindowRect(data_area_Rect);

	ScreenToClient(&data_area_Rect); 

	this->GetClientRect(wndRect);

	nWidth	= (wndRect.right - wndRect.left);
	nHeight = (wndRect.bottom - data_area_Rect.bottom) / 4;
	data_area_Rect.bottom = wndRect.bottom - nHeight;

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
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
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
	

	
	int item_Space			=10; 
	int	item_NR_PER_LINE	=6;
	int item_LINES			=1;

	int item_W = nWidth  / item_NR_PER_LINE - item_Space;
	int item_H = nHeight / item_LINES       - item_Space;//Ellipse_W;
	

	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷
	
	//oldbrush=MemDC.SelectObject(&groundbrush);

	MemDC.Rectangle(0,0, item_W,item_H);

	CFont tempFont;
	CFont *oldFont;
	tempFont.CreatePointFont(160,_T("宋体"),&MemDC);
	oldFont = MemDC.SelectObject(&tempFont);

	MemDC.SetBkColor(RGB(0,0,0));
	MemDC.SetTextColor(RGB(0,255,0));


	CString Title[6][2]= 
	{	
		{_T("电压A"),_T("Volt(A)")},
		{_T("电流A"),_T("Amp(A)")},
		
		{_T("电压B"),_T("Volt(B)")},
		{_T("电流B"),_T("Amp(B)")},
		
		{_T("电压C"),_T("Volt(C)")},
		{_T("电流C"),_T("Amp(C)")},

	};

	MemDC.TextOut(0,0,Title[ID][G_NormalConfigPara.Language]);

	tempFont.DeleteObject(); 
	tempFont.CreatePointFont(400,_T("宋体"),&MemDC);
	MemDC.SelectObject(&tempFont);

	MemDC.SetBkColor(RGB(0,0,0));
	MemDC.SetTextColor(RGB(255,0,0));
	CString s;
	switch(ID)
	{
	case 0:
		s.Format(_T("%5.1f"),m_Volt[0]); 
		break;
	case 2:
		s.Format(_T("%5.1f"),m_Volt[1]); 
		break;
	case 4:
		s.Format(_T("%5.1f"),m_Volt[2]); 
		break;
	case 1:
		s.Format(_T("%5.1f"),m_Amp[0]); 
		break;

	case 3:
		s.Format(_T("%5.1f"),m_Amp[1]); 
		break;

	case 5:
		s.Format(_T("%5.1f"),m_Amp[2]); 
		break;


	}
	MemDC.TextOut(10,50,s);


	dc->BitBlt((item_W + item_Space)*ID,data_area_Rect.bottom,item_W ,item_H,&MemDC,0,0,SRCCOPY); 	//显示原图形
	this->UpdateData(false); 


	MemDC.SelectObject(oldbrush);

	if(NotUsed_groundbrush.GetSafeHandle () != NULL)
	{
		NotUsed_groundbrush.DeleteObject(); 
	}
	if(groundbrush.GetSafeHandle () != NULL)
	{
		groundbrush.DeleteObject(); 
	}
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

void CHeatBox_CGPDlg::SaveOpenStatus(void)
{
	int i;
	
	SWITCH_TEST_RESULT SwitchStatus_bak[MAX_LED_NR];
	for(i = 0; i < MAX_LED_NR; i++)
	{
		SwitchStatus_bak[i] = m_SwitchStatus[m_CurTestPro][i];
	}

	
	for(i = 0; i < MAX_LED_NR; i++)
	{
		if(m_SwitchStatus[m_CurTestPro][i].IsUsed)
		{
			if(m_TestPara.IsNormalOpen == 0)  
			{//常闭
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == false && m_SwitchStatus[m_CurTestPro][i].IsOpenned == false )
				{
					m_SwitchStatus[m_CurTestPro][i].IsOpenned  = true;
					m_SwitchStatus[m_CurTestPro][i].OpenTemp = m_CurTemp; 

					if(m_TestPara.IsDoubleSwitch)
					{
						if(i < 64)
						{
							/*if(SwitchStatus_bak[i+64].IsClosing  == true)
							{
								m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
							}*/
							if(SwitchStatus_bak[i+64].IsOpenned)
							{
								if(abs(m_SwitchStatus[m_CurTestPro][i].OpenTemp-m_SwitchStatus[m_CurTestPro][i+64].OpenTemp) > 0.1f)
								{
									m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
								}
							}
						}
						else
						{
							/*if(SwitchStatus_bak[i-64].IsClosing  == true)
							{
								m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
							}*/
							if(SwitchStatus_bak[i-64].IsOpenned)
							{
								if(abs(m_SwitchStatus[m_CurTestPro][i].OpenTemp-m_SwitchStatus[m_CurTestPro][i-64].OpenTemp) > 0.1f)
								{
									m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
								}
							}
						}
					}
				}
				
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == true && m_SwitchStatus[m_CurTestPro][i].IsOpenned)
				{
					m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
				}
				
			}
			else
			{//常开
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == true && m_SwitchStatus[m_CurTestPro][i].IsOpenned == false )
				{
					m_SwitchStatus[m_CurTestPro][i].IsOpenned  = true;
					m_SwitchStatus[m_CurTestPro][i].OpenTemp = m_CurTemp; 
					if(m_TestPara.IsDoubleSwitch)
					{
						if(i < 64)
						{
							if(SwitchStatus_bak[i+64].IsClosing  == false )
							{
								m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
							}
						}
						else
						{
							if(SwitchStatus_bak[i-64].IsClosing  == false )
							{
								m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
							}
						}
					}
				}
				
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == false && m_SwitchStatus[m_CurTestPro][i].IsOpenned)
				{
					m_SwitchStatus[m_CurTestPro][i].IsOpenFlash   = true;
				}

			}
		}
	}
}

void CHeatBox_CGPDlg::SaveCloseStatus(void)
{
	int i;
	SWITCH_TEST_RESULT SwitchStatus_bak[MAX_LED_NR];
	

	for(i = 0; i < MAX_LED_NR; i++)
	{
		SwitchStatus_bak[i] = m_SwitchStatus[m_CurTestPro][i];
	}

	for(i = 0; i < MAX_LED_NR; i++)
	{
		if(m_SwitchStatus[m_CurTestPro][i].IsUsed && m_SwitchStatus[m_CurTestPro][i].IsOpenned)
		{
			if(this->m_TestPara.IsNormalOpen == 0)
			{//常闭
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == true && m_SwitchStatus[m_CurTestPro][i].IsClosed == false)
				{
					m_SwitchStatus[m_CurTestPro][i].IsClosed  = true;
					m_SwitchStatus[m_CurTestPro][i].CloseTemp = m_CurTemp; 
					if(m_TestPara.IsDoubleSwitch)
					{
						if(i < 64)
						{
							/*if(SwitchStatus_bak[i+64].IsClosing   == false )
							{
								m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
							}*/
							if(SwitchStatus_bak[i+64].IsClosed)
							{
								if(abs(m_SwitchStatus[m_CurTestPro][i].CloseTemp-m_SwitchStatus[m_CurTestPro][i+64].CloseTemp) > 0.1f)
								{
									m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
								}
							}
						}
						else
						{
							/*if(SwitchStatus_bak[i-64].IsClosing  == false)
							{
								m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
							}*/
							if(SwitchStatus_bak[i-64].IsClosed)
							{
								if(abs(m_SwitchStatus[m_CurTestPro][i].CloseTemp-m_SwitchStatus[m_CurTestPro][i-64].CloseTemp) > 0.1f)
								{
									m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
								}
							}
						}
					}
				}
				
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == false && m_SwitchStatus[m_CurTestPro][i].IsClosed)
				{
					m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
				}
			}
			else
			{//常开
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == false && m_SwitchStatus[m_CurTestPro][i].IsClosed == false)
				{
					m_SwitchStatus[m_CurTestPro][i].IsClosed  = true;
					m_SwitchStatus[m_CurTestPro][i].CloseTemp = m_CurTemp; 
					if(m_TestPara.IsDoubleSwitch)
					{
						if(i < 64)
						{
							if(SwitchStatus_bak[i+64].IsClosing   == true )
							{
								m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
							}
						}
						else
						{
							if(SwitchStatus_bak[i-64].IsClosing  == true )
							{
								m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
							}
						}
					}
				}
				
				if(m_SwitchStatus[m_CurTestPro][i].IsClosing == true && m_SwitchStatus[m_CurTestPro][i].IsClosed)
				{
					m_SwitchStatus[m_CurTestPro][i].IsCloseFlash   = true;
				}
			}
		}
	}
}

void CHeatBox_CGPDlg::DrawUN(CDC * pDC,CRect rect,bool U_Flag)
{
	CPen UNpen,*oldpen;
//	AfxMessageBox(_T("4"));
	UNpen.DeleteObject(); 
	UNpen.CreatePen(PS_SOLID,5, RGB(255,0,0));  		//建立画笔
	oldpen=pDC->SelectObject(&UNpen);
	if(U_Flag)
	{
		pDC->MoveTo(rect.left,		rect.top);
		pDC->LineTo(rect.left,		rect.bottom - 3);
		pDC->LineTo(rect.left+3,	rect.bottom);
		pDC->LineTo(rect.right-3,	rect.bottom);
		pDC->LineTo(rect.right,		rect.bottom -3);
		pDC->LineTo(rect.right,		rect.top);
	}
	else
	{
		pDC->MoveTo(rect.left,		rect.bottom);
		pDC->LineTo(rect.left,		rect.top + 3);
		pDC->LineTo(rect.left+3,	rect.top);
		pDC->LineTo(rect.right-3,	rect.top);
		pDC->LineTo(rect.right,		rect.top + 3);
		pDC->LineTo(rect.right,		rect.bottom);
	}
	
	pDC->SelectObject(oldpen);
	
	UNpen.DeleteObject(); 
	
}

char bin[64*1024];//D:\work\icc_avr_new\HeatBox_TM1638\bin D:\work\icc_avr_new\HeatBox_TM1638_TM7705_Interface\bin
TCHAR buf[1024];

void CHeatBox_CGPDlg::XModem_Start(int BoxNr)
{

		CListDlg dlg;

	
	dlg.m_ListItem.Add(_T("HeatBox_CGP_无晶振.hex"));
	dlg.m_ListItem.Add(_T("HeatBox_CGP_有晶振.hex"));
	

	if(dlg.DoModal() != IDOK)
	{
		return;
	}
unsigned int ID[]=
{
	IDR_HEX_HeatBox_CGP_NO_OSC,
	IDR_HEX_HeatBox_CGP_WITH_OSC,
};
	
	memset(bin,0xFF,sizeof(bin));

	
	dlg.m_SelectString.Trim(); 
	if(dlg.m_SelectString.GetLength() <= 0)
	{
		return;
	}

	if(dlg.m_iSelectItem < 0)
	{
		return;
	}

	memset(HexBuf,0,sizeof(HexBuf));
	::FreeMyData(MAKEINTRESOURCE(ID[dlg.m_iSelectItem]),_T("HEX"),0,HexBuf,&HexBufLen);


	MODBUS_RS485_CMD_PROGRAM cmd;

	this->KillTimer(1);

	this->SetTimer(1,500,NULL); 
/*
	cmd.RS485_Addr	= '1';
	cmd.para_len	= sizeof(cmd.para);
	cmd.cmd			= CMD_PROGRAM_START;
	memset(cmd.para,0,sizeof(cmd.para));
	CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,cmd.crc ); 
	this->m_SendmCmdProgram.Add(cmd);

*/

	
	memset(bin,0xFF,sizeof(bin));
	int datalen = Hex2Bin(HexBuf,HexBufLen,bin,sizeof(bin));

		
		
		
	int page = (datalen / 128);
	if((datalen % 128) > 0)
	{
		page ++;
	}

	if(page > 15*8)
	{
		AfxMessageBox(_T("文件太大,超出单片机容量!!!!!!!!!!!"));
		return;
	}

	
	for(int i= 0; i < page; i++)
	{
		memset(&cmd,0,sizeof(cmd));
		cmd.RS485_Addr	= '1';
		cmd.para_len	= sizeof(cmd.para);
		cmd.cmd			= CMD_PROGRAM_DATA;
		memcpy(&cmd.para[1],&bin[128 * i],128);
		cmd.para[0] = i;
		CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,cmd.crc); 
		this->m_SendmCmdProgram.Add(cmd);
	}
		

	
	
}

unsigned char Hex2Char(unsigned char Hex[2])
{
	unsigned char c = 0;
	if(Hex[0] >= '0' && Hex[0] <= '9')
	{
		c = 16 * (Hex[0] - '0');
	}
	else if(Hex[0] >= 'A' && Hex[0] <= 'F')
	{
		c = 16 * (Hex[0] - 'A' + 10);
	}
	else if(Hex[0] >= 'a' && Hex[0] <= 'f')
	{
		c = 16 * (Hex[0] - 'a' + 10);
	}

	if(Hex[1] >= '0' && Hex[1] <= '9')
	{
		c +=  (Hex[1] - '0');
	}
	else if(Hex[1] >= 'A' && Hex[1] <= 'F')
	{
		c +=  (Hex[1] - 'A' + 10);
	}
	else if(Hex[1] >= 'a' && Hex[1] <= 'f')
	{
		c +=  (Hex[1] - 'a' + 10);
	}
	return c;

}


int CHeatBox_CGPDlg::Hex2Bin(unsigned char *Hex,int HexBufLen ,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	unsigned int BinPos = 0;

//	CFile f;
//	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
//		f.SeekToBegin();
//		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
//		f.Close(); 

		if(HexBufLen <= 0)
		{
			return 0;
		}
		
		while(HexPos < HexBufLen)
		{
			HEX_LINE hex_line;
			int i;
			if(Hex[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = Hex[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);
				for(int j = 0; j < data_len; j++)
				{
					binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

					if(BinPos < (dataAddr + j))
					{
						BinPos = (dataAddr + j);
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	
	return BinPos;
}

int CHeatBox_CGPDlg::Hex2Bin(CString HexFile,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	int BinPos = 0;

	CFile f;
	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
		f.SeekToBegin();
		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
		f.Close(); 

		if(fileLen == 0)
		{
			return 0;
		}
		
		while(HexPos < fileLen)
		{
			HEX_LINE hex_line;
			int i;
			if(HexBuf[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = HexBuf[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);
				for(int j = 0; j < data_len; j++)
				{
					binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

					if(BinPos < (dataAddr + j))
					{
						BinPos = (dataAddr + j);
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	else
	{
		CString s = HexFile;
		s+= _T("\r\n 没有找到此文件"); 
		AfxMessageBox(s);
	}
	return BinPos;
}

CString CHeatBox_CGPDlg::CreateCurDateDirection(void)
{
	CTime cur;
	
	cur = cur.GetCurrentTime();

	
	CString sDir;
	CString s;

	sDir = _T("log");

	sDir = theAppDirectory + sDir;

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	s.Format(_T("\\%04d年"),cur.GetYear()); 
	sDir += s ;



	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	s.Format(_T("\\%02d月"),cur.GetMonth() ); 
	sDir +=  s ;



	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	s.Format(_T("\\%02d日"),cur.GetDay()); 
	sDir +=  s ;



	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	return sDir;
}

CString CHeatBox_CGPDlg::GetDateDirection(CTime t)
{
	CTime cur = t;
	
	CString sDir;
	CString s;

	sDir = _T("log");

	sDir = theAppDirectory + sDir;

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	

	s.Format(_T("\\%04d年"),cur.GetYear()); 
	sDir += s ;



	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	s.Format(_T("\\%02d月"),cur.GetMonth() ); 
	sDir +=  s ;



	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	s.Format(_T("\\%02d日"),cur.GetDay()); 
	sDir +=  s ;



	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	return sDir;
}

void	CHeatBox_CGPDlg::SaveTestRecord_Hex(void)
{
	CFile RecFile;
	CString fName;
	TEST_RECORD	record;
	
	CTimeSpan span;

	m_EndTime = m_EndTime.GetCurrentTime();

	memset(&record,0,sizeof(record));

	
	record.BeginTime = this->m_BeginTime.GetTime(); 
	record.EndTime   = this->m_EndTime.GetTime(); 

	memcpy(&record.ParaName[0],&G_NormalConfigPara.ParaName[0],sizeof(record.ParaName));  
	
	record.TestPara  = this->m_TestPara;

	memcpy(record.SwitchStatus,m_SwitchStatus,sizeof(record.SwitchStatus));
	
	record.size = sizeof(record);
	
	//保存生产记录

	CFile f;
	CString file_name;

	CString sDir;
	
	sDir = CreateCurDateDirection();

	

	file_name = G_NormalConfigPara.ParaName;

	if(file_name.GetLength() > sizeof(G_NormalConfigPara.ParaName)) 
	{
		file_name = file_name.Left(sizeof(G_NormalConfigPara.ParaName));
	}

	if(file_name.GetLength() > 5) 
	{
		file_name = file_name.Left(file_name.GetLength() - 5);
	}

	CString s;

	CTime t = m_BeginTime;

	s.Format(_T("\\%02d时%02d分%02d秒_"), t.GetHour(),t.GetMinute(),t.GetSecond());   
	file_name =	s +file_name +_T(".record"); 

	if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		f.Write(&record,sizeof(record)); 
		f.Close(); 
	}

	file_name.Replace(_T(".record"),_T(".cur")); 

	if(f.Open(sDir+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		f.Write(::G_TempRecordArray.GetData(),sizeof(TEMP_RECORD)*G_TempRecordArray.GetSize()); 
		f.Close(); 
	}
	
	///////////////////////////////
	

	file_name = _T("record.bin");
	if(f.Open(theAppDirectory+file_name, CFile::modeReadWrite | CFile::modeCreate))
	{
		
		f.SeekToBegin();
		//f.Write(&G_SwitchTestResult[m_BoxNr],sizeof(G_SwitchTestResult[m_BoxNr])); 
		f.Write(&record,sizeof(record)); 
		f.Close(); 
	}


	
}

void CHeatBox_CGPDlg::ShowBarcode(void)
{
	CString s;

	m_ListBarcode.ResetContent();

	for(int i = 0; i < 64; i++)
	{
		s.Format(_T("%02d "), i + 1);
		s += m_Barcode[i];
		this->m_ListBarcode.AddString(s);
	}

	m_ListBarcode.SetCurSel(m_CurScannerID);

}
void CHeatBox_CGPDlg::OnBnClickedButtonDelBarcode()
{
	// TODO: 在此添加控件通知处理程序代码

	m_Barcode[this->m_ListBarcode.GetCurSel()] = _T("");

	ShowBarcode();
	
	
	//m_Barcode[0] = _T("123");
	//m_Barcode[1] = _T("ABC");
	//this->SaveTestResult();

}

void CHeatBox_CGPDlg::OnLbnDblclkListBarcode()
{
	// TODO: 在此添加控件通知处理程序代码

	this->m_CurScannerID = this->m_ListBarcode.GetCurSel();
}

void CHeatBox_CGPDlg::OnLbnSelchangeListBarcode()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_CurScannerID = this->m_ListBarcode.GetCurSel();
}
