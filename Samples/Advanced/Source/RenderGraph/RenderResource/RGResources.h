#pragma once
#include <stdio.h>
#include <unordered_map>
#include <webgpu/webgpu_cpp.h>
#include <string>
#include "RGResourceHandle.h"
#include "RGTextureResHandle.h"
#include "RGBufferResHandle.h"

class RGResources
{
public:
    RGResources();
    ~RGResources();
    wgpu::TextureDescriptor *GetTextureDesc(RGTextureResHandle *pHandle);
    RGTextureResHandle *CreateTexture(std::string szName, wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers = 0, uint32_t unMipLevelCount = 1, uint32_t unSampleCount = 1);
    RGTextureResHandle *GetFrameBuffer();
    bool IsFrameBuffer(RGTextureResHandle *pHandle);
    RGBufferResHandle *CreateBuffer(std::string szName, wgpu::BufferUsage eBufferUsage);
    RGBufferResHandle *GetBuffer(std::string szName);

private:
    uint32_t GetTextureDescriptorIdx(wgpu::TextureFormat eTextureFormat, uint32_t unWidth, uint32_t unHeight, uint32_t unDepthOrArrayLayers, uint32_t unMipLevelCount, uint32_t unSampleCount);

private:
    RGTextureResHandle *m_pFrameBufferHandle;
    std::unordered_map<std::string, RGResourceHandle *> m_AllResourceHandles;
    std::unordered_map<uint32_t, wgpu::TextureDescriptor *> m_AllTextureDescs;
};

RGResources *GetResources();
