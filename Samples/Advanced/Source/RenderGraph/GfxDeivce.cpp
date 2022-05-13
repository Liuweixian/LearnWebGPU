#include "GfxDevice.h"
#include <stdio.h>
#include <cassert>

GfxDevice::GfxDevice()
{
    m_Device = nullptr;
    m_SwapChain = nullptr;
    m_Adapter = nullptr;
    m_pSupportedLimits = nullptr;
    m_CommandEncoder = nullptr;
    m_RenderPassEncoders.clear();
    m_bInitialized = false;

    InitWGPUDevice();
}

GfxDevice::~GfxDevice()
{
}

void GfxDevice::DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata)
{
    if (eType == WGPUErrorType_NoError)
        return;
    printf("GfxDevice Error Happened %d: %s\n", eType, chMessage);
}

void GfxDevice::AdapterRequestDeviceCallback(WGPURequestDeviceStatus eStatus, WGPUDevice pDevice, char const *chMessage, void *pUserdata)
{
    if (eStatus != WGPURequestDeviceStatus_Success)
    {
        printf("GfxDevice::InitWGPUDevice Failed! wgpuAdapterRequestDevice return %d \n", eStatus);
        return;
    }

    GfxDevice *pGfxDevice = (GfxDevice *)pUserdata;
    pGfxDevice->m_Device = wgpu::Device::Acquire(pDevice);
    pGfxDevice->m_Device.SetUncapturedErrorCallback(DeviceSetUncapturedErrorCallback, nullptr);
    pGfxDevice->InitWGPUSwapChain();
    printf("GfxDevice::InitWGPUDevice Success\n");
}

void GfxDevice::InstanceRequestAdapterCallback(WGPURequestAdapterStatus eStatus, WGPUAdapter pAdapter, char const *chMessage, void *pUserdata)
{
    if (eStatus != WGPURequestAdapterStatus_Success)
    {
        printf("GfxDevice::InitWGPUDevice Failed! wgpuInstanceRequestAdapter return %d \n", eStatus);
        return;
    }
    GfxDevice *pGfxDevice = (GfxDevice *)pUserdata;
    pGfxDevice->m_Adapter = wgpu::Adapter::Acquire(pAdapter);
    pGfxDevice->GetAdapterLimits();
    wgpuAdapterRequestDevice(pAdapter, NULL, AdapterRequestDeviceCallback, pUserdata);
}

void GfxDevice::InitWGPUDevice()
{
    WGPURequestAdapterOptions *adapterOptinons = new WGPURequestAdapterOptions();
    adapterOptinons->powerPreference = WGPUPowerPreference_HighPerformance;
    wgpuInstanceRequestAdapter(NULL, adapterOptinons, InstanceRequestAdapterCallback, this);
}

void GfxDevice::InitWGPUSwapChain()
{
    int nCanvasWidth = 0;
    int nCanvasHeight = 0;
    emscripten_get_canvas_element_size("#canvas", &nCanvasWidth, &nCanvasHeight);

    wgpu::SurfaceDescriptorFromCanvasHTMLSelector canvasHTMLSelectorDesc;
    canvasHTMLSelectorDesc.selector = "#canvas";
    wgpu::SurfaceDescriptor surfaceDesc;
    surfaceDesc.nextInChain = &canvasHTMLSelectorDesc;

    wgpu::Instance instance;
    wgpu::Surface surface = instance.CreateSurface(&surfaceDesc);

    wgpu::SwapChainDescriptor swapchainDesc;
    swapchainDesc.usage = wgpu::TextureUsage::RenderAttachment;
    swapchainDesc.format = wgpu::TextureFormat::BGRA8Unorm;
    swapchainDesc.width = nCanvasWidth;
    swapchainDesc.height = nCanvasHeight;
    swapchainDesc.presentMode = wgpu::PresentMode::Fifo;

    m_SwapChain = m_Device.CreateSwapChain(surface, &swapchainDesc);
    printf("GfxDevice::InitWGPUSwapChain Success\n");
    m_bInitialized = true;
}

void GfxDevice::GetAdapterLimits()
{
    wgpu::AdapterProperties properties;
    m_Adapter.GetProperties(&properties);
    printf("GfxDevice vendorID:%d, deviceID:%d, name:%s, driverDescription:%s, adapterType:%d, backendType:%d\n",
           properties.vendorID, properties.deviceID, properties.name, properties.driverDescription, properties.adapterType, properties.backendType);

    /*m_pSupportedLimits = new wgpu::SupportedLimits();
    m_Adapter.GetLimits(m_pSupportedLimits);

    printf("GfxDevice Limits:\n");
    printf("maxTextureDimension1D:%d\n", m_pSupportedLimits->limits.maxTextureDimension1D);
    printf("maxTextureDimension2D:%d\n", m_pSupportedLimits->limits.maxTextureDimension2D);
    printf("maxTextureDimension3D:%d\n", m_pSupportedLimits->limits.maxTextureDimension3D);
    printf("maxTextureArrayLayers:%d\n", m_pSupportedLimits->limits.maxTextureArrayLayers);
    printf("maxBindGroups:%d\n", m_pSupportedLimits->limits.maxBindGroups);
    printf("maxDynamicUniformBuffersPerPipelineLayout:%d\n", m_pSupportedLimits->limits.maxDynamicUniformBuffersPerPipelineLayout);
    printf("maxDynamicStorageBuffersPerPipelineLayout:%d\n", m_pSupportedLimits->limits.maxDynamicStorageBuffersPerPipelineLayout);
    printf("maxSampledTexturesPerShaderStage:%d\n", m_pSupportedLimits->limits.maxSampledTexturesPerShaderStage);
    printf("maxSamplersPerShaderStage:%d\n", m_pSupportedLimits->limits.maxSamplersPerShaderStage);
    printf("maxStorageBuffersPerShaderStage:%d\n", m_pSupportedLimits->limits.maxStorageBuffersPerShaderStage);
    printf("maxStorageTexturesPerShaderStage:%d\n", m_pSupportedLimits->limits.maxStorageTexturesPerShaderStage);
    printf("maxUniformBuffersPerShaderStage:%d\n", m_pSupportedLimits->limits.maxUniformBuffersPerShaderStage);
    printf("maxUniformBufferBindingSize:%llu\n", m_pSupportedLimits->limits.maxUniformBufferBindingSize);
    printf("maxStorageBufferBindingSize:%llu\n", m_pSupportedLimits->limits.maxStorageBufferBindingSize);
    printf("minUniformBufferOffsetAlignment:%d\n", m_pSupportedLimits->limits.minUniformBufferOffsetAlignment);
    printf("minStorageBufferOffsetAlignment:%d\n", m_pSupportedLimits->limits.minStorageBufferOffsetAlignment);
    printf("maxVertexBuffers:%d\n", m_pSupportedLimits->limits.maxVertexBuffers);
    printf("maxVertexAttributes:%d\n", m_pSupportedLimits->limits.maxVertexAttributes);
    printf("maxVertexBufferArrayStride:%d\n", m_pSupportedLimits->limits.maxVertexBufferArrayStride);
    printf("maxInterStageShaderComponents:%d\n", m_pSupportedLimits->limits.maxInterStageShaderComponents);
    printf("maxComputeWorkgroupStorageSize:%d\n", m_pSupportedLimits->limits.maxComputeWorkgroupStorageSize);
    printf("maxComputeInvocationsPerWorkgroup:%d\n", m_pSupportedLimits->limits.maxComputeInvocationsPerWorkgroup);
    printf("maxComputeWorkgroupSizeX:%d\n", m_pSupportedLimits->limits.maxComputeWorkgroupSizeX);
    printf("maxComputeWorkgroupSizeY:%d\n", m_pSupportedLimits->limits.maxComputeWorkgroupSizeY);
    printf("maxComputeWorkgroupSizeZ:%d\n", m_pSupportedLimits->limits.maxComputeWorkgroupSizeZ);
    printf("maxComputeWorkgroupsPerDimension:%d\n", m_pSupportedLimits->limits.maxComputeWorkgroupsPerDimension);*/
}

void GfxDevice::BeginFrame()
{
    if (!m_bInitialized)
        return;
    assert(m_CommandEncoder == nullptr);
    assert(m_RenderPassEncoders.size() == 0);
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
    wgpu::CommandBuffer commandBuffer = m_CommandEncoder.Finish();
    wgpu::Queue queue = m_Device.GetQueue();
    queue.OnSubmittedWorkDone(0, QueueWorkDoneCallback, this);
    queue.Submit(1, &commandBuffer);
    m_CommandEncoder = nullptr;
}

void GfxDevice::SetRenderTarget(std::list<RenderResourceHandle *> targetColorBuffers, RenderResourceHandle *pTargetDepthBuffer)
{
    uint32_t unEncoderIdx = 0;
    int nColorAttachmentCount = targetColorBuffers.size();
    wgpu::RenderPassColorAttachment colorAttachments[nColorAttachmentCount];
    for (int i = 0; i < nColorAttachmentCount; i++)
    {
        //colorAttachments[0].view = m_SwapChain.GetCurrentTextureView();
        //colorAttachments[0].loadOp = wgpu::LoadOp::Undefined;
        //colorAttachments[0].clearColor = {0.0f, 0.0f, 0.0f, 1.0f}; // clear color, loadop must be Undefiend
    }

    wgpu::RenderPassDepthStencilAttachment *pDepthAttachment = nullptr;
    if (pTargetDepthBuffer != nullptr)
    {
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

}

void GfxDevice::SetRenderState()
{
}

void GfxDevice::DrawBuffer()
{
}

static GfxDevice *g_pGfxDevice = nullptr;
GfxDevice *GetGfxDevice()
{
    if (g_pGfxDevice == nullptr)
        g_pGfxDevice = new GfxDevice();
    return g_pGfxDevice;
}