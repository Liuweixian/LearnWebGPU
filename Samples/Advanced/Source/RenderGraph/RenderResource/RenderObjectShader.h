#pragma once
#include <stdio.h>
#include <list>
#include <stdarg.h>
#include "RenderShader.h"
#include "RenderVBOLayout.h"
#include "webgpu/webgpu_cpp.h"

class RenderObjectShader : public RenderShader
{
public:
    RenderObjectShader();
    virtual ~RenderObjectShader();

    RenderVBOLayout *CreateVBOLayout(int nAttributeCount, ...)
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
        RenderVBOLayout *pVBOLayout = new RenderVBOLayout(nAttributeCount, vertexFormats);
        m_VBOLayouts.push_back(pVBOLayout);
        return pVBOLayout;
    }

    std::list<RenderVBOLayout *> GetVBOLayouts()
    {
        return m_VBOLayouts;
    }

protected:
    std::list<RenderVBOLayout *> m_VBOLayouts;
};