#include "SimpleRenderGraph.h"
#include <stdio.h>

SimpleRenderGraph::SimpleRenderGraph()
{

}

SimpleRenderGraph::~SimpleRenderGraph()
{

}

bool SimpleRenderGraph::Execute()
{
    RenderGraph::Execute();
    return true;
}

void SimpleRenderGraph::InitializeComplete()
{
    
}