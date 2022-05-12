#include "RenderMeshBuffer.h"
#include <cassert>

RenderMeshBuffer::RenderMeshBuffer()
{
    m_ulDataSize = 0;
    m_pData = nullptr;
}

RenderMeshBuffer::~RenderMeshBuffer()
{
    m_ulDataSize = 0;
    m_pData = nullptr;
}

template void RenderMeshBuffer::SetData<float>(size_t ulDataLength, float *pData);
template void RenderMeshBuffer::SetData<uint16_t>(size_t ulDataLength, uint16_t *pData);

template <typename T>
void RenderMeshBuffer::SetData(size_t ulDataLength, T *pData)
{
    m_ulDataSize = ulDataLength * sizeof(T);
    m_pData = pData;
}