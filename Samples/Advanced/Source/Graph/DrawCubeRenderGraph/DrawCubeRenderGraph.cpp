#include <stdio.h>
#include "DrawCubeRenderGraph.h"
#include "DrawCubePass.h"
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../RenderGraph/RenderResource/RGResources.h"

DrawCubeRenderGraph::DrawCubeRenderGraph()
{
}

DrawCubeRenderGraph::~DrawCubeRenderGraph()
{
}

void DrawCubeRenderGraph::InitializeScene()
{
    RenderGraphPlayer *pRDGPlayer = GetRDGPlayer();
    RGObject *pRenderObject = pRDGPlayer->AddRenderObject<RGObject>("Cube");
    pRenderObject->CreateMaterial<RGMaterial>((RGPassIdx)PassIdx::DrawCubePass);
    RGMesh *pMesh = pRenderObject->CreateMesh<RGMesh>("Cube");
    RGBufferResHandle *pRenderVertexBuffer = pMesh->CreateVertexBuffer();
    //(x,y,z,uv)
    float vertexData[24 * 5] = {
        0.5f, -0.5f, 0.5f, 0, 0,
        -0.5f, -0.5f, 0.5f, 1, 0,
        0.5f, 0.5f, 0.5f, 0, 1,
        -0.5f, 0.5f, 0.5f, 1, 1,
        0.5f, 0.5f, -0.5f, 0, 1,
        -0.5f, 0.5f, -0.5f, 1, 1,
        0.5f, -0.5f, -0.5f, 0, 1,
        -0.5f, -0.5f, -0.5f, 1, 1,
        0.5f, 0.5f, 0.5f, 0, 0,
        -0.5f, 0.5f, 0.5f, 1, 0,
        0.5f, 0.5f, -0.5f, 0, 0,
        -0.5f, 0.5f, -0.5f, 1, 0,
        0.5f, -0.5f, -0.5f, 0, 0,
        0.5f, -0.5f, 0.5f, 0, 1,
        -0.5f, -0.5f, 0.5f, 1, 1,
        -0.5f, -0.5f, -0.5f, 1, 0,
        -0.5f, -0.5f, 0.5f, 0, 0,
        -0.5f, 0.5f, 0.5f, 0, 1,
        -0.5f, 0.5f, -0.5f, 1, 1,
        -0.5f, -0.5f, -0.5f, 1, 0,
        0.5f, -0.5f, -0.5f, 0, 0,
        0.5f, 0.5f, -0.5f, 0, 1,
        0.5f, 0.5f, 0.5f, 1, 1,
        0.5f, -0.5f, 0.5f, 1, 0
    };
    pRenderVertexBuffer->SetData<float>(24 * 5, vertexData);
    // Create IBO for RenderObject
    RGBufferResHandle *pRenderIndexBuffer = pMesh->CreateIndexBuffer();
    uint16_t indexData[36] = {0,2,3,0,3,1,8,4,5,8,5,9,10,6,7,10,7,11,12,13,14,12,14,15,16,17,18,16,18,19,20,21,22,20,22,23};
    pRenderIndexBuffer->SetData<uint16_t>(36, indexData);
}

void DrawCubeRenderGraph::InitializePass()
{
    DrawCubePass *pDrawCubePass = this->AddPass<DrawCubePass>((RGPassIdx)PassIdx::DrawCubePass);
    RGTextureResHandle *pFrameBuffer = GetResources()->GetFrameBuffer();
    pDrawCubePass->SetRenderTarget(pFrameBuffer);
}

void DrawCubeRenderGraph::Initialize()
{
    InitializeScene();
    InitializePass();
}
