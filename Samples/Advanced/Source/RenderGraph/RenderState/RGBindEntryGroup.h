#pragma once
#include <vector>
#include "webgpu/webgpu_cpp.h"
#include "RGBindEntry.h"
#include "../RenderResource/RGResourceHandle.h"

class RGBindEntryGroup
{
public:
    RGBindEntryGroup();
    ~RGBindEntryGroup();
    void SetBindEntry(wgpu::ShaderStage eShaderStage, uint32_t uBindIndex, RGResourceHandle *pResourceHandle);
    void UploadBindEntryIfDirty();
    wgpu::BindGroupLayout GetBindGroupLayout();
    wgpu::BindGroup GetBindGroup();
    
private:
    std::vector<RGBindEntry> m_bindEntries;
    wgpu::BindGroupLayout m_bindGroupLayout;
    wgpu::BindGroup m_bindGroup;
};
