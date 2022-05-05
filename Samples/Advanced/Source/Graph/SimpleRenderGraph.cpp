#include <stdio.h>
#include "SimpleRenderGraph.h"
#include "DrawObjectPass.h"
#include "../RenderGraph/RenderPass/RenderPassIdx.h"
#include "../RenderGraph/RenderObject/RenderObject.h"
#include "../RenderGraph/RenderObject/RenderVertexBuffer.h"
#include "../RenderGraph/RenderObject/RenderMaterial.h"
#include "../RenderGraph/RenderObject/RenderShader.h"
#include "../RenderGraph/RenderObject/RenderShaderProgram.h"

SimpleRenderGraph::SimpleRenderGraph()
{

}

SimpleRenderGraph::~SimpleRenderGraph()
{

}

void SimpleRenderGraph::InitializeComplete()
{
    InitializeRenderObjects();
    InitializeRenderGraph();
}

void SimpleRenderGraph::InitializeRenderObjects()
{
    //Create RenderObject into RenderGraph
    RenderObject* pRenderObject = this->AddRenderObject<RenderObject>("Cube");
    //Create Material for RenderObject
    RenderMaterial* pMaterial = pRenderObject->Create<RenderMaterial>(RenderPassIdx::DrawObjectPass);
    //Create VBO for RenderObject
    RenderVertexBuffer* pRenderVertexBuffer = pRenderObject->Create<RenderVertexBuffer>();

}

void SimpleRenderGraph::InitializeRenderGraph()
{
    this->AddRenderPass<DrawObjectPass>();
}