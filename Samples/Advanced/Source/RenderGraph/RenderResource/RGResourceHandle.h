#pragma once

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
        m_unDescIdx = 0;
    }

    Type m_eType;
    std::string m_szName;
    uint32_t m_unDescIdx;
};
