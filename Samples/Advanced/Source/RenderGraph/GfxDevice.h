#pragma once
#include <webgpu/webgpu_cpp.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

class GfxDevice
{
private:
    void InitWGPUDevice();
    void InitWGPUSwapChain();
    static void DeviceSetUncapturedErrorCallback(WGPUErrorType type, char const *message, void *userdata);
    static void AdapterRequestDeviceCallback(WGPURequestDeviceStatus status, WGPUDevice device, char const *message, void *userdata);
    static void InstanceRequestAdapterCallback(WGPURequestAdapterStatus status, WGPUAdapter adapter, char const *message, void *userdata);
public:
    GfxDevice();
    ~GfxDevice();
    bool IsInitialized()
    {
        return m_bInitialized;
    }

private:
    wgpu::Device m_oDevice;
    wgpu::SwapChain m_oSwapChain;
    bool m_bInitialized;
};
