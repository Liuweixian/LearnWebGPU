#pragma once
#include "RenderGraphPass.h"
#include "RenderPassDefine.h"

class RenderGraphDrawPass : public RenderGraphPass
{
public:
    RenderGraphDrawPass();
    virtual ~RenderGraphDrawPass();
protected:
    RenderPassIdx m_eIdx;
};