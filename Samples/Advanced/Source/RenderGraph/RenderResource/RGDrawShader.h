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
    RGDrawShader(int nAttributeCount, ...);
    virtual ~RGDrawShader();

    std::list<RGVertexLayout *> GetVertexLayouts()
    {
        return m_VertexLayouts;
    }

protected:
    std::list<RGVertexLayout *> m_VertexLayouts;
};
