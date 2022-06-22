#pragma once
#include "../../RenderGraph/RenderPass/RGDrawPass.h"

class DrawCubePass : public RGDrawPass
{
public:
    DrawCubePass(RGPassIdx uPassIdx);
    virtual ~DrawCubePass();
    virtual void Update() override;


protected:
    virtual RGShader *SetupShader() override;
    
private:
    RGBufferResHandle * m_pMVPMatrixBufferHandle;
};
