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
    void SetData(size_t ulDataLength, T *pData);

protected:
    size_t m_ulDataSize;
    void *m_pData;
};