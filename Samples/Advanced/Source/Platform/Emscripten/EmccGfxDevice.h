#pragma once

#include "../../RenderGraph/GfxDevice.h"

class EmccGfxDevice : public GfxDevice
{
public:
    EmccGfxDevice();
    ~EmccGfxDevice();

protected:
    void InitWebGPU() override;
    void RequestAdapter() override;
    void GetSupportLimits() override;
    void CreateDevice() override;
    void CreateSwapChain() override;
    static void InstanceRequestAdapterCallback(WGPURequestAdapterStatus eStatus, WGPUAdapter pAdapter, char const *chMessage, void *pUserdata);
    static void AdapterRequestDeviceCallback(WGPURequestDeviceStatus eStatus, WGPUDevice pDevice, char const *chMessage, void *pUserdata);
    static void DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata);

private:
    wgpu::Adapter m_Adapter;
};
