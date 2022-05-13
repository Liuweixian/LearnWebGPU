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
    for (auto passIt = m_Passes.begin(); passIt != m_Passes.end(); passIt++)
    {
        RenderGraphPass *pPass = *passIt;
        pPass->Compile();
    }
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
        GfxDevice *pGfxDevice = GetGfxDevice();
        pGfxDevice->BeginFrame();
        for (auto passIt = m_Passes.begin(); passIt != m_Passes.end(); passIt++)
        {
            RenderGraphPass *pPass = *passIt;
            GraphPassType eType = pPass->GetPassType();
            if (eType == GraphPassType::Draw)
            {
                RenderGraphDrawPass *pDrawPass = reinterpret_cast<RenderGraphDrawPass *>(pPass);
                pDrawPass->Execute(renderObjects);
            }
            else if (eType == GraphPassType::Compute)
            {
                RenderGraphComputePass *pComputePass = reinterpret_cast<RenderGraphComputePass *>(pPass);
                pComputePass->Execute();
            }
        }
        pGfxDevice->EndFrame();
        return true;
    }
    }

    return true;
}