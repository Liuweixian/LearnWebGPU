#pragma once
#include <stdio.h>
#include <list>
#include <stdarg.h>
#include "RenderShader.h"
#include "RenderVertexState.h"
#include "webgpu/webgpu_cpp.h"

class RenderObjectShader : public RenderShader
{
public:
    RenderObjectShader();
    virtual ~RenderObjectShader();

    RenderVertexState *CreateVertexState(int nAttributeCount, ...)
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
        RenderVertexState *pVertexState = new RenderVertexState(nAttributeCount, vertexFormats);
        return pVertexState;
    }

protected:
    std::list<RenderVertexState *> m_VertexStates;
};