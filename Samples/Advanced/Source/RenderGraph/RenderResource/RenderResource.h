#pragma once
#include <stdio.h>
#include <unordered_map>
#include <webgpu/webgpu_cpp.h>
#include <string>

class RenderTextureHandle
{
public:
    RenderTextureHandle()
    {
        m_szName = nullptr;
        m_unDescIdx = 0;
    }

    std::string m_szName;
    uint32_t m_unDescIdx;
};

class RenderResource
{
public:
    RenderResource();
    ~RenderResource();
    RenderTextureHandle* GetFrameBuffer()
    {
        return &m_FrameBufferHandle;
    }
    //RenderTextureHandle CreateTexture(std::string szName, wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight);

private:
    RenderTextureHandle m_FrameBufferHandle;
    std::unordered_map<uint32_t, wgpu::TextureDescriptor *> m_AllTextureDescs;
};

RenderResource *GetRenderResource();