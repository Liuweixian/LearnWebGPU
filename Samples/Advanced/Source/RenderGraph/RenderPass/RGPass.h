#pragma once
#include <list>
#include "../RGDefine.h"
#include "../RenderResource/RGShader.h"

class RGPass
{
public:
    RGPass(RGPassIdx uPassIdx);
    virtual ~RGPass();
    virtual bool EnsureSetupFinish();
    virtual void Compile() = 0;
    RGPassType GetPassType()
    {
        return m_ePassType;
    }

protected:
    virtual void SetupShader() = 0;

protected:
    RGShader *m_pShader;
    RGPassType m_ePassType;
    RGPassIdx m_uPassIdx;
};
