#include "RenderGraph.h"
#include <stdio.h>

RenderGraph::RenderGraph()
{
    m_eStatus = Invalid;
}

RenderGraph::~RenderGraph()
{

}

void RenderGraph::Initialize()
{

}

bool RenderGraph::Execute()
{
    if (m_eStatus == Invalid)
    {
        Initialize();
        m_eStatus = Initialized;
        InitializeComplete();
        return true;
    }

    for (auto passIt = m_Passes.begin(); passIt != m_Passes.end(); passIt++)
    {
        RenderGraphPass* pPass = *passIt;
        bool bRet = pPass->EnsureSetupFinish();
        if (!bRet)
            continue;

        pPass->SetRenderTarget();
        for (auto objIt = m_RenderObjects.begin(); objIt != m_RenderObjects.end(); objIt++)
        {
            RenderObject* pObj = *objIt;
            RenderMaterial* pMaterial = pObj->GetMaterial(pPass->GetIdx());
            if (pMaterial == nullptr)
                continue;
            printf("RenderObject -> %s\n", pObj->GetName().c_str());
        }
    }
    return true;
}