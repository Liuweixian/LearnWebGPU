#include "RGVertexLayout.h"

RGVertexLayout::RGVertexLayout(int nAttributeCount, wgpu::VertexFormat *pVertexFormats)
{
    m_pLayout = new wgpu::VertexBufferLayout();
    m_pLayout->stepMode = wgpu::VertexStepMode::Vertex;
    m_pLayout->attributeCount = nAttributeCount;
    wgpu::VertexAttribute *pAttributes = new wgpu::VertexAttribute[nAttributeCount];
    m_pLayout->attributes = pAttributes;

    uint32_t uOffset = 0;
    for (int i = 0; i < nAttributeCount; i++)
    {
        pAttributes[i].format = pVertexFormats[i];
        pAttributes[i].offset = uOffset;
        pAttributes[i].shaderLocation = i;
        uOffset += GetVertexFormatSize(pVertexFormats[i]);
    }
    m_pLayout->arrayStride = uOffset;
}

uint32_t RGVertexLayout::GetVertexFormatSize(wgpu::VertexFormat eVertexFormat)
{
    uint32_t unRet = 0;
    switch (eVertexFormat)
    {
    case wgpu::VertexFormat::Float32x2:
        unRet = sizeof(float) * 2;
        break;
    case wgpu::VertexFormat::Float32x3:
        unRet = sizeof(float) * 3;
        break;
    }
    return unRet;
}

RGVertexLayout::~RGVertexLayout()
{
}
