#pragma once
#include <webgpu/webgpu_cpp.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

class GfxDevice
{
private:
    void InitWGPUDevice();
    void InitWGPUSwapChain();
    static void DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata);
    static void AdapterRequestDeviceCallback(WGPURequestDeviceStatus eStatus, WGPUDevice pDevice, char const *chMessage, void *pUserdata);
    static void InstanceRequestAdapterCallback(WGPURequestAdapterStatus eStatus, WGPUAdapter pAdapter, char const *chMessage, void *pUserdata);

public:
    GfxDevice();
    ~GfxDevice();
    bool IsInitialized()
    {
        return m_bInitialized;
    }

private:
    wgpu::Device m_Device;
    wgpu::SwapChain m_SwapChain;
    WGPUAdapter m_pAdapter;
    bool m_bInitialized;
};
