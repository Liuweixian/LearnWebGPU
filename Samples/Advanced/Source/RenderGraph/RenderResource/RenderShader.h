#pragma once
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "RGShaderProgram.h"

class RenderShader
{
public:
    RenderShader();
    virtual ~RenderShader();

    template <typename T>
    T *Create(RGShaderProgram::Type eType, std::string szEntryPoint, std::string szFilePath)
    {
        static_assert(std::is_base_of<RGShaderProgram, T>::value, "T must be a descendant of RenderShaderProgram");
        auto found = m_Programs.find(eType);
        if (found != m_Programs.end())
        {
            printf("This shader already had program type %hhu\n", eType);
            return found->second;
        }

        T *pProgram = new T(eType, szEntryPoint, szFilePath);
        m_Programs.insert(std::make_pair(eType, pProgram));
        return pProgram;
    }

    RGShaderProgram *GetProgram(RGShaderProgram::Type eType);

protected:
    std::unordered_map<RGShaderProgram::Type, RGShaderProgram *> m_Programs;
};
