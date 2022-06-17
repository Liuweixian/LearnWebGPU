#include <stdio.h>
#include "DrawTriangleRenderGraph.h"
#include "DrawTrianglePass.h"
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../RenderGraph/RenderResource/RGResources.h"

DrawTriangleRenderGraph::DrawTriangleRenderGraph()
{
}

DrawTriangleRenderGraph::~DrawTriangleRenderGraph()
{
}

void DrawTriangleRenderGraph::InitializeScene()
{
    RenderGraphPlayer *pRDGPlayer = GetRDGPlayer();
    // Create RenderObject into RenderGraphPlayer
    RGObject *pRenderObject = pRDGPlayer->AddRenderObject<RGObject>("Triganle");
    // Create Material for RenderObject
    pRenderObject->CreateMaterial<RGMaterial>((RGPassIdx)PassIdx::DrawObjectPass);
    RGMesh *pMesh = pRenderObject->CreateMesh<RGMesh>("Triganle");
    // Create VBO for RenderObject
    RGBufferResHandle *pRenderVertexBuffer = pMesh->CreateVertexBuffer();
    //(x,y,r,g,b)
    float vertexData[15] = {
        -0.8f, -0.8f, 0.0f, 0.0f, 1.0f, // BL
        0.8f, -0.8f, 0.0f, 1.0f, 0.0f,  // BR
        0.0f, 0.8f, 1.0f, 0.0f, 0.0f,   // top
    };
    pRenderVertexBuffer->SetData<float>(15, vertexData);
    // Create IBO for RenderObject
    RGBufferResHandle *pRenderIndexBuffer = pMesh->CreateIndexBuffer();
    uint16_t indexData[3] = {0, 1, 2};
    pRenderIndexBuffer->SetData<uint16_t>(3, indexData);
}

void DrawTriangleRenderGraph::InitializePass()
{
    DrawTrianglePass *pDrawObjectPass = this->AddPass<DrawTrianglePass>((RGPassIdx)PassIdx::DrawObjectPass);
    RGTextureResHandle *pFrameBuffer = GetResources()->GetFrameBuffer();
    pDrawObjectPass->SetRenderTarget(pFrameBuffer);
}

void DrawTriangleRenderGraph::Initialize()
{
    InitializeScene();
    InitializePass();
}
