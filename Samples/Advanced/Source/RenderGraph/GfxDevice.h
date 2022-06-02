#pragma once
#include <list>
#include "RenderResource/RGResources.h"
#include "RenderResource/RenderState.h"
#include "RenderResource/RGBuffer.h"

class GfxDevice
{
private:
    static void QueueWorkDoneCallback(WGPUQueueWorkDoneStatus eStatus, void *pUserdata);
    void FinishCurrentRenderPassEncoder();

protected:
    enum Status
    {
        Invalid,
        Initializing,
        Initialized,
    };
    virtual void InitWebGPU() = 0;
    void PrintSupportedLimits();
    virtual void RequestAdapter() = 0;
    virtual void GetSupportLimits() = 0;
    virtual void CreateDevice() = 0;
    virtual void CreateSwapChain() = 0;

public:
    void Initialize();
    bool IsNotInitialized()
    {
        return m_eStatus != Status::Initialized;
    }
    bool ErrorHappened()
    {
        return m_bErrorHappened;
    }

    void BeginFrame();
    void EndFrame();
    void SetRenderTarget(std::list<RGResourceHandle *> targetColorBuffers, RGResourceHandle *pTargetDepthBuffer);
    void SetRenderState(RenderState *pRenderState);
    void DrawBuffer(std::list<RGBuffer *> vertexBuffers, RGBuffer *pIndexBuffer);

protected:
    wgpu::Device m_Device;
    wgpu::SwapChain m_SwapChain;
    wgpu::SupportedLimits *m_pSupportedLimits;
    wgpu::CommandEncoder m_CommandEncoder;
    uint32_t m_unCurrentRenderEncoderIdx;
    wgpu::RenderPassEncoder m_CurrentRenderPassEncoder;
    Status m_eStatus;
    bool m_bErrorHappened;
};

GfxDevice *GetGfxDevice();

void CreateGfxDevice(std::function<void(GfxDevice *&)> callback);
