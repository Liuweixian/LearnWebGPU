#include "DrawObjectPass.h"
#include "../../RenderGraph/RenderResource/RenderObjectShader.h"
#include "../../RenderGraph/RenderResource/RenderVBOLayout.h"

DrawObjectPass::DrawObjectPass(RGPassIdx uPassIdx) : RenderGraphDrawPass(uPassIdx)
{
}

DrawObjectPass::~DrawObjectPass()
{

}

void DrawObjectPass::SetupShader()
{
    RenderObjectShader* pDrawShader = new RenderObjectShader();
    pDrawShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Vertex, "vs_main", "SimpleRenderGraph/Shaders/DrawObjectPassVS.wgsl");
    pDrawShader->Create<RenderShaderProgram>(RenderShaderProgram::Type::Fragment, "fs_main", "SimpleRenderGraph/Shaders/DrawObjectPassFS.wgsl");
    pDrawShader->CreateVBOLayout(2, wgpu::VertexFormat::Float32x2, wgpu::VertexFormat::Float32x3);
    m_pShader = pDrawShader;
    printf("DrawObjectPass::SetupShader\n");
}
