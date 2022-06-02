#pragma once
#include <list>
#include "RenderGraphPass.h"
#include "../RGDefine.h"
#include "../RenderResource/RenderResource.h"
#include "../RenderResource/RenderState.h"
#include "../RenderObject/RenderObject.h"

class RenderGraphDrawPass : public RenderGraphPass
{
public:
    RenderGraphDrawPass(RGPassIdx uPassIdx);
    virtual ~RenderGraphDrawPass();
    virtual bool EnsureSetupFinish() override;
    virtual void Compile() override;
    void SetRenderTarget(RenderResourceHandle *pTargetColorBuffer);
    void Execute(const std::list<RenderObject *> renderObjects);

protected:
    std::list<RenderResourceHandle *> m_TargetColorBuffers;
    RenderResourceHandle *m_pTargetDepthBuffer;
    RenderState *m_pRenderState;
};
