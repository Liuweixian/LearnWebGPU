#pragma once
#include <webgpu/webgpu_cpp.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <list>
#include <unordered_map>
#include "RenderResource/RenderResource.h"

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

    void BeginFrame();
    void EndFrame();
    void SetRenderTarget(std::list<RenderResourceHandle *> targetColorBuffers, RenderResourceHandle *pTargetDepthBuffer);
    void SetRenderState();
    void DrawBuffer();

private:
    wgpu::Device m_Device;
    wgpu::SwapChain m_SwapChain;
    wgpu::Adapter m_Adapter;
    wgpu::SupportedLimits *m_pSupportedLimits;
    wgpu::CommandEncoder m_CommandEncoder;
    std::unordered_map<uint32_t, wgpu::RenderPassEncoder> m_RenderPassEncoders;
    bool m_bInitialized;
};

GfxDevice *GetGfxDevice();