#include "RenderGraphPlayer.h"
#include <stdio.h>

RenderGraphPlayer::RenderGraphPlayer()
{
    m_pCurRDG = nullptr;
    m_pGfxDevice = nullptr;
    m_bInitialized = false;
}

void RenderGraphPlayer::Initialize()
{
    printf("RenderGraphPlayer::Initialize\n");
    m_pGfxDevice = new GfxDevice();
}

bool RenderGraphPlayer::RenderLoop()
{
    if (!m_bInitialized)
    {
        Initialize();
        m_bInitialized = true;
    }

    if (!m_pGfxDevice->IsInitialized())
        return true;

    if (m_pCurRDG != nullptr)
        return m_pCurRDG->Execute();

    return true;
}