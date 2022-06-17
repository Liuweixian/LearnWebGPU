#include "RGDrawShader.h"

RGDrawShader::RGDrawShader(int nAttributeCount, ...)
{
    wgpu::VertexFormat vertexFormats[nAttributeCount];
    va_list ap;
    va_start(ap, nAttributeCount);
    for (int i = 0; i < nAttributeCount; i++)
    {
        wgpu::VertexFormat eVertexFormat = va_arg(ap, wgpu::VertexFormat);
        vertexFormats[i] = eVertexFormat;
    }
    va_end(ap);
    RGVertexLayout *pVertexLayout = new RGVertexLayout(nAttributeCount, vertexFormats);
    m_VertexLayouts.push_back(pVertexLayout);
}

RGDrawShader::~RGDrawShader()
{

}
