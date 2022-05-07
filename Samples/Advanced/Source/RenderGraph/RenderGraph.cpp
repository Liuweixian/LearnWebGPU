#include <stdio.h>
#include "RenderGraph.h"
#include "GfxDevice.h"

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
    switch (m_eStatus)
    {
    case Invalid:
    {
        Initialize();
        m_eStatus = Initialized;
        InitializeComplete();
        return true;
    }
    case Initialized:
    {
        bool bSetupFinish = true;
        for (auto passIt = m_Passes.begin(); passIt != m_Passes.end(); passIt++)
        {
            RenderGraphPass *pPass = *passIt;
            bool bRet = pPass->EnsureSetupFinish();
            if (!bRet)
            {
                bSetupFinish = false;
                continue;
            }
        }

        if (bSetupFinish)
        {
            m_eStatus = Compiled;
        }
        return true;
    }
    case Compiled:
    {
        GfxDevice* pGfxDevice = GetGfxDevice();
        pGfxDevice->BeginCommandEncode();
        for (auto passIt = m_Passes.begin(); passIt != m_Passes.end(); passIt++)
        {
            RenderGraphPass *pPass = *passIt;
            pGfxDevice->BeginPassEncode();
            for (auto objIt = m_RenderObjects.begin(); objIt != m_RenderObjects.end(); objIt++)
            {
                RenderObject *pObj = *objIt;
                RenderMaterial *pMaterial = pObj->GetMaterial(pPass->GetIdx());
                if (pMaterial == nullptr)
                    continue;
                printf("RenderObject -> %s\n", pObj->GetName().c_str());
            }
            pGfxDevice->EndPassEncode();
        }
        pGfxDevice->EndCommandEncode();
        return true;
    }
    }

    return true;
}