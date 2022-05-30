#pragma once
#include <list>
#include "RenderResource/RenderResource.h"
#include "RenderResource/RenderState.h"
#include "RenderResource/RenderBuffer.h"

class GfxDevice
{
private:
    static void QueueWorkDoneCallback(WGPUQueueWorkDoneStatus eStatus, void *pUserdata);
    void FinishCurrentRenderPassEncoder();

protected:
    virtual void InitWebGPU() = 0;
    void PrintSupportedLimits();
    virtual void RequestAdapter() = 0;
    virtual void GetSupportLimits() = 0;
    virtual void CreateDevice() = 0;
    virtual void CreateSwapChain() = 0;

public:
    void Initialize();
    bool IsInitialized()
    {
        return m_bInitialized && m_SwapChain != nullptr;
    }
    bool ErrorHappened()
    {
        return m_bErrorHappened;
    }

    void BeginFrame();
    void EndFrame();
    void SetRenderTarget(std::list<RenderResourceHandle *> targetColorBuffers, RenderResourceHandle *pTargetDepthBuffer);
    void SetRenderState(RenderState *pRenderState);
    void DrawBuffer(std::list<RenderBuffer *> vertexBuffers, RenderBuffer *pIndexBuffer);

protected:
    wgpu::Device m_Device;
    wgpu::SwapChain m_SwapChain;
    wgpu::SupportedLimits *m_pSupportedLimits;
    wgpu::CommandEncoder m_CommandEncoder;
    uint32_t m_unCurrentRenderEncoderIdx;
    wgpu::RenderPassEncoder m_CurrentRenderPassEncoder;
    bool m_bInitialized;
    bool m_bErrorHappened;
};

GfxDevice *GetGfxDevice();

void CreateGfxDevice(std::function<void(GfxDevice *&)> callback);
