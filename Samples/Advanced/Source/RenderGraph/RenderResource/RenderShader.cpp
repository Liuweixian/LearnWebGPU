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

