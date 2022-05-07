#pragma once
#include "../RenderGraph/RenderPass/RenderGraphPass.h"

class DrawObjectPass : public RenderGraphPass
{
public:
    DrawObjectPass();
    virtual ~DrawObjectPass();
protected:
    virtual void SetupShader() override;
    virtual void SetupRenderTarget() override;

};