#pragma once
#include <list>
#include "RenderGraphDefine.h"
#include "../RenderResource/RenderShader.h"

class RenderGraphPass
{
public:
    RenderGraphPass();
    virtual ~RenderGraphPass();
    virtual bool EnsureSetupFinish();
    virtual void Compile() = 0;
    GraphPassType GetPassType()
    {
        return m_eType;
    }
protected:
    virtual void SetupShader() = 0;

protected:
    RenderShader *m_pShader;
    GraphPassType m_eType;
};