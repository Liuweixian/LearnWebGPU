#include "RGShader.h"

RGShader::RGShader()
{
    m_Programs.empty();
}

RGShader::~RGShader()
{
    for (auto it = m_Programs.begin(); it != m_Programs.end(); it++)
    {
        delete it->second;
    }
    m_Programs.clear();
}

RGShaderProgram *RGShader::GetProgram(RGShaderProgram::Type eType)
{
    auto found = m_Programs.find(eType);
    if (found != m_Programs.end())
        return found->second;
    return nullptr;
}
