#include "RGBuffer.h"
#include <cassert>
#include <cstring>

RGBuffer::RGBuffer()
{
    m_ulDataCount = 0;
    m_ulDataLength = 0;
    m_pData = nullptr;
    m_GPUBuffer = nullptr;
}

RGBuffer::~RGBuffer()
{
    m_ulDataCount = 0;
    m_ulDataLength = 0;
    m_pData = nullptr;
}

template void RGBuffer::SetData<float>(size_t ulDataCount, float *pData);
template void RGBuffer::SetData<uint16_t>(size_t ulDataCount, uint16_t *pData);

template <typename T>
void RGBuffer::SetData(size_t ulDataCount, T *pData)
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
    m_pData = (int8_t*)malloc(m_ulDataLength);
    memcpy(m_pData, pData, ulDataLength);
}