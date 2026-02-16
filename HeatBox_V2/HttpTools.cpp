#include "StdAfx.h"
#include "HttpTools.h"
#include "ListDlg.h"

char strRawResponse[1024*1024];

int HttpTools::HttpRequest(CString strMethod, 
	CString strUrl, 
	CString postData, 
	CString response,
	CString strAgent,
	bool IsOkMsg)
{
	
    CString strServer;
    CString strObject;
    DWORD dwServiceType;
    INTERNET_PORT nPort;
	//response.clear();
	response = "";
 
	
 
	//先解析一下url
	BOOL bParseUrl = AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
 
	if(AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
	{
		//if(IsOkMsg)
		{
			AfxMessageBox(_T("连接 MES 系统失败 1 \r\n") + strUrl,MB_ICONQUESTION);
		}
		return HTTP_FAILURE;
	}

	CInternetSession *pSession    = new CInternetSession(strAgent);
	CHttpConnection  *pConnection = NULL;
	CHttpFile        *pHttpFile   = NULL;
	
	CString return_string;

    try
    {


		//创建一个http链接
        pConnection = pSession->GetHttpConnection(strServer,
            dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,
            nPort);

		//开始一个http请求，映射成HttpFile
        pHttpFile = pConnection->OpenRequest(strMethod, strObject,
            NULL, 1, NULL, NULL,
            (dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));
	 
        //DWORD dwFlags;
        //m_pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
        //dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
        //set web server option
        //m_pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
		
		//pHttpFile->AddRequestHeaders(L"Accept: *,*//*" );
		//pHttpFile->AddRequestHeaders(L"Accept-Language: zh-cn");
		//pHttpFile->AddRequestHeaders(L"Content-Type: application/x-www-form-urlencoded");
		//pHttpFile->AddRequestHeaders(L"Accept-Encoding: gzip, deflate");

		
		
		//发送请求
        pHttpFile->SendRequest(NULL, 0, (LPVOID)postData.GetBuffer(), postData.GetLength() * sizeof(wchar_t));
		postData.ReleaseBuffer();
;
        char szChars[1024] = {0};
        
        UINT nReaded = 0;
		memset(strRawResponse,0,sizeof(strRawResponse));
        while ((nReaded = pHttpFile->Read((void*)szChars, 1024)) > 0)
        {
			for(int i = 0; i < nReaded; i++)
			{
				strRawResponse[i] = szChars[i];
			}
        }
 
		// 把回复结果转为unicode编码，大多数情况下是需要这么做的
		int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strRawResponse, -1, NULL, 0);
		WCHAR *pUnicode = new WCHAR[unicodeLen];
		memset(pUnicode,0,(unicodeLen)*sizeof(wchar_t));
 
		MultiByteToWideChar(CP_UTF8,0,strRawResponse,-1, pUnicode,unicodeLen);
	//	std::wstring unicodeRsp;
	//	unicodeRsp.assign(pUnicode, unicodeLen);

		return_string = strRawResponse;
		

		delete []pUnicode;
		pUnicode = NULL;
		
 
		if(NULL != pHttpFile)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if(NULL != pConnection)
		{
			pConnection->Close();
			delete pConnection;
			pConnection = NULL;
		}
		if(NULL != pSession)
		{
			pSession->Close();
			delete pSession;
			pSession = NULL;
		}
	}
    catch (CInternetException* e)
    {
		if(NULL != pHttpFile)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if(NULL != pConnection)
		{
			pConnection->Close();
			delete pConnection;
			pConnection = NULL;
		}
		if(NULL != pSession)
		{
			pSession->Close();
			delete pSession;
			pSession = NULL;
		}
 
 
        DWORD dwErrorCode = e->m_dwError;
		TCHAR msg[1000];
		e->GetErrorMessage(msg,1000);
        e->Delete();
 
		AfxMessageBox(msg);

        DWORD dwError = GetLastError();
 
        if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
        {
			//if(IsOkMsg)
			{
				AfxMessageBox(_T("连接 MES 系统失败 2 \r\n") + strUrl,MB_ICONQUESTION);
			}
            return HTTP_OUTTIME;
        }
        else
        {
			//if(IsOkMsg)
			{
				AfxMessageBox(_T("连接 MES 系统失败 3 \r\n") + strUrl,MB_ICONQUESTION);
			}
            return HTTP_FAILURE;
        }
    }

	if(return_string.Find(_T("OK;OK")) > 0)
	{
		if(IsOkMsg)
		{
			AfxMessageBox(_T("成功连接 MES 系统 \r\n") + strUrl);
		}
		
		/*CListDlg dlg_list;

		dlg_list.m_Tiltle = _T("成功连接 MES 系统 \r\n");
	
		dlg_list.m_ListItem.Add(strUrl);
		
		
		if(dlg_list.DoModal() != IDOK)
		{
			//return;
		}*/

		return HTTP_SUCCESS;
	}
	else 
	{
		//if(IsOkMsg)
		{
			AfxMessageBox(_T("连接 MES 系统失败 4 \r\n") + strUrl,MB_ICONQUESTION);
		}
		return HTTP_FAILURE;
	}
	
    return HTTP_SUCCESS;



}

 /*
int HttpTools::HttpRequest(const std::wstring& strMethod, 
	const std::wstring& strUrl, 
	const std::wstring& postData, 
	std::wstring& response,
	const std::wstring& strAgent)

{
	
    CString strServer;
    CString strObject;
    DWORD dwServiceType;
    INTERNET_PORT nPort;
	//response.clear();
	response = "";
 
	/*
 
	//先解析一下url
	BOOL bParseUrl = AfxParseURL(strUrl.c_str(), dwServiceType, strServer, strObject, nPort);
 
	if(AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)
	{
		return HTTP_FAILURE;
	}

	CInternetSession *pSession    = new CInternetSession(strAgent.c_str());
	CHttpConnection  *pConnection = NULL;
	CHttpFile        *pHttpFile   = NULL;
	
	CString return_string;

    try
    {


		//创建一个http链接
        pConnection = pSession->GetHttpConnection(strServer,
            dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,
            nPort);

		//开始一个http请求，映射成HttpFile
        pHttpFile = pConnection->OpenRequest(strMethod.c_str(), strObject,
            NULL, 1, NULL, NULL,
            (dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));
	 
        //DWORD dwFlags;
        //m_pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
        //dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
        //set web server option
        //m_pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
		
		//pHttpFile->AddRequestHeaders(L"Accept: *,*//*" );
		//pHttpFile->AddRequestHeaders(L"Accept-Language: zh-cn");
		//pHttpFile->AddRequestHeaders(L"Content-Type: application/x-www-form-urlencoded");
		//pHttpFile->AddRequestHeaders(L"Accept-Encoding: gzip, deflate");

		//发送请求
        pHttpFile->SendRequest(NULL, 0, (LPVOID)postData.data(), postData.length() * sizeof(wchar_t));
;
        char szChars[1024] = {0};
        std::string strRawResponse;
        UINT nReaded = 0;
        while ((nReaded = pHttpFile->Read((void*)szChars, 1024)) > 0)
        {
            strRawResponse.append(szChars, nReaded);
        }
 
		// 把回复结果转为unicode编码，大多数情况下是需要这么做的
		int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, NULL, 0);
		WCHAR *pUnicode = new WCHAR[unicodeLen];
		memset(pUnicode,0,(unicodeLen)*sizeof(wchar_t));
 
		MultiByteToWideChar(CP_UTF8,0,strRawResponse.c_str(),-1, pUnicode,unicodeLen);
		std::wstring unicodeRsp;
		unicodeRsp.assign(pUnicode, unicodeLen);

		return_string = *pUnicode;
		

		delete []pUnicode;
		pUnicode = NULL;
		
 
		if(NULL != pHttpFile)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if(NULL != pConnection)
		{
			pConnection->Close();
			delete pConnection;
			pConnection = NULL;
		}
		if(NULL != pSession)
		{
			pSession->Close();
			delete pSession;
			pSession = NULL;
		}
	}
    catch (CInternetException* e)
    {
		if(NULL != pHttpFile)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if(NULL != pConnection)
		{
			pConnection->Close();
			delete pConnection;
			pConnection = NULL;
		}
		if(NULL != pSession)
		{
			pSession->Close();
			delete pSession;
			pSession = NULL;
		}
 
 
        DWORD dwErrorCode = e->m_dwError;
        e->Delete();
 
        DWORD dwError = GetLastError();
 
        if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
        {
            return HTTP_OUTTIME;
        }
        else
        {
            return HTTP_FAILURE;
        }
    }

	if(return_string.Find(_T("OK;OK")) > 0)
	{
		AfxMessageBox(_T("Find(_T(\"OK;OK\")) > 0"));
	}
	else 
	{
		AfxMessageBox(_T("not Find(_T(\"OK;OK\")) > 0"));
	}
	
    return HTTP_SUCCESS;


return 0;
}*/