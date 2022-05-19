#include "DrawObjectPass.h"
#include "../RenderGraph/RenderResource/RenderObjectShader.h"
#include "../RenderGraph/RenderResource/RenderVertexState.h"

DrawObjectPass::DrawObjectPass()
{
    m_eIdx = RenderPassIdx::DrawObjectPass;
}

DrawObjectPass::~DrawObjectPass()
{

}

void DrawObjectPass::SetupShader()
{
    RenderObjectShader* pDrawShader = new RenderObjectShader();
    pDrawShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Vertex, "vs_main", "SimpleRenderGraph/Shaders/DrawObjectPassVS.wgsl");
    pDrawShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Fragment, "fs_main", "SimpleRenderGraph/Shaders/DrawObjectPassFS.wgsl");
    pDrawShader->CreateVertexState(3, wgpu::VertexFormat::Float32x3, wgpu::VertexFormat::Float32x3, wgpu::VertexFormat::Float32x2);
    m_pShader = pDrawShader;
    printf("DrawObjectPass::SetupShader\n");
}