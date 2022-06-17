#pragma once
#include "../../RenderGraph/RenderPass/RGDrawPass.h"

class DrawObjectPass : public RGDrawPass
{
public:
    DrawObjectPass(RGPassIdx uPassIdx);
    virtual ~DrawObjectPass();
    virtual void Update() override;


protected:
    virtual RGShader *SetupShader() override;
    
private:
    float m_fRotation;
    RGBufferResHandle * m_pRotationBufferHandle;
};
