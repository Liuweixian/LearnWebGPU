#include "MetalDawnSwapChain.h"

MetalDawnSwapChain::MetalDawnSwapChain(NSWindow* pWindow)
{
    m_pWindow = pWindow;
}

void MetalDawnSwapChain::Init(DawnWSIContextMetal *pContext)
{
    m_pDevice = pContext->device;
    m_pQueue = pContext->queue;
}

DawnSwapChainError MetalDawnSwapChain::Configure(WGPUTextureFormat eFormat, WGPUTextureUsage eUsage, uint32_t uWidth, uint32_t uHeight)
{
    CGSize size;
    size.width  = static_cast<CGFloat>(uWidth);
    size.height = static_cast<CGFloat>(uHeight);
    m_pLayer = CAMetalLayer.layer;
    m_pLayer.device = m_pDevice;
    m_pLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    m_pLayer.pixelFormat  = MTLPixelFormatBGRA8Unorm;
    m_pLayer.drawableSize = size;
    if (eUsage & (WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_Present))
    {
        m_pLayer.framebufferOnly = YES;
    }
    NSView* pView = m_pWindow.contentView;
    pView.wantsLayer = YES;
    pView.layer = m_pLayer;
    return DAWN_SWAP_CHAIN_NO_ERROR;
}

DawnSwapChainError MetalDawnSwapChain::GetNextTexture(DawnSwapChainNextTexture *pNext)
{
    m_pDrawable = m_pLayer.nextDrawable;
    m_pTexture = m_pDrawable.texture;
    pNext->texture.ptr = (__bridge void*)m_pTexture;
    return DAWN_SWAP_CHAIN_NO_ERROR;
}

DawnSwapChainError MetalDawnSwapChain::Present()
{
    id<MTLCommandBuffer> pCommandBuffer = [m_pQueue commandBuffer];
    [pCommandBuffer presentDrawable: m_pDrawable];
    [pCommandBuffer commit];
    return DAWN_SWAP_CHAIN_NO_ERROR;
}

void MetalDawnSwapChain::GetWindowSize(uint32_t &uWidth, uint32_t &uHeight)
{
    CGSize size = m_pWindow.contentView.frame.size;
    CGFloat scaleFactor = m_pWindow.backingScaleFactor;
    uWidth = size.width * scaleFactor;
    uHeight = size.height * scaleFactor;
}
