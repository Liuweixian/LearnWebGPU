#pragma once
#include <stdio.h>
#include <unordered_map>
#include <webgpu/webgpu_cpp.h>
#include <string>

class RGResourceHandle
{
public:
    enum Type
    {
        Invalid,
        Texture,
        Buffer
    };

    RGResourceHandle()
    {
        m_eType = Type::Invalid;
        m_szName = "";
        m_unDescIdx = 0;
    }

    Type m_eType;
    std::string m_szName;
    uint32_t m_unDescIdx;
};

class RGResources
{
public:
    RGResources();
    ~RGResources();
    wgpu::TextureDescriptor *GetTextureDesc(RGResourceHandle *pHandle);
    RGResourceHandle *CreateTexture(std::string szName, wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers = 0, uint32_t unMipLevelCount = 1, uint32_t unSampleCount = 1);
    RGResourceHandle *GetFrameBuffer();
    bool IsFrameBuffer(RGResourceHandle *pHandle)
    {
        return pHandle->m_unDescIdx == m_pFrameBufferHandle->m_unDescIdx;
    }

private:
    uint32_t GetTextureDescriptorIdx(wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount);

private:
    RGResourceHandle *m_pFrameBufferHandle;
    std::unordered_map<std::string, RGResourceHandle *> m_AllResourceHandles;
    std::unordered_map<uint32_t, wgpu::TextureDescriptor *> m_AllTextureDescs;
};

RGResources *GetRGResources();
