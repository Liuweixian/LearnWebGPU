#include "RenderGraphPass.h"
#include <cassert>

RenderGraphPass::RenderGraphPass()
{
    m_eIdx = RenderPassIdx::Invalid;
}

RenderGraphPass::~RenderGraphPass()
{

}

bool RenderGraphPass::EnsureSetupFinish()
{
    if (m_pShader != nullptr)
        return true;
    Setup();
    assert(m_eIdx != RenderPassIdx::Invalid);
    assert(m_pShader != nullptr);
    return false;
}