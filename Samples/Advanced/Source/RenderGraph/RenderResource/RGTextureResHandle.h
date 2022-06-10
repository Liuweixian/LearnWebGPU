#pragma once
#include "RGResourceHandle.h"

class RGTextureResHandle : public RGResourceHandle
{
public:
    RGTextureResHandle() : RGResourceHandle()
    {
        m_unDescIdx = 0;
    }
    
    uint32_t m_unDescIdx;
};
