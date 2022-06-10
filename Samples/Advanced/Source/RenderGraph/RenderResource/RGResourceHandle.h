#pragma once
#include <string>

class RGResourceHandle
{
public:
    enum Type
    {
        Invalid,
        Texture,
        Buffer
    };

    RGResourceHandle()
    {
        m_eType = Type::Invalid;
        m_szName = "";
    }

    Type m_eType;
    std::string m_szName;
};
