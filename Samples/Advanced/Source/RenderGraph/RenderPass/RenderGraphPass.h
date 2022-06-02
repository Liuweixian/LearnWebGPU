#pragma once
#include <list>
#include "../RGDefine.h"
#include "../RenderResource/RenderShader.h"

class RenderGraphPass
{
public:
    RenderGraphPass(RGPassIdx uPassIdx);
    virtual ~RenderGraphPass();
    virtual bool EnsureSetupFinish();
    virtual void Compile() = 0;
    RGPassType GetPassType()
    {
        return m_ePassType;
    }
protected:
    virtual void SetupShader() = 0;

protected:
    RenderShader *m_pShader;
    RGPassType m_ePassType;
    RGPassIdx m_uPassIdx;
};
