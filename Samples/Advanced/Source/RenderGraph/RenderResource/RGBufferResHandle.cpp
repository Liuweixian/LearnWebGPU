#include "RGBufferResHandle.h"

RGBufferResHandle::RGBufferResHandle(wgpu::BufferUsage eBufferUsage) : RGResourceHandle()
{
    m_ulDataCount = 0;
    m_ulDataLength = 0;
    m_pData = nullptr;
    m_bIsDirty = false;
    m_eShaderStage = wgpu::ShaderStage::None;
    m_eBufferUsage = eBufferUsage;
}

template void RGBufferResHandle::SetData<float>(size_t ulDataCount, const float *pData);
template void RGBufferResHandle::SetData<uint16_t>(size_t ulDataCount, const uint16_t *pData);

template <typename T>
void RGBufferResHandle::SetData(size_t ulDataCount, const T *pData)
{
    if (ulDataCount == 0)
        return;
    
    //calc padding
    size_t ulNewDataLength = ulDataCount * sizeof(T);
    uint32_t uAlignment = 4;
    bool bNeedPadding = ulNewDataLength % uAlignment != 0;
    if (bNeedPadding)
        ulNewDataLength = ((ulNewDataLength + (uAlignment - 1)) & ~(uAlignment - 1));
    
    //already existing data;
    if (m_ulDataLength != 0 && ulNewDataLength > m_ulDataLength)
        m_pData = (int8_t*)realloc(m_pData, ulNewDataLength);
    
    m_ulDataLength = ulNewDataLength;
    m_ulDataCount = ulDataCount;
    
    //no existing data;
    if (m_pData == nullptr)
        m_pData = (int8_t*)malloc(m_ulDataLength);
    
    //replace data
    memcpy(m_pData, pData, m_ulDataLength);
    m_bIsDirty = true;
}

void RGBufferResHandle::SetShaderStage(wgpu::ShaderStage eShaderStage)
{
    m_eShaderStage = eShaderStage;
}
