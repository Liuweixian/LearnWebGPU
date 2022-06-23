#include "DrawCubePass.h"
#include "../../RenderGraph/RenderResource/RGDrawShader.h"
#include "../../RenderGraph/RenderResource/RGVertexLayout.h"
#include "../../Math/mat4.h"
#include "../../Math/scalar.h"
typedef filament::math::details::TMat44<float> Matrix4x4;
typedef filament::math::details::TQuaternion<float> Quaternion;
typedef filament::math::details::TVec3<float> Vector3;
typedef filament::math::details::TVec4<float> Vector4;

DrawCubePass::DrawCubePass(RGPassIdx uPassIdx) : RGDrawPass(uPassIdx)
{
    m_pMVPMatrixBufferHandle = GetResources()->CreateBuffer("MVPMatrix", wgpu::BufferUsage::Uniform);
    Bind(wgpu::ShaderStage::Vertex, 0, 0, m_pMVPMatrixBufferHandle);
    Matrix4x4 cameraProjectionMatrix = Matrix4x4::perspective(60.0f, 960.0f / 720.0f, 0.3f, 1000.0f);
    cameraProjectionMatrix.Printf2Debug();
    
    //rotaion y
    Vector3 yRotaionAxis(0, 1, 0);
    Quaternion modelYRotation = Quaternion::fromAxisAngle(yRotaionAxis, 45 * filament::math::f::DEG_TO_RAD);
    Matrix4x4 modelYRotationMat(modelYRotation);
    //rotaion x
    Vector3 xRotaionAxis(1, 0, 0);
    Quaternion modelXRotation = Quaternion::fromAxisAngle(xRotaionAxis, -15 * filament::math::f::DEG_TO_RAD);
    Matrix4x4 modelXRotationMat(modelXRotation);
    //rotaion z
    Matrix4x4 modelZRotaionMat;
    // local space rotation y-x-z
    Matrix4x4 modelRotationMat = filament::math::highPrecisionMultiply(modelYRotationMat, modelXRotationMat);
    modelRotationMat = filament::math::highPrecisionMultiply(modelRotationMat, modelZRotaionMat);
    //scale
    Vector3 scaling(2, 1, 1);
    Matrix4x4 modelScalingMat = Matrix4x4::scaling(scaling);
    //translation
    Vector4 translation(-0.3f, 0.06f, 0.36f, 1.0f);
    // local to world matrix
    Matrix4x4 modelLocalToWorldMat = filament::math::highPrecisionMultiply(modelRotationMat, modelScalingMat);
    modelLocalToWorldMat[3] = translation;
    modelLocalToWorldMat.Printf2Debug();
   
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
