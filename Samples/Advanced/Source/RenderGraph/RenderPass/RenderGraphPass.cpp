#include "RenderGraphPass.h"
#include <stdio.h>

RenderGraphPass::RenderGraphPass()
{
    m_eIdx = RenderPassIdx::Invalid;
    m_ColorBuffers.clear();
    m_pDepthBuffer = nullptr;
}

RenderGraphPass::~RenderGraphPass()
{

}

static wgpu::TextureDescriptor* g_pBackBuffer;
void RenderGraphPass::SetBackBufferAsRenderTarget()
{
    m_ColorBuffers.clear();
    if (g_pBackBuffer == nullptr)
    {
        g_pBackBuffer = new wgpu::TextureDescriptor();
        g_pBackBuffer->label = "BackBuffer";
        g_pBackBuffer->usage = wgpu::TextureUsage::RenderAttachment;
        g_pBackBuffer->size.width = 0;
        g_pBackBuffer->size.height = 0;
        g_pBackBuffer->size.depthOrArrayLayers = 0;
    }
    m_ColorBuffers.push_back(g_pBackBuffer);
}

bool RenderGraphPass::EnsureSetupFinish()
{
    if (m_eIdx == RenderPassIdx::Invalid)
    {
        printf("RenderPassIdx is Invalid!!\n");
        return false;
    }

    if (m_pShader == nullptr)
    {
        SetupShader();
        if (m_pShader == nullptr)
        {
            printf("Shader is null!!\n");
            return false;
        }
    }
    
    if (m_ColorBuffers.empty())
    {
        SetupRenderTarget();
        if (m_ColorBuffers.empty())
        {
            printf("RenderTarget is null!!\n");
            return false;
        }
    }
    return true;
}