#include "SimpleRenderGraph.h"
#include <stdio.h>
#include "../RenderGraph/RenderPass/RenderPassIdx.h"
#include "../RenderGraph/RenderPass/DrawObjectPass.h"
#include "../RenderGraph/RenderObject/RenderObject.h"
#include "../RenderGraph/RenderObject/RenderVertexBuffer.h"
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
    //Create Shader for RenderObject
    RenderShader* pShader = pRenderObject->Create<RenderShader>(RenderPassIdx::DrawObjectPass);
    pShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Vertex, "vs_main", "Shaders/vs.wgsl");
    pShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Fragment, "vs_main", "Shaders/vs.wgsl");
    //Create VBO for RenderObject
    RenderVertexBuffer* pRenderVertexBuffer = pRenderObject->Create<RenderVertexBuffer>();

}

void SimpleRenderGraph::InitializeRenderGraph()
{
    this->AddRenderPass<DrawObjectPass>();
}