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

wgpu::TextureDescriptor *RGResources::GetTextureDesc(RGTextureResHandle *pTexHandle)
{
    auto found = m_AllTextureDescs.find(pTexHandle->m_unDescIdx);
    if (found == m_AllTextureDescs.end())
        return nullptr;
    return found->second;
}

RGTextureResHandle *RGResources::GetFrameBuffer()
{
    if (m_pFrameBufferHandle == nullptr)
        m_pFrameBufferHandle = CreateTexture("FrameBuffer_Tex", wgpu::TextureFormat::BGRA8Unorm, 0, 0, 0, 0, 0);
    return m_pFrameBufferHandle;
}

bool RGResources::IsFrameBuffer(RGTextureResHandle *pTexHandle)
{
    return pTexHandle->m_unDescIdx == m_pFrameBufferHandle->m_unDescIdx;
}

RGTextureResHandle *RGResources::CreateTexture(std::string szName, wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount)
{
    auto it = m_AllResourceHandles.find(szName);
    if (it != m_AllResourceHandles.end())
        return reinterpret_cast<RGTextureResHandle*>(it->second);

    RGTextureResHandle *pTexHandle = new RGTextureResHandle();
    pTexHandle->m_szName = szName;
    pTexHandle->m_eType = RGResourceHandle::Type::Texture;
    pTexHandle->m_unDescIdx = GetTextureDescriptorIdx(eTextureFormat, unWidth, unHeight, unDepthOrArrayLayers, unMipLevelCount, unSampleCount);
    m_AllResourceHandles.insert(std::make_pair(szName, pTexHandle));
    auto found = m_AllTextureDescs.find(pTexHandle->m_unDescIdx);
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
        m_AllTextureDescs.insert(std::make_pair(pTexHandle->m_unDescIdx, pTextureDesc));
    }
    return pTexHandle;
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

RGBufferResHandle *RGResources::CreateBuffer(std::string szName, wgpu::BufferUsage eBufferUsage)
{
    RGBufferResHandle *pBufferHandle = GetBuffer(szName);
    if (pBufferHandle != nullptr)
        return pBufferHandle;
    
    pBufferHandle = new RGBufferResHandle(eBufferUsage);
    pBufferHandle->m_szName = szName;
    pBufferHandle->m_eType = RGResourceHandle::Type::Buffer;
    m_AllResourceHandles.insert(std::make_pair(szName, pBufferHandle));
    return pBufferHandle;
}

RGBufferResHandle *RGResources::GetBuffer(std::string szName)
{
    auto it = m_AllResourceHandles.find(szName);
    if (it != m_AllResourceHandles.end())
        return reinterpret_cast<RGBufferResHandle*>(it->second);
    return nullptr;
}

static RGResources *g_pRGResources = nullptr;
RGResources *GetResources()
{
    if (g_pRGResources == nullptr)
        g_pRGResources = new RGResources();
    return g_pRGResources;
}
