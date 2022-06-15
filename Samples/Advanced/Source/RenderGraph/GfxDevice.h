#pragma once
#include <list>
#include "RenderResource/RGResources.h"
#include "RenderState/RGRenderState.h"
#include "RenderState/RGBindEntryGroup.h"

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
    void SetRenderTarget(std::list<RGTextureResHandle *> targetColorBuffers, RGTextureResHandle *pTargetDepthBuffer);
    void SetRenderPipeline(RGRenderState *pRenderState);
    void SetBindGroup(uint32_t uGroupIndex, RGBindEntryGroup *pBindEntryGroup);
    void EnsurceBufferUploaded(RGBufferResHandle *pBufferHandle);
    void DrawBuffer(std::list<RGBufferResHandle *> vertexBuffers, RGBufferResHandle *pIndexBuffer);
    wgpu::ShaderModule CreateShaderModule(RGShaderProgram *pShaderProgram);
    wgpu::BindGroupLayout CreateBindGroupLayout(uint32_t uEntryCount, wgpu::BindGroupLayoutEntry const *pBindGroupLayoutEntries);
    wgpu::PipelineLayout CreatePipelineLayout(uint32_t uBindGroupLayoutCount, wgpu::BindGroupLayout const *pBindGroupLayouts);
    wgpu::RenderPipeline CreateRenderPipeline(wgpu::RenderPipelineDescriptor *pRenderPipelineDesc);
    wgpu::BindGroup CreateBindGroup(wgpu::BindGroupLayout &bindGroupLayout, uint32_t uEntryCount, wgpu::BindGroupEntry const *pEntries);

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
