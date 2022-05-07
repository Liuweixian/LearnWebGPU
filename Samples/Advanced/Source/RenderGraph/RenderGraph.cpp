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

void RenderGraph::Compile()
{

}

bool RenderGraph::Execute(const std::list<RenderObject *> renderObjects)
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
            Compile();
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
            for (auto objIt = renderObjects.begin(); objIt != renderObjects.end(); objIt++)
            {
                RenderObject *pObj = *objIt;
                //RenderMaterial *pMaterial = pObj->GetMaterial(pPass->GetIdx());
                //if (pMaterial == nullptr)
                    //continue;
            }
        }
        pGfxDevice->EndCommandEncode();
        return true;
    }
    }

    return true;
}