#pragma once
#include "webgpu/webgpu_cpp.h"
#include "../RenderResource/RGResourceHandle.h"

class RGBindEntry
{
public:
    RGBindEntry();
    ~RGBindEntry();
    
public:
    wgpu::ShaderStage m_eShaderStage;
    uint32_t m_uBindIndex;
    RGResourceHandle *m_pResouceHandle;
};
