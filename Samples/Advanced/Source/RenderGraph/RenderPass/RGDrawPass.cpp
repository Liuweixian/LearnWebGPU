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

bool RGDrawPass::Setup()
{
    bool bRet = RGPass::Setup();
    if (!bRet)
        return bRet;

    if (m_TargetColorBuffers.size() == 0)
    {
        printf("DrawPass dont have render target!!\n");
        return false;
    }

    return true;
}

void RGDrawPass::SetRenderTarget(RGTextureResHandle *pTargetColorBuffer)
{
    m_TargetColorBuffers.clear();
    m_TargetColorBuffers.push_back(pTargetColorBuffer);
}

void RGDrawPass::Compile()
{
    assert(m_pDefaultRenderState == nullptr);
    m_pDefaultRenderState = new RGRenderState();
    m_pDefaultRenderState->Initialize((RGDrawShader *)m_pShader, m_TargetColorBuffers, m_pTargetDepthBuffer, &m_BindEntryGroups);
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
        uint32_t uTotalBindings = (uint32_t)m_BindEntryGroups.size();
        for (uint32_t uGroupIndex = 0; uGroupIndex < uTotalBindings; uGroupIndex++)
        {
            RGBindEntryGroup *pBindEntryGroup = &(m_BindEntryGroups[uGroupIndex]);
            pBindEntryGroup->UploadBindEntryIfDirty();
            pGfxDevice->SetBindGroup(uGroupIndex, pBindEntryGroup);
        }
        pGfxDevice->DrawBuffer(pMesh->GetVertexBuffers(), pMesh->GetIndexBuffer());
    }
}

void RGDrawPass::Bind(wgpu::ShaderStage eShaderStage, uint32_t uGroupIndex, uint32_t uBindIndex, RGResourceHandle *pResourceHandle)
{
    uint32_t uTotalCount = uGroupIndex + 1;
    if (uTotalCount > m_BindEntryGroups.size())
        m_BindEntryGroups.resize(uTotalCount);
    RGBindEntryGroup *pBindEntryGroup = &(m_BindEntryGroups[uGroupIndex]);
    pBindEntryGroup->SetBindEntry(eShaderStage, uBindIndex, pResourceHandle);
}
