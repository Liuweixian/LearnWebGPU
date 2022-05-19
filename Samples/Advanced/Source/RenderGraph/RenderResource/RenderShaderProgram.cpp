#include "RenderShaderProgram.h"
#include "../../FileSystem/Resources.h"

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
    return GetResources()->Load(m_szFilePath);
}