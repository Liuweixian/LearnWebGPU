#include <emscripten/emscripten.h>
#include "RenderGraph/RenderGraphPlayer.h"
#include "Graph/SimpleRenderGraph.h"


static RenderGraphPlayer* ms_pRDGPlayer = nullptr;

void main_loop()
{
    bool looping = ms_pRDGPlayer->RenderLoop();
    if (!looping)
        emscripten_cancel_main_loop();
}

int main(int argc, char **argv)
{
    ms_pRDGPlayer = new RenderGraphPlayer();
    ms_pRDGPlayer->RunGraph<SimpleRenderGraph>();
    emscripten_set_main_loop(main_loop, 0, false);
    return 0;
}