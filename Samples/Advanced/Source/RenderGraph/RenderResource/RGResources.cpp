#include "RGResources.h"

RGResources::RGResources()
{
    m_AllTextureDescs.clear();
    m_AllResourceHandles.clear();
    m_pFrameBufferHandle = nullptr;
}

RGResources::~RGResources()
{
}

wgpu::TextureDescriptor *RGResources::GetTextureDesc(RGResourceHandle *pHandle)
{
    auto found = m_AllTextureDescs.find(pHandle->m_unDescIdx);
    if (found == m_AllTextureDescs.end())
        return nullptr;
    return found->second;
}

RGResourceHandle *RGResources::GetFrameBuffer()
{
    if (m_pFrameBufferHandle == nullptr)
        m_pFrameBufferHandle = CreateTexture("FrameBuffer", wgpu::TextureFormat::BGRA8Unorm, 0, 0, 0, 0, 0);
    return m_pFrameBufferHandle;
}

bool RGResources::IsFrameBuffer(RGResourceHandle *pHandle)
{
    return pHandle->m_unDescIdx == m_pFrameBufferHandle->m_unDescIdx;
}

RGResourceHandle *RGResources::CreateTexture(std::string szName, wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount)
{
    auto it = m_AllResourceHandles.find(szName);
    if (it != m_AllResourceHandles.end())
        return it->second;

    RGResourceHandle *pHandle = new RGResourceHandle();
    pHandle->m_szName = szName;
    pHandle->m_eType = RGResourceHandle::Type::Texture;
    pHandle->m_unDescIdx = GetTextureDescriptorIdx(eTextureFormat, unWidth, unHeight, unDepthOrArrayLayers, unMipLevelCount, unSampleCount);
    m_AllResourceHandles.insert(std::make_pair(szName, pHandle));
    auto found = m_AllTextureDescs.find(pHandle->m_unDescIdx);
    if (found == m_AllTextureDescs.end())
    {
        wgpu::TextureDescriptor *pTextureDesc = new wgpu::TextureDescriptor();
        pTextureDesc->usage = wgpu::TextureUsage::None;
        if (unHeight == 1 && unDepthOrArrayLayers == 1)
            pTextureDesc->dimension = wgpu::TextureDimension::e1D;
        else if (unHeight > 1 && unDepthOrArrayLayers == 1)
            pTextureDesc->dimension = wgpu::TextureDimension::e2D;
        else
            pTextureDesc->dimension = wgpu::TextureDimension::e3D;
        pTextureDesc->size.width = unWidth;
        pTextureDesc->size.height = unHeight;
        pTextureDesc->size.depthOrArrayLayers = unDepthOrArrayLayers;
        pTextureDesc->format = eTextureFormat;
        pTextureDesc->mipLevelCount = unMipLevelCount;
        pTextureDesc->sampleCount = unSampleCount;
        m_AllTextureDescs.insert(std::make_pair(pHandle->m_unDescIdx, pTextureDesc));
    }
    return pHandle;
}

uint32_t RGResources::GetTextureDescriptorIdx(wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount)
{
    uint32_t unHash = 0;
    unHash = unHash * 31 + (uint32_t)eTextureFormat;
    unHash = unHash * 31 + unWidth;
    unHash = unHash * 31 + unHeight;
    unHash = unHash * 31 + unDepthOrArrayLayers;
    unHash = unHash * 31 + unMipLevelCount;
    unHash = unHash * 31 + unSampleCount;
    return unHash;
}

static RGResources *g_pRGResources = nullptr;
RGResources *GetRGResources()
{
    if (g_pRGResources == nullptr)
        g_pRGResources = new RGResources();
    return g_pRGResources;
}
