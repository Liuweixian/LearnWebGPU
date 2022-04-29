#pragma once
#include "webgpu/webgpu_cpp.h"

class RenderShader
{
public:
    RenderShader(char* entryPoint, char* filePath);
    virtual ~RenderShader();
    wgpu::ShaderModule Get();
private:
    char* m_chEntryPoint;
    char* m_chFilePath;
    wgpu::ShaderModule m_ShaderModule;
};