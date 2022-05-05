#pragma once
#include "RenderPassIdx.h"
#include "../RenderObject/RenderShader.h"

class RenderGraphPass
{
public:
    RenderGraphPass();
    virtual ~RenderGraphPass();
protected:
    RenderPassIdx m_eIdx;
    RenderShader* m_pShader;
};