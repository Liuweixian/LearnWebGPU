#pragma once
#include "RenderPassIdx.h"

class RenderGraphPass
{
public:
    RenderGraphPass();
    virtual ~RenderGraphPass();
protected:
    RenderPassIdx m_eIdx;
};