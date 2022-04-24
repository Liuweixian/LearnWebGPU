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
    }
    return true;
}