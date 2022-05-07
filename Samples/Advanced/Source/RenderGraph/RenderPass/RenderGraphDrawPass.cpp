#include "RenderGraphDrawPass.h"
RenderGraphDrawPass::RenderGraphDrawPass()
{
    m_eIdx = RenderPassIdx::Invalid;
    m_eType = RenderPassType::Draw;
}

RenderGraphDrawPass::~RenderGraphDrawPass()
{

}
