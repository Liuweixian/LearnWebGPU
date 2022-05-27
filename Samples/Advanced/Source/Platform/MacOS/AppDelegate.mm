//
//  AppDelegate.m
//  WebGPURenderGraph
//
//  Created by 刘伟贤 on 2022/5/24.
//

#import "AppDelegate.h"
#include <stdio.h>
#import <CoreVideo/CoreVideo.h>

@interface AppDelegate ()

@property (strong) IBOutlet NSWindow *window;

@end

CVDisplayLinkRef _displayLinkRef;
static WGPUDevice ms_gpuDevice;
static WGPUQueue ms_gpuQueue;
static WGPUSwapChain ms_SwapChain;
static WGPURenderPipeline ms_Pipeline;
static float rotDeg = 0.0f;
WGPUBuffer vertBuf; // vertex buffer with triangle position and colours
WGPUBuffer indxBuf; // index buffer
WGPUBuffer uRotBuf; // uniform buffer (containing the rotation angle)
WGPUBindGroup bindGroup;

static char const triangle_vert_wgsl[] = R"(
    struct VertexIn {
        @location(0) aPos : vec2<f32>;
        @location(1) aCol : vec3<f32>;
    };
    struct VertexOut {
        @location(0) vCol : vec3<f32>;
        @builtin(position) Position : vec4<f32>;
    };
    struct Rotation {
        @location(0) degs : f32;
    };
    @group(0) @binding(0) var<uniform> uRot : Rotation;
    @stage(vertex)
    fn main(input : VertexIn) -> VertexOut {
        var rads : f32 = radians(uRot.degs);
        var cosA : f32 = cos(rads);
        var sinA : f32 = sin(rads);
        var rot : mat3x3<f32> = mat3x3<f32>(
            vec3<f32>( cosA, sinA, 0.0),
            vec3<f32>(-sinA, cosA, 0.0),
            vec3<f32>( 0.0,  0.0,  1.0));
        var output : VertexOut;
        output.Position = vec4<f32>(rot * vec3<f32>(input.aPos, 1.0), 1.0);
        output.vCol = input.aCol;
        return output;
    }
)";

static char const triangle_frag_wgsl[] = R"(
    @stage(fragment)
    fn main(@location(0) vCol : vec3<f32>) -> @location(0) vec4<f32> {
        return vec4<f32>(vCol, 1.0);
    }
)";

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    CVDisplayLinkCreateWithActiveCGDisplays(&_displayLinkRef);
    CVDisplayLinkSetOutputCallback(_displayLinkRef, &displayLinkRepaint, nullptr);
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closeWindow) name:NSWindowWillCloseNotification object:nil];
    
    ms_gpuDevice = webgpu::create(self.window, WGPUBackendType_Metal);
    ms_gpuQueue = wgpuDeviceGetQueue(ms_gpuDevice);
    ms_SwapChain = webgpu::createSwapChain(ms_gpuDevice);
    createPipelineAndBuffers();
    redraw();
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    wgpuBindGroupRelease(bindGroup);
    wgpuBufferRelease(uRotBuf);
    wgpuBufferRelease(indxBuf);
    wgpuBufferRelease(vertBuf);
    wgpuRenderPipelineRelease(ms_Pipeline);
    wgpuSwapChainRelease(ms_SwapChain);
    wgpuQueueRelease(ms_gpuQueue);
    wgpuDeviceRelease(ms_gpuDevice);
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
    CVDisplayLinkStart(_displayLinkRef);
}

- (void)applicationDidResignActive:(NSNotification *)notification {
}

- (void)closeWindow {
    CVDisplayLinkStop(_displayLinkRef);
    [NSApp terminate:self];
}

@end

static CVReturn displayLinkRepaint(CVDisplayLinkRef dispLink, const CVTimeStamp* inNow, const CVTimeStamp* inOutputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    redraw();
    return kCVReturnSuccess;
}

static WGPUShaderModule createShader(const char* const code) {
    WGPUShaderModuleWGSLDescriptor wgsl = {};
    wgsl.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    wgsl.source = code;
    WGPUShaderModuleDescriptor desc = {};
    desc.nextInChain = reinterpret_cast<WGPUChainedStruct*>(&wgsl);
    desc.label = NULL;
    return wgpuDeviceCreateShaderModule(ms_gpuDevice, &desc);
}

static WGPUBuffer createBuffer(const void* data, size_t size, WGPUBufferUsage usage) {
    WGPUBufferDescriptor desc = {};
    desc.usage = WGPUBufferUsage_CopyDst | usage;
    desc.size  = size;
    WGPUBuffer buffer = wgpuDeviceCreateBuffer(ms_gpuDevice, &desc);
    wgpuQueueWriteBuffer(ms_gpuQueue, buffer, 0, data, size);
    return buffer;
}

static void createPipelineAndBuffers() {
    // compile shaders
    // NOTE: these are now the WGSL shaders (tested with Dawn and Chrome Canary)
    WGPUShaderModule vertMod = createShader(triangle_vert_wgsl);
    WGPUShaderModule fragMod = createShader(triangle_frag_wgsl);
    
    // keep the old unused SPIR-V shaders around for a while...
    //(void) triangle_vert_spirv;
    //(void) triangle_frag_spirv;

    WGPUBufferBindingLayout buf = {};
    buf.type = WGPUBufferBindingType_Uniform;

    // bind group layout (used by both the pipeline layout and uniform bind group, released at the end of this function)
    WGPUBindGroupLayoutEntry bglEntry = {};
    bglEntry.binding = 0;
    bglEntry.visibility = WGPUShaderStage_Vertex;
    bglEntry.buffer = buf;

    WGPUBindGroupLayoutDescriptor bglDesc = {};
    bglDesc.entryCount = 1;
    bglDesc.entries = &bglEntry;
    WGPUBindGroupLayout bindGroupLayout = wgpuDeviceCreateBindGroupLayout(ms_gpuDevice, &bglDesc);

    // pipeline layout (used by the render pipeline, released after its creation)
    WGPUPipelineLayoutDescriptor layoutDesc = {};
    layoutDesc.bindGroupLayoutCount = 1;
    layoutDesc.bindGroupLayouts = &bindGroupLayout;
    WGPUPipelineLayout pipelineLayout = wgpuDeviceCreatePipelineLayout(ms_gpuDevice, &layoutDesc);

    // describe buffer layouts
    WGPUVertexAttribute vertAttrs[2] = {};
    vertAttrs[0].format = WGPUVertexFormat_Float32x2;
    vertAttrs[0].offset = 0;
    vertAttrs[0].shaderLocation = 0;
    vertAttrs[1].format = WGPUVertexFormat_Float32x3;
    vertAttrs[1].offset = 2 * sizeof(float);
    vertAttrs[1].shaderLocation = 1;
    WGPUVertexBufferLayout vertexBufferLayout = {};
    vertexBufferLayout.arrayStride = 5 * sizeof(float);
    vertexBufferLayout.attributeCount = 2;
    vertexBufferLayout.attributes = vertAttrs;

    // Fragment state
    WGPUBlendState blend = {};
    blend.color.operation = WGPUBlendOperation_Add;
    blend.color.srcFactor = WGPUBlendFactor_One;
    blend.color.dstFactor = WGPUBlendFactor_One;
    blend.alpha.operation = WGPUBlendOperation_Add;
    blend.alpha.srcFactor = WGPUBlendFactor_One;
    blend.alpha.dstFactor = WGPUBlendFactor_One;

    WGPUColorTargetState colorTarget = {};
    colorTarget.format = webgpu::getSwapChainFormat(ms_gpuDevice);
    colorTarget.blend = &blend;
    colorTarget.writeMask = WGPUColorWriteMask_All;

    WGPUFragmentState fragment = {};
    fragment.module = fragMod;
    fragment.entryPoint = "main";
    fragment.targetCount = 1;
    fragment.targets = &colorTarget;

    WGPURenderPipelineDescriptor desc = {};
    desc.fragment = &fragment;

    // Other state
    desc.layout = pipelineLayout;
    desc.depthStencil = nullptr;

    desc.vertex.module = vertMod;
    desc.vertex.entryPoint = "main";
    desc.vertex.bufferCount = 1;//0;
    desc.vertex.buffers = &vertexBufferLayout;

    desc.multisample.count = 1;
    desc.multisample.mask = 0xFFFFFFFF;
    desc.multisample.alphaToCoverageEnabled = false;

    desc.primitive.frontFace = WGPUFrontFace_CCW;
    desc.primitive.cullMode = WGPUCullMode_None;
    desc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    desc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;

    ms_Pipeline = wgpuDeviceCreateRenderPipeline(ms_gpuDevice, &desc);

    // partial clean-up (just move to the end, no?)
    wgpuPipelineLayoutRelease(pipelineLayout);

    wgpuShaderModuleRelease(fragMod);
    wgpuShaderModuleRelease(vertMod);

    // create the buffers (x, y, r, g, b)
    float const vertData[] = {
        -0.8f, -0.8f, 0.0f, 0.0f, 1.0f, // BL
         0.8f, -0.8f, 0.0f, 1.0f, 0.0f, // BR
        -0.0f,  0.8f, 1.0f, 0.0f, 0.0f, // top
    };
    uint16_t const indxData[] = {
        0, 1, 2,
        0 // padding (better way of doing this?)
    };
    vertBuf = createBuffer(vertData, sizeof(vertData), WGPUBufferUsage_Vertex);
    indxBuf = createBuffer(indxData, sizeof(indxData), WGPUBufferUsage_Index);

    // create the uniform bind group (note 'rotDeg' is copied here, not bound in any way)
    uRotBuf = createBuffer(&rotDeg, sizeof(rotDeg), WGPUBufferUsage_Uniform);

    WGPUBindGroupEntry bgEntry = {};
    bgEntry.binding = 0;
    bgEntry.buffer = uRotBuf;
    bgEntry.offset = 0;
    bgEntry.size = sizeof(rotDeg);

    WGPUBindGroupDescriptor bgDesc = {};
    bgDesc.layout = bindGroupLayout;
    bgDesc.entryCount = 1;
    bgDesc.entries = &bgEntry;

    bindGroup = wgpuDeviceCreateBindGroup(ms_gpuDevice, &bgDesc);

    // last bit of clean-up
    wgpuBindGroupLayoutRelease(bindGroupLayout);
}

void redraw() {
    WGPUTextureView backBufView = wgpuSwapChainGetCurrentTextureView(ms_SwapChain);            // create textureView

    WGPURenderPassColorAttachment colorDesc = {};
    colorDesc.view    = backBufView;
    colorDesc.loadOp  = WGPULoadOp_Clear;
    colorDesc.storeOp = WGPUStoreOp_Store;
#ifdef __EMSCRIPTEN__
    // Dawn has both clearValue/clearColor but only Color works; Emscripten only has Value
    colorDesc.clearValue.r = 0.3f;
    colorDesc.clearValue.g = 0.3f;
    colorDesc.clearValue.b = 0.3f;
    colorDesc.clearValue.a = 1.0f;
#else
    colorDesc.clearValue.r = 0.3f;
    colorDesc.clearValue.g = 0.3f;
    colorDesc.clearValue.b = 0.3f;
    colorDesc.clearValue.a = 1.0f;
#endif

    WGPURenderPassDescriptor renderPass = {};
    renderPass.colorAttachmentCount = 1;
    renderPass.colorAttachments = &colorDesc;

    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(ms_gpuDevice, nullptr);            // create encoder
    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPass);    // create pass

    // update the rotation
    rotDeg += 0.1f;
    wgpuQueueWriteBuffer(ms_gpuQueue, uRotBuf, 0, &rotDeg, sizeof(rotDeg));

    // draw the triangle (comment these five lines to simply clear the screen)
    wgpuRenderPassEncoderSetPipeline(pass, ms_Pipeline);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, bindGroup, 0, 0);
    wgpuRenderPassEncoderSetVertexBuffer(pass, 0, vertBuf, 0, WGPU_WHOLE_SIZE);
    wgpuRenderPassEncoderSetIndexBuffer(pass, indxBuf, WGPUIndexFormat_Uint16, 0, WGPU_WHOLE_SIZE);
    wgpuRenderPassEncoderDrawIndexed(pass, 3, 1, 0, 0, 0);

    wgpuRenderPassEncoderEnd(pass);
    wgpuRenderPassEncoderRelease(pass);                                                        // release pass
    WGPUCommandBuffer commands = wgpuCommandEncoderFinish(encoder, nullptr);                // create commands
    wgpuCommandEncoderRelease(encoder);                                                        // release encoder

    wgpuQueueSubmit(ms_gpuQueue, 1, &commands);
    wgpuCommandBufferRelease(commands);                                                        // release commands
#ifndef __EMSCRIPTEN__
    /*
     * TODO: wgpuSwapChainPresent is unsupported in Emscripten, so what do we do?
     */
    wgpuSwapChainPresent(ms_SwapChain);
#endif
    wgpuTextureViewRelease(backBufView);                                                    // release textureView
}
