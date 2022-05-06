#include "RenderObject.h"

RenderObject::RenderObject(std::string szName)
{
    m_szName = szName;
    m_pRenderIndexBuffer = nullptr;
}

RenderObject::~RenderObject()
{
    for (auto it = m_VertexBuffers.begin(); it != m_VertexBuffers.end(); it++)
    {
        delete *it;
    }
    m_VertexBuffers.clear();

    if (m_pRenderIndexBuffer != nullptr)
    {
        delete m_pRenderIndexBuffer;
        m_pRenderIndexBuffer = nullptr;
    }

    for (auto it = m_Materials.begin(); it != m_Materials.end(); it++)
    {
        delete it->second;
    }
    m_Materials.clear();
}

RenderMaterial *RenderObject::GetMaterial(RenderPassIdx ePassIdx)
{
    auto found = m_Materials.find(ePassIdx);
    if (found != m_Materials.end())
    {
        return found->second;
    }
    return nullptr;
}