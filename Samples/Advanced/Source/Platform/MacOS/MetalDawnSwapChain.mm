#include "MetalDawnSwapChain.h"

MetalDawnSwapChain::MetalDawnSwapChain(NSWindow* pWindow)
{
    m_pWindow = pWindow;
}

void MetalDawnSwapChain::Init(DawnWSIContextMetal *pContext)
{
    
}

DawnSwapChainError MetalDawnSwapChain::Configure(WGPUTextureFormat eFormat, WGPUTextureUsage eUsage, uint32_t uWidth, uint32_t uHeight)
{
    return DAWN_SWAP_CHAIN_NO_ERROR;
}

DawnSwapChainError MetalDawnSwapChain::GetNextTexture(DawnSwapChainNextTexture *pNext)
{
    return DAWN_SWAP_CHAIN_NO_ERROR;
}

DawnSwapChainError MetalDawnSwapChain::Present()
{
    return DAWN_SWAP_CHAIN_NO_ERROR;
}

void MetalDawnSwapChain::GetWindowSize(uint32_t &uWidth, uint32_t &uHeight)
{
    CGSize size = m_pWindow.contentView.frame.size;
    CGFloat scaleFactor = m_pWindow.backingScaleFactor;
    uWidth = size.width * scaleFactor;
    uHeight = size.height * scaleFactor;
}
