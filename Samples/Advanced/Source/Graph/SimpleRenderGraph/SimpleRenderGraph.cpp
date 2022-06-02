#include <stdio.h>
#include "SimpleRenderGraph.h"
#include "DrawObjectPass.h"
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../RenderGraph/RenderResource/RenderResource.h"

SimpleRenderGraph::SimpleRenderGraph()
{
}

SimpleRenderGraph::~SimpleRenderGraph()
{
}

void SimpleRenderGraph::InitializeScene()
{
    RenderGraphPlayer *pRDGPlayer = GetRDGPlayer();
    // Create RenderObject into RenderGraphPlayer
    RenderObject *pRenderObject = pRDGPlayer->AddRenderObject<RenderObject>("Triganle");
    // Create Material for RenderObject
    pRenderObject->CreateMaterial<RenderMaterial>((RGPassIdx)PassIdx::DrawObjectPass);
    RenderMesh *pMesh = pRenderObject->CreateMesh<RenderMesh>();
    // Create VBO for RenderObject
    RenderBuffer *pRenderVertexBuffer = pMesh->CreateVertexBuffer<RenderBuffer>();
    //(x,y,r,g,b)
    float vertexData[15] = {
        -0.8f, -0.8f, 0.0f, 0.0f, 1.0f, // BL
        0.8f, -0.8f, 0.0f, 1.0f, 0.0f,  // BR
        0.0f, 0.8f, 1.0f, 0.0f, 0.0f,   // top
    };
    pRenderVertexBuffer->SetData<float>(15, vertexData);
    // Create IBO for RenderObject
    RenderBuffer *pRenderIndexBuffer = pMesh->CreateIndexBuffer<RenderBuffer>();
    uint16_t indexData[4] = {0, 1, 2};
    pRenderIndexBuffer->SetData<uint16_t>(3, indexData);
}

void SimpleRenderGraph::InitializePass()
{
    RenderResourceHandle *pFrameBuffer = GetRenderResource()->GetFrameBuffer();
    DrawObjectPass *pDrawObjectPass = this->AddRenderGraphPass<DrawObjectPass>((RGPassIdx)PassIdx::DrawObjectPass);
    pDrawObjectPass->SetRenderTarget(pFrameBuffer);
}

void SimpleRenderGraph::Initialize()
{
    InitializeScene();
    InitializePass();
}
