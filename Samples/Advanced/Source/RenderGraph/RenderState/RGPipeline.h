#pragma once
#include <stdio.h>
#include <list>
#include "webgpu/webgpu_cpp.h"
#include "../RenderResource/RGDrawShader.h"
#include "../RenderResource/RGResources.h"

class RGPipeline
{
public:
    RGPipeline();
    virtual ~RGPipeline();
    void Initialize(RGDrawShader *pRenderObjectShader, std::list<RGResourceHandle *> targetColorBuffers, RGResourceHandle *pTargetDepthBuffer);
    void Cleanup();

private:
    void InitVertexState(RGDrawShader *pRenderObjectShader);
    void InitFragmentState(RGDrawShader *pRenderObjectShader, std::list<RGResourceHandle *> targetColorBuffers);
    void InitPrimitiveState();
    void InitDepthStencilState(RGResourceHandle *pTargetDepthBuffer);
    void InitMultisampleState();

public:
    wgpu::PipelineLayoutDescriptor *m_pLayoutDesc;
    wgpu::ShaderModuleWGSLDescriptor *m_pVertexShaderDesc;
    wgpu::VertexState *m_pVertexState;
    wgpu::ShaderModuleWGSLDescriptor *m_pFragmentShaderDesc;
    wgpu::FragmentState *m_pFragmentState;
    wgpu::PrimitiveState *m_pPrimitiveState;
    wgpu::DepthStencilState *m_pDepthStencilState;
    wgpu::MultisampleState *m_pMultisampleState;
    wgpu::RenderPipeline m_RenderPipeline;
};
