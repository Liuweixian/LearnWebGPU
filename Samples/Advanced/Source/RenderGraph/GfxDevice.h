#pragma once
#include <webgpu/webgpu_cpp.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

class GfxDevice
{
private:
    void InitWGPUDevice();
    void InitWGPUSwapChain();
    void GetAdapterLimits();
    static void DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata);
    static void AdapterRequestDeviceCallback(WGPURequestDeviceStatus eStatus, WGPUDevice pDevice, char const *chMessage, void *pUserdata);
    static void InstanceRequestAdapterCallback(WGPURequestAdapterStatus eStatus, WGPUAdapter pAdapter, char const *chMessage, void *pUserdata);
    static void QueueWorkDoneCallback(WGPUQueueWorkDoneStatus eStatus, void *pUserdata);

public:
    GfxDevice();
    ~GfxDevice();
    bool IsInitialized()
    {
        return m_bInitialized;
    }
    void BeginCommandEncode();
    void EndCommandEncode();
    void BeginPassEncode();
    void EndPassEncode();

private:
    wgpu::Device m_Device;
    wgpu::SwapChain m_SwapChain;
    wgpu::Adapter m_Adapter;
    wgpu::SupportedLimits* m_pSupportedLimits;
    wgpu::CommandEncoder m_CommandEncoder;
    bool m_bInitialized;
};

GfxDevice* GetGfxDevice();