#ifndef CAMERADS_H
#define CAMERADS_H
/*
#include "stdafx.h"

#include <dshow.h>
//#include <thread>
//#include <mutex>
#include <queue>
//#include <atomic>
//#include <chrono>
#include <stdexcept>
#include <vector>

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "quartz.lib")

// 帧数据结构体
struct CameraFrame
{
    unsigned char* data;
    int width;
    int height;
    int bitCount;
    //std::chrono::steady_clock::time_point timeStamp;
	CTime timeStamp;
    CameraFrame();
    ~CameraFrame();
    // 禁止拷贝，仅移动
    CameraFrame(CameraFrame&&) noexcept;
    CameraFrame& operator=(CameraFrame&&) noexcept;
};

// 线程安全帧队列
class FrameQueue
{
public:
    void Push(CameraFrame&& frame);
    bool Pop(CameraFrame& outFrame, int timeoutMs = 50);
    void Clear();
private:
    std::queue<CameraFrame> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};

// DirectShow 摄像头采集类
class DShowCamera
{
public:
    DShowCamera();
    ~DShowCamera();

    // 枚举所有可用摄像头
    std::vector<std::wstring> EnumCameraDevices();
    // 打开指定摄像头索引，设置分辨率
    bool Open(int devIndex, int w, int h);
    // 停止并释放资源
    void Close();
    // 是否正在采集
    bool IsRunning() const;
    // 主线程/UI线程获取图像
    bool GetLatestFrame(CameraFrame& frame, int timeout = 30);

private:
    // 采集工作线程
    void CaptureWorkLoop();
    // 重建链路、断线重连
    bool RebuildGraph();
    // DirectShow 过滤器图释放
    void ReleaseAllDSInterface();

    // DirectShow 核心接口
    IGraphBuilder*       m_pGraph = nullptr;
    ICaptureGraphBuilder2* m_pCapture = nullptr;
    IBaseFilter*        m_pCamFilter = nullptr;
    ISampleGrabber*     m_pSampleGrab = nullptr;
    IMediaControl*      m_pMediaCtrl = nullptr;

    // 运行标记、线程
    std::atomic<bool>   m_bRun;
    std::atomic<bool>   m_bThreadExit;
    std::thread         m_workThread;
    FrameQueue          m_frameQueue;

    // 配置参数
    int m_devIdx;
    int m_width;
    int m_height;
};
*/
#endif