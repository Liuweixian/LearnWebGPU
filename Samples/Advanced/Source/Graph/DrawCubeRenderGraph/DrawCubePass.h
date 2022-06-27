#pragma once
#include "../../RenderGraph/RenderPass/RGDrawPass.h"
#include "../../RenderGraph/RenderResource/RGBufferResHandle.h"

class DrawCubePass : public RGDrawPass
{
public:
    DrawCubePass(RGPassIdx uPassIdx);
    virtual ~DrawCubePass();
    virtual void Update() override;


protected:
    virtual RGShader *SetupShader() override;
    
private:
    RGBufferResHandle *m_pVPMatrixBufferHandle;
    RGBufferResHandle *m_pObject2WorldBufferHandle;
};
