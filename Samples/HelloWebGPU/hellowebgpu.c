#include <stdio.h>
#include <emscripten/html5_webgpu.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int main(int argc, char **argv)
{
    WGPUDevice wgpuDevice = emscripten_webgpu_get_device();
    printf("hello, world!\n");
    return 0;
}