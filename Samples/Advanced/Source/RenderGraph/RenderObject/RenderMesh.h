#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "RenderMaterial.h"
#include "../RenderResource/RGBuffer.h"
#include "../RGDefine.h"

class RenderMesh
{
public:
    RenderMesh();
    virtual ~RenderMesh();
    template <typename T>
    T *CreateVertexBuffer()
    {
        static_assert(std::is_base_of<RGBuffer, T>::value, "T must be a descendant of RenderBuffer");
        T *pVertexBuffer = new T();
        m_VertexBuffers.push_back(pVertexBuffer);
        return pVertexBuffer;
    }

    template <typename T>
    T *CreateIndexBuffer()
    {
        static_assert(std::is_base_of<RGBuffer, T>::value, "T must be a descendant of RenderBuffer");
        if (m_pIndexBuffer != nullptr)
        {
            delete m_pIndexBuffer;
            m_pIndexBuffer = nullptr;
        }
        m_pIndexBuffer = new T();
        return m_pIndexBuffer;
    }

    std::list<RGBuffer *> GetVertexBuffers()
    {
        return m_VertexBuffers;
    }

    RGBuffer *GetIndexBuffer()
    {
        return m_pIndexBuffer;
    }

private:
    std::list<RGBuffer *> m_VertexBuffers;
    RGBuffer *m_pIndexBuffer;
};
