#pragma once
#include <list>
#include "RenderGraphPass.h"
#include "RenderGraphDefine.h"
#include "../RenderResource/RenderResource.h"
#include "../RenderResource/RenderState.h"
#include "../RenderObject/RenderObject.h"

class RenderGraphDrawPass : public RenderGraphPass
{
public:
    RenderGraphDrawPass();
    virtual ~RenderGraphDrawPass();
    virtual bool EnsureSetupFinish() override;
    virtual void Compile() override;
    void SetRenderTarget(RenderResourceHandle *pTargetColorBuffer);
    void Execute(const std::list<RenderObject *> renderObjects);

protected:
    RenderPassIdx m_eIdx;
    std::list<RenderResourceHandle *> m_TargetColorBuffers;
    RenderResourceHandle *m_pTargetDepthBuffer;
    RenderState *m_pRenderState;
};