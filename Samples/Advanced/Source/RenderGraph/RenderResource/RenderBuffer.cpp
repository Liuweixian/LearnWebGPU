#include "RenderBuffer.h"
#include <cassert>

RenderBuffer::RenderBuffer()
{
    m_ulDataCount = 0;
    m_ulDataLength = 0;
    m_pData = nullptr;
    m_GPUBuffer = nullptr;
}

RenderBuffer::~RenderBuffer()
{
    m_ulDataCount = 0;
    m_ulDataLength = 0;
    m_pData = nullptr;
}

template void RenderBuffer::SetData<float>(size_t ulDataCount, float *pData);
template void RenderBuffer::SetData<uint16_t>(size_t ulDataCount, uint16_t *pData);

template <typename T>
void RenderBuffer::SetData(size_t ulDataCount, T *pData)
{
    m_ulDataCount = ulDataCount;
    m_ulDataLength = ulDataCount * sizeof(T);
    m_pData = pData;
}