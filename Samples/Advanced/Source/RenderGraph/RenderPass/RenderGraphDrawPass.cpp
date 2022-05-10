#include "RenderGraphDrawPass.h"
#include "../GfxDevice.h"
#include <cassert>

RenderGraphDrawPass::RenderGraphDrawPass()
{
    m_eIdx = RenderPassIdx::Invalid;
    m_eType = GraphPassType::Draw;
    m_TargetColorBuffers.clear();
    m_pTargetDepthBuffer = nullptr;
    m_CompiledTargetColorBuffers.clear();
    m_pCompiledTargetDepthBuffer = nullptr;
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
    m_CompiledTargetColorBuffers.clear();
    m_pCompiledTargetDepthBuffer = nullptr;

    RenderResource* pResources = GetRenderResource();
    for (auto it = m_TargetColorBuffers.begin(); it != m_TargetColorBuffers.end(); it++)
    {
        RenderResourceHandle* pHandle = *it;
        wgpu::TextureDescriptor* pTextureDesc = pResources->GetTextureDesc(pHandle);
        assert(pTextureDesc != nullptr);
        m_CompiledTargetColorBuffers.push_back(pTextureDesc);
    }
    m_TargetColorBuffers.clear();
    if (m_pTargetDepthBuffer != nullptr)
        m_pCompiledTargetDepthBuffer = pResources->GetTextureDesc(m_pTargetDepthBuffer);
}

void RenderGraphDrawPass::Execute(const std::list<RenderObject *> renderObjects)
{
    GfxDevice* pGfxDevice = GetGfxDevice();
    pGfxDevice->BeginPassEncode(m_CompiledTargetColorBuffers, m_pCompiledTargetDepthBuffer);
    for (auto objIt = renderObjects.begin(); objIt != renderObjects.end(); objIt++)
    {
        RenderObject* pObj = *objIt;
        RenderMaterial *pMaterial = pObj->GetMaterial(m_eIdx);
        if (pMaterial == nullptr)
            continue;
    }
    pGfxDevice->EndPassEncode();
}
