#include "GfxDevice.h"
#include <stdio.h>

GfxDevice::GfxDevice()
{
    m_oDevice = nullptr;
    m_oSwapChain = nullptr;
    m_bInitialized = false;
    InitWGPUDevice();
}

GfxDevice::~GfxDevice()
{
}

void GfxDevice::DeviceSetUncapturedErrorCallback(WGPUErrorType type, char const *message, void *userdata)
{
    if (type == WGPUErrorType_NoError)
        return;
    printf("GfxDevice Error Happened %d: %s\n", type, message);
}

void GfxDevice::AdapterRequestDeviceCallback(WGPURequestDeviceStatus status, WGPUDevice device, char const *message, void *userdata)
{
    if (status != WGPURequestDeviceStatus_Success)
    {
        printf("GfxDevice::InitWGPUDevice Failed! wgpuAdapterRequestDevice return %d \n", status);
        return;
    }

    GfxDevice *pGfxDevice = (GfxDevice *)userdata;
    pGfxDevice->m_oDevice = wgpu::Device::Acquire(device);
    pGfxDevice->m_oDevice.SetUncapturedErrorCallback(DeviceSetUncapturedErrorCallback, nullptr);
    pGfxDevice->InitWGPUSwapChain();
    printf("GfxDevice::InitWGPUDevice Success\n");
}

void GfxDevice::InstanceRequestAdapterCallback(WGPURequestAdapterStatus status, WGPUAdapter adapter, char const *message, void *userdata)
{
    if (status != WGPURequestAdapterStatus_Success)
    {
        printf("GfxDevice::InitWGPUDevice Failed! wgpuInstanceRequestAdapter return %d \n", status);
        return;
    }
    wgpuAdapterRequestDevice(adapter, NULL, AdapterRequestDeviceCallback, userdata);
}

void GfxDevice::InitWGPUDevice()
{
    WGPURequestAdapterOptions adapterOptinons;
    adapterOptinons.powerPreference = WGPUPowerPreference_HighPerformance;
    wgpuInstanceRequestAdapter(NULL, &adapterOptinons, InstanceRequestAdapterCallback, this);
}

void GfxDevice::InitWGPUSwapChain()
{
    int canvasWidth = 0;
    int canvasHeight = 0;
    emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);

    wgpu::SurfaceDescriptorFromCanvasHTMLSelector canvasHTMLSelectorDesc;
    canvasHTMLSelectorDesc.selector = "#canvas";
    wgpu::SurfaceDescriptor surfaceDesc;
    surfaceDesc.nextInChain = &canvasHTMLSelectorDesc;

    wgpu::Instance instance;
    wgpu::Surface surface = instance.CreateSurface(&surfaceDesc);

    wgpu::SwapChainDescriptor swapchainDesc;
    swapchainDesc.usage = wgpu::TextureUsage::RenderAttachment;
    swapchainDesc.format = wgpu::TextureFormat::BGRA8Unorm;
    swapchainDesc.width = canvasWidth;
    swapchainDesc.height = canvasHeight;
    swapchainDesc.presentMode = wgpu::PresentMode::Fifo;

    m_oSwapChain = m_oDevice.CreateSwapChain(surface, &swapchainDesc);
    printf("GfxDevice::InitWGPUSwapChain Success\n");
    m_bInitialized = true;
}