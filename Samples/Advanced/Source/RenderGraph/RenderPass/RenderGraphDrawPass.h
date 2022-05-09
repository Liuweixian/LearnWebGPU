#pragma once
#include <list>
#include "RenderGraphPass.h"
#include "RenderGraphDefine.h"
#include "../RenderResource/RenderResource.h"

class RenderGraphDrawPass : public RenderGraphPass
{
public:
    RenderGraphDrawPass();
    virtual ~RenderGraphDrawPass();
    virtual bool EnsureSetupFinish() override;
    void SetRenderTarget(RenderResourceHandle *pTargetColorBuffer);

protected:
    RenderPassIdx m_eIdx;
    std::list<RenderResourceHandle *> m_TargetColorBuffers;
    RenderResourceHandle *m_TargetDepthBuffer;
};