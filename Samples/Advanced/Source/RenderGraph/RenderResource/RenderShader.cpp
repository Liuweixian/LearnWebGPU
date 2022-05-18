#include "RenderShader.h"

RenderShader::RenderShader()
{
}

RenderShader::~RenderShader()
{
    for (auto it = m_Programs.begin(); it != m_Programs.end(); it++)
    {
        delete it->second;
    }
    m_Programs.clear();
}

RenderShaderProgram *RenderShader::GetProgram(RenderShaderProgram::Type eType)
{
    auto found = m_Programs.find(eType);
    if (found != m_Programs.end())
        return found->second;
    return nullptr;
}