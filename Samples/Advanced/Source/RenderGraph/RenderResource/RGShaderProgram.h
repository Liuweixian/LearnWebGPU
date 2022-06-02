#pragma once
#include <stdio.h>
#include <string>

class RGShaderProgram
{
public:
    enum class Type : uint8_t
    {
        Invalid,
        Vertex,
        Fragment,
        Compute,
    };
    RGShaderProgram(Type eType, std::string szEntryPoint, std::string szFilePath);
    virtual ~RGShaderProgram();
    const char *LoadSource();
    const char *EntryPoint()
    {
        return m_szEntryPoint.c_str();
    }

private:
    std::string m_szEntryPoint;
    std::string m_szFilePath;
    RGShaderProgram::Type m_eType;
};
