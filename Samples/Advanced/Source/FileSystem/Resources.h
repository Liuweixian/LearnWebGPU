#pragma once
#include <stdio.h>
#include <string>

class Resources
{
public:
    Resources();
    virtual ~Resources();
    const char* Load(std::string szFilePath);
};

Resources *GetResources();