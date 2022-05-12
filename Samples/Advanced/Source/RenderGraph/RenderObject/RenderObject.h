#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "RenderMaterial.h"
#include "RenderMeshBuffer.h"
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
    T *CreateVertexBuffer()
    {
        static_assert(std::is_base_of<RenderMeshBuffer, T>::value, "T must be a descendant of RenderMeshBuffer");
        T *pVertexBuffer = new T();
        m_VertexBuffers.push_back(pVertexBuffer);
        return pVertexBuffer;
    }

    template <typename T>
    T *CreateIndexBuffer()
    {
        static_assert(std::is_base_of<RenderMeshBuffer, T>::value, "T must be a descendant of RenderMeshBuffer");
        if (m_pRenderIndexBuffer != nullptr)
        {
            delete m_pRenderIndexBuffer;
            m_pRenderIndexBuffer = nullptr;
        }
        m_pRenderIndexBuffer = new T();
        return m_pRenderIndexBuffer;
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

private:
    std::string m_szName;
    std::list<RenderMeshBuffer *> m_VertexBuffers;
    RenderMeshBuffer *m_pRenderIndexBuffer;
    std::unordered_map<RenderPassIdx, RenderMaterial *> m_Materials;
};