#include <stdio.h>
#include "SimpleRenderGraph.h"
#include "DrawObjectPass.h"
#include "../RenderGraph/RenderResource/RenderResource.h"

SimpleRenderGraph::SimpleRenderGraph()
{
}

SimpleRenderGraph::~SimpleRenderGraph()
{
}

void SimpleRenderGraph::InitializeComplete()
{
    RenderResourceHandle* pFrameBuffer = GetRenderResource()->GetFrameBuffer();
    DrawObjectPass *pDrawObjectPass = this->AddRenderPass<DrawObjectPass>();
    pDrawObjectPass->SetRenderTarget(pFrameBuffer);
}