#include "SimpleRenderGraph.h"
#include <stdio.h>
#include "../RenderGraph/RenderPass/DrawObjectPass.h"
#include "../RenderGraph/RenderObject/RenderObject.h"

SimpleRenderGraph::SimpleRenderGraph()
{

}

SimpleRenderGraph::~SimpleRenderGraph()
{

}

void SimpleRenderGraph::InitializeComplete()
{
    RenderObject* pRenderObject = this->AddRenderObject<RenderObject>("Cube");
    this->AddRenderPass<DrawObjectPass>();
}