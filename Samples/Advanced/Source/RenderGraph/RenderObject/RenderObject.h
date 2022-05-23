#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "RenderMaterial.h"
#include "RenderMesh.h"
#include "../RenderPass/RenderGraphDefine.h"

class RenderObject
{
public:
    RenderObject(std::string szName);
    virtual ~RenderObject();
    std::string GetName()
    {
        return m_szName;
    }

    template <typename T>
    T *CreateMaterial(RenderPassIdx ePassIdx)
    {
        static_assert(std::is_base_of<RenderMaterial, T>::value, "T must be a descendant of RenderMaterial");
        auto found = m_Materials.find(ePassIdx);
        if (found != m_Materials.end())
        {
            printf("This render object already had material pass idx %hu\n", ePassIdx);
            return found->second;
        }

        T *pMaterial = new T(ePassIdx);
        m_Materials.insert(std::make_pair(ePassIdx, pMaterial));
        return pMaterial;
    }

    RenderMaterial *GetMaterial(RenderPassIdx ePassIdx);

    template <typename T>
    T *CreateMesh()
    {
        static_assert(std::is_base_of<RenderMesh, T>::value, "T must be a descendant of RenderMesh");
        if (m_pRenderMesh != nullptr)
        {
            delete m_pRenderMesh;
            m_pRenderMesh = nullptr;
        }
        m_pRenderMesh = new T();
        return m_pRenderMesh;
    }

    RenderMesh *GetMesh()
    {
        return m_pRenderMesh;
    }

private:
    std::string m_szName;
    RenderMesh *m_pRenderMesh;
    std::unordered_map<RenderPassIdx, RenderMaterial *> m_Materials;
};