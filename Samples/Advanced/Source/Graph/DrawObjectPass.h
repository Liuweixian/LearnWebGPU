#pragma once
#include "../RenderGraph/RenderPass/RenderGraphDrawPass.h"

class DrawObjectPass : public RenderGraphDrawPass
{
public:
    DrawObjectPass();
    virtual ~DrawObjectPass();
protected:
    virtual void SetupShader() override;
};