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

    return m_pCurRDG->Execute(m_RenderObjects);
}

static RenderGraphPlayer* g_pRenderGraphPlayer = nullptr;
RenderGraphPlayer* GetRDGPlayer()
{
    if (g_pRenderGraphPlayer == nullptr)
        g_pRenderGraphPlayer = new RenderGraphPlayer();
    return g_pRenderGraphPlayer;
}