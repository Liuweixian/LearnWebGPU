#pragma once
#include "RGResourceHandle.h"
#include <webgpu/webgpu_cpp.h>

class RGBufferResHandle : public RGResourceHandle
{
public:
    RGBufferResHandle(wgpu::BufferUsage eBufferUsage);
    template <typename T>
    void SetData(size_t ulDataCount, T *pData);
    void SetShaderStage(wgpu::ShaderStage eShaderStage);
    
    wgpu::Buffer &GetGPUBuffer()
    {
        return m_gpuBuffer;
    }
    
    int8_t *GetData()
    {
        return m_pData;
    }
    
    size_t GetDataCount()
    {
        return m_ulDataCount;
    }
    
    size_t GetDataLength()
    {
        return m_ulDataLength;
    }
    
    wgpu::BufferUsage GetBufferUsage()
    {
        return m_eBufferUsage;
    }
    
    bool IsDirty()
    {
        return m_bIsDirty;
    }
    
    void SetNotDirty()
    {
        m_bIsDirty = false;
    }

private:
    size_t m_ulDataCount;
    size_t m_ulDataLength;
    int8_t *m_pData;
    bool m_bIsDirty;
    wgpu::ShaderStage m_eShaderStage;
    wgpu::BufferUsage m_eBufferUsage;
    wgpu::Buffer m_gpuBuffer;
};
