#include "RGBindEntryGroup.h"
#include "../GfxDevice.h"

RGBindEntryGroup::RGBindEntryGroup()
{
    m_bindGroupLayout = nullptr;
    m_bindGroup = nullptr;
}

RGBindEntryGroup::~RGBindEntryGroup()
{
    
}

void RGBindEntryGroup::SetBindEntry(wgpu::ShaderStage eShaderStage, uint32_t uBindIndex, RGResourceHandle *pResourceHandle)
{
    uint32_t totalCount = uBindIndex + 1;
    if (totalCount > m_bindEntries.size())
        m_bindEntries.resize(totalCount);
    RGBindEntry *pBindEntry = &(m_bindEntries[uBindIndex]);
    pBindEntry->m_eShaderStage = eShaderStage;
    pBindEntry->m_uBindIndex = uBindIndex;
    pBindEntry->m_pResouceHandle = pResourceHandle;
}

wgpu::BindGroupLayout RGBindEntryGroup::GetBindGroupLayout()
{
    if (m_bindGroupLayout != nullptr)
        return m_bindGroupLayout;
    
    uint32_t uEntryCount = (uint32_t)m_bindEntries.size();
    wgpu::BindGroupLayoutEntry layoutEntries[uEntryCount];
    for (int i = 0; i < uEntryCount; i++)
    {
        RGBindEntry *pBindEntry = &(m_bindEntries[i]);
        layoutEntries[i].binding = pBindEntry->m_uBindIndex;
        layoutEntries[i].visibility = pBindEntry->m_eShaderStage;
        if (pBindEntry->m_pResouceHandle->m_eType == RGResourceHandle::Type::Buffer)
        {
            wgpu::BufferBindingLayout bufferBindingLayout;
            bufferBindingLayout.type = wgpu::BufferBindingType::Uniform;
            layoutEntries[i].buffer = bufferBindingLayout;
        }
    }
    m_bindGroupLayout = GetGfxDevice()->CreateBindGroupLayout(uEntryCount, layoutEntries);
    return m_bindGroupLayout;
}

wgpu::BindGroup RGBindEntryGroup::GetBindGroup()
{
    if (m_bindGroup != nullptr)
        return m_bindGroup;
    
    uint32_t uEntryCount = (uint32_t)m_bindEntries.size();
    wgpu::BindGroupEntry bindGroupEntry[uEntryCount];
    for (int i = 0; i < uEntryCount; i++)
    {
        RGBindEntry *pBindEntry = &(m_bindEntries[i]);
        bindGroupEntry[i].binding = pBindEntry->m_uBindIndex;
        bindGroupEntry[i].offset = 0;
        if (pBindEntry->m_pResouceHandle->m_eType == RGResourceHandle::Type::Buffer)
        {
            RGBufferResHandle *pBufferHandle = reinterpret_cast<RGBufferResHandle*>(pBindEntry->m_pResouceHandle);
            bindGroupEntry[i].buffer = pBufferHandle->GetGPUBuffer();
            bindGroupEntry[i].size = pBufferHandle->GetDataLength();
        }
    }
    
    return GetGfxDevice()->CreateBindGroup(m_bindGroupLayout, uEntryCount, bindGroupEntry);
}

void RGBindEntryGroup::UploadBindEntryIfDirty()
{
    uint32_t uEntryCount = (uint32_t)m_bindEntries.size();
    wgpu::BindGroupEntry bindGroupEntry[uEntryCount];
    for (int i = 0; i < uEntryCount; i++)
    {
        RGBindEntry *pBindEntry = &(m_bindEntries[i]);
        if (pBindEntry->m_pResouceHandle->m_eType == RGResourceHandle::Type::Buffer)
        {
            RGBufferResHandle *pBufferHandle = reinterpret_cast<RGBufferResHandle*>(pBindEntry->m_pResouceHandle);
            GetGfxDevice()->EnsurceBufferUploaded(pBufferHandle);
        }
    }
}
