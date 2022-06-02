#pragma once
#include <stdio.h>
#include <list>
#include "webgpu/webgpu_cpp.h"
#include "RGDrawShader.h"
#include "RenderResource.h"

class RenderState
{
public:
    RenderState();
    virtual ~RenderState();
    void Initialize(RGDrawShader *pRenderObjectShader, std::list<RenderResourceHandle *> targetColorBuffers, RenderResourceHandle *pTargetDepthBuffer);
    void Cleanup();

private:
    void InitVertexState(RGDrawShader *pRenderObjectShader);
    void InitFragmentState(RGDrawShader *pRenderObjectShader, std::list<RenderResourceHandle *> targetColorBuffers);
    void InitPrimitiveState();
    void InitDepthStencilState(RenderResourceHandle *pTargetDepthBuffer);
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
