#pragma once
#include "../../RenderGraph/RenderPass/RGDrawPass.h"

class DrawObjectPass : public RGDrawPass
{
public:
    DrawObjectPass(RGPassIdx uPassIdx);
    virtual ~DrawObjectPass();

protected:
    virtual void SetupShader() override;
};
