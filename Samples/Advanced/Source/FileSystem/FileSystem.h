#pragma once
#include <stdio.h>
#include <string>

class FileSystem
{
public:
    FileSystem();
    virtual ~FileSystem();
    const char *ReadFile(std::string szFilePath);
    void SetRootDirectory(std::string szRootDirPath);

private:
    std::string m_szRootDirPath;
};

FileSystem *GetFileSystem();
