#include "MacDawnGfxDevice.h"
#include "stdio.h"

MacDawnGfxDevice::MacDawnGfxDevice()
{
    m_Adapter = nullptr;
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
    m_Device = wgpu::Device(wgpuDevice);
    DawnProcTable procTable(dawn_native::GetProcs());
    procTable.deviceSetUncapturedErrorCallback(wgpuDevice, DeviceSetUncapturedErrorCallback, this);
}

void MacDawnGfxDevice::DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata)
{
    if (eType == WGPUErrorType_NoError)
        return;
    printf("GfxDevice Error Happened %d: %s\n", eType, chMessage);
    MacDawnGfxDevice *pGfxDevice = (MacDawnGfxDevice *)pUserdata;
    pGfxDevice->m_bErrorHappened = true;
}

void MacDawnGfxDevice::CreateSwapChain()
{
    
}
