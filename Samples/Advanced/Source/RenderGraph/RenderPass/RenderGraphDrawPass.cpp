#include "RenderGraphDrawPass.h"
#include "../GfxDevice.h"
#include <cassert>

RenderGraphDrawPass::RenderGraphDrawPass()
{
    m_eIdx = RenderPassIdx::Invalid;
    m_eType = GraphPassType::Draw;
    m_TargetColorBuffers.clear();
    m_pTargetDepthBuffer = nullptr;
    m_pRenderState = nullptr;
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

void RenderGraphDrawPass::Compile()
{
    assert(m_pRenderState == nullptr);
    m_pRenderState = new RenderState();
    m_pRenderState->Initialize((RenderObjectShader *)m_pShader, m_TargetColorBuffers, m_pTargetDepthBuffer);
}

void RenderGraphDrawPass::Execute(const std::list<RenderObject *> renderObjects)
{
    GfxDevice *pGfxDevice = GetGfxDevice();
    pGfxDevice->SetRenderTarget(m_TargetColorBuffers, m_pTargetDepthBuffer);
    pGfxDevice->SetRenderState(m_pRenderState);
    for (auto objIt = renderObjects.begin(); objIt != renderObjects.end(); objIt++)
    {
        RenderObject *pObj = *objIt;
        RenderMaterial *pMaterial = pObj->GetMaterial(m_eIdx);
        if (pMaterial == nullptr)
            continue;
        RenderMesh *pMesh = pObj->GetMesh();
        if (pMesh == nullptr)
            continue;
        pGfxDevice->DrawBuffer(pMesh->GetVertexBuffers(), pMesh->GetIndexBuffer());
    }
}
