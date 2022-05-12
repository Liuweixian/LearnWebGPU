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
    void SetData(size_t ulDataLength, T *pData);

protected:
    size_t m_ulDataSize;
    void *m_pData;
};