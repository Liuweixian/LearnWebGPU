#include <stdio.h>
#include "RenderGraphPass.h"

RenderGraphPass::RenderGraphPass(RGPassIdx uPassIdx)
{
    m_pShader = nullptr;
    m_ePassType = RGPassType::Invalid;
    m_uPassIdx = uPassIdx;
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
