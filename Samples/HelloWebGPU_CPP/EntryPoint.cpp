#include <stdio.h>
#include <assert.h>
#include <webgpu/webgpu_cpp.h>
#include <emscripten/emscripten.h>

enum RunningStatus
{
    Invalid,
    InitializingDevice,
    DeviceInitialized,
    SwapChainInitialized,
    Running,
    End
};

static RunningStatus ms_Status = Invalid;
static wgpu::Device ms_Device;
static wgpu::SwapChain ms_SwapChain;
static wgpu::TextureView ms_ColorBuffer;
static wgpu::TextureView ms_DepthBuffer;

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

    ms_Device = wgpu::Device::Acquire(device);
    ms_Device.SetUncapturedErrorCallback(DeviceSetUncapturedErrorCallback, nullptr);
    ms_Status = DeviceInitialized;
}

void InstanceRequestAdapterCallback(WGPURequestAdapterStatus status, WGPUAdapter adapter, char const *message, void *userdata)
{
    if (status != WGPURequestAdapterStatus_Success)
    {
        printf("wgpuInstanceRequestAdapter Failed!\n");
        return;
    }
    wgpuAdapterRequestDevice(adapter, NULL, AdapterRequestDeviceCallback, NULL);
}

void InitWGPUDevice()
{
    assert(ms_Status == Invalid);
    ms_Status = InitializingDevice;
    WGPURequestAdapterOptions adapterOptinons;
    adapterOptinons.powerPreference = WGPUPowerPreference_HighPerformance;
    wgpuInstanceRequestAdapter(NULL, &adapterOptinons, InstanceRequestAdapterCallback, NULL);
    printf("InitWGPUDevice\n");
}

void InitSwapChain()
{
    assert(ms_Status == DeviceInitialized);

    wgpu::SurfaceDescriptorFromCanvasHTMLSelector canvasHTMLSelectorDesc;
    canvasHTMLSelectorDesc.selector = "#canvas";
    wgpu::SurfaceDescriptor surfaceDesc;
    surfaceDesc.nextInChain = &canvasHTMLSelectorDesc;

    wgpu::Instance instance;
    wgpu::Surface surface = instance.CreateSurface(&surfaceDesc);

    wgpu::SwapChainDescriptor swapchainDesc;
    swapchainDesc.usage = wgpu::TextureUsage::RenderAttachment;
    swapchainDesc.format = wgpu::TextureFormat::BGRA8Unorm;
    swapchainDesc.width = 500;
    swapchainDesc.height = 500;
    swapchainDesc.presentMode = wgpu::PresentMode::Fifo;

    ms_SwapChain = ms_Device.CreateSwapChain(surface, &swapchainDesc);
    ms_Status = SwapChainInitialized;
    printf("InitSwapChain\n");
}

void SetupFrameBuffer()
{
    assert(ms_Status == SwapChainInitialized);
    ms_ColorBuffer = ms_SwapChain.GetCurrentTextureView();

    wgpu::TextureDescriptor textureDesc;
    textureDesc.label = "DepthBuffer";
    textureDesc.usage = wgpu::TextureUsage::RenderAttachment;
    textureDesc.dimension = wgpu::TextureDimension::e2D;
    textureDesc.size.width = 500;
    textureDesc.size.height = 500;
    textureDesc.size.depthOrArrayLayers = 1;
    textureDesc.format = wgpu::TextureFormat::Depth24PlusStencil8;
    textureDesc.mipLevelCount = 1;
    textureDesc.sampleCount = 1;
    wgpu::Texture depthTexture = ms_Device.CreateTexture(&textureDesc);
    ms_DepthBuffer = depthTexture.CreateView(nullptr);
    ms_Status = Running;
    printf("SetupFrameBuffer\n");
}

void BeginRender(wgpu::CommandEncoder& commandEncoder, wgpu::RenderPassEncoder& renderPassEncoder)
{
    wgpu::RenderPassColorAttachment colorAttachments[1];
    colorAttachments[0].view = ms_ColorBuffer;
    colorAttachments[0].resolveTarget = nullptr;
    colorAttachments[0].loadOp = wgpu::LoadOp::Clear;
    colorAttachments[0].storeOp = wgpu::StoreOp::Discard;
    colorAttachments[0].clearColor = {1, 0, 0, 1};

    wgpu::RenderPassDepthStencilAttachment depthAttachment;
    depthAttachment.view = ms_DepthBuffer;
    depthAttachment.depthLoadOp = wgpu::LoadOp::Clear;
    depthAttachment.depthStoreOp = wgpu::StoreOp::Store;
    depthAttachment.clearDepth = 0;
    depthAttachment.depthReadOnly = false;
    depthAttachment.stencilLoadOp = wgpu::LoadOp::Clear;
    depthAttachment.stencilStoreOp = wgpu::StoreOp::Store;
    depthAttachment.clearStencil = 0;
    depthAttachment.stencilReadOnly = false;

    wgpu::RenderPassDescriptor renderPassDesc;
    renderPassDesc.label = "ColorAttachment";
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = colorAttachments;
    renderPassDesc.depthStencilAttachment = &depthAttachment;
    renderPassDesc.occlusionQuerySet = nullptr;
    commandEncoder = ms_Device.CreateCommandEncoder(nullptr);
    renderPassEncoder = commandEncoder.BeginRenderPass(&renderPassDesc);
}

void QueueWorkDoneCallback(WGPUQueueWorkDoneStatus status, void *userdata)
{
    if (status == WGPUQueueWorkDoneStatus_Success)
        return;
    printf("QueueWorkDoneCallback %d\n", status);
}

void EndRender(wgpu::CommandEncoder& commandEncoder, wgpu::RenderPassEncoder& renderPassEncoder)
{
    renderPassEncoder.End();
    wgpu::CommandBuffer commandBuffer = commandEncoder.Finish();
    wgpu::Queue queue = ms_Device.GetQueue();
    queue.OnSubmittedWorkDone(0, QueueWorkDoneCallback, nullptr);
    queue.Submit(1, &commandBuffer);
}

void Render()
{
    wgpu::CommandEncoder commandEncoder = nullptr;
    wgpu::RenderPassEncoder renderPassEncoder = nullptr;
    BeginRender(commandEncoder, renderPassEncoder);
    EndRender(commandEncoder, renderPassEncoder);
}

void Loop()
{
    switch (ms_Status)
    {
    case Invalid:
        InitWGPUDevice();
        break;
    case DeviceInitialized:
        InitSwapChain();
        break;
    case SwapChainInitialized:
        SetupFrameBuffer();
        break;
    case Running:
        Render();
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    ms_Device = nullptr;
    ms_SwapChain = nullptr;
    ms_ColorBuffer = nullptr;
    ms_DepthBuffer = nullptr;
    emscripten_set_main_loop(Loop, 0, false);
    return 0;
}