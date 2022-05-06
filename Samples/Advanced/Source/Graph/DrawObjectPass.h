#pragma once
#include "../RenderGraph/RenderPass/RenderGraphPass.h"

class DrawObjectPass : public RenderGraphPass
{
public:
    DrawObjectPass();
    virtual ~DrawObjectPass();
    virtual void SetRenderTarget() override;
protected:
    virtual void Setup() override;

};