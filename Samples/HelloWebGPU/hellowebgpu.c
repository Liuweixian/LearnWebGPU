#include <stdio.h>
#include <webgpu/webgpu.h>

void DeviceSetUncapturedErrorCallback(WGPUErrorType type, char const * message, void * userdata)
{
    printf("DeviceSetUncapturedErrorCallback %d: %s\n", type, message);
}

void AdapterRequestDeviceCallback(WGPURequestDeviceStatus status, WGPUDevice device, char const * message, void * userdata)
{
    if (status != WGPURequestDeviceStatus_Success)
    {
        printf("wgpuAdapterRequestDevice Failed!\n");
        return;
    }
    printf("wgpuDeviceSetUncapturedErrorCallback!\n");
    wgpuDeviceSetUncapturedErrorCallback(device, DeviceSetUncapturedErrorCallback, NULL);

    WGPUSwapChainDescriptor swapchainDesc;
    swapchainDesc.format = WGPUTextureFormat_BGRA8Unorm;
    swapchainDesc.width = 500;
    swapchainDesc.height = 500;
    swapchainDesc.presentMode = WGPUPresentMode_Fifo;
    printf("wgpuDeviceCreateSwapChain!\n");
    wgpuDeviceCreateSwapChain(device, NULL, &swapchainDesc);

    printf("wgpuDeviceGetQueue!\n");
    WGPUQueue wgpuQueue = wgpuDeviceGetQueue(device);

    
}

void InstanceRequestAdapterCallback(WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * userdata)
{
    if (status != WGPURequestAdapterStatus_Success)
    {
        printf("wgpuInstanceRequestAdapter Failed!\n");
        return;
    }
    printf("wgpuAdapterRequestDevice!\n");
    wgpuAdapterRequestDevice(adapter, NULL, AdapterRequestDeviceCallback, NULL);
}

int main(int argc, char **argv)
{
    WGPURequestAdapterOptions adapterOptinons;
    adapterOptinons.powerPreference = WGPUPowerPreference_HighPerformance;
    printf("wgpuInstanceRequestAdapter!\n");
    wgpuInstanceRequestAdapter(NULL, &adapterOptinons, InstanceRequestAdapterCallback, NULL);
    return 0;
}