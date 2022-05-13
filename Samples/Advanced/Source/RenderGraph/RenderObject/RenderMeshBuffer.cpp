#include "RenderMeshBuffer.h"
#include <cassert>

RenderMeshBuffer::RenderMeshBuffer()
{
    m_ulDataCount = 0;
    m_ulDataLength = 0;
    m_pData = nullptr;
}

RenderMeshBuffer::~RenderMeshBuffer()
{
    m_ulDataCount = 0;
    m_ulDataLength = 0;
    m_pData = nullptr;
}

template void RenderMeshBuffer::SetData<float>(size_t ulDataCount, float *pData);
template void RenderMeshBuffer::SetData<uint16_t>(size_t ulDataCount, uint16_t *pData);

template <typename T>
void RenderMeshBuffer::SetData(size_t ulDataCount, T *pData)
{
    m_ulDataCount = ulDataCount;
    m_ulDataLength = ulDataCount * sizeof(T);
    m_pData = pData;
}