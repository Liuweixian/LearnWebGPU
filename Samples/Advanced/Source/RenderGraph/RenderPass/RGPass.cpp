#include <stdio.h>
#include "RGPass.h"

RGPass::RGPass(RGPassIdx uPassIdx)
{
    m_pShader = nullptr;
    m_ePassType = RGPassType::Invalid;
    m_uPassIdx = uPassIdx;
}

RGPass::~RGPass()
{

}

bool RGPass::EnsureSetupFinish()
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
