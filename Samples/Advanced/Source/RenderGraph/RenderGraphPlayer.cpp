#include "RenderGraphPlayer.h"
#include <stdio.h>

RenderGraphPlayer::RenderGraphPlayer()
{
    m_pCurRDG = nullptr;
    m_pGfxDevice = nullptr;
}

bool RenderGraphPlayer::RenderLoop()
{
    if (m_pCurRDG == nullptr)
        return true;
    
    if (m_pGfxDevice == nullptr)
        m_pGfxDevice = new GfxDevice();

    if (!m_pGfxDevice->IsInitialized())
        return true;

    return m_pCurRDG->Execute();
}