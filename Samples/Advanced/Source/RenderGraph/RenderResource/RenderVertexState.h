#pragma once
#include <stdio.h>
#include "webgpu/webgpu_cpp.h"

class RenderVertexState
{
public:
    RenderVertexState(int nAttributeCount, wgpu::VertexFormat *pVertexFormats);
    virtual ~RenderVertexState();
    wgpu::VertexBufferLayout GetLayout()
    {
        return *m_pLayout;
    }

private:
    uint32_t GetVertexFormatSize(wgpu::VertexFormat);

private:
    wgpu::VertexBufferLayout *m_pLayout;
};