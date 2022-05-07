#pragma once
#include <list>
#include "RenderPassIdx.h"
#include "../RenderObject/RenderShader.h"

class RenderGraphPass
{
public:
    RenderGraphPass();
    virtual ~RenderGraphPass();
    bool EnsureSetupFinish();
    RenderPassIdx GetIdx()
    {
        return m_eIdx;
    }
protected:
    virtual void SetupShader() = 0;
    virtual void SetupRenderTarget() = 0;
    void SetBackBufferAsRenderTarget();

protected:
    RenderPassIdx m_eIdx;
    RenderShader* m_pShader;
    std::list<wgpu::TextureDescriptor *> m_ColorBuffers;
    wgpu::TextureDescriptor * m_pDepthBuffer;
};