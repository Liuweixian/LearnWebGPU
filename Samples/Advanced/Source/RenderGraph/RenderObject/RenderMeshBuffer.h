#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <typeinfo>

class RenderMeshBuffer
{
public:
    RenderMeshBuffer();
    virtual ~RenderMeshBuffer();
    template <typename T>
    void SetData(size_t ulDataCount, T *pData);

protected:
    size_t m_ulDataCount;
    size_t m_ulDataLength;
    void *m_pData;
};