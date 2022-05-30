#pragma once
#import <QuartzCore/QuartzCore.h>
#import <AppKit/NSWindow.h>

#define DAWN_ENABLE_BACKEND_METAL
#include "lib/dawn/inc/dawn/dawn_wsi.h"

class MetalDawnSwapChain
{
public:
    MetalDawnSwapChain(NSWindow *pWindow);
    void Init(DawnWSIContextMetal *pContext);
    DawnSwapChainError Configure(WGPUTextureFormat eFormat, WGPUTextureUsage eUsage, uint32_t uWidth, uint32_t uHeight);
    DawnSwapChainError GetNextTexture(DawnSwapChainNextTexture *pNext);
    DawnSwapChainError Present();
    void GetWindowSize(uint32_t &uWidth, uint32_t &uHeight);

private:
    NSWindow *m_pWindow;
    id<MTLDevice> m_pDevice;
    id<MTLCommandQueue> m_pQueue;
    CAMetalLayer *m_pLayer;
    id<CAMetalDrawable> m_pDrawable;
    id<MTLTexture> m_pTexture;
};
