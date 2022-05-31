#pragma once
#include <stdio.h>
#include <string>

class Resources
{
public:
    Resources();
    virtual ~Resources();
    const char* Load(std::string szFilePath);
    void SetRootDirectory(std::string szRootDirPath);
private:
    std::string m_szRootDirPath;
};

Resources *GetResources();
