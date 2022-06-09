#include "FileSystem.h"
#include <assert.h>

FileSystem::FileSystem()
{
    m_szRootDirPath = "";
}

FileSystem::~FileSystem()
{
}

void FileSystem::SetRootDirectory(std::string szRootDirPath)
{
    m_szRootDirPath = szRootDirPath;
}

const char *FileSystem::ReadFile(std::string szFilePath)
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

static FileSystem *g_pFileSystem = nullptr;
FileSystem *GetFileSystem()
{
    if (g_pFileSystem == nullptr)
        g_pFileSystem = new FileSystem();
    return g_pFileSystem;
}
