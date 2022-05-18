#include "RenderShaderProgram.h"

RenderShaderProgram::RenderShaderProgram(Type eType, std::string szEntryPoint, std::string szFilePath)
{
    m_eType = eType;
    m_szEntryPoint = szEntryPoint;
    m_szFilePath = szFilePath;
}

RenderShaderProgram::~RenderShaderProgram()
{
}

const char *RenderShaderProgram::LoadSource()
{
    return nullptr;
}