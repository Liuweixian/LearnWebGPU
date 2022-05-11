#pragma once
#include <stdio.h>
#include "webgpu/webgpu_cpp.h"

class RenderVertexState
{
public:
    RenderVertexState(int nAttributeCount, wgpu::VertexFormat *pVertexFormats);
    virtual ~RenderVertexState();
private:
    uint32_t GetVertexFormatSize(wgpu::VertexFormat);

private:
    wgpu::VertexBufferLayout *m_pLayout;
};