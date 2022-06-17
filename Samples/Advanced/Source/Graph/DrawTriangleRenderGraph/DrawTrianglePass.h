#pragma once
#include "../../RenderGraph/RenderPass/RGDrawPass.h"

class DrawTrianglePass : public RGDrawPass
{
public:
    DrawTrianglePass(RGPassIdx uPassIdx);
    virtual ~DrawTrianglePass();
    virtual void Update() override;


protected:
    virtual RGShader *SetupShader() override;
    
private:
    float m_fRotation;
    RGBufferResHandle * m_pRotationBufferHandle;
};
