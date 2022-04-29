#pragma once
#include <string>
#include "RenderShader.h"

class RenderObject
{
public:
    RenderObject(std::string szName);
    virtual ~RenderObject();
    std::string GetName()
    {
        return m_szName;
    }
private:
    std::string m_szName;
};