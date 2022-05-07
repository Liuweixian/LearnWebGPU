#pragma once
#include <stdio.h>

class RenderIndexBuffer
{
public:
    RenderIndexBuffer();
    virtual ~RenderIndexBuffer();
    template <typename T>
    void SetData(size_t ulDataLength, T *pData)
    {
        m_ulDataSize = ulDataLength * sizeof(T);
        m_pData = pData;
    }

protected:
    size_t m_ulDataSize;
    void *m_pData;
};