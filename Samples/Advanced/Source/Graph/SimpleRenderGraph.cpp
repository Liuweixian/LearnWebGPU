#include <stdio.h>
#include "SimpleRenderGraph.h"
#include "DrawObjectPass.h"
#include "../RenderGraph/RenderPass/RenderPassIdx.h"
#include "../RenderGraph/RenderObject/RenderObject.h"
#include "../RenderGraph/RenderObject/RenderVertexBuffer.h"
#include "../RenderGraph/RenderObject/RenderIndexBuffer.h"
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
    // Create RenderObject into RenderGraph
    RenderObject *pRenderObject = this->AddRenderObject<RenderObject>("Cube");
    // Create Material for RenderObject
    RenderMaterial *pMaterial = pRenderObject->CreateMaterial<RenderMaterial>(RenderPassIdx::DrawObjectPass);
    // Create VBO for RenderObject
    RenderVertexBuffer *pRenderVertexBuffer = pRenderObject->CreateVertexBuffer<RenderVertexBuffer>();
    float vertexData[192] = {0.5, -0.5, 0.5, 0, 0, 1, 0, 0, -0.5, -0.5, 0.5, 0, 0, 1, 1, 0, 0.5, 0.5, 0.5, 0, 0, 1, 0, 1, -0.5, 0.5, 0.5, 0, 0, 1, 1, 1, 0.5, 0.5, -0.5, 0, 1, 0, 0, 1, -0.5, 0.5, -0.5, 0, 1, 0, 1, 1, 0.5, -0.5, -0.5, 0, 0, -1, 0, 1, -0.5, -0.5, -0.5, 0, 0, -1, 1, 1, 0.5, 0.5, 0.5, 0, 1, 0, 0, 0, -0.5, 0.5, 0.5, 0, 1, 0, 1, 0, 0.5, 0.5, -0.5, 0, 0, -1, 0, 0, -0.5, 0.5, -0.5, 0, 0, -1, 1, 0, 0.5, -0.5, -0.5, 0, -1, 0, 0, 0, 0.5, -0.5, 0.5, 0, -1, 0, 0, 1, -0.5, -0.5, 0.5, 0, -1, 0, 1, 1, -0.5, -0.5, -0.5, 0, -1, 0, 1, 0, -0.5, -0.5, 0.5, -1, 0, 0, 0, 0, -0.5, 0.5, 0.5, -1, 0, 0, 0, 1, -0.5, 0.5, -0.5, -1, 0, 0, 1, 1, -0.5, -0.5, -0.5, -1, 0, 0, 1, 0, 0.5, -0.5, -0.5, 1, 0, 0, 0, 0, 0.5, 0.5, -0.5, 1, 0, 0, 0, 1, 0.5, 0.5, 0.5, 1, 0, 0, 1, 1, 0.5, -0.5, 0.5, 1, 0, 0, 1, 0};
    pRenderVertexBuffer->SetData<float>(192, vertexData, 3, 3, 3, 2);
    // Create IBO for RenderObject
    RenderIndexBuffer *pRenderIndexBuffer = pRenderObject->CreateIndexBuffer<RenderIndexBuffer>();
    uint16_t indexData[36] = {0, 2, 3, 0, 3, 1, 8, 4, 5, 8, 5, 9, 10, 6, 7, 10, 7, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};
    pRenderIndexBuffer->SetData<uint16_t>(36, indexData);
}

void SimpleRenderGraph::InitializeRenderGraph()
{
    DrawObjectPass *pDrawObjectPass = this->AddRenderPass<DrawObjectPass>();
}