#include "DrawTrianglePass.h"
#include "../../RenderGraph/RenderResource/RGDrawShader.h"
#include "../../RenderGraph/RenderResource/RGVertexLayout.h"

DrawTrianglePass::DrawTrianglePass(RGPassIdx uPassIdx) : RGDrawPass(uPassIdx)
{
    m_fRotation = 0;
    m_pRotationBufferHandle = GetResources()->CreateBuffer("RotationUniform", wgpu::BufferUsage::Uniform);
    Bind(wgpu::ShaderStage::Vertex, 0, 0, m_pRotationBufferHandle);
}

DrawTrianglePass::~DrawTrianglePass()
{
}

RGShader *DrawTrianglePass::SetupShader()
{
    RGDrawShader *pDrawShader = new RGDrawShader(2, wgpu::VertexFormat::Float32x2, wgpu::VertexFormat::Float32x3);
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Vertex, "vs_main", "DrawTriangleRenderGraph/Shaders/DrawTrianglePassVS.wgsl");
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Fragment, "fs_main", "DrawTriangleRenderGraph/Shaders/DrawTrianglePassFS.wgsl");
    return pDrawShader;
}

void DrawTrianglePass::Update()
{
    m_fRotation += 0.1f;
    m_pRotationBufferHandle->SetData<float>(1, &m_fRotation);
}
