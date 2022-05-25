#include <emscripten/emscripten.h>
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../Graph/SimpleRenderGraph.h"

void main_loop()
{
    bool looping = GetRDGPlayer()->RenderLoop();
    if (!looping)
        emscripten_cancel_main_loop();
}

int main(int argc, char **argv)
{
    emscripten_set_main_loop(main_loop, 0, false);

    RenderGraphPlayer* pRDGPlayer = GetRDGPlayer();
    // Create RenderObject into RenderGraphPlayer
    RenderObject *pRenderObject = pRDGPlayer->AddRenderObject<RenderObject>("Cube");
    // Create Material for RenderObject
    RenderMaterial *pMaterial = pRenderObject->CreateMaterial<RenderMaterial>(RenderPassIdx::DrawObjectPass);
    RenderMesh *pMesh = pRenderObject->CreateMesh<RenderMesh>();
    // Create VBO for RenderObject
    RenderBuffer *pRenderVertexBuffer = pMesh->CreateVertexBuffer<RenderBuffer>();
    float vertexData[192] = {0.5, -0.5, 0.5, 0, 0, 1, 0, 0, -0.5, -0.5, 0.5, 0, 0, 1, 1, 0, 0.5, 0.5, 0.5, 0, 0, 1, 0, 1, -0.5, 0.5, 0.5, 0, 0, 1, 1, 1, 0.5, 0.5, -0.5, 0, 1, 0, 0, 1, -0.5, 0.5, -0.5, 0, 1, 0, 1, 1, 0.5, -0.5, -0.5, 0, 0, -1, 0, 1, -0.5, -0.5, -0.5, 0, 0, -1, 1, 1, 0.5, 0.5, 0.5, 0, 1, 0, 0, 0, -0.5, 0.5, 0.5, 0, 1, 0, 1, 0, 0.5, 0.5, -0.5, 0, 0, -1, 0, 0, -0.5, 0.5, -0.5, 0, 0, -1, 1, 0, 0.5, -0.5, -0.5, 0, -1, 0, 0, 0, 0.5, -0.5, 0.5, 0, -1, 0, 0, 1, -0.5, -0.5, 0.5, 0, -1, 0, 1, 1, -0.5, -0.5, -0.5, 0, -1, 0, 1, 0, -0.5, -0.5, 0.5, -1, 0, 0, 0, 0, -0.5, 0.5, 0.5, -1, 0, 0, 0, 1, -0.5, 0.5, -0.5, -1, 0, 0, 1, 1, -0.5, -0.5, -0.5, -1, 0, 0, 1, 0, 0.5, -0.5, -0.5, 1, 0, 0, 0, 0, 0.5, 0.5, -0.5, 1, 0, 0, 0, 1, 0.5, 0.5, 0.5, 1, 0, 0, 1, 1, 0.5, -0.5, 0.5, 1, 0, 0, 1, 0};
    pRenderVertexBuffer->SetData<float>(192, vertexData);
    // Create IBO for RenderObject
    RenderBuffer *pRenderIndexBuffer = pMesh->CreateIndexBuffer<RenderBuffer>();
    uint16_t indexData[36] = {0, 2, 3, 0, 3, 1, 8, 4, 5, 8, 5, 9, 10, 6, 7, 10, 7, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};
    pRenderIndexBuffer->SetData<uint16_t>(36, indexData);

    pRDGPlayer->RunGraph<SimpleRenderGraph>();
    return 0;
}