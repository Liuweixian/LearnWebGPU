#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <typeinfo>
#include "webgpu/webgpu_cpp.h"

class RenderVertexBuffer
{
public:
    RenderVertexBuffer();
    virtual ~RenderVertexBuffer();
    template <typename T>
    void SetData(size_t ulDataLength, T *pData, int nAttributeCount, ...);

protected:
    wgpu::VertexFormat GetVertexFormat(const std::type_info& typeInfo, int nStride);

protected:
    size_t m_ulDataSize;
    void *m_pData;
    wgpu::VertexBufferLayout *m_pLayout;
};