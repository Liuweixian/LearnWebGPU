#include "GfxDevice.h"
#include <stdio.h>
#include <cassert>

void GfxDevice::Initialize()
{
    if (m_eStatus == Status::Initializing)
        return;
    m_eStatus = Status::Initializing;
    m_pSupportedLimits = new wgpu::SupportedLimits();
    InitWebGPU();
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
    if (IsNotInitialized())
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
    if (IsNotInitialized())
        return;

    FinishCurrentRenderPassEncoder();

    wgpu::CommandBuffer commandBuffer = m_CommandEncoder.Finish();
    m_CommandEncoder.Release();
    m_CommandEncoder = nullptr;

    wgpu::Queue queue = m_Device.GetQueue();
    queue.OnSubmittedWorkDone(0, QueueWorkDoneCallback, this);
    queue.Submit(1, &commandBuffer);
    commandBuffer.Release();

#ifndef __EMSCRIPTEN__
    m_SwapChain.Present();
#endif
}

void GfxDevice::SetRenderTarget(std::list<RGTextureResHandle *> targetColorBuffers, RGTextureResHandle *pTargetDepthBuffer)
{
    uint32_t unEncoderHash = 0;

    uint32_t nColorAttachmentCount = (uint32_t)targetColorBuffers.size();
    unEncoderHash = unEncoderHash * 31 + nColorAttachmentCount;

    RGTextureResHandle *pFrameBufferHandle = GetResources()->GetFrameBuffer();
    wgpu::RenderPassColorAttachment colorAttachments[nColorAttachmentCount];
    int nIndex = 0;
    for (auto bufferIt = targetColorBuffers.begin(); bufferIt != targetColorBuffers.end(); bufferIt++)
    {
        RGTextureResHandle *pResHandle = *bufferIt;
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
        colorAttachments[nIndex].storeOp = wgpu::StoreOp::Store;
        colorAttachments[nIndex].loadOp = wgpu::LoadOp::Undefined;
        colorAttachments[nIndex].clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
#else
        colorAttachments[nIndex].storeOp = wgpu::StoreOp::Store;
        colorAttachments[nIndex].loadOp = wgpu::LoadOp::Clear;
        colorAttachments[nIndex].clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
#endif
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
    m_CurrentRenderPassEncoder.Release();
    m_CurrentRenderPassEncoder = nullptr;
    m_unCurrentRenderEncoderIdx = 0;
}

void GfxDevice::EnsurceBufferUploaded(RGBufferResHandle *pBufferHandle)
{
    wgpu::Buffer &gpuBuffer = pBufferHandle->GetGPUBuffer();
    if (gpuBuffer == nullptr)
    {
        wgpu::BufferDescriptor bufferDesc;
        bufferDesc.label = pBufferHandle->m_szName.c_str();
        bufferDesc.usage = wgpu::BufferUsage::CopyDst | pBufferHandle->GetBufferUsage();
        bufferDesc.size = pBufferHandle->GetDataLength();
        gpuBuffer = m_Device.CreateBuffer(&bufferDesc);
    }
    
    if (pBufferHandle->IsDirty())
    {
        wgpu::Queue queue = m_Device.GetQueue();
        queue.WriteBuffer(gpuBuffer, 0, pBufferHandle->GetData(), pBufferHandle->GetDataLength());
        pBufferHandle->SetNotDirty();
    }
}

void GfxDevice::DrawBuffer(std::list<RGBufferResHandle *> vertexBuffers, RGBufferResHandle *pIndexBuffer)
{
    for (auto it = vertexBuffers.begin(); it != vertexBuffers.end(); it++)
    {
        RGBufferResHandle *pBufferHandle = *it;
        EnsurceBufferUploaded(pBufferHandle);
        m_CurrentRenderPassEncoder.SetVertexBuffer(0, pBufferHandle->GetGPUBuffer(), 0, pBufferHandle->GetDataLength());
    }

    assert(pIndexBuffer != nullptr);
    EnsurceBufferUploaded(pIndexBuffer);
    m_CurrentRenderPassEncoder.SetIndexBuffer(pIndexBuffer->GetGPUBuffer(), wgpu::IndexFormat::Uint16, 0, pIndexBuffer->GetDataLength());
    m_CurrentRenderPassEncoder.DrawIndexed((uint32_t)pIndexBuffer->GetDataCount(), 1, 0, 0, 0);
}

void GfxDevice::SetRenderPipeline(RGRenderState *pRenderState)
{
    m_CurrentRenderPassEncoder.SetPipeline(pRenderState->GetCurrentPipeline());
}

void GfxDevice::SetBindGroup(uint32_t uGroupIndex, RGBindEntryGroup *pBindEntryGroup)
{
    m_CurrentRenderPassEncoder.SetBindGroup(uGroupIndex, pBindEntryGroup->GetBindGroup());
}

wgpu::ShaderModule GfxDevice::CreateShaderModule(RGShaderProgram *pShaderProgram)
{
    wgpu::ShaderModuleWGSLDescriptor shaderModuleWGSLDesc;
    shaderModuleWGSLDesc.source = pShaderProgram->LoadSource();
    wgpu::ShaderModuleDescriptor shaderModuleDesc;
    shaderModuleDesc.nextInChain = &shaderModuleWGSLDesc;
    wgpu::ShaderModule shaderModule = m_Device.CreateShaderModule(&shaderModuleDesc);
    pShaderProgram->UnloadSource();
    return shaderModule;
}

wgpu::BindGroupLayout GfxDevice::CreateBindGroupLayout(uint32_t uEntryCount, const wgpu::BindGroupLayoutEntry *pBindGroupLayoutEntries)
{
    wgpu::BindGroupLayoutDescriptor layoutDesc;
    layoutDesc.nextInChain = nullptr;
    layoutDesc.label = nullptr;
    layoutDesc.entryCount = uEntryCount;
    layoutDesc.entries = pBindGroupLayoutEntries;
    return m_Device.CreateBindGroupLayout(&layoutDesc);
    
}

wgpu::PipelineLayout GfxDevice::CreatePipelineLayout(uint32_t uBindGroupLayoutCount, wgpu::BindGroupLayout const * pBindGroupLayouts)
{
    wgpu::PipelineLayoutDescriptor layoutDesc;
    layoutDesc.nextInChain = nullptr;
    layoutDesc.label = nullptr;
    layoutDesc.bindGroupLayoutCount = uBindGroupLayoutCount;
    layoutDesc.bindGroupLayouts = pBindGroupLayouts;
    return m_Device.CreatePipelineLayout(&layoutDesc);
}

wgpu::RenderPipeline GfxDevice::CreateRenderPipeline(wgpu::RenderPipelineDescriptor *pRenderPipelineDesc)
{
    return m_Device.CreateRenderPipeline(pRenderPipelineDesc);
}

wgpu::BindGroup GfxDevice::CreateBindGroup(wgpu::BindGroupLayout &bindGroupLayout, uint32_t uEntryCount, wgpu::BindGroupEntry const *pEntries)
{
    wgpu::BindGroupDescriptor bindGroupDesc;
    bindGroupDesc.nextInChain = nullptr;
    bindGroupDesc.label = nullptr;
    bindGroupDesc.layout = bindGroupLayout;
    bindGroupDesc.entryCount = uEntryCount;
    bindGroupDesc.entries = pEntries;
    return m_Device.CreateBindGroup(&bindGroupDesc);
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
