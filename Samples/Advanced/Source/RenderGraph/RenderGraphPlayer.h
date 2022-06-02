#pragma once
#include "RenderGraph.h"
#include "GfxDevice.h"

class RenderGraphPlayer
{
public:
    RenderGraphPlayer();
    ~RenderGraphPlayer();
    bool RenderLoop();

    template <typename T>
    void RunGraph()
    {
        static_assert(std::is_base_of<RenderGraph, T>::value, "T must be a descendant of RenderGraph");

        if (m_pCurRDG != nullptr)
        {
            delete m_pCurRDG;
            m_pCurRDG = nullptr;
        }
        m_pCurRDG = new T();
    }

    template <typename T>
    T *AddRenderObject(std::string szName)
    {
        static_assert(std::is_base_of<RenderObject, T>::value, "T must be a descendant of RenderObject");
        T *pRenderObject = new T(szName);
        m_RenderObjects.push_back(pRenderObject);
        return pRenderObject;
    }

private:
    RenderGraph *m_pCurRDG;
    std::list<RenderObject *> m_RenderObjects;
};

RenderGraphPlayer *GetRDGPlayer();