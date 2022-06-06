#pragma once
#include <list>
#include "RGPass.h"
#include "../RGDefine.h"
#include "../RenderResource/RGResources.h"
#include "../RenderState/RGRenderState.h"
#include "../RenderObject/RenderObject.h"

class RGDrawPass : public RGPass
{
public:
    RGDrawPass(RGPassIdx uPassIdx);
    virtual ~RGDrawPass();
    virtual bool EnsureSetupFinish() override;
    virtual void Compile() override;
    void SetRenderTarget(RGResourceHandle *pTargetColorBuffer);
    void Execute(const std::list<RenderObject *> renderObjects);

protected:
    std::list<RGResourceHandle *> m_TargetColorBuffers;
    RGResourceHandle *m_pTargetDepthBuffer;
    RGRenderState *m_pDefaultRenderState;
};
