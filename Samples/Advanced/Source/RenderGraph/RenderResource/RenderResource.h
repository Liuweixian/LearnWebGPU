#pragma once
#include <stdio.h>
#include <unordered_map>
#include <webgpu/webgpu_cpp.h>
#include <string>

class RenderResourceHandle
{
public:
    enum Type
    {
        Invalid,
        Texture,
        Buffer
    };

    RenderResourceHandle()
    {
        m_eType = Type::Invalid;
        m_szName = nullptr;
        m_unDescIdx = 0;
    }

    std::string m_szName;
    uint32_t m_unDescIdx;
    Type m_eType;
};

class RenderResource
{
public:
    RenderResource();
    ~RenderResource();
    wgpu::TextureDescriptor *GetTextureDesc(RenderResourceHandle *pHandle);
    RenderResourceHandle *CreateTexture(std::string szName, wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers = 0, uint32_t unMipLevelCount = 1, uint32_t unSampleCount = 1);
    RenderResourceHandle *GetFrameBuffer()
    {
        return m_pFrameBufferHandle;
    }
    bool IsFrameBuffer(RenderResourceHandle *pHandle)
    {
        return pHandle->m_unDescIdx == m_pFrameBufferHandle->m_unDescIdx;
    }

private:
    uint32_t GetTextureDescriptorIdx(wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount);

private:
    RenderResourceHandle *m_pFrameBufferHandle;
    std::unordered_map<std::string, RenderResourceHandle *> m_AllResourceHandles;
    std::unordered_map<uint32_t, wgpu::TextureDescriptor *> m_AllTextureDescs;
};

RenderResource *GetRenderResource();