#include <stdio.h>
#include <webgpu/webgpu.h>

typedef struct SurfaceDescriptorFromCanvasHTMLSelector
{
    struct WGPUChainedStruct const * next;
    WGPUSType sType;
    char const *selector;
}SurfaceDescriptorFromCanvasHTMLSelector;

void QueueWorkDoneCallback(WGPUQueueWorkDoneStatus status, void * userdata)
{
    printf("QueueWorkDoneCallback %d\n", status);
}

void DeviceSetUncapturedErrorCallback(WGPUErrorType type, char const *message, void *userdata)
{
    printf("DeviceSetUncapturedErrorCallback %d: %s\n", type, message);
}

void AdapterRequestDeviceCallback(WGPURequestDeviceStatus status, WGPUDevice device, char const *message, void *userdata)
{
    if (status != WGPURequestDeviceStatus_Success)
    {
        printf("wgpuAdapterRequestDevice Failed!\n");
        return;
    }

    printf("wgpuDeviceSetUncapturedErrorCallback!\n");
    wgpuDeviceSetUncapturedErrorCallback(device, DeviceSetUncapturedErrorCallback, NULL);

    SurfaceDescriptorFromCanvasHTMLSelector canvasDesc;
    canvasDesc.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
    canvasDesc.selector = "#canvas";
    WGPUSurfaceDescriptor surfaceDesc;
    surfaceDesc.nextInChain = (WGPUChainedStruct *)&canvasDesc;
    printf("wgpuInstanceCreateSurface!\n");
    WGPUSurface surface = wgpuInstanceCreateSurface(NULL, &surfaceDesc);

    WGPUSwapChainDescriptor swapchainDesc;
    swapchainDesc.format = WGPUTextureFormat_BGRA8Unorm;
    swapchainDesc.width = 500;
    swapchainDesc.height = 500;
    swapchainDesc.presentMode = WGPUPresentMode_Fifo;
    swapchainDesc.usage = WGPUTextureUsage_RenderAttachment;
    printf("wgpuDeviceCreateSwapChain!\n");
    WGPUSwapChain gpuSwapChain = wgpuDeviceCreateSwapChain(device, surface, &swapchainDesc);
    printf("wgpuSwapChainGetCurrentTextureView!\n");
    WGPUTextureView colorBuffer = wgpuSwapChainGetCurrentTextureView(gpuSwapChain);

    WGPUTextureDescriptor textureDesc;
    textureDesc.dimension = WGPUTextureDimension_2D;
    textureDesc.format = WGPUTextureFormat_Depth24PlusStencil8;
    textureDesc.label = "DepthBuffer";
    textureDesc.mipLevelCount = 1;
    WGPUExtent3D extend3d;
    extend3d.width = 500;
    extend3d.height = 500;
    extend3d.depthOrArrayLayers = 1;
    textureDesc.size = extend3d;
    textureDesc.usage = WGPUTextureUsage_RenderAttachment;
    textureDesc.sampleCount = 1;
    printf("wgpuDeviceCreateTexture!\n");
    WGPUTexture depthTexture = wgpuDeviceCreateTexture(device, &textureDesc);
    printf("wgpuTextureCreateView!\n");
    WGPUTextureView depthBuffer = wgpuTextureCreateView(depthTexture, NULL);

    printf("wgpuDeviceCreateCommandEncoder!\n");
    WGPUCommandEncoder commandEncoder = wgpuDeviceCreateCommandEncoder(device, NULL);

    WGPURenderPassColorAttachment colorAttachments[1];
    colorAttachments[0].view = colorBuffer;
    WGPUColor clearColor;
    clearColor.r = 1.0f;
    clearColor.g = 0.0f;
    clearColor.b = 0.0f;
    clearColor.a = 1.0f;
    colorAttachments[0].clearColor = clearColor;
    WGPURenderPassDepthStencilAttachment depthAttachment;
    depthAttachment.view = depthBuffer;
    depthAttachment.depthLoadOp = WGPULoadOp_Clear;
    depthAttachment.depthStoreOp = WGPUStoreOp_Discard;
    depthAttachment.stencilLoadOp = WGPULoadOp_Clear;
    WGPURenderPassDescriptor renderPassDesc;
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = colorAttachments;
    renderPassDesc.depthStencilAttachment = &depthAttachment;
    printf("wgpuCommandEncoderBeginRenderPass!\n");
    WGPURenderPassEncoder renderPassEncoder = wgpuCommandEncoderBeginRenderPass(commandEncoder, &renderPassDesc);


    printf("wgpuRenderPassEncoderEnd!\n");
    wgpuRenderPassEncoderEnd(renderPassEncoder);
    WGPUCommandBuffer commandBuffer = wgpuCommandEncoderFinish(commandEncoder, NULL);

    printf("wgpuDeviceGetQueue!\n");
    WGPUQueue wgpuQueue = wgpuDeviceGetQueue(device);
    printf("wgpuQueueOnSubmittedWorkDone!\n");
    wgpuQueueOnSubmittedWorkDone(wgpuQueue, 0, QueueWorkDoneCallback, NULL);
    printf("wgpuQueueSubmit!\n");
    wgpuQueueSubmit(wgpuQueue, 1, &commandBuffer);
}

void InstanceRequestAdapterCallback(WGPURequestAdapterStatus status, WGPUAdapter adapter, char const *message, void *userdata)
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