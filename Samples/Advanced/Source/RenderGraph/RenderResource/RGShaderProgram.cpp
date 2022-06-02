#include "RGShaderProgram.h"
#include "../../FileSystem/Resources.h"

RGShaderProgram::RGShaderProgram(Type eType, std::string szEntryPoint, std::string szFilePath)
{
    m_eType = eType;
    m_szEntryPoint = szEntryPoint;
    m_szFilePath = szFilePath;
}

RGShaderProgram::~RGShaderProgram()
{
}

const char *RGShaderProgram::LoadSource()
{
    return GetResources()->Load(m_szFilePath);
}
