#include <stdio.h>
#include "SimpleRenderGraph.h"
#include "DrawObjectPass.h"

SimpleRenderGraph::SimpleRenderGraph()
{
}

SimpleRenderGraph::~SimpleRenderGraph()
{
}

void SimpleRenderGraph::InitializeComplete()
{
    DrawObjectPass *pDrawObjectPass = this->AddRenderPass<DrawObjectPass>();
}