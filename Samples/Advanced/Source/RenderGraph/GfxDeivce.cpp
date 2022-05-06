#include "GfxDevice.h"
#include <stdio.h>

GfxDevice::GfxDevice()
{
    m_Device = nullptr;
    m_SwapChain = nullptr;
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
    pGfxDevice->m_pAdapter = pAdapter;
    wgpuAdapterRequestDevice(pGfxDevice->m_pAdapter, NULL, AdapterRequestDeviceCallback, pUserdata);
}

void GfxDevice::InitWGPUDevice()
{
    WGPURequestAdapterOptions adapterOptinons;
    adapterOptinons.powerPreference = WGPUPowerPreference_HighPerformance;
    wgpuInstanceRequestAdapter(NULL, &adapterOptinons, InstanceRequestAdapterCallback, this);
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