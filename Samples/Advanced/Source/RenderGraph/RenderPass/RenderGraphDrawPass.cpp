#include "RenderGraphDrawPass.h"
RenderGraphDrawPass::RenderGraphDrawPass()
{
    m_eIdx = RenderPassIdx::Invalid;
    m_eType = GraphPassType::Draw;
    m_TargetColorBuffers.clear();
    m_TargetDepthBuffer = nullptr;
}

RenderGraphDrawPass::~RenderGraphDrawPass()
{
}

bool RenderGraphDrawPass::EnsureSetupFinish()
{
    bool bRet = RenderGraphPass::EnsureSetupFinish();
    if (!bRet)
        return bRet;

    if (m_TargetColorBuffers.size() == 0)
    {
        printf("Render Pass dont have render target!!\n");
        return false;
    }

    return true;
}

void RenderGraphDrawPass::SetRenderTarget(RenderResourceHandle *pTargetColorBuffer)
{
    m_TargetColorBuffers.clear();
    m_TargetColorBuffers.push_back(pTargetColorBuffer);
}
