#pragma once
#include <list>
#include "RGPass.h"
#include "../RGDefine.h"
#include "../RenderResource/RenderResource.h"
#include "../RenderResource/RenderState.h"
#include "../RenderObject/RenderObject.h"

class RGDrawPass : public RGPass
{
public:
    RGDrawPass(RGPassIdx uPassIdx);
    virtual ~RGDrawPass();
    virtual bool EnsureSetupFinish() override;
    virtual void Compile() override;
    void SetRenderTarget(RenderResourceHandle *pTargetColorBuffer);
    void Execute(const std::list<RenderObject *> renderObjects);

protected:
    std::list<RenderResourceHandle *> m_TargetColorBuffers;
    RenderResourceHandle *m_pTargetDepthBuffer;
    RenderState *m_pRenderState;
};
