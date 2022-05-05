#include "RenderGraph.h"
#include <stdio.h>

RenderGraph::RenderGraph()
{
    m_eStatus = Invalid;
}

RenderGraph::~RenderGraph()
{

}

void RenderGraph::Initialize()
{

}

bool RenderGraph::Execute()
{
    if (m_eStatus == Invalid)
    {
        Initialize();
        m_eStatus = Initialized;
        InitializeComplete();
        return true;
    }

    for (auto pass = m_Passes.begin(); pass != m_Passes.end(); pass++)
    {
        for (auto obj = m_RenderObjects.begin(); obj != m_RenderObjects.end(); obj++)
        {
            printf("RenderObject -> %s\n", (*obj)->GetName().c_str());
        }
    }
    return true;
}