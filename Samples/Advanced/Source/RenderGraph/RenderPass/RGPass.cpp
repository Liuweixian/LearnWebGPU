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

bool RGPass::Setup()
{
    m_pShader = SetupShader();
    if (m_pShader == nullptr)
    {
        printf("Pass dont have shader!!\n");
        return false;
    }
    return true;
}
