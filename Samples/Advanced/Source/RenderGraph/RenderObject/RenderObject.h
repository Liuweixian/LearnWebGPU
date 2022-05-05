#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "RenderShader.h"
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
        static_assert(std::is_base_of<RenderShader, T>::value, "T must be a descendant of RenderShader");
        T *pShader = new T(passIdx);
        m_Shaders.insert(std::make_pair(passIdx, pShader));
        return pShader;
    }

private:
    std::string m_szName;
    std::list<RenderVertexBuffer *> m_VertexBuffers;
    std::unordered_map<RenderPassIdx, RenderShader *> m_Shaders;
};