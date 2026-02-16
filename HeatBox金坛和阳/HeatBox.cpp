// HeatBox.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HeatBox.h"
#include "HeatBoxDlg.h"


CTime STD_TIME(2010,1,1,0,0,0);
unsigned short UNICODE_FILE_HEAR = 0xFEFF;
unsigned short CSV_COMMMA		 = 0x0009;

#ifdef _OIL_
	CString strMsgCaption = "热保护器测试系统    油槽版  ";
#elif _ASCII_
	CString strMsgCaption = "热保护器测试系统  ASCII版";	
#else
	CString strMsgCaption = _T("热保护器测试系统  综合版");
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHeatBoxApp

BEGIN_MESSAGE_MAP(CHeatBoxApp, CWinApp)
	//{{AFX_MSG_MAP(CHeatBoxApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeatBoxApp construction

CHeatBoxApp::CHeatBoxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHeatBoxApp object

CHeatBoxApp theApp;
CString theAppDirectory;
CString	G_sCompany;
CBrush	G_bkBrush;
CBrush  G_WhiteBrush;

int G_iHighHighLevelDelta = 0;

int G_iMaxLedNr = 128;
int	G_iLedLines = 13;
int	G_iLedNrPerLine = 10;
/////////////////////////////////////////////////////////////////////////////
// CHeatBoxApp initialization
bool reset = 0,switch_status = 0 ,FlashFlag = 0;
int last_switch =0;
int down_time = 0;
#define MAX_DOWN_TIME	200
#define MIN_DOWN_TIME   0
int		G_CurMaxHeatBox	=	1;

float   G_SwitchTempAdjust[6][500][2];

BOOL CHeatBoxApp::InitInstance()
{
	AfxEnableControlContainer();


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.



	G_bkBrush.CreateSolidBrush(BK_COLOR); //创建一把黄色的背景刷子
	G_WhiteBrush.CreateSolidBrush(WHITE_COLOR); //创建一把黄色的背景刷子
	

	G_sCompany = AfxGetApp()->GetProfileStringW(_T("SETTING"),_T("USER"),_T("常州市润邦电子科技有限公司")) ;

	int MaxLedNr = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("MAX LED NR"),0);

		G_iMaxLedNr = 500;
		G_iLedLines = 20;
		G_iLedNrPerLine = 25;
	
	  
	
	GetModuleFileName(NULL,theAppDirectory.GetBufferSetLength(MAX_PATH+1),MAX_PATH+1); 
	theAppDirectory.ReleaseBuffer(); 
	CString exeStr;
	exeStr = AfxGetApp()->m_pszExeName;
	exeStr += _T(".exe"); 
	theAppDirectory = theAppDirectory.Left( theAppDirectory.GetLength() - exeStr.GetLength());

	//::CopyFile( theAppDirectory + AfxGetApp()->m_pszExeName + _T(".exe"),_T("C:\\aa.exe"),false);
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

//	this->SetRegistryKey("常州市润邦电子科技有限公司");


	CWnd* w =CWnd::FindWindow(_T("BRUCEQIU"),NULL);
	if(w != NULL)
	{
		w->ShowWindow(SW_SHOWMAXIMIZED);
		return false;
	}


	WNDCLASS wc;

	// Get the info for this class.
	// #32770 is the default class name for dialogs boxes.
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);

	// Change the name of the class.
	wc.lpszClassName = _T("BRUCEQIU");

	// Register this class so that MFC can use it.
	AfxRegisterClass(&wc);


	G_CurMaxHeatBox = 1;//AfxGetApp()->GetProfileInt(_T("SETTING"),_T("MAX_HEAT_BOX"),0) + 1;	


	CHeatBoxDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
