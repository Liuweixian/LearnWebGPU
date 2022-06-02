#pragma once
#include <stdio.h>
#include "webgpu/webgpu_cpp.h"

class RGVertexLayout
{
public:
    RGVertexLayout(int nAttributeCount, wgpu::VertexFormat *pVertexFormats);
    virtual ~RGVertexLayout();
    wgpu::VertexBufferLayout GetLayout()
    {
        return *m_pLayout;
    }

private:
    uint32_t GetVertexFormatSize(wgpu::VertexFormat);

private:
    wgpu::VertexBufferLayout *m_pLayout;
};
