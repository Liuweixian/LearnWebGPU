#include "RenderVertexBuffer.h"
#include <cassert>

RenderVertexBuffer::RenderVertexBuffer()
{
    m_ulDataSize = 0;
    m_pData = nullptr;
    m_pLayout = nullptr;
}

RenderVertexBuffer::~RenderVertexBuffer()
{
    m_ulDataSize = 0;
    m_pData = nullptr;
    if (m_pLayout != nullptr)
    {
        delete m_pLayout;
        m_pLayout = nullptr;
    }
}

template void RenderVertexBuffer::SetData<float>(size_t ulDataLength, float *pData, int nAttributeCount, ...);

template <typename T>
void RenderVertexBuffer::SetData(size_t ulDataLength, T *pData, int nAttributeCount, ...)
{
    m_pLayout = new wgpu::VertexBufferLayout();
    m_pLayout->stepMode = wgpu::VertexStepMode::Vertex;
    m_pLayout->attributeCount = nAttributeCount;
    wgpu::VertexAttribute *pAttributes = new wgpu::VertexAttribute[nAttributeCount];
    m_pLayout->attributes = pAttributes;

    uint64_t unTotalArrayStride = 0;
    int nOffset = 0;
    va_list ap;
    va_start(ap, nAttributeCount);
    for (int i = 0; i < nAttributeCount; i++)
    {
        int nCurStride = va_arg(ap, int);

        pAttributes[i].format = GetVertexFormat(typeid(T), nCurStride);
        pAttributes[i].offset = nOffset;
        pAttributes[i].shaderLocation = i;
        unTotalArrayStride += nCurStride;
        nOffset = nCurStride * sizeof(T);
    }
    va_end(ap);
    m_pLayout->arrayStride = unTotalArrayStride;
    m_ulDataSize = ulDataLength * sizeof(T);
    m_pData = pData;
}

wgpu::VertexFormat RenderVertexBuffer::GetVertexFormat(const std::type_info &typeInfo, int nStride)
{
    if (typeInfo == typeid(float))
    {
        uint32_t ret = (uint32_t)wgpu::VertexFormat::Float32;
        assert(nStride >= 1 && nStride <= 4);
        ret += (nStride - 1);
        return (wgpu::VertexFormat)ret;
    }
    return wgpu::VertexFormat::Undefined;
}