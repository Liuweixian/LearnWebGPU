#include "RGObject.h"

RGObject::RGObject(std::string szName)
{
    m_szName = szName;
    m_pMesh = nullptr;
    m_Materials.clear();
}

RGObject::~RGObject()
{
    if (m_pMesh != nullptr)
    {
        delete m_pMesh;
        m_pMesh = nullptr;
    }

    for (auto it = m_Materials.begin(); it != m_Materials.end(); it++)
    {
        delete it->second;
    }
    m_Materials.clear();
}

RGMaterial *RGObject::GetMaterial(RGPassIdx uPassIdx)
{
    auto found = m_Materials.find(uPassIdx);
    if (found != m_Materials.end())
    {
        return found->second;
    }
    return nullptr;
}
