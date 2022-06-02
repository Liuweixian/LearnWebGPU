#include <emscripten/emscripten.h>
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../Graph/SimpleRenderGraph/SimpleRenderGraph.h"
#include "../../RenderGraph/GfxDevice.h"
#include "EmccGfxDevice.h"

void main_loop()
{
    bool looping = GetRDGPlayer()->RenderLoop();
    if (!looping)
        emscripten_cancel_main_loop();
}

int main(int argc, char **argv)
{
    emscripten_set_main_loop(main_loop, 0, false);

    CreateGfxDevice([](GfxDevice*& pDevice)
    {
        pDevice = new EmccGfxDevice();
    });

    RenderGraphPlayer *pRDGPlayer = GetRDGPlayer();
    // Create RenderObject into RenderGraphPlayer
    RenderObject *pRenderObject = pRDGPlayer->AddRenderObject<RenderObject>("Triganle");
    // Create Material for RenderObject
    RenderMaterial *pMaterial = pRenderObject->CreateMaterial<RenderMaterial>(RenderPassIdx::DrawObjectPass);
    RenderMesh *pMesh = pRenderObject->CreateMesh<RenderMesh>();
    // Create VBO for RenderObject
    RenderBuffer *pRenderVertexBuffer = pMesh->CreateVertexBuffer<RenderBuffer>();
    //(x,y,r,g,b)
    float vertexData[15] = {
        -0.8f, -0.8f, 0.0f, 0.0f, 1.0f, // BL
         0.8f, -0.8f, 0.0f, 1.0f, 0.0f, // BR
        -0.0f,  0.8f, 1.0f, 0.0f, 0.0f, // top
    };
    pRenderVertexBuffer->SetData<float>(15, vertexData);
    // Create IBO for RenderObject
    RenderBuffer *pRenderIndexBuffer = pMesh->CreateIndexBuffer<RenderBuffer>();
    uint16_t indexData[4] = {0, 1, 2, 0};
    pRenderIndexBuffer->SetData<uint16_t>(4, indexData);

    pRDGPlayer->RunGraph<SimpleRenderGraph>();
    return 0;
}