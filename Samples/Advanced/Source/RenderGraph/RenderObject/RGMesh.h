#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "RGMaterial.h"
#include "../RenderResource/RGResources.h"
#include "../RGDefine.h"

class RGMesh
{
public:
    RGMesh(std::string szName);
    virtual ~RGMesh();
    RGBufferResHandle *CreateVertexBuffer();
    RGBufferResHandle *CreateIndexBuffer();
    std::list<RGBufferResHandle *> GetVertexBuffers()
    {
        return m_VertexBuffers;
    }
    RGBufferResHandle *GetIndexBuffer()
    {
        return m_pIndexBuffer;
    }

private:
    std::list<RGBufferResHandle *> m_VertexBuffers;
    RGBufferResHandle *m_pIndexBuffer;
    std::string m_szName;
};
