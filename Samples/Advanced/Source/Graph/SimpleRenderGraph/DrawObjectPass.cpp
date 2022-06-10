#include "DrawObjectPass.h"
#include "../../RenderGraph/RenderResource/RGDrawShader.h"
#include "../../RenderGraph/RenderResource/RGVertexLayout.h"

DrawObjectPass::DrawObjectPass(RGPassIdx uPassIdx) : RGDrawPass(uPassIdx)
{
    m_fRotation = 0;
    m_pRotationBufferHandle = GetResources()->CreateBuffer("RotationUniform", wgpu::BufferUsage::Uniform);
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
    m_fRotation += 0.1f;
    m_pRotationBufferHandle->SetData<float>(1, &m_fRotation);
}
