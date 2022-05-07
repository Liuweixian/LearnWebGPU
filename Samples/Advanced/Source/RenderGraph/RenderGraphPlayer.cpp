#include "RenderGraphPlayer.h"

RenderGraphPlayer::RenderGraphPlayer()
{
    m_pCurRDG = nullptr;
}

bool RenderGraphPlayer::RenderLoop()
{
    if (m_pCurRDG == nullptr)
        return true;
    
    if (!GetGfxDevice()->IsInitialized())
        return true;

    return m_pCurRDG->Execute();
}