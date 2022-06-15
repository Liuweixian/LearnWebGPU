#pragma once
#include <list>
#include <vector>
#include "RGPass.h"
#include "../RGDefine.h"
#include "../RenderResource/RGResources.h"
#include "../RenderState/RGRenderState.h"
#include "../RenderState/RGBindEntryGroup.h"
#include "../RenderObject/RGObject.h"

class RGDrawPass : public RGPass
{
public:
    RGDrawPass(RGPassIdx uPassIdx);
    virtual ~RGDrawPass();
    virtual bool EnsureSetupFinish() override;
    virtual void Compile() override;
    void SetRenderTarget(RGTextureResHandle *pTargetColorBuffer);
    void Execute(const std::list<RGObject *> renderObjects);
    void Bind(wgpu::ShaderStage eShaderStage, uint32_t uGroupIndex, uint32_t uBindIndex, RGResourceHandle *pResourceHandle);

protected:
    std::list<RGTextureResHandle *> m_TargetColorBuffers;
    RGTextureResHandle *m_pTargetDepthBuffer;
    RGRenderState *m_pDefaultRenderState;
    std::vector<RGBindEntryGroup> m_BindEntryGroups;
};
