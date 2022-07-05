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
    m_pVPMatrixBufferHandle = GetResources()->CreateBuffer("VPMatrix", wgpu::BufferUsage::Uniform);
    Bind(wgpu::ShaderStage::Vertex, 0, 0, m_pVPMatrixBufferHandle);
    m_pObject2WorldBufferHandle = GetResources()->CreateBuffer("Object2WorldMatrix", wgpu::BufferUsage::Uniform);
    Bind(wgpu::ShaderStage::Vertex, 0, 1, m_pObject2WorldBufferHandle);
    
    //camera projection matrix
    Matrix4x4 cameraProjectionMatrix = Matrix4x4::perspective(60.0f, 960.0f / 720.0f, 0.3f, 1000.0f);
    cameraProjectionMatrix.Printf2Debug();
    //because of filament math was OpenGL style, but the WebGPU was DX style, so we need to mapping the z in the NDC space from [-1,1] to [0, 1]
    cameraProjectionMatrix[0][2] = cameraProjectionMatrix[0][2] * -0.5f + cameraProjectionMatrix[0][3] * 0.5f;
    cameraProjectionMatrix[1][2] = cameraProjectionMatrix[1][2] * -0.5f + cameraProjectionMatrix[1][3] * 0.5f;
    cameraProjectionMatrix[2][2] = cameraProjectionMatrix[2][2] * -0.5f + cameraProjectionMatrix[2][3] * 0.5f;
    cameraProjectionMatrix[3][2] = cameraProjectionMatrix[3][2] * -0.5f + cameraProjectionMatrix[3][3] * 0.5f;
    cameraProjectionMatrix.Printf2Debug();
    
    //camera local to world matrix
    Matrix4x4 cameraLocalToWorldMat = Matrix4x4::eulerYXZ(14.62f * filament::math::f::DEG_TO_RAD, 0, 0);
    Vector4 cameraTranslation(0, 0.86f, -2.12f, 1.0f);
    cameraLocalToWorldMat[3] = cameraTranslation;
    //camera world to local matrix
    Matrix4x4 cameraWorldToLocalMat = filament::math::details::matrix::inverse(cameraLocalToWorldMat);
    //still in OpenGL style
    Vector3 cameraScaling(1, 1, -1);
    Matrix4x4 cameraForwardMat = Matrix4x4::scaling(cameraScaling);
    //camera view matrix
    Matrix4x4 cameraViewMat = filament::math::details::matrix::multiply<Matrix4x4>(cameraForwardMat, cameraWorldToLocalMat);
    cameraViewMat.Printf2Debug();
    
    //VP matrix
    Matrix4x4 vpMat = filament::math::details::matrix::multiply<Matrix4x4>(cameraProjectionMatrix, cameraViewMat);
    vpMat.Printf2Debug();
    m_pVPMatrixBufferHandle->SetData(4 * 4, vpMat.asArray());
    
    //model rotaion y
    Vector3 yRotaionAxis(0, 1, 0);
    Quaternion modelYRotation = Quaternion::fromAxisAngle(yRotaionAxis, 45 * filament::math::f::DEG_TO_RAD);
    Matrix4x4 modelYRotationMat(modelYRotation);
    //model rotaion x
    Vector3 xRotaionAxis(1, 0, 0);
    Quaternion modelXRotation = Quaternion::fromAxisAngle(xRotaionAxis, -15 * filament::math::f::DEG_TO_RAD);
    Matrix4x4 modelXRotationMat(modelXRotation);
    //model rotaion z
    Matrix4x4 modelZRotaionMat;
    //model local space rotation y-x-z
    Matrix4x4 modelRotationMat = filament::math::highPrecisionMultiply(modelYRotationMat, modelXRotationMat);
    modelRotationMat = filament::math::highPrecisionMultiply(modelRotationMat, modelZRotaionMat);
    //model scale
    Vector3 modelScaling(2, 1, 1);
    Matrix4x4 modelScalingMat = Matrix4x4::scaling(modelScaling);
    //model translation
    Vector4 modelTranslation(-0.3f, 0.06f, 0.36f, 1.0f);
    //model local to world matrix
    Matrix4x4 modelLocalToWorldMat = filament::math::highPrecisionMultiply(modelRotationMat, modelScalingMat);
    modelLocalToWorldMat[3] = modelTranslation;
    modelLocalToWorldMat.Printf2Debug();
    m_pObject2WorldBufferHandle->SetData(4 * 4, modelLocalToWorldMat.asArray());
}

DrawCubePass::~DrawCubePass()
{
}

RGShader *DrawCubePass::SetupShader()
{
    RGDrawShader *pDrawShader = new RGDrawShader(2, wgpu::VertexFormat::Float32x3, wgpu::VertexFormat::Float32x2);
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Vertex, "vs_main", "DrawCubeRenderGraph/Shaders/DrawCubePassVS.wgsl");
    pDrawShader->Create<RGShaderProgram>(RGShaderProgram::Type::Fragment, "fs_main", "DrawCubeRenderGraph/Shaders/DrawCubePassFS.wgsl");
    return pDrawShader;
}

void DrawCubePass::Update()
{
}
