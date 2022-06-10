#include "RGMesh.h"

RGMesh::RGMesh(std::string szName)
{
    m_szName = szName;
    m_VertexBuffers.clear();
    m_pIndexBuffer = nullptr;
}

RGMesh::~RGMesh()
{
}

RGBufferResHandle *RGMesh::CreateVertexBuffer()
{
    size_t ulIndex = m_VertexBuffers.size();
    RGBufferResHandle *pBufferHandle = GetResources()->CreateBuffer(m_szName + "_VBO_" + std::to_string(ulIndex), wgpu::BufferUsage::Vertex);
    pBufferHandle->SetShaderStage(wgpu::ShaderStage::Vertex);
    m_VertexBuffers.push_back(pBufferHandle);
    return pBufferHandle;
}

RGBufferResHandle *RGMesh::CreateIndexBuffer()
{
    m_pIndexBuffer = GetResources()->CreateBuffer(m_szName + "_IBO", wgpu::BufferUsage::Index);
    m_pIndexBuffer->SetShaderStage(wgpu::ShaderStage::Vertex);
    return m_pIndexBuffer;
}
