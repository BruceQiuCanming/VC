#include "stdafx.h"
#include "capturevideo.h"
#include "Qedit.h"

#include "HotLong_PCB.h"

BOOL bOneShot=FALSE;//全局变量
int G_PIC_Width,G_PIC_Height;
class CSampleGrabberCB : public ISampleGrabberCB
{
	public:
	long lWidth;
	long lHeight;
	TCHAR m_szFileName[MAX_PATH];// 位图文件名称

	//char m_szFileName[MAX_PATH];// 位图文件名称

	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{
		if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
		{
			*ppv = (void *)this;//static_cast<this>;
			return NOERROR;
		}
		return E_NOINTERFACE;
	}
	STDMETHODIMP_(ULONG) AddRef() 
	{ 
		return 2; 
	}
	STDMETHODIMP_(ULONG) Release() 
	{ 
		return 1; 
	}

	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
	{
		return 0;
	}
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
	{

	//	::G_MainDlg->StartVideo();
	//	memset(pBuffer,0,3 * 1280 * 100);
		if( !bOneShot )
			return 0;
		if (!pBuffer)
			return E_POINTER;
		SaveBitmap(pBuffer, lBufferSize);
		bOneShot = FALSE;
		return 0;
	}

	CSampleGrabberCB()
	{
		//strcpy_s((char*)m_szFileName,MAX_PATH,"c:\\donaldo.bmp");
		wcscpy(m_szFileName,_T("c:\\a.bmp"));
	}
	
	
	
	
	
	//创建位图文件
	BOOL SaveBitmap(BYTE * pBuffer, long lBufferSize )
	{
		HANDLE hf = CreateFile(
		m_szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
		CREATE_ALWAYS, NULL, NULL );
		if( hf == INVALID_HANDLE_VALUE )return 0;
		// 写文件头
		BITMAPFILEHEADER bfh;
		memset( &bfh, 0, sizeof( bfh ) );
		bfh.bfType = 'MB';
		bfh.bfSize = sizeof( bfh ) + lBufferSize + sizeof( BITMAPINFOHEADER );
		bfh.bfOffBits = sizeof( BITMAPINFOHEADER ) + sizeof( BITMAPFILEHEADER );
		DWORD dwWritten = 0;
		WriteFile( hf, &bfh, sizeof( bfh ), &dwWritten, NULL );
		// 写位图格式
		BITMAPINFOHEADER bih;
		memset( &bih, 0, sizeof( bih ) );
		bih.biSize = sizeof( bih );
		bih.biWidth = lWidth;
		bih.biHeight = lHeight;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		WriteFile( hf, &bih, sizeof( bih ), &dwWritten, NULL );
		// 写位图数据
		WriteFile( hf, pBuffer, lBufferSize, &dwWritten, NULL );
		CloseHandle( hf );
		return 0;
	}

};

CSampleGrabberCB G_mCB;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCaptureVideo::CCaptureVideo()
{
	//COM Library Intialization
	if(FAILED(CoInitialize(NULL))) /*, COINIT_APARTMENTTHREADED)))*/
	{
		AfxMessageBox(_T("CoInitialize Failed!\r\n"));
		return;
	}
	m_hWnd = NULL;
	m_pVW = NULL;
	m_pMC = NULL;
	m_pGB = NULL;
	m_pCapture = NULL;
	m_pBF  = NULL;
}
CCaptureVideo::~CCaptureVideo()
{
	// Stop media playback
	if(m_pMC)m_pMC->Stop();
	if(m_pVW)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Owner(NULL);
	}
	SAFE_RELEASE(m_pCapture);

	SAFE_RELEASE(m_pMC);
	SAFE_RELEASE(m_pGB);
	if(m_pBF > 0)
	{
		SAFE_RELEASE(m_pBF);
	}
	CoUninitialize( );
}
int CCaptureVideo::EnumDevices(CListBox* list,HWND hList)
{
	if (!hList)
		return -1;
	int id = 0;
	//枚举视频扑捉设备
	ICreateDevEnum *pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)return -1;
	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
	if (hr != NOERROR)return -1;
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{
				//TCHAR str[2048];
				char str[2048];
				id++;
				WideCharToMultiByte(CP_ACP,0,var.bstrVal, -1, str, 2048, NULL, NULL);
				list->AddString(var.bstrVal);
				::SendMessage(hList, CB_ADDSTRING, 0,(LPARAM)str);
				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
	}
	return id;
}

int CCaptureVideo::EnumDevices(CStringArray * CameraName)
{
	if (CameraName <= 0)
		return -1;
	int id = 0;
	//枚举视频扑捉设备
	ICreateDevEnum *pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)return -1;
	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
	if (hr != NOERROR)return -1;
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{
				//TCHAR str[2048];
				char str[2048];
				id++;
				WideCharToMultiByte(CP_ACP,0,var.bstrVal, -1, str, 2048, NULL, NULL);
				CameraName->Add(var.bstrVal);
				
				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
	}
	return id;
}

ISampleGrabber * m_pGrabber ;

HRESULT CCaptureVideo::Init(int iDeviceID, HWND hWnd)
{
	HRESULT hr;
	hr = InitCaptureGraphBuilder();
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to get video interfaces!"));
		return hr;
	}
	// Bind Device Filter. We know the device because the id was passed in
	if(!BindFilter(iDeviceID, &m_pBF))return S_FALSE;
	hr = m_pGB->AddFilter(m_pBF, L"Capture Filter");
	// hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,
	// m_pBF, NULL, NULL);
	// create a sample grabber
	//hr = m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );
	hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabber, (void**)&m_pGrabber );

	if( !m_pGrabber )
	{
		AfxMessageBox(_T("Fail to create SampleGrabber, maybe qedit.dll is not registered?"));
		return hr;
	}
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );
	//设置视频格式
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;
	hr = m_pGrabber->SetMediaType(&mt);
	if( FAILED( hr ) )
	{
		AfxMessageBox(_T("Fail to set media type!"));
		return hr;
	}
	hr = m_pGB->AddFilter( pGrabBase, L"Grabber" );
	if( FAILED( hr ) ){
		AfxMessageBox(_T("Fail to put sample grabber in graph"));
		return hr;
	}
	// try to render preview/capture pin
	hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);
	if( FAILED( hr ) )
		hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);
		if( FAILED( hr ) ){
			AfxMessageBox(_T("Can’t build the graph"));
		return hr;
	}
	hr = m_pGrabber->GetConnectedMediaType( &mt );
	if ( FAILED( hr) ){
		AfxMessageBox(_T("Failt to read the connected media type"));
		return hr;
	}
	VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
	G_mCB.lWidth = vih->bmiHeader.biWidth;
	G_mCB.lHeight = vih->bmiHeader.biHeight;
	G_PIC_Width	= G_mCB.lWidth / 3;
	G_PIC_Height = G_mCB.lHeight / 3;
	FreeMediaType(mt);
	hr = m_pGrabber->SetBufferSamples( FALSE );
	hr = m_pGrabber->SetOneShot( FALSE );
	hr = m_pGrabber->SetCallback( &G_mCB, 1 );
	//设置视频捕捉窗口
	m_hWnd = hWnd ;
	SetupVideoWindow();
	hr = m_pMC->Run();//开始视频捕捉
	if(FAILED(hr))
	{
		AfxMessageBox(_T("Couldn’t run the graph!"));
		return hr;
	}
	return S_OK;
}
bool CCaptureVideo::BindFilter(int deviceId, IBaseFilter **pFilter)
{
	if (deviceId < 0)
		return false;
	// enumerate all video capture devices
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
			IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (hr != NOERROR)
	{
		return false;
	}
	CComPtr<IEnumMoniker> pEm;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
	if (hr != NOERROR)
	{
		return false;
	}
	pEm->Reset();
	ULONG cFetched;
	IMoniker *pM;
	int index = 0;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= deviceId)
	{
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr))
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR)
			{
				if (index == deviceId)
				{
					pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
				}
				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
		index++;
	}
	return true;
}
HRESULT CCaptureVideo::InitCaptureGraphBuilder()
{
	HRESULT hr;
	// 创建IGraphBuilder接口
	hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB);
	// 创建ICaptureGraphBuilder2接口
	hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2, (void **) &m_pCapture);
	if (FAILED(hr))
		return hr;
	m_pCapture->SetFiltergraph(m_pGB);
	hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
	if (FAILED(hr))
		return hr;
	hr = m_pGB->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVW);
	if (FAILED(hr))
		return hr;
	return hr;
}

HRESULT CCaptureVideo::SetupVideoWindow()
{
	HRESULT hr;
	hr = m_pVW->put_Owner((OAHWND)m_hWnd);
	if (FAILED(hr))return hr;
	hr = m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	if (FAILED(hr))return hr;
	ResizeVideoWindow();
	hr = m_pVW->put_Visible(OATRUE);
	return hr;
}
void CCaptureVideo::ResizeVideoWindow()
{
	if (m_pVW)
	{
		//让图像充满整个窗口
		CRect rc;
		::GetClientRect(m_hWnd,&rc);
		m_pVW->SetWindowPosition(0, 0, rc.right, rc.bottom);
	}
}
void CCaptureVideo::GrabOneFrame(BOOL bGrab)
{
	bOneShot = bGrab;
}
void CCaptureVideo::FreeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0) {
		CoTaskMemFree((PVOID)mt.pbFormat);
		// Strictly unnecessary but tidier
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL) 
	{
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
}
/*
如何使用视频捕捉类CCaptureVideo
　　构建CCaptureVideo类以后，使用就方便多了，我们在编程中只需要是要下面三个类成员函数就可以实现用摄像头进行视频捕捉：
　　①int EnumDevices(HWND hList); //hList是下拉列表框的句柄，本函数用于枚举当前系统安装的所有视频捕捉设备
　　②HRESULT Init(int iDeviceID,HWND hWnd);//iDeviceID是视频捕捉设备序号，hWnd是视频捕捉窗口的句柄
　　③void GrabOneFrame(BOOL bGrab);//调用GrabOneFrame(true)就可以捕获当前的静态图像并保存到硬盘上
　　具体示例：用MFC AppWizard(exe)创建一个对话框应用程序，取名为ds，给对话框添加一个下拉列表框(IDC_COMBO1)、两个按钮（IDC_PHOTO、IDC_HAVEALOOK）和一个Picture控件（ID:IDC_STATIC_SCREEN，Type: Rectangle，Color:Gray）。
　　1、使用向导添加成员变量
CStatic m_staticScreen; // IDC_STATIC_SCREEN
CComboBox m_ListCtrl; // IDC_COMBO1
CCaptureVideo m_cap;
　　2、为BOOL CDsDlg::OnInitDialog()添加如下代码：
// TODO: Add extra initialization here
m_cap.EnumDevices (m_ListCtrl);
m_ListCtrl.SetCurSel (0);
　　3、为确定按钮添加代码如下：
void CDsDlg::OnOK()
{
　//只需要四行代码就可以进行视频捕捉了
　UpdateData();
　HWND hWnd = m_staticScreen.GetSafeHwnd() ;
　HRESULT hr = m_cap.Init(m_ListCtrl.GetCurSel (),hWnd);
　GetDlgItem(IDOK)->EnableWindow(FALSE);
}
　　4、如果希望捕捉静态图像，为照相按钮添加如下代码：
void CDsDlg::OnPhoto()
{
　m_cap.GrabOneFrame(true);
}
*/

  /*
运行程序时，选定摄像头后只需要按确定就可以了,实际效果如下图所示：
结束语
本文提供的视频捕捉类CcaptureVideo和示例，在Win2K + DirectX9 SDK + VC6 环境下调试通过。注意：编译时需要Strmiids.lib Quartz.lib两个库文件(DirectX9 SDK自带)。

usb廉价摄像头抓取实时视频数据，呵呵，这可是一个非常不错视频开发试验。通过这个试验，可以在学习中体验视频给我们带来的乐趣。网上常用的是CCaptureVideo类，来处理usb摄像头数据捕获和读取。我也用的是这个类来在win32环境下面做做试验。很好用，而且很轻松就得到了usb摄像头的实时数据。但是这个类就是十全十美的吗？我看不见得。比如说，我直接定义CCaptureVideo的一个对象，没有调用它的init()函数，即没有调用它来获取视频数据，程序退出的时候会抛出异常。经过检查才发现原来m_pBF要不就是m_pGB没有初始化成NULL。这是一个很低级的错误拉，不过很容易就可以修改过来。这样哪怕你不使用这个类，只是声明一个对象，那么程序推出的时候也不会出现什么异常。

刚刚那个是比较明显的bug了，还有一些不太明显的。不知道大家在使用CCaptureVideo类的时候是否遇见过。例如，我们可能希望按下某个按钮的时候开始摄像头视频捕捉，再按一次，就停止视频捕捉；然后如果需要，那么再次重复按下该按钮，再次重新启动视频捕捉，这就好比qq的视频聊天一样，qq主程序没有关掉，但是你可以在不同的时间段和不同的人视频聊天。简单的描述就是在不关闭主程序的前提下进行：

打开摄像头--->关闭摄像头--->再打开摄像头--->再关闭摄像头--->再打开摄像头...等等以此类推。

对于网上流传的CCaptureVideo类而言，它是有问题的，假设我们声明了一个CCaptureVideo类的指针

CCaptureVideo * m_cap ;

然后，在OnInitDialog函数中创建该对象，

m_cap = new CCaptureVideo() ;

然后就是例牌的枚举可用摄像头（m_cap->EnumDevices (m_cam_lst);），或者直接调用init函数

（m_cap->Init(m_cam_lst.GetCurSel (),hWnd);），开始捕获视频数据。

对于缺省的CCaptureVideo而言，第一次这么用是完全没有问题的，可以很容易的把usb摄像头的数据得到。

假定，现在我们按下了“stop”按钮，此时，我们可以用delete m_cap ;可以看到实时显示的那个窗口不见了。这也是正确的。

当我们再次开启摄像头的时候，问题就出现了。我们还是按照刚刚一样的顺序初始化m_cap对象，但是这次调用init函数的时候确弹出一个对话框“can't build graph”错误。区区怪事！

后来仔细跟踪才知道问题出在：

CComPtr<ISampleGrabber> m_pGrabber;

这个智能指针上面。它第一次工作是正常的，但是第二次就有问题了，似乎是资源没有释放掉。

修改的方法也很简单，直接手工定义：

ISampleGrabber * m_pGrabber ;

...

使用这个指针的代码也需要修改：

原来的初始化代码是：

hr = m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );

我们可以改为：

hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabber, (void**)&m_pGrabber );

当然，在析构的时候千万不要忘记了：

SAFE_RELEASE(m_pGrabber);

这样，我们想要开关摄像头多少次都没问题了。
*/