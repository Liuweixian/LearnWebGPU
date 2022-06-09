#include "DrawObjectPass.h"
#include "../../RenderGraph/RenderResource/RGDrawShader.h"
#include "../../RenderGraph/RenderResource/RGVertexLayout.h"

DrawObjectPass::DrawObjectPass(RGPassIdx uPassIdx) : RGDrawPass(uPassIdx)
{
}

DrawObjectPass::~DrawObjectPass()
{
}

void DrawObjectPass::SetupShader()
{
    RGDrawShader *pDrawShader = new RGDrawShader();
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Vertex, "vs_main", "SimpleRenderGraph/Shaders/DrawObjectPassVS.wgsl");
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Fragment, "fs_main", "SimpleRenderGraph/Shaders/DrawObjectPassFS.wgsl");
    pDrawShader->CreateVertexLayout(2, wgpu::VertexFormat::Float32x2, wgpu::VertexFormat::Float32x3);
    m_pShader = pDrawShader;
    printf("DrawObjectPass::SetupShader\n");
}

void DrawObjectPass::Update()
{
    
}
