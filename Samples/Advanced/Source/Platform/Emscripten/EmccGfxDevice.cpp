#include "EmccGfxDevice.h"
#include "stdio.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

EmccGfxDevice::EmccGfxDevice()
{
    printf("Create Emcc GfxDevice\n");
}

EmccGfxDevice::~EmccGfxDevice()
{
}

void EmccGfxDevice::InitWebGPU()
{
    RequestAdapter();
}

void EmccGfxDevice::RequestAdapter()
{
    WGPURequestAdapterOptions *adapterOptinons = new WGPURequestAdapterOptions();
    adapterOptinons->powerPreference = WGPUPowerPreference_HighPerformance;
    wgpuInstanceRequestAdapter(NULL, adapterOptinons, InstanceRequestAdapterCallback, this);
}

void EmccGfxDevice::InstanceRequestAdapterCallback(WGPURequestAdapterStatus eStatus, WGPUAdapter pAdapter, char const *chMessage, void *pUserdata)
{
    if (eStatus != WGPURequestAdapterStatus_Success)
    {
        printf("wgpuInstanceRequestAdapter Failed! return %d \n", eStatus);
        return;
    }
    EmccGfxDevice *pGfxDevice = (EmccGfxDevice *)pUserdata;
    pGfxDevice->m_Adapter = wgpu::Adapter::Acquire(pAdapter);
    wgpu::AdapterProperties properties;
    pGfxDevice->m_Adapter.GetProperties(&properties);
    printf("Found Adapter: name:%s, driverDescription:%s, adapterType:%d, backendType:%d\n",
           properties.name, properties.driverDescription, properties.adapterType, properties.backendType);
    pGfxDevice->GetSupportLimits();
    pGfxDevice->CreateDevice();
}

void EmccGfxDevice::GetSupportLimits()
{
    /*m_Adapter.GetLimits(m_pSupportedLimits);
    PrintSupportedLimits();*/
}

void EmccGfxDevice::CreateDevice()
{
    m_Adapter.RequestDevice(NULL, AdapterRequestDeviceCallback, this);
}

void EmccGfxDevice::AdapterRequestDeviceCallback(WGPURequestDeviceStatus eStatus, WGPUDevice pDevice, char const *chMessage, void *pUserdata)
{
    if (eStatus != WGPURequestDeviceStatus_Success)
    {
        printf("wgpuAdapterRequestDevice Failed! return %d \n", eStatus);
        return;
    }

    EmccGfxDevice *pGfxDevice = (EmccGfxDevice *)pUserdata;
    pGfxDevice->m_Device = wgpu::Device::Acquire(pDevice);
    pGfxDevice->m_Device.SetUncapturedErrorCallback(DeviceSetUncapturedErrorCallback, pGfxDevice);
    pGfxDevice->CreateSwapChain();
}

void EmccGfxDevice::DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata)
{
    if (eType == WGPUErrorType_NoError)
        return;
    printf("GfxDevice Error Happened %d: %s\n", eType, chMessage);
    EmccGfxDevice *pGfxDevice = (EmccGfxDevice *)pUserdata;
    pGfxDevice->m_bErrorHappened = true;
}

void EmccGfxDevice::CreateSwapChain()
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
    printf("CreateSwapChain Success\n");
}