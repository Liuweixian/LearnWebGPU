#include "RenderVertexBuffer.h"
#include <cassert>

RenderVertexBuffer::RenderVertexBuffer()
{
    m_ulDataSize = 0;
    m_pData = nullptr;
}

RenderVertexBuffer::~RenderVertexBuffer()
{
    m_ulDataSize = 0;
    m_pData = nullptr;
}

template void RenderVertexBuffer::SetData<float>(size_t ulDataLength, float *pData);

template <typename T>
void RenderVertexBuffer::SetData(size_t ulDataLength, T *pData)
{
    m_ulDataSize = ulDataLength * sizeof(T);
    m_pData = pData;
}