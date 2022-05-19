#include "RenderGraphPass.h"
#include <stdio.h>

RenderGraphPass::RenderGraphPass()
{
    m_eType = GraphPassType::Invalid;
    m_pShader = nullptr;
}

RenderGraphPass::~RenderGraphPass()
{

}

bool RenderGraphPass::EnsureSetupFinish()
{
    if (m_pShader == nullptr)
    {
        SetupShader();
        if (m_pShader == nullptr)
        {
            printf("Shader is null!!\n");
            return false;
        }
    }
    return true;
}