#include "MacDawnGfxDevice.h"
#include "stdio.h"

MacDawnGfxDevice::MacDawnGfxDevice(NSWindow* pWindow)
{
    m_pDawnSwapChainImpl = nullptr;
    m_Adapter = nullptr;
    CreateDawnSwapChainImplementation(pWindow);
    printf("Create Dawn GfxDevice for Mac\n");
}

MacDawnGfxDevice::~MacDawnGfxDevice()
{
    
}

void MacDawnGfxDevice::InitWebGPU()
{
    RequestAdapter();
    GetSupportLimits();
    CreateDevice();
    CreateSwapChain();
}

void MacDawnGfxDevice::RequestAdapter()
{
    m_Instance.DiscoverDefaultAdapters();
    wgpu::AdapterProperties properties;
    std::vector<dawn_native::Adapter> adapters = m_Instance.GetAdapters();
    for (auto it = adapters.begin(); it != adapters.end(); it++)
    {
        it->GetProperties(&properties);
        if (static_cast<WGPUBackendType>(properties.backendType) == WGPUBackendType_Metal)
        {
            m_Adapter = (*it);
            break;
        }
    }
    printf("Found Adapter: GPU:%s, Driver:%s\n", properties.name, properties.driverDescription);
}

void MacDawnGfxDevice::GetSupportLimits()
{
    m_Adapter.GetLimits((WGPUSupportedLimits*)m_pSupportedLimits);
    PrintSupportedLimits();
}

void MacDawnGfxDevice::CreateDevice()
{
    WGPUDevice wgpuDevice = m_Adapter.CreateDevice();
    m_Device = wgpu::Device::Acquire(wgpuDevice);
    DawnProcTable procTable(dawn_native::GetProcs());
    procTable.deviceSetUncapturedErrorCallback(m_Device.Get(), DeviceSetUncapturedErrorCallback, this);
    dawnProcSetProcs(&procTable);
}

void MacDawnGfxDevice::DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata)
{
    if (eType == WGPUErrorType_NoError)
        return;
    printf("GfxDevice Error Happened %d: %s\n", eType, chMessage);
    MacDawnGfxDevice *pGfxDevice = (MacDawnGfxDevice *)pUserdata;
    pGfxDevice->m_bErrorHappened = true;
}

MetalDawnSwapChain* MacDawnGfxDevice::GetMetalDawnSwapChain()
{
    assert(m_pDawnSwapChainImpl != nullptr && m_pDawnSwapChainImpl->userData != nullptr);
    return static_cast<MetalDawnSwapChain*>(m_pDawnSwapChainImpl->userData);
}

void MacDawnGfxDevice::CreateDawnSwapChainImplementation(NSWindow *pWindow)
{
    m_pDawnSwapChainImpl = new DawnSwapChainImplementation();
    m_pDawnSwapChainImpl->userData = new MetalDawnSwapChain(pWindow);
    m_pDawnSwapChainImpl->Init = [](void *pUserData, void* pWSIContext){
        static_cast<MetalDawnSwapChain*>(pUserData)->Init(static_cast<DawnWSIContextMetal*>(pWSIContext));
    };
    m_pDawnSwapChainImpl->Destroy = [](void *pUserData){
        delete static_cast<MetalDawnSwapChain*>(pUserData);
    };
    m_pDawnSwapChainImpl->Configure = [](void *pUserData, WGPUTextureFormat eFormat, WGPUTextureUsage eUsage, uint32_t width, uint32_t height){
        return static_cast<MetalDawnSwapChain*>(pUserData)->Configure(eFormat, eUsage, width, height);
    };
    m_pDawnSwapChainImpl->GetNextTexture = [](void *pUserData, DawnSwapChainNextTexture *pNext){
        return static_cast<MetalDawnSwapChain*>(pUserData)->GetNextTexture(pNext);
    };
    m_pDawnSwapChainImpl->Present = [](void *pUserData){
        return static_cast<MetalDawnSwapChain*>(pUserData)->Present();
    };
}

void MacDawnGfxDevice::CreateSwapChain()
{
    WGPUSwapChainDescriptor swapchainDesc = {};
    swapchainDesc.implementation = reinterpret_cast<uintptr_t>(&m_pDawnSwapChainImpl);
    WGPUSwapChain swapchain = wgpuDeviceCreateSwapChain(m_Device.Get(), nullptr, &swapchainDesc);
    m_SwapChain = wgpu::SwapChain::Acquire(swapchain);
    uint32_t uWidth = 0;
    uint32_t uHeight = 0;
    GetMetalDawnSwapChain()->GetWindowSize(uWidth, uHeight);
    wgpuSwapChainConfigure(swapchain, WGPUTextureFormat_BGRA8Unorm, WGPUTextureUsage_RenderAttachment, uWidth, uHeight);
}
