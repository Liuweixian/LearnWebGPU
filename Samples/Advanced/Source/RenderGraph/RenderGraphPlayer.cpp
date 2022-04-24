#include "RenderGraphPlayer.h"
#include <stdio.h>

RenderGraphPlayer::RenderGraphPlayer()
{
    m_pCurRDG = nullptr;
    m_bInitialized = false;
}

void RenderGraphPlayer::Initialize()
{
    printf("RenderGraphPlayer Initialize Complete\n");
}

bool RenderGraphPlayer::RenderLoop()
{
    if (!m_bInitialized)
    {
        Initialize();
        m_bInitialized = true;
    }

    if (m_pCurRDG != nullptr)
        return m_pCurRDG->Execute();

    return true;
}