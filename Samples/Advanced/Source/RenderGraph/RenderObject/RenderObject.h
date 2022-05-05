#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "RenderMaterial.h"
#include "RenderVertexBuffer.h"
#include "../RenderPass/RenderPassIdx.h"

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
    T *Create()
    {
        static_assert(std::is_base_of<RenderVertexBuffer, T>::value, "T must be a descendant of RenderVertexBuffer");
        T *pVertexBuffer = new T();
        m_VertexBuffers.push_back(pVertexBuffer);
        return pVertexBuffer;
    }

    template <typename T>
    T *Create(RenderPassIdx passIdx)
    {
        static_assert(std::is_base_of<RenderMaterial, T>::value, "T must be a descendant of RenderMaterial");
        T *pMaterial = new T(passIdx);
        m_Materials.insert(std::make_pair(passIdx, pMaterial));
        return pMaterial;
    }

private:
    std::string m_szName;
    std::list<RenderVertexBuffer *> m_VertexBuffers;
    std::unordered_map<RenderPassIdx, RenderMaterial *> m_Materials;
};