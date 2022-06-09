#include <stdio.h>
#include "RenderGraph.h"
#include "GfxDevice.h"
#include "RenderPass/RGDrawPass.h"
#include "RenderPass/RGComputePass.h"

RenderGraph::RenderGraph()
{
    m_eStatus = Invalid;
    m_Passes.empty();
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
        RGPass *pPass = *passIt;
        pPass->Compile();
    }
}

bool RenderGraph::Execute(const std::list<RGObject *> renderObjects)
{
    switch (m_eStatus)
    {
    case Invalid:
    {
        Initialize();
        m_eStatus = Initialized;
        return true;
    }
    case Initialized:
    {
        bool bSetupFinish = true;
        for (auto passIt = m_Passes.begin(); passIt != m_Passes.end(); passIt++)
        {
            RGPass *pPass = *passIt;
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
            RGPass *pPass = *passIt;
            pPass->Update();
            RGPassType ePassType = pPass->GetPassType();
            if (ePassType == RGPassType::Draw)
            {
                RGDrawPass *pDrawPass = reinterpret_cast<RGDrawPass *>(pPass);
                pDrawPass->Execute(renderObjects);
            }
            else if (ePassType == RGPassType::Compute)
            {
                RGComputePass *pComputePass = reinterpret_cast<RGComputePass *>(pPass);
                pComputePass->Execute();
            }
        }
        pGfxDevice->EndFrame();
        return true;
    }
    }

    return true;
}
