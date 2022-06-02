#include "RGDrawPass.h"
#include "../GfxDevice.h"
#include <cassert>

RGDrawPass::RGDrawPass(RGPassIdx uPassIdx) : RGPass(uPassIdx)
{
    m_ePassType = RGPassType::Draw;
    m_TargetColorBuffers.clear();
    m_pTargetDepthBuffer = nullptr;
    m_pPipeline = nullptr;
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
    assert(m_pPipeline == nullptr);
    m_pPipeline = new RGPipeline();
    m_pPipeline->Initialize((RGDrawShader *)m_pShader, m_TargetColorBuffers, m_pTargetDepthBuffer);
}

void RGDrawPass::Execute(const std::list<RenderObject *> renderObjects)
{
    GfxDevice *pGfxDevice = GetGfxDevice();
    pGfxDevice->SetRenderTarget(m_TargetColorBuffers, m_pTargetDepthBuffer);
    pGfxDevice->SetRenderState(m_pPipeline);
    for (auto objIt = renderObjects.begin(); objIt != renderObjects.end(); objIt++)
    {
        RenderObject *pObj = *objIt;
        RenderMaterial *pMaterial = pObj->GetMaterial(m_uPassIdx);
        if (pMaterial == nullptr)
            continue;
        RenderMesh *pMesh = pObj->GetMesh();
        if (pMesh == nullptr)
            continue;
        pGfxDevice->DrawBuffer(pMesh->GetVertexBuffers(), pMesh->GetIndexBuffer());
    }
}
