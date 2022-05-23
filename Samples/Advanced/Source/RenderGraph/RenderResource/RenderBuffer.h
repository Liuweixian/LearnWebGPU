#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <typeinfo>
#include "webgpu/webgpu_cpp.h"

class RenderBuffer
{
public:
    RenderBuffer();
    virtual ~RenderBuffer();
    template <typename T>
    void SetData(size_t ulDataCount, T *pData);
    wgpu::Buffer &GetGPUBuffer()
    {
        return m_GPUBuffer;
    }
    size_t GetDataCount()
    {
        return m_ulDataCount;
    }
    size_t GetDataLength()
    {
        return m_ulDataLength;
    }
    void *GetData()
    {
        return m_pData;
    }

protected:
    size_t m_ulDataCount;
    size_t m_ulDataLength;
    void *m_pData;
    wgpu::Buffer m_GPUBuffer;
};