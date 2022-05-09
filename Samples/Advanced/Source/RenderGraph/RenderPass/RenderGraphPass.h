#pragma once
#include <list>
#include "RenderGraphDefine.h"
#include "../RenderObject/RenderShader.h"

class RenderGraphPass
{
public:
    RenderGraphPass();
    virtual ~RenderGraphPass();
    virtual bool EnsureSetupFinish();

protected:
    virtual void SetupShader() = 0;

protected:
    RenderShader *m_pShader;
    GraphPassType m_eType;
};