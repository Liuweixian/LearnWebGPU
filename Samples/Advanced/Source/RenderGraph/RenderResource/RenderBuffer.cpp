#include "RenderBuffer.h"
#include <cassert>
#include <cstring>

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
    size_t ulDataLength = ulDataCount * sizeof(T);
    uint32_t uAlignment = 4;
    bool bNeedPadding = ulDataLength % uAlignment != 0;
    if (bNeedPadding)
        m_ulDataLength = ((ulDataLength + (uAlignment - 1)) & ~(uAlignment - 1));
    else
        m_ulDataLength = ulDataLength;
    assert(m_ulDataLength != 0);
    m_pData = malloc(m_ulDataLength);
    memcpy(m_pData, pData, ulDataLength);
}
