#include "DrawCubePass.h"
#include "../../RenderGraph/RenderResource/RGDrawShader.h"
#include "../../RenderGraph/RenderResource/RGVertexLayout.h"
#include "../../Math/mat4.h"
typedef filament::math::details::TMat44<float> Matrix;

DrawCubePass::DrawCubePass(RGPassIdx uPassIdx) : RGDrawPass(uPassIdx)
{
    m_pMVPMatrixBufferHandle = GetResources()->CreateBuffer("MVPMatrix", wgpu::BufferUsage::Uniform);
    Bind(wgpu::ShaderStage::Vertex, 0, 0, m_pMVPMatrixBufferHandle);
    Matrix cameraProjectionMatrix = Matrix::perspective(60.0f, 960.0f / 720.0f, 0.3f, 1000.0f);
}

DrawCubePass::~DrawCubePass()
{
}

RGShader *DrawCubePass::SetupShader()
{
    RGDrawShader *pDrawShader = new RGDrawShader(2, wgpu::VertexFormat::Float32x2, wgpu::VertexFormat::Float32x3);
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Vertex, "vs_main", "DrawTriangleRenderGraph/Shaders/DrawTrianglePassVS.wgsl");
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Fragment, "fs_main", "DrawTriangleRenderGraph/Shaders/DrawTrianglePassFS.wgsl");
    return pDrawShader;
}

void DrawCubePass::Update()
{
}
