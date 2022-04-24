#pragma once
#include "RenderGraph.h"

class RenderGraphPlayer
{
public:
    RenderGraphPlayer();
    ~RenderGraphPlayer();
    bool RenderLoop();

    template <typename T>
    void RunGraph()
    {
        if (m_pCurRDG != nullptr)
        {
            delete m_pCurRDG;
            m_pCurRDG = nullptr;
        }
        m_pCurRDG = new T();
    }

private:
    void Initialize();

    RenderGraph *m_pCurRDG;
    bool m_bInitialized;
};