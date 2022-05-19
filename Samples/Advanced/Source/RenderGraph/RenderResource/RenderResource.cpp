#include "RenderResource.h"

RenderResource::RenderResource()
{
    m_AllTextureDescs.clear();
    m_AllResourceHandles.clear();
    m_pFrameBufferHandle = nullptr;
}

RenderResource::~RenderResource()
{
}

wgpu::TextureDescriptor *RenderResource::GetTextureDesc(RenderResourceHandle *pHandle)
{
    auto found = m_AllTextureDescs.find(pHandle->m_unDescIdx);
    if (found == m_AllTextureDescs.end())
        return nullptr;
    return found->second;
}

RenderResourceHandle *RenderResource::GetFrameBuffer()
{
    if (m_pFrameBufferHandle == nullptr)
        m_pFrameBufferHandle = CreateTexture("FrameBuffer", wgpu::TextureFormat::BGRA8Unorm, 0, 0, 0, 0, 0);
    return m_pFrameBufferHandle;
}

RenderResourceHandle *RenderResource::CreateTexture(std::string szName, wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount)
{
    auto it = m_AllResourceHandles.find(szName);
    if (it != m_AllResourceHandles.end())
        return it->second;

    RenderResourceHandle *pHandle = new RenderResourceHandle();
    pHandle->m_szName = szName;
    pHandle->m_eType = RenderResourceHandle::Type::Texture;
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

uint32_t RenderResource::GetTextureDescriptorIdx(wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount)
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

static RenderResource *g_pRenderResource = nullptr;
RenderResource *GetRenderResource()
{
    if (g_pRenderResource == nullptr)
        g_pRenderResource = new RenderResource();
    return g_pRenderResource;
}