#include "RenderGraphPass.h"
#include <stdio.h>

RenderGraphPass::RenderGraphPass()
{
    m_eType = RenderPassType::Invalid;
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