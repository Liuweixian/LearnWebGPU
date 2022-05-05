#pragma once
#include <stdio.h>

class RenderVertexBuffer
{
public:
    RenderVertexBuffer();
    virtual ~RenderVertexBuffer();
private:
    uint8_t* m_pData;
};