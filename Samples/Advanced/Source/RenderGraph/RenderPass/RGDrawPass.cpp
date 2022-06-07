#include "RGDrawPass.h"
#include "../GfxDevice.h"
#include <cassert>

RGDrawPass::RGDrawPass(RGPassIdx uPassIdx) : RGPass(uPassIdx)
{
    m_ePassType = RGPassType::Draw;
    m_TargetColorBuffers.clear();
    m_pTargetDepthBuffer = nullptr;
    m_pDefaultRenderState = nullptr;
}

RGDrawPass::~RGDrawPass()
{
}

bool RGDrawPass::EnsureSetupFinish()
{
    bool bRet = RGPass::EnsureSetupFinish();
    if (!bRet)
        return bRet;

    if (m_TargetColorBuffers.size() == 0)
    {
        printf("Render Pass dont have render target!!\n");
        return false;
    }

    return true;
}

void RGDrawPass::SetRenderTarget(RGResourceHandle *pTargetColorBuffer)
{
    m_TargetColorBuffers.clear();
    m_TargetColorBuffers.push_back(pTargetColorBuffer);
}

void RGDrawPass::Compile()
{
    assert(m_pDefaultRenderState == nullptr);
    m_pDefaultRenderState = new RGRenderState();
    m_pDefaultRenderState->Initialize((RGDrawShader *)m_pShader, m_TargetColorBuffers, m_pTargetDepthBuffer);
}

void RGDrawPass::Execute(const std::list<RGObject *> renderObjects)
{
    GfxDevice *pGfxDevice = GetGfxDevice();
    pGfxDevice->SetRenderTarget(m_TargetColorBuffers, m_pTargetDepthBuffer);
    for (auto objIt = renderObjects.begin(); objIt != renderObjects.end(); objIt++)
    {
        RGObject *pObj = *objIt;
        RGMaterial *pMaterial = pObj->GetMaterial(m_uPassIdx);
        if (pMaterial == nullptr)
            continue;
        RGMesh *pMesh = pObj->GetMesh();
        if (pMesh == nullptr)
            continue;
        m_pDefaultRenderState->UpdateRenderState(pMaterial);
        pGfxDevice->SetRenderPipeline(m_pDefaultRenderState);
        pGfxDevice->DrawBuffer(pMesh->GetVertexBuffers(), pMesh->GetIndexBuffer());
    }
}
