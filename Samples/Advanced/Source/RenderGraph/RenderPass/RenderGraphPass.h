#pragma once
#include <list>
#include "RenderPassDefine.h"
#include "../RenderObject/RenderShader.h"

class RenderGraphPass
{
public:
    RenderGraphPass();
    virtual ~RenderGraphPass();
    bool EnsureSetupFinish();

protected:
    virtual void SetupShader() = 0;

protected:
    RenderShader *m_pShader;
    RenderPassType m_eType;
};