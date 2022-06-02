#pragma once
#include <stdio.h>
#include <list>
#include <stdarg.h>
#include "RGShader.h"
#include "RGVertexLayout.h"
#include "webgpu/webgpu_cpp.h"

class RGDrawShader : public RGShader
{
public:
    RGDrawShader();
    virtual ~RGDrawShader();

    RGVertexLayout *CreateVertexLayout(int nAttributeCount, ...)
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
        return pVertexLayout;
    }

    std::list<RGVertexLayout *> GetVertexLayouts()
    {
        return m_VertexLayouts;
    }

protected:
    std::list<RGVertexLayout *> m_VertexLayouts;
};
