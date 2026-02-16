
#if !defined(AFX_HttpTools_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_)
#define AFX_HttpTools_H__A8C8097B_F75A_446E_9CEB_A92F2B63F790__INCLUDED_

#ifndef _HttpTools_H_
#define _HttpTools_H_

#pragma once
//#include <iostream>
#include "afxinet.h"
 



enum HttpResult
	{
		HTTP_SUCCESS = 0, //成功
		HTTP_FAILURE = 1, //失败
		HTTP_OUTTIME = 2, //超时
	};
 
	enum HttpConnectType
	{
		NORMAL_CONNECT = INTERNET_FLAG_KEEP_CONNECTION,           //http
		SECURE_CONNECT = NORMAL_CONNECT | INTERNET_FLAG_SECURE,   //https
	};
	
	enum HttpRequestType
	{
		NORMAL_REQUEST = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, //http
		SECURE_REQUEST =  NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID, //https
	};

//namespace HttpTools

static const wchar_t* IE_AGENT = L"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)";

class HttpTools
{
	
	
 
	
 
	//使用CHttpFile来实现的http请求功能。
	/*int HttpRequest(const std::wstring& strMethod, //get or post
		const std::wstring& strUrl,  //请求地址
		const std::wstring& postData,  //发送的数据
		std::wstring& response, //回复
		const std::wstring& strAgent = IE_AGENT //agent 默认是IE  （IE_AGENT）
		);
		*/
public:

	static int HttpRequest(CString strMethod, //get or post
		CString strUrl,  //请求地址
		CString postData,  //发送的数据
		CString response, //回复
		CString strAgent = IE_AGENT, //agent 默认是IE  （IE_AGENT）
		bool IsOkMsg = false
		);

		
};

#endif
#endif