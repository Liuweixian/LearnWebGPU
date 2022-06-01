#include "Resources.h"
#include <assert.h>

Resources::Resources()
{
    m_szRootDirPath = "";
}

Resources::~Resources()
{
}

void Resources::SetRootDirectory(std::string szRootDirPath)
{
    m_szRootDirPath = szRootDirPath;
}

const char *Resources::Load(std::string szFilePath)
{
    std::string szFullPath = m_szRootDirPath + "/Resources/" + szFilePath;
    FILE *pFileHandle = fopen(szFullPath.c_str(), "r");
    if (pFileHandle == nullptr)
    {
        printf("File %s not exist in Resources Folder\n", szFullPath.c_str());
        return nullptr;
    }
    fseek(pFileHandle, 0, SEEK_END);
    long lFileSize = ftell(pFileHandle);
    rewind(pFileHandle);
    char *pRetBuffer = (char *)malloc(sizeof(char) * (lFileSize + 1));
    size_t ulReadSize = fread(pRetBuffer, 1, lFileSize, pFileHandle);
    fclose(pFileHandle);
    pRetBuffer[lFileSize] = '\0';
    assert(ulReadSize == lFileSize);
    return pRetBuffer;
}

static Resources *g_pResources = nullptr;
Resources *GetResources()
{
    if (g_pResources == nullptr)
        g_pResources = new Resources();
    return g_pResources;
}
