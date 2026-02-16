// HotLong_PCB.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HotLong_PCB.h"
#include "HotLong_PCBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//得到当前桌面路径
BOOL GetDesktopPath(TCHAR *pszDesktopPath)
{
#ifndef WINCE
       LPITEMIDLIST  ppidl = NULL;
      
       if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
       {
              BOOL flag =   SHGetPathFromIDList(ppidl, pszDesktopPath);
              CoTaskMemFree(ppidl);
              return flag;
       }
#endif
       return FALSE;
}    
//得到当前桌面路径
BOOL GetProgramFilePath(TCHAR *pszDesktopPath)
{
#ifndef WINCE
       LPITEMIDLIST  ppidl = NULL;
      
       if (SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES, &ppidl) == S_OK)
       {
              BOOL flag =   SHGetPathFromIDList(ppidl, pszDesktopPath);
              CoTaskMemFree(ppidl);
              return flag;
       }
#endif
       return FALSE;
} 

//函数功能：对指定文件在指定的目录下创建其快捷方式
//函数参数：
//lpszFileName    指定文件，为NULL表示当前进程的EXE文件。
//lpszLnkFileDir  指定目录，不能为NULL。
//lpszLnkFileName 快捷方式名称，为NULL表示EXE文件名。
//wHotkey         为0表示不设置快捷键
//pszDescription  备注
//iShowCmd        运行方式，默认为常规窗口

BOOL CreateFileShortcut(TCHAR* lpszFileName, TCHAR* lpszLnkFileDir, TCHAR* lpszLnkFileName,       TCHAR* lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL)
{
	    HRESULT hr = NULL;
#ifndef WINCE
       if (lpszLnkFileDir == NULL)
              return FALSE;
 

       IShellLink     *pLink;  //IShellLink对象指针
       IPersistFile   *ppf; //IPersisFil对象指针
      
       //创建IShellLink对象
       hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
       if (FAILED(hr))
              return FALSE;
      
       //从IShellLink对象中获取IPersistFile接口
       hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
       if (FAILED(hr))
       {
              pLink->Release();
              return FALSE;
       }
      
       //目标
       if (lpszFileName == NULL)
		   pLink->SetPath(_T("C:\\"));//_wpgmptr);
       else
              pLink->SetPath(lpszFileName);
      
       //工作目录
       if (lpszWorkDir != NULL)
              pLink->SetPath(lpszWorkDir);
      
       //快捷键
       if (wHotkey != 0)
              pLink->SetHotkey(wHotkey);
      
       //备注
       if (lpszDescription != NULL)
              pLink->SetDescription(lpszDescription);
      
       //显示方式
       pLink->SetShowCmd(iShowCmd);
 
 
       //快捷方式的路径 + 名称
       TCHAR szBuffer[MAX_PATH];
       if (lpszLnkFileName != NULL) //指定了快捷方式的名称
              wsprintf(szBuffer, _T("%s\\%s"), lpszLnkFileDir, lpszLnkFileName);
       else  
       {
              //没有指定名称，就从取指定文件的文件名作为快捷方式名称。
              TCHAR *pstr;
              if (lpszFileName != NULL)
                     pstr = wcsrchr(lpszFileName, '\\');
              else
                     pstr = wcsrchr(_wpgmptr, '\\');
 
              if (pstr == NULL)
              {    
                     ppf->Release();
                     pLink->Release();
                     return FALSE;
              }
              //注意后缀名要从.exe改为.lnk
              wsprintf(szBuffer, _T("%s\\%s"), lpszLnkFileDir, pstr);
              int nLen = wcslen(szBuffer);
              szBuffer[nLen - 3] = 'l';
              szBuffer[nLen - 2] = 'n';
              szBuffer[nLen - 1] = 'k';
       }
       //保存快捷方式到指定目录下
 //      WCHAR  wsz[MAX_PATH];  //定义Unicode字符串
//	   MultiByteToWideChar(CP_ACP, 0, "C:\\热保护器检测.lnk", -1, wsz, MAX_PATH);
	
     // hr = ppf->Save(wsz, TRUE);
      hr = ppf->Save(szBuffer, TRUE);

       ppf->Release();
       pLink->Release();
#endif
       return SUCCEEDED(hr);
}

int  CopyAndCreatShortCut(void)
{
#ifdef _DEBUG
	return 1;
#endif
#ifndef WINCE
       TCHAR  szPath[MAX_PATH];
	   TCHAR  szFilePath[MAX_PATH];
       CoInitialize(NULL);
 
       GetDesktopPath(szPath);
	
	   GetProgramFilePath(szFilePath);

	   wcscat_s(szFilePath,_T("\\常州市润邦电子科技有限公司"));
	   if(!PathIsDirectory(szFilePath))
		{
			::CreateDirectory(szFilePath,NULL); 
		}
	   
		wcscat_s(szFilePath,_T("\\"));
	 
	   if(::theAppDirectory.Compare(szFilePath) == 0)
	   {
		   return 0;
	   }

	   wcscat_s(szFilePath,_T("HotLong_PCB.exe"));

		CopyFile(_wpgmptr,szFilePath,false);

	   if (CreateFileShortcut(szFilePath, szPath, _T("江阴辉龙电气.lnk"), szFilePath, MAKEWORD(VK_F12, HOTKEYF_CONTROL), _T("常州市润邦电子科技有限公司 013915838598")))
	   {
		   CString s= _T("程序已经成功拷贝到计算机目录\r\n\r\n");
		   s += szFilePath;
		   s += _T("\r\n\r\n请在桌面点击快捷方式运行");
              AfxMessageBox( s );
			  exit(0);
	   }
       CoUninitialize();
#endif
       return 1;
}

// CHotLong_PCBApp

BEGIN_MESSAGE_MAP(CHotLong_PCBApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHotLong_PCBApp 构造

CHotLong_PCBApp::CHotLong_PCBApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHotLong_PCBApp 对象

CHotLong_PCBApp theApp;

CString theAppDirectory;

// CHotLong_PCBApp 初始化
CHotLong_PCBDlg	*G_MainDlg;
BOOL CHotLong_PCBApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	GetModuleFileName(NULL,theAppDirectory.GetBufferSetLength(MAX_PATH+1),MAX_PATH+1); 
	theAppDirectory.ReleaseBuffer(); 
	CString exeStr;
	exeStr = AfxGetApp()->m_pszExeName;
	exeStr += _T(".exe"); 
	theAppDirectory = theAppDirectory.Left( theAppDirectory.GetLength() - exeStr.GetLength());

	CopyAndCreatShortCut();


	CWnd* w =CWnd::FindWindow(_T("BRUCE_QIU[HOT_LONG]"),NULL);
	if(w != NULL)
	{
		w->ShowWindow(SW_SHOWMAXIMIZED);
		return false;
	}


	WNDCLASS wc;

	// Get the info for this class.
	// #32770 is the default class name for dialogs boxes.
	if(::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc))
	{

		// Change the name of the class.
		wc.lpszClassName = _T("BRUCE_QIU[HOT_LONG]");

		// Register this class so that MFC can use it.
		AfxRegisterClass(&wc);
	}
	else
	{
		AfxMessageBox(_T("GetClassInfo Error"));
	}


	CHotLong_PCBDlg dlg;
	m_pMainWnd = &dlg;
	G_MainDlg  = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
BOOL CHotLong_PCBApp::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F11:
			//if(!G_MainDlg->m_ParaDlg.IsWindowVisible())
			{
				
				G_MainDlg->m_ParaDlg.DoModal();
				
				return true;
			}
			break;
		
		case VK_F12:
			{
				CAboutDlg dlg;
				dlg.DoModal();
				return true;
			}
			break;
		
		}
	}
	return CWinApp::PreTranslateMessage(pMsg); 
}