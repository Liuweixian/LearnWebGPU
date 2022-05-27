#pragma once

#include <stdio.h>
#include "../../RenderGraph/GfxDevice.h"
#include "lib/dawn/inc/dawn/native/DawnNative.h"

class MacDawnGfxDevice : public GfxDevice
{
public:
    MacDawnGfxDevice();
    ~MacDawnGfxDevice();

protected:
    void InitWebGPU() override;
    void RequestAdapter() override;
    void GetSupportLimits() override;
    void CreateDevice() override;
    void CreateSwapChain() override;
    static void DeviceSetUncapturedErrorCallback(WGPUErrorType eType, char const *chMessage, void *pUserdata);

private:
    dawn_native::Instance m_Instance;
    dawn_native::Adapter m_Adapter;
};
