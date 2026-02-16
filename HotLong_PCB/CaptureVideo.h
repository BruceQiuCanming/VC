/*

VC++ 捕捉摄像头视频!DirectX 9.0 SDK
VC++ 捕捉摄像头视频!DirectX 9.0 SDK

2009-02-25 15:39

VC++ 捕捉摄像头视频!摘要：本文主要讲述用Directshow进行视频捕捉（捕捉静态图像）的编程思路，并提供针对摄像头编程的一个视频捕捉类CcaptureVideo和一个示例。
　　前言
　　DirectShow是微软公司提供的一套在Windows平台上进行流媒体处理的开发包，与DirectX开发包一起发布。DirectShow为多媒体流的捕捉和回放提供了强有力的支持。用DirectShow开发应用程序，我们可以很方便地从支持WDM驱动模型的采集卡上捕获数据，并且进行相应的后期处理乃至存储到文件中。
　　DirectShow是基于COM的，为了编写DirectShow应用程序，需要了解COM客户程序编写的基础知识。DirectShow提供了大量的接口，但在编程中发现还是不够方便，如果能构建一个视频捕捉类把常用的一些动作封装起来，那么就更方便了。
　　编程思路
　　为了更加容易建立视频捕捉应用程序，DirectShow提供了一个叫做Capture Graph Builder的对象，Capture Graph Builder提供IcaptureGraphBuilder2接口，该接口可以建立和控制Capture Graph。
　　建立视频捕捉程序，必须首先获取并初始化IcaptureGraphBuilder2接口，然后选择一个适当的视频捕捉设备。选择好设备后，为该设备创建Capture filter，然后调用AddFilter把Capture filter添加到Filter Graph。
　　如果仅仅希望用摄像头来进行实时监控的话，只需要在上面的基础上调用ICaptureGraphBuilder2::RenderStream就可以了：


ICaptureGraphBuilder2 *pBuild; // Capture Graph Builder
//省略初始化部分代码
IBaseFilter *pCap; // Video capture filter.
//省略初始化和添加到Filter Graph部分代码

pBuild->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pCap, NULL, NULL);
　　DirectShow提供了一个捕捉静态图像的方法：使用Sample Grabber filter。依次按照以下三个步骤就可以了：
　　第一步, 定义一个类实现Sample Grabber的回调接口IsampleGrabberCB：

class CSampleGrabberCB : public ISampleGrabberCB
{
　//在后面提供的类中具体完成
}
CSampleGrabberCB mCB;
　　第二步、调用RenderStream依次把Still pin、Sample Grabber和系统默认Renderer Filter连接起来。
　　第三步、配置Sample Grabber以捕获数据。
　　视频捕捉类CCaptureVideo的具体实现
  */

// CCaptureVideo视频捕捉类头文件
/////////////////////////////////////////////////////////////////////
#if !defined(AFX_CAPTUREVIDEO_H__F5345AA4_A39F_4B07_B843_3D87C4287AA0__INCLUDED_)
#define AFX_CAPTUREVIDEO_H__F5345AA4_A39F_4B07_B843_3D87C4287AA0__INCLUDED_
/////////////////////////////////////////////////////////////////////
// CaptureVideo.h : header file
/////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>
#include <windows.h>
#include <dshow.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( x ) \
if ( NULL != x ) \
{ \
	x->Release( ); \
	x = NULL; \
}
#endif
class CSampleGrabberCB;
class CCaptureVideo : public CWnd
{
	friend class CSampleGrabberCB;
	public:
	void GrabOneFrame(BOOL bGrab);
	HRESULT Init(int iDeviceID,HWND hWnd);
	int EnumDevices(CListBox* list,HWND hList);
	static int EnumDevices(CStringArray * CameraName);
	CCaptureVideo();
	virtual ~CCaptureVideo();
	private:
	HWND m_hWnd;
	IGraphBuilder *m_pGB;
	ICaptureGraphBuilder2* m_pCapture;
	IBaseFilter* m_pBF;
	IMediaControl* m_pMC;
	IVideoWindow* m_pVW;
//	CComPtr<ISampleGrabber> m_pGrabber;
protected:
	void FreeMediaType(AM_MEDIA_TYPE& mt);
	bool BindFilter(int deviceId, IBaseFilter **pFilter);
	void ResizeVideoWindow();
	HRESULT SetupVideoWindow();
	HRESULT InitCaptureGraphBuilder();
};

extern int G_PIC_Width,G_PIC_Height;
#endif // !defined(AFX_CAPTUREVIDEO_H__F5345AA4_A39F_4B07_B843_3D87C4287AA0__INCLUDED_)