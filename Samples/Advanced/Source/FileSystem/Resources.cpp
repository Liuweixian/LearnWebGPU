#include "Resources.h"
#include <assert.h>

Resources::Resources()
{
}

Resources::~Resources()
{
}

const char *Resources::Load(std::string szFilePath)
{
    std::string szFullPath = "Resources/" + szFilePath;
    FILE *pFileHandle = fopen(szFullPath.c_str(), "r");
    if (pFileHandle == nullptr)
    {
        printf("File %s not exist in Resources Folder\n", szFullPath.c_str());
        return nullptr;
    }
    fseek(pFileHandle, 0, SEEK_END);
    long lFileSize = ftell(pFileHandle);
    rewind(pFileHandle);
    char *pRetBuffer = (char *)malloc(sizeof(char) * lFileSize);
    size_t ulReadSize = fread(pRetBuffer, 1, lFileSize, pFileHandle);
    fclose(pFileHandle);
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