#pragma once
#include <stdio.h>
#include <list>
#include "webgpu/webgpu_cpp.h"
#include "../RenderResource/RGDrawShader.h"
#include "../RenderResource/RGResources.h"
#include "../RenderObject/RGMaterial.h"

class RGRenderState
{
public:
    RGRenderState();
    virtual ~RGRenderState();
    void Initialize(RGDrawShader *pDrawShader, std::list<RGTextureResHandle *> targetColorBuffers, RGTextureResHandle *pTargetDepthBuffer);
    void UpdateRenderState(RGMaterial *pMaterial);
    wgpu::RenderPipeline& GetCurrentPipeline()
    {
        return *m_pCurrentRenderPipeline;
    }

private:
    void InitVertexState(RGDrawShader *pDrawShader);
    void InitFragmentState(RGDrawShader *pRenderObjectShader, std::list<RGTextureResHandle *> targetColorBuffers);
    void InitBindBufferState();
    void InitPrimitiveState();
    void InitDepthStencilState(RGTextureResHandle *pTargetDepthBuffer);
    void InitMultisampleState();

private:
    wgpu::PipelineLayout m_PipelineLayout;
    wgpu::VertexState m_VertexState;
    wgpu::FragmentState m_FragmentState;
    wgpu::PrimitiveState m_PrimitiveState;
    wgpu::DepthStencilState *m_pDepthStencilState;
    wgpu::MultisampleState m_MultisampleState;
    std::unordered_map<uint32_t, wgpu::RenderPipeline> m_RenderPipelines;
    wgpu::RenderPipeline *m_pCurrentRenderPipeline;
};
