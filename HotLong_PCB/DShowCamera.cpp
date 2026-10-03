#include "stdafx.h"

#include "DShowCamera.h"
#include <iostream>
/*


CameraFrame::CameraFrame() : data(nullptr), width(0), height(0), bitCount(0) {}
CameraFrame::~CameraFrame() { delete[] data; }
CameraFrame::CameraFrame(CameraFrame&& other) noexcept
{
    data = other.data;
    width = other.width;
    height = other.height;
    bitCount = other.bitCount;
    timeStamp = other.timeStamp;
    other.data = nullptr;
}
CameraFrame& CameraFrame::operator=(CameraFrame&& other) noexcept
{
    if(this != &other)
    {
        delete[] data;
        data = other.data;
        width = other.width;
        height = other.height;
        bitCount = other.bitCount;
        timeStamp = other.timeStamp;
        other.data = nullptr;
    }
    return *this;
}

// 帧队列实现
void FrameQueue::Push(CameraFrame&& frame)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_queue.push(std::move(frame));
    m_cv.notify_one();
}
bool FrameQueue::Pop(CameraFrame& outFrame, int timeoutMs)
{
    std::unique_lock<std::mutex> lock(m_mtx);
    bool ready = m_cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
        [this]() { return !m_queue.empty(); });
    if(!ready) return false;
    outFrame = std::move(m_queue.front());
    m_queue.pop();
    return true;
}
void FrameQueue::Clear()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    while(!m_queue.empty()) m_queue.pop();
}

// DShowCamera 实现
DShowCamera::DShowCamera()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    m_bRun = false;
    m_bThreadExit = false;
}

DShowCamera::~DShowCamera()
{
    Close();
    CoUninitialize();
}

void DShowCamera::ReleaseAllDSInterface()
{
    if(m_pMediaCtrl) m_pMediaCtrl->Release();
    if(m_pSampleGrab) m_pSampleGrab->Release();
    if(m_pCamFilter) m_pCamFilter->Release();
    if(m_pCapture) m_pCapture->Release();
    if(m_pGraph) m_pGraph->Release();
    m_pGraph = nullptr;
    m_pCapture = nullptr;
    m_pCamFilter = nullptr;
    m_pSampleGrab = nullptr;
    m_pMediaCtrl = nullptr;
}

std::vector<std::wstring> DShowCamera::EnumCameraDevices()
{
    std::vector<std::wstring> devList;
    ICreateDevEnum* pDevEnum = nullptr;
    IEnumMoniker* pEnumMoniker = nullptr;

    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pDevEnum);
    if(SUCCEEDED(hr))
    {
        hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
        if(hr == S_OK)
        {
            IMoniker* pMoniker = nullptr;
            ULONG fetched;
            while(pEnumMoniker->Next(1, &pMoniker, &fetched) == S_OK)
            {
                IPropertyBag* pPropBag = nullptr;
                pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);
                VARIANT varName;
                VariantInit(&varName);
                pPropBag->Read(L"FriendlyName", &varName, NULL);
                devList.emplace_back(varName.bstrVal);
                VariantClear(&varName);
                pPropBag->Release();
                pMoniker->Release();
            }
        }
        pEnumMoniker->Release();
    }
    pDevEnum->Release();
    return devList;
}

bool DShowCamera::RebuildGraph()
{
    ReleaseAllDSInterface();
    HRESULT hr;
    // 创建 FilterGraph
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder, (void**)&m_pGraph);
    if(FAILED(hr)) return false;
    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER,
        IID_ICaptureGraphBuilder2, (void**)&m_pCapture);
    if(FAILED(hr)) return false;
    m_pCapture->SetFiltergraph(m_pGraph);
    m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pMediaCtrl);

    // 枚举并选中相机
    ICreateDevEnum* pDevEnum = NULL;//nullptr;
    IEnumMoniker* pEnumMoniker = nullptr;
    CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pDevEnum);
    pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);

    IMoniker* pMoniker = nullptr;
    ULONG idx = 0, fetched;
    while(pEnumMoniker->Next(1, &pMoniker, &fetched) == S_OK)
    {
        if(idx == m_devIdx) break;
        idx++;
        pMoniker->Release();
    }
    pDevEnum->Release();
    pEnumMoniker->Release();

    if(!pMoniker) return false;
    hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&m_pCamFilter);
    pMoniker->Release();
    if(FAILED(hr)) return false;

    // 配置 SampleGrabber（抓帧）
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
        IID_ISampleGrabber, (void**)&m_pSampleGrab);
    if(FAILED(hr)) return false;
    AM_MEDIA_TYPE mt = {0};
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_RGB24;
    mt.formattype = FORMAT_VideoInfo;
    m_pSampleGrab->SetMediaType(&mt);
    m_pSampleGrab->SetBufferSamples(TRUE);

    // 连接相机过滤器 + SampleGrabber
    hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, NULL, m_pCamFilter, m_pSampleGrab, nullptr);
    if(FAILED(hr)) return false;

    // 设置分辨率
    IAMStreamConfig* pStreamCfg = nullptr;
    m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, NULL, m_pCamFilter, IID_IAMStreamConfig, (void**)&pStreamCfg);
    if(pStreamCfg)
    {
        int cnt, size;
        pStreamCfg->GetNumberOfCapabilities(&cnt, &size);
        VIDEO_STREAM_CONFIG_CAPS scc;
        AM_MEDIA_TYPE* pmt;
        for(int i = 0; i < cnt; i++)
        {
            pStreamCfg->GetStreamCaps(i, &pmt, (BYTE*)&scc);
            if(scc.MinFrameSize.cx <= m_width && scc.MaxFrameSize.cx >= m_width
                && scc.MinFrameSize.cy <= m_height && scc.MaxFrameSize.cy >= m_height)
            {
                ((VIDEOINFOHEADER*)pmt->pbFormat)->bmiHeader.biWidth = m_width;
                ((VIDEOINFOHEADER*)pmt->pbFormat)->bmiHeader.biHeight = m_height;
                pStreamCfg->SetFormat(pmt);
                break;
            }
            DeleteMediaType(pmt);
        }
        pStreamCfg->Release();
    }
    return true;
}

bool DShowCamera::Open(int devIndex, int w, int h)
{
    try
    {
        if(m_bRun) throw std::runtime_error("相机已开启");
        m_devIdx = devIndex;
        m_width = w;
        m_height = h;

        if(!RebuildGraph())
            throw std::runtime_error("DirectShow 构图失败，设备不存在/被占用");

        m_bRun = true;
        m_bThreadExit = false;
        // 启动采集子线程
        m_workThread = std::thread(&DShowCamera::CaptureWorkLoop, this);
        // 启动媒体流
        m_pMediaCtrl->Run();
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << "打开相机异常：" << e.what() << std::endl;
        ReleaseAllDSInterface();
        return false;
    }
}

void DShowCamera::Close()
{
    m_bRun = false;
    m_bThreadExit = true;
    if(m_workThread.joinable())
        m_workThread.join();
    if(m_pMediaCtrl)
        m_pMediaCtrl->Stop();
    ReleaseAllDSInterface();
    m_frameQueue.Clear();
}

bool DShowCamera::IsRunning() const
{
    return m_bRun.load();
}

bool DShowCamera::GetLatestFrame(CameraFrame& frame, int timeout)
{
    return m_frameQueue.Pop(frame, timeout);
}

void DShowCamera::CaptureWorkLoop()
{
    while(!m_bThreadExit)
    {
        try
        {
            if(!m_bRun)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                continue;
            }

            long bufLen = 0;
            HRESULT hr = m_pSampleGrab->GetCurrentBuffer(&bufLen, nullptr);
            if(FAILED(hr) || bufLen <= 0)
            {
                throw std::runtime_error("读取图像缓冲区失败，相机断开");
            }

            // 取出图像数据
            CameraFrame newFrame;
            newFrame.width = m_width;
            newFrame.height = m_height;
            newFrame.bitCount = 24;
            newFrame.timeStamp = std::chrono::steady_clock::now();
            newFrame.data = new unsigned char[bufLen];

            m_pSampleGrab->GetCurrentBuffer(&bufLen, newFrame.data);
            m_frameQueue.Push(std::move(newFrame));
        }
        catch(const std::exception& err)
        {
            std::cerr << "采集异常：" << err.what() << "，3s后重连" << std::endl;
            if(m_pMediaCtrl) m_pMediaCtrl->Stop();
            ReleaseAllDSInterface();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            // 自动重连
            if(RebuildGraph())
            {
                m_pMediaCtrl->Run();
                std::cout << "相机重连成功" << std::endl;
            }
        }
        catch(...)
        {
            // 兜底捕获所有未知COM异常，防止线程崩溃
            std::cerr << "DirectShow 未知异常，执行重连" << std::endl;
            ReleaseAllDSInterface();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            RebuildGraph();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

*/