#pragma once
#include <stdio.h>
#include <string>

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
    const char *LoadSource();
    const char *EntryPoint()
    {
        return m_szEntryPoint.c_str();
    }

private:
    std::string m_szEntryPoint;
    std::string m_szFilePath;
    RenderShaderProgram::Type m_eType;
};