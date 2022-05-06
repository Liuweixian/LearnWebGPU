#pragma once
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "webgpu/webgpu_cpp.h"
#include "../RenderPass/RenderPassIdx.h"
#include "RenderShaderProgram.h"

class RenderShader
{
public:
    RenderShader();
    virtual ~RenderShader();

    template <typename T>
    T *Create(RenderShaderProgram::Type eType, std::string szEntryPoint, std::string szFilePath)
    {
        static_assert(std::is_base_of<RenderShaderProgram, T>::value, "T must be a descendant of RenderShaderProgram");
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
private:
    std::unordered_map<RenderShaderProgram::Type, RenderShaderProgram*> m_Programs;
};