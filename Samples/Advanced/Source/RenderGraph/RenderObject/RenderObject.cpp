#include "RenderObject.h"

RenderObject::RenderObject(std::string szName)
{
    m_szName = szName;
    m_pRenderMesh = nullptr;
    m_Materials.clear();
}

RenderObject::~RenderObject()
{
    if (m_pRenderMesh != nullptr)
    {
        delete m_pRenderMesh;
        m_pRenderMesh = nullptr;
    }

    for (auto it = m_Materials.begin(); it != m_Materials.end(); it++)
    {
        delete it->second;
    }
    m_Materials.clear();
}

RGMaterial *RenderObject::GetMaterial(RGPassIdx uPassIdx)
{
    auto found = m_Materials.find(uPassIdx);
    if (found != m_Materials.end())
    {
        return found->second;
    }
    return nullptr;
}
