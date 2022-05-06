#include "DrawObjectPass.h"

DrawObjectPass::DrawObjectPass()
{
    m_eIdx = RenderPassIdx::DrawObjectPass;
}

DrawObjectPass::~DrawObjectPass()
{

}

void DrawObjectPass::Setup()
{
    m_pShader = new RenderShader();
    m_pShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Vertex, "vs_main", "Shaders/DrawObjectPassVS.wgsl");
    m_pShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Fragment, "fs_main", "Shaders/DrawObjectPassFS.wgsl");
    printf("DrawObjectPass::Setup\n");
}

void DrawObjectPass::SetRenderTarget()
{
    
}