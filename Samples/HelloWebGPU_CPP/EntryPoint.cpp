#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <webgpu/webgpu_cpp.h>
#include <emscripten/emscripten.h>

enum RunningStatus
{
    Invalid,
    InitializingDevice,
    DeviceInitialized,
    SwapChainInitialized,
    FrameBufferInitialized,
    Running,
    Error,
    End
};

enum SampleCase
{
    NoBufferDrawTriangle,
    UseBufferDrawQuad,
};

static RunningStatus ms_Status = Invalid;
static SampleCase ms_SampleCase = UseBufferDrawQuad;
static wgpu::Device ms_Device = nullptr;
static wgpu::SwapChain ms_SwapChain = nullptr;
static wgpu::TextureView ms_DepthBuffer = nullptr;
static wgpu::RenderPipeline ms_RenderPipeline = nullptr;
static wgpu::Buffer ms_VertexBuffer = nullptr;
static wgpu::Buffer ms_IndexBuffer = nullptr;

void DeviceSetUncapturedErrorCallback(WGPUErrorType type, char const *message, void *userdata)
{
    if (type == WGPUErrorType_NoError)
        return;
    printf("DeviceSetUncapturedErrorCallback %d: %s\n", type, message);
    ms_Status = Error;
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
    ms_Status = FrameBufferInitialized;
    printf("SetupFrameBuffer\n");
}

wgpu::PipelineLayout CreatePipelineLayout()
{
    wgpu::PipelineLayoutDescriptor layoutDesc;
    wgpu::PipelineLayout layout = ms_Device.CreatePipelineLayout(&layoutDesc);
    return layout;
}

const char *ReadAllText(const char *filePath)
{
    FILE *handle = fopen(filePath, "r");
    assert(handle != nullptr);
    fseek(handle, 0, SEEK_END);
    int fileSize = ftell(handle);
    rewind(handle);
    char *buffer = (char *)malloc(sizeof(char) * fileSize);
    size_t read = fread(buffer, 1, fileSize, handle);
    fclose(handle);
    assert(read == fileSize);
    return buffer;
}

wgpu::ShaderModule CreateVertexShaderModule()
{
    wgpu::ShaderModuleWGSLDescriptor wgslDesc;
    wgslDesc.source = ReadAllText("Shaders/VertexShader.wgsl");
    wgpu::ShaderModuleDescriptor shaderModuleDesc;
    shaderModuleDesc.nextInChain = &wgslDesc;
    wgpu::ShaderModule shaderModule = ms_Device.CreateShaderModule(&shaderModuleDesc);
    return shaderModule;
}

wgpu::VertexState CreateVertexState()
{
    wgpu::VertexState vertexState;
    vertexState.module = CreateVertexShaderModule();
    vertexState.entryPoint = "vs_main";
    return vertexState;
}

wgpu::PrimitiveState CreatePrimitiveState()
{
    wgpu::PrimitiveState state;
    state.topology = wgpu::PrimitiveTopology::TriangleList;
    state.stripIndexFormat = wgpu::IndexFormat::Undefined;
    state.frontFace = wgpu::FrontFace::CCW;
    state.cullMode = wgpu::CullMode::None;
    return state;
}

wgpu::DepthStencilState *CreateDepthStencilState()
{
    wgpu::DepthStencilState *state = new wgpu::DepthStencilState();
    state->format = wgpu::TextureFormat::Depth24PlusStencil8;
    return state;
}

wgpu::MultisampleState CreateMultisampleState()
{
    wgpu::MultisampleState state;
    return state;
}

wgpu::ShaderModule CreateFragmentShaderModule()
{
    wgpu::ShaderModuleWGSLDescriptor wgslDesc;
    wgslDesc.source = ReadAllText("Shaders/FragShader.wgsl");
    wgpu::ShaderModuleDescriptor shaderModuleDesc;
    shaderModuleDesc.nextInChain = &wgslDesc;
    wgpu::ShaderModule shaderModule = ms_Device.CreateShaderModule(&shaderModuleDesc);
    return shaderModule;
}

wgpu::FragmentState *CreateFragmentState()
{
    wgpu::FragmentState *fragmentState = new wgpu::FragmentState();
    fragmentState->module = CreateFragmentShaderModule();
    fragmentState->entryPoint = "fs_main";

    wgpu::ColorTargetState colorTargetState[1];
    colorTargetState[0].format = wgpu::TextureFormat::BGRA8Unorm;
    fragmentState->targetCount = 1;
    fragmentState->targets = colorTargetState;
    return fragmentState;
}

void SetupRenderPipeline()
{
    assert(ms_RenderPipeline == nullptr);
    wgpu::RenderPipelineDescriptor renderPipelineDesc;
    renderPipelineDesc.layout = CreatePipelineLayout();
    renderPipelineDesc.vertex = CreateVertexState();
    renderPipelineDesc.primitive = CreatePrimitiveState();
    renderPipelineDesc.depthStencil = CreateDepthStencilState();
    renderPipelineDesc.multisample = CreateMultisampleState();
    renderPipelineDesc.fragment = CreateFragmentState();
    ms_RenderPipeline = ms_Device.CreateRenderPipeline(&renderPipelineDesc);
    ms_Status = Running;
}

void BeginRender(wgpu::CommandEncoder &commandEncoder, wgpu::RenderPassEncoder &renderPassEncoder)
{
    assert(commandEncoder == nullptr && renderPassEncoder == nullptr);
    wgpu::RenderPassColorAttachment colorAttachments[1];
    colorAttachments[0].view = ms_SwapChain.GetCurrentTextureView();
    colorAttachments[0].loadOp = wgpu::LoadOp::Undefined;
    colorAttachments[0].clearColor = {1.0f, 0.0f, 0.0f, 1.0f}; // clear color, loadop must be Undefiend

    wgpu::RenderPassDepthStencilAttachment depthAttachment;
    depthAttachment.view = ms_DepthBuffer;
    depthAttachment.depthLoadOp = wgpu::LoadOp::Clear;
    depthAttachment.depthStoreOp = wgpu::StoreOp::Discard;
    depthAttachment.clearDepth = 0;
    depthAttachment.depthReadOnly = false;
    depthAttachment.stencilLoadOp = wgpu::LoadOp::Clear;
    depthAttachment.stencilStoreOp = wgpu::StoreOp::Discard;
    depthAttachment.clearStencil = 0;
    depthAttachment.stencilReadOnly = false;

    wgpu::RenderPassDescriptor renderPassDesc;
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

void EndRender(wgpu::CommandEncoder &commandEncoder, wgpu::RenderPassEncoder &renderPassEncoder)
{
    assert(commandEncoder != nullptr && renderPassEncoder != nullptr);
    renderPassEncoder.End();
    wgpu::CommandBuffer commandBuffer = commandEncoder.Finish();
    wgpu::Queue queue = ms_Device.GetQueue();
    queue.OnSubmittedWorkDone(0, QueueWorkDoneCallback, nullptr);
    queue.Submit(1, &commandBuffer);
}

void BufferMappedCallback(WGPUBufferMapAsyncStatus status, void *userdata)
{
}

void SetupVertexBuffer(wgpu::RenderPassEncoder &renderPassEncoder)
{
    int totalSize = 8 * sizeof(float);
    if (ms_VertexBuffer == nullptr)
    {
        float verts[8] = {-0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5};
        wgpu::BufferDescriptor bufferDesc;
        bufferDesc.usage = wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst;
        bufferDesc.size = totalSize;
        ms_VertexBuffer = ms_Device.CreateBuffer(&bufferDesc);
        wgpu::Queue queue = ms_Device.GetQueue();
        queue.WriteBuffer(ms_VertexBuffer, 0, verts, totalSize);
    }
    renderPassEncoder.SetVertexBuffer(0, ms_VertexBuffer, 0, totalSize);
}

void SetupIndexBuffer(wgpu::RenderPassEncoder &renderPassEncoder)
{
    int totalSize = 8 * sizeof(uint16_t);
    if (ms_IndexBuffer == nullptr)
    {
        uint16_t trianges[6] = {0, 1, 2, 0, 2, 3};
        wgpu::BufferDescriptor bufferDesc;
        bufferDesc.usage = wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst;
        bufferDesc.size = totalSize;
        ms_IndexBuffer = ms_Device.CreateBuffer(&bufferDesc);
        wgpu::Queue queue = ms_Device.GetQueue();
        queue.WriteBuffer(ms_IndexBuffer, 0, trianges, totalSize);
    }
    renderPassEncoder.SetIndexBuffer(ms_IndexBuffer, wgpu::IndexFormat::Uint16, 0, totalSize);
}

void Render()
{
    wgpu::CommandEncoder commandEncoder = nullptr;
    wgpu::RenderPassEncoder renderPassEncoder = nullptr;
    BeginRender(commandEncoder, renderPassEncoder);
    switch (ms_SampleCase)
    {
    case NoBufferDrawTriangle:
        renderPassEncoder.SetPipeline(ms_RenderPipeline);
        renderPassEncoder.Draw(3, 1, 0, 0);
        break;
    case UseBufferDrawQuad:
        SetupVertexBuffer(renderPassEncoder);
        SetupIndexBuffer(renderPassEncoder);
        break;

    default:
        break;
    }

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
    case FrameBufferInitialized:
        SetupRenderPipeline();
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
    ms_DepthBuffer = nullptr;
    emscripten_set_main_loop(Loop, 0, false);
    return 0;
}