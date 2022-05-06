#pragma once
#include "RenderPassIdx.h"
#include "../RenderObject/RenderShader.h"

class RenderGraphPass
{
public:
    RenderGraphPass();
    virtual ~RenderGraphPass();
    bool EnsureSetupFinish();
    virtual void SetRenderTarget() = 0;
    RenderPassIdx GetIdx()
    {
        return m_eIdx;
    }
protected:
    virtual void Setup() = 0;

protected:
    RenderPassIdx m_eIdx;
    RenderShader* m_pShader;
};