#pragma once
#include <webgpu/webgpu_cpp.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <list>
#include "RenderResource/RenderResource.h"
#include "RenderResource/RenderState.h"

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
    void FinishCurrentRenderPassEncoder();

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
    void SetRenderState(RenderState *pRenderState);
    void DrawBuffer();
    bool ErrorHappened()
    {
        return m_bErrorHappened;
    }

private:
    wgpu::Device m_Device;
    wgpu::SwapChain m_SwapChain;
    wgpu::Adapter m_Adapter;
    wgpu::SupportedLimits *m_pSupportedLimits;
    wgpu::CommandEncoder m_CommandEncoder;
    uint32_t m_unCurrentRenderEncoderIdx;
    wgpu::RenderPassEncoder m_CurrentRenderPassEncoder;
    bool m_bInitialized;
    bool m_bErrorHappened;
};

GfxDevice *GetGfxDevice();