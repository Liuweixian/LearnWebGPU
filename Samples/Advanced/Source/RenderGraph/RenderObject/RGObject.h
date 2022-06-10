#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "RGMaterial.h"
#include "RGMesh.h"
#include "../RGDefine.h"

class RGObject
{
public:
    RGObject(std::string szName);
    virtual ~RGObject();
    std::string GetName()
    {
        return m_szName;
    }

    template <typename T>
    T *CreateMaterial(RGPassIdx uPassIdx)
    {
        static_assert(std::is_base_of<RGMaterial, T>::value, "T must be a descendant of RenderMaterial");
        auto found = m_Materials.find(uPassIdx);
        if (found != m_Materials.end())
        {
            printf("This render object already had material pass idx %hu\n", uPassIdx);
            return found->second;
        }

        T *pMaterial = new T(uPassIdx);
        m_Materials.insert(std::make_pair(uPassIdx, pMaterial));
        return pMaterial;
    }

    RGMaterial *GetMaterial(RGPassIdx uPassIdx);

    template <typename T>
    T *CreateMesh(std::string szName)
    {
        static_assert(std::is_base_of<RGMesh, T>::value, "T must be a descendant of RenderMesh");
        if (m_pMesh != nullptr)
        {
            delete m_pMesh;
            m_pMesh = nullptr;
        }
        m_pMesh = new T(szName);
        return m_pMesh;
    }

    RGMesh *GetMesh()
    {
        return m_pMesh;
    }

private:
    std::string m_szName;
    RGMesh *m_pMesh;
    std::unordered_map<RGPassIdx, RGMaterial *> m_Materials;
};
