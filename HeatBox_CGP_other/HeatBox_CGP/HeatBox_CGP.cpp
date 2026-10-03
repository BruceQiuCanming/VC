// HeatBox_CGP.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "HeatBox_CGPDlg.h"
#include "Setupapi.h"
#include "shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHeatBox_CGPApp

BEGIN_MESSAGE_MAP(CHeatBox_CGPApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHeatBox_CGPApp 构造

CHeatBox_CGPApp::CHeatBox_CGPApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHeatBox_CGPApp 对象

CHeatBox_CGPApp theApp;


// CHeatBox_CGPApp 初始化
extern CString theAppDirectory;
bool bAct = true;


//得到当前桌面路径
BOOL GetDesktopPath(TCHAR *pszDesktopPath)
{
       LPITEMIDLIST  ppidl = NULL;
      
       if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
       {
              BOOL flag =   SHGetPathFromIDList(ppidl, pszDesktopPath);
              CoTaskMemFree(ppidl);
              return flag;
       }
 
       return FALSE;
}    
//得到当前桌面路径
BOOL GetProgramFilePath(TCHAR *pszDesktopPath)
{
       LPITEMIDLIST  ppidl = NULL;
      
       if (SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAM_FILES, &ppidl) == S_OK)
       {
              BOOL flag =   SHGetPathFromIDList(ppidl, pszDesktopPath);
              CoTaskMemFree(ppidl);
              return flag;
       }
 
       return FALSE;
} 

//得到快速启动栏的路径
BOOL GetIEQuickLaunchPath(TCHAR *pszIEQueickLaunchPath)
{    
       LPITEMIDLIST  ppidl;                   
      
       if (SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &ppidl) == S_OK)
       {
              BOOL flag =   SHGetPathFromIDList(ppidl, pszIEQueickLaunchPath);
              _tcscat_s(pszIEQueickLaunchPath,sizeof(pszIEQueickLaunchPath), _T("\\Microsoft\\Internet Explorer\\Quick Launch"));
              CoTaskMemFree(ppidl);
              return flag;
       }
 
       return FALSE;
}
//得到 开始->程序组 的路径
BOOL GetProgramsPath(TCHAR *pszProgramsPath)
{
       LPITEMIDLIST  ppidl;
      
       if (SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &ppidl) == S_OK)
       {
              BOOL flag = SHGetPathFromIDList(ppidl, pszProgramsPath);
              CoTaskMemFree(ppidl);
              return flag;
       }
 
       return FALSE;
}
/*
函数功能：对指定文件在指定的目录下创建其快捷方式
函数参数：
lpszFileName    指定文件，为NULL表示当前进程的EXE文件。
lpszLnkFileDir  指定目录，不能为NULL。
lpszLnkFileName 快捷方式名称，为NULL表示EXE文件名。
wHotkey         为0表示不设置快捷键
pszDescription  备注
iShowCmd        运行方式，默认为常规窗口
*/
BOOL CreateFileShortcut(TCHAR* lpszFileName, TCHAR* lpszLnkFileDir, TCHAR* lpszLnkFileName,       TCHAR* lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL)
{
       if (lpszLnkFileDir == NULL)
              return FALSE;
 
       HRESULT hr;
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
              //wsprintf(szBuffer, _T("%s\\%s"), lpszLnkFileDir, lpszLnkFileName);
			 _stprintf(szBuffer, _T("%s\\%s"), lpszLnkFileDir, lpszLnkFileName);
       else  
       {
              //没有指定名称，就从取指定文件的文件名作为快捷方式名称。
              TCHAR *pstr;
              if (lpszFileName != NULL)
			  {
                     //pstr = wcsrchr(lpszFileName, '\\');
					 pstr = _tcschr(lpszFileName, '\\');
			  }
              else
			  {
                     //pstr = wcsrchr(_wpgmptr, '\\');
					pstr = _tcschr(_wpgmptr, '\\');
					}
              if (pstr == NULL)
              {    
                     ppf->Release();
                     pLink->Release();
                     return FALSE;
              }
              //注意后缀名要从.exe改为.lnk
              //wsprintf(szBuffer, _T("%s\\%s"), lpszLnkFileDir, pstr);
              _stprintf(szBuffer, _T("%s\\%s"), lpszLnkFileDir, pstr);
              int nLen = _tcslen(szBuffer);//wcslen(szBuffer);
              szBuffer[nLen - 3] = 'l';
              szBuffer[nLen - 2] = 'n';
              szBuffer[nLen - 1] = 'k';
       }
       //保存快捷方式到指定目录下
       //WCHAR  wsz[MAX_PATH];  //定义Unicode字符串
	   //MultiByteToWideChar(CP_ACP, 0, "C:\\热保护器检测.lnk"/*(char*)szBuffer*/, -1, wsz, MAX_PATH);
	   //MultiByteToWideChar(CP_ACP, 0, (char*)szBuffer, -1, wsz, MAX_PATH);
	
	
      //hr = ppf->Save(wsz, TRUE);
       hr = ppf->Save(szBuffer, TRUE);

       ppf->Release();
       pLink->Release();
       return SUCCEEDED(hr);
}

int  CopyAndCreatShortCut(void)
{
#ifdef _DEBUG
	//return 1;
#endif
       TCHAR  szPath[MAX_PATH];
	   TCHAR  szFilePath[MAX_PATH];
       CoInitialize(NULL);
 
       GetDesktopPath(szPath);
	
	   GetProgramFilePath(szFilePath);

	  // wcscat(szFilePath,_T("\\常州市润邦电子科技有限公司"));
	   _tcscat(szFilePath,_T("\\常州市电子科技有限公司"));
	   if(!PathIsDirectory(szFilePath))
		{
			::CreateDirectory(szFilePath,NULL); 
		}
	   
		//wcscat(szFilePath,_T("\\"));
		_tcscat(szFilePath,_T("\\"));
	 

	
	   if(::theAppDirectory.Compare(szFilePath) == 0)
	   {
		   
		   return 0;
	   }

	   //wcscat(szFilePath,_T("HeatBox_HV_128.exe"));
		_tcscat(szFilePath,_T("HeatBox_CGP.exe"));

		//CopyFile(_wpgmptr,szFilePath,false);
		CopyFile(_wpgmptr,szFilePath,false);
//压力式温控器
		
	   if (CreateFileShortcut(szFilePath, szPath, _T("HeatBox_CGP.lnk"), szFilePath, MAKEWORD(VK_F10, HOTKEYF_CONTROL), _T("常州市电子科技有限公司")))
	   {
		    CString s= _T("程序已经成功拷贝到计算机目录\r\n\r\n");
		   s += szFilePath;
		   s += _T("\r\n\r\n请在桌面点击快捷方式运行");
              AfxMessageBox( s );

			  /*
		   STARTUPINFO si = { sizeof(si) };   
			PROCESS_INFORMATION pi;   
			//TCHAR cmdline[] =_T("\\Harddisk\\CeSvr.exe");  
			BOOL bRet = ::CreateProcess (   
				szFilePath,	//cmdline,  
				NULL, //在Unicode版本中此参数不能为常量字符串，因为此参数会被修改    
				NULL,   
				NULL,   
				FALSE,   
				0,   
				NULL,   
				NULL,   
				&si,   
				&pi);  

		  */
			   CoUninitialize();
#ifndef _DEBUG
			  exit(0);
#endif
	   }
       CoUninitialize();

       return 1;
}

BOOL CHeatBox_CGPApp::InitInstance()
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
	

	//取消屏幕保护
	
	::SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &bAct, 0);
	if (bAct)
	{
//		::SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, false, 0, 0);
	}
	//取消取消电源管理，避免睡眠、待机：
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED | ES_DISPLAY_REQUIRED); 


	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    //char buf[1024];
	//GetCurrentDirectory(1024,buf);
	//theAppDirectory.Format("%s\\",buf);

	GetModuleFileName(NULL,theAppDirectory.GetBufferSetLength(MAX_PATH+1),MAX_PATH+1); 
	theAppDirectory.ReleaseBuffer(); 
	CString exeStr;
	exeStr = AfxGetApp()->m_pszExeName;
	exeStr += _T(".exe"); 
	theAppDirectory = theAppDirectory.Left( theAppDirectory.GetLength() - exeStr.GetLength());


	CopyAndCreatShortCut();

	CHeatBox_CGPDlg dlg;
	m_pMainWnd = &dlg;
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
