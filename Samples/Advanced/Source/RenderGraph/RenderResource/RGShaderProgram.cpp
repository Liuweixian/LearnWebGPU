#include "RGShaderProgram.h"
#include "../../FileSystem/FileSystem.h"

RGShaderProgram::RGShaderProgram(Type eType, std::string szEntryPoint, std::string szFilePath)
{
    m_eType = eType;
    m_szEntryPoint = szEntryPoint;
    m_szFilePath = szFilePath;
    m_pSource = nullptr;
}

RGShaderProgram::~RGShaderProgram()
{
    UnloadSource();
}

const char *RGShaderProgram::LoadSource()
{
    UnloadSource();
    m_pSource = GetFileSystem()->ReadFile(m_szFilePath);
    return m_pSource;
}

void RGShaderProgram::UnloadSource()
{
    if (m_pSource != nullptr)
    {
        delete m_pSource;
        m_pSource = nullptr;
    }
}
