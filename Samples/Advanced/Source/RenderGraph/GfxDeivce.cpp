#include "GfxDevice.h"
#include <stdio.h>
#include <cassert>
void GfxDevice::Initialize()
{
    if (m_bInitialized && m_SwapChain == nullptr)
        return;
    m_pSupportedLimits = new wgpu::SupportedLimits();
    InitWebGPU();
    m_bInitialized = true;
}

void GfxDevice::PrintSupportedLimits()
{
    printf("SupportedLimits:\n");
    printf("maxTextureDimension1D: %d\n", m_pSupportedLimits->limits.maxTextureDimension1D);
    printf("maxTextureDimension2D: %d\n", m_pSupportedLimits->limits.maxTextureDimension2D);
    printf("maxTextureDimension3D: %d\n", m_pSupportedLimits->limits.maxTextureDimension3D);
    printf("maxTextureArrayLayers: %d\n", m_pSupportedLimits->limits.maxTextureArrayLayers);
    printf("maxBindGroups: %d\n", m_pSupportedLimits->limits.maxBindGroups);
    printf("maxDynamicUniformBuffersPerPipelineLayout: %d\n", m_pSupportedLimits->limits.maxDynamicUniformBuffersPerPipelineLayout);
    printf("maxDynamicStorageBuffersPerPipelineLayout: %d\n", m_pSupportedLimits->limits.maxDynamicStorageBuffersPerPipelineLayout);
    printf("maxSampledTexturesPerShaderStage: %d\n", m_pSupportedLimits->limits.maxSampledTexturesPerShaderStage);
    printf("maxSamplersPerShaderStage: %d\n", m_pSupportedLimits->limits.maxSamplersPerShaderStage);
    printf("maxStorageBuffersPerShaderStage: %d\n", m_pSupportedLimits->limits.maxStorageBuffersPerShaderStage);
    printf("maxStorageTexturesPerShaderStage: %d\n", m_pSupportedLimits->limits.maxStorageTexturesPerShaderStage);
    printf("maxUniformBuffersPerShaderStage: %d\n", m_pSupportedLimits->limits.maxUniformBuffersPerShaderStage);
    printf("maxUniformBufferBindingSize: %llu\n", m_pSupportedLimits->limits.maxUniformBufferBindingSize);
    printf("maxStorageBufferBindingSize: %llu\n", m_pSupportedLimits->limits.maxStorageBufferBindingSize);
    printf("minUniformBufferOffsetAlignment: %d\n", m_pSupportedLimits->limits.minUniformBufferOffsetAlignment);
    printf("minStorageBufferOffsetAlignment: %d\n", m_pSupportedLimits->limits.minStorageBufferOffsetAlignment);
    printf("maxVertexBuffers: %d\n", m_pSupportedLimits->limits.maxVertexBuffers);
    printf("maxVertexAttributes: %d\n", m_pSupportedLimits->limits.maxVertexAttributes);
    printf("maxVertexBufferArrayStride: %d\n", m_pSupportedLimits->limits.maxVertexBufferArrayStride);
    printf("maxInterStageShaderComponents: %d\n", m_pSupportedLimits->limits.maxInterStageShaderComponents);
    printf("maxComputeWorkgroupStorageSize: %d\n", m_pSupportedLimits->limits.maxComputeWorkgroupStorageSize);
    printf("maxComputeInvocationsPerWorkgroup: %d\n", m_pSupportedLimits->limits.maxComputeInvocationsPerWorkgroup);
    printf("maxComputeWorkgroupSizeX: %d\n", m_pSupportedLimits->limits.maxComputeWorkgroupSizeX);
    printf("maxComputeWorkgroupSizeY: %d\n", m_pSupportedLimits->limits.maxComputeWorkgroupSizeY);
    printf("maxComputeWorkgroupSizeZ: %d\n", m_pSupportedLimits->limits.maxComputeWorkgroupSizeZ);
    printf("maxComputeWorkgroupsPerDimension: %d\n", m_pSupportedLimits->limits.maxComputeWorkgroupsPerDimension);
}


void GfxDevice::BeginFrame()
{
    if (!m_bInitialized)
        return;
    assert(m_CommandEncoder == nullptr);
    m_CommandEncoder = m_Device.CreateCommandEncoder(nullptr);
}

void GfxDevice::QueueWorkDoneCallback(WGPUQueueWorkDoneStatus eStatus, void *pUserdata)
{
    if (eStatus != WGPUQueueWorkDoneStatus_Success)
    {
        printf("GfxDevice::QueueWorkDoneCallback Failed! return %d \n", eStatus);
        return;
    }
}

void GfxDevice::EndFrame()
{
    if (!m_bInitialized)
        return;

    FinishCurrentRenderPassEncoder();
    wgpu::CommandBuffer commandBuffer = m_CommandEncoder.Finish();
    wgpu::Queue queue = m_Device.GetQueue();
    queue.OnSubmittedWorkDone(0, QueueWorkDoneCallback, this);
    queue.Submit(1, &commandBuffer);
    m_CommandEncoder = nullptr;
}

void GfxDevice::SetRenderTarget(std::list<RenderResourceHandle *> targetColorBuffers, RenderResourceHandle *pTargetDepthBuffer)
{
    uint32_t unEncoderHash = 0;

    uint32_t nColorAttachmentCount = (uint32_t)targetColorBuffers.size();
    unEncoderHash = unEncoderHash * 31 + nColorAttachmentCount;

    RenderResourceHandle *pFrameBufferHandle = GetRenderResource()->GetFrameBuffer();
    wgpu::RenderPassColorAttachment colorAttachments[nColorAttachmentCount];
    int nIndex = 0;
    for (auto bufferIt = targetColorBuffers.begin(); bufferIt != targetColorBuffers.end(); bufferIt++)
    {
        RenderResourceHandle *pResHandle = *bufferIt;
        unEncoderHash = unEncoderHash * 31 + pResHandle->m_unDescIdx;

        if (pResHandle->m_unDescIdx == pFrameBufferHandle->m_unDescIdx)
        {
            colorAttachments[nIndex].view = m_SwapChain.GetCurrentTextureView();
        }
        else
        {
            // todo: render into texture
        }
#ifdef __EMSCRIPTEN__
        colorAttachments[nIndex].loadOp = wgpu::LoadOp::Undefined;
#else
        colorAttachments[nIndex].storeOp = wgpu::StoreOp::Discard;
        colorAttachments[nIndex].loadOp = wgpu::LoadOp::Clear;
#endif
        colorAttachments[nIndex].clearColor = {0.0f, 0.0f, 0.0f, 1.0f}; // clear color, loadop must be Undefiend
        nIndex++;
    }

    wgpu::RenderPassDepthStencilAttachment *pDepthAttachment = nullptr;
    if (pTargetDepthBuffer == nullptr)
    {
        unEncoderHash = unEncoderHash * 31;
    }
    else
    {
        unEncoderHash = unEncoderHash * 31 + pTargetDepthBuffer->m_unDescIdx;
        /*wgpu::RenderPassDepthStencilAttachment depthAttachment;
        depthAttachment.view = ms_DepthBuffer;
        depthAttachment.depthLoadOp = wgpu::LoadOp::Clear;
        depthAttachment.depthStoreOp = wgpu::StoreOp::Discard;
        depthAttachment.clearDepth = 0;
        depthAttachment.depthReadOnly = false;
        depthAttachment.stencilLoadOp = wgpu::LoadOp::Clear;
        depthAttachment.stencilStoreOp = wgpu::StoreOp::Discard;
        depthAttachment.clearStencil = 0;
        depthAttachment.stencilReadOnly = false;*/
    }

    if (unEncoderHash != m_unCurrentRenderEncoderIdx)
    {
        FinishCurrentRenderPassEncoder();
        // printf("GfxDevice BeginRenderPass last:%d current:%d\n", m_unCurrentRenderEncoderIdx, unEncoderHash);
        wgpu::RenderPassDescriptor renderPassDesc;
        renderPassDesc.colorAttachmentCount = nColorAttachmentCount;
        renderPassDesc.colorAttachments = colorAttachments;
        renderPassDesc.depthStencilAttachment = pDepthAttachment;
        renderPassDesc.occlusionQuerySet = nullptr;
        m_CurrentRenderPassEncoder = m_CommandEncoder.BeginRenderPass(&renderPassDesc);
        m_unCurrentRenderEncoderIdx = unEncoderHash;
    }
}

void GfxDevice::FinishCurrentRenderPassEncoder()
{
    if (m_CurrentRenderPassEncoder == nullptr)
        return;
    // printf("GfxDevice EndRenderPass %d\n", m_unCurrentRenderEncoderIdx);
    m_CurrentRenderPassEncoder.End();
    m_CurrentRenderPassEncoder = nullptr;
    m_unCurrentRenderEncoderIdx = 0;
}

void GfxDevice::SetRenderState(RenderState *pRenderState)
{
    if (pRenderState->m_RenderPipeline == nullptr)
    {
        assert(pRenderState->m_pLayoutDesc);
        wgpu::RenderPipelineDescriptor renderPipelineDesc;
        renderPipelineDesc.layout = m_Device.CreatePipelineLayout(pRenderState->m_pLayoutDesc);
        wgpu::ShaderModuleDescriptor vertexShaderModuleDesc;
        assert(pRenderState->m_pVertexShaderDesc != nullptr);
        vertexShaderModuleDesc.nextInChain = pRenderState->m_pVertexShaderDesc;
        pRenderState->m_pVertexState->module = m_Device.CreateShaderModule(&vertexShaderModuleDesc);
        renderPipelineDesc.vertex = *(pRenderState->m_pVertexState);
        renderPipelineDesc.primitive = *(pRenderState->m_pPrimitiveState);
        renderPipelineDesc.depthStencil = pRenderState->m_pDepthStencilState;
        renderPipelineDesc.multisample = *(pRenderState->m_pMultisampleState);
        wgpu::ShaderModuleDescriptor fragShaderModuleDesc;
        assert(pRenderState->m_pFragmentShaderDesc != nullptr);
        fragShaderModuleDesc.nextInChain = pRenderState->m_pFragmentShaderDesc;
        pRenderState->m_pFragmentState->module = m_Device.CreateShaderModule(&fragShaderModuleDesc);
        renderPipelineDesc.fragment = pRenderState->m_pFragmentState;
        pRenderState->m_RenderPipeline = m_Device.CreateRenderPipeline(&renderPipelineDesc);
    }
    else
    {
        pRenderState->Cleanup();
    }
    m_CurrentRenderPassEncoder.SetPipeline(pRenderState->m_RenderPipeline);
}

void GfxDevice::DrawBuffer(std::list<RenderBuffer *> vertexBuffers, RenderBuffer *pIndexBuffer)
{
    for (auto it = vertexBuffers.begin(); it != vertexBuffers.end(); it++)
    {
        RenderBuffer *pRenderBuffer = *it;
        wgpu::Buffer &gpuBuffer = pRenderBuffer->GetGPUBuffer();
        size_t ulBufferSize = pRenderBuffer->GetDataLength();
        if (gpuBuffer == nullptr)
        {
            wgpu::BufferDescriptor bufferDesc;
            bufferDesc.usage = wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst;
            bufferDesc.size = ulBufferSize;
            gpuBuffer = m_Device.CreateBuffer(&bufferDesc);
            wgpu::Queue queue = m_Device.GetQueue();
            queue.WriteBuffer(gpuBuffer, 0, pRenderBuffer->GetData(), ulBufferSize);
        }
        m_CurrentRenderPassEncoder.SetVertexBuffer(0, gpuBuffer, 0, ulBufferSize);
    }

    assert(pIndexBuffer != nullptr);
    wgpu::Buffer &gpuBuffer = pIndexBuffer->GetGPUBuffer();
    size_t ulBufferSize = pIndexBuffer->GetDataLength();
    if (gpuBuffer == nullptr)
    {
        wgpu::BufferDescriptor bufferDesc;
        bufferDesc.usage = wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst;
        bufferDesc.size = ulBufferSize;
        gpuBuffer = m_Device.CreateBuffer(&bufferDesc);
        wgpu::Queue queue = m_Device.GetQueue();
        queue.WriteBuffer(gpuBuffer, 0, pIndexBuffer->GetData(), ulBufferSize);
    }
    m_CurrentRenderPassEncoder.SetIndexBuffer(gpuBuffer, wgpu::IndexFormat::Uint16, 0, ulBufferSize);
    m_CurrentRenderPassEncoder.DrawIndexed((uint32_t)pIndexBuffer->GetDataCount(), 1, 0, 0, 0);
}

static GfxDevice *g_pGfxDevice = nullptr;
GfxDevice *GetGfxDevice()
{
    return g_pGfxDevice;
}

void CreateGfxDevice(std::function<void(GfxDevice *&)> callback)
{
    if (g_pGfxDevice == nullptr)
    {
        callback(g_pGfxDevice);
        assert(g_pGfxDevice != nullptr);
    }
}
