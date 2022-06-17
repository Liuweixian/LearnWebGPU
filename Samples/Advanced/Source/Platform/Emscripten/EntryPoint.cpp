#include <emscripten/emscripten.h>
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../Graph/DrawTriangleRenderGraph/DrawTriangleRenderGraph.h"
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

    pRDGPlayer->RunGraph<DrawTriangleRenderGraph>();
    return 0;
}