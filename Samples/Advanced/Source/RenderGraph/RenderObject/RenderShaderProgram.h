#pragma once
#include <stdio.h>
#include <string>
#include "webgpu/webgpu_cpp.h"
#include "../RenderPass/RenderPassIdx.h"

class RenderShaderProgram
{
public:
    enum class Type : uint8_t
    {
        Invalid,
        Vertex,
        Fragment,
        Compute,
    };
    RenderShaderProgram(Type eType, std::string szEntryPoint, std::string szFilePath);
    virtual ~RenderShaderProgram();
    wgpu::ShaderModule Get();

private:
    std::string m_szEntryPoint;
    std::string m_szFilePath;
    wgpu::ShaderModule m_ShaderModule;
    RenderShaderProgram::Type m_eType;
};