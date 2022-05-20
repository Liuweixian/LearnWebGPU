#include "RenderState.h"
#include <cassert>

RenderState::RenderState()
{
    m_pLayoutDesc = nullptr;
    m_pVertexShaderDesc = nullptr;
    m_pVertexState = nullptr;
    m_pFragmentShaderDesc = nullptr;
    m_pFragmentState = nullptr;
    m_pPrimitiveState = nullptr;
    m_pDepthStencilState = nullptr;
    m_pMultisampleState = nullptr;
    m_RenderPipeline = nullptr;
}

RenderState::~RenderState()
{
}

void RenderState::Initialize(RenderObjectShader *pRenderObjectShader, std::list<RenderResourceHandle *> targetColorBuffers, RenderResourceHandle *pTargetDepthBuffer)
{
    assert(m_pLayoutDesc == nullptr);
    m_pLayoutDesc = new wgpu::PipelineLayoutDescriptor();
    InitVertexState(pRenderObjectShader);
    InitFragmentState(pRenderObjectShader, targetColorBuffers);
    InitPrimitiveState();
    InitDepthStencilState(pTargetDepthBuffer);
    InitMultisampleState();
    assert(m_RenderPipeline == nullptr);
}

void RenderState::InitVertexState(RenderObjectShader *pRenderObjectShader)
{
    assert(m_pVertexState == nullptr && m_pVertexShaderDesc == nullptr);
    RenderShaderProgram *pVertexProgram = pRenderObjectShader->GetProgram(RenderShaderProgram::Type::Vertex);

    m_pVertexState = new wgpu::VertexState();
    m_pVertexState->entryPoint = pVertexProgram->EntryPoint();
    // module
    m_pVertexShaderDesc = new wgpu::ShaderModuleWGSLDescriptor();
    m_pVertexShaderDesc->source = pVertexProgram->LoadSource();
    // vbo
    m_pVertexState->bufferCount = pRenderObjectShader->GetVBOLayoutsCount();
    wgpu::VertexBufferLayout *layout = new wgpu::VertexBufferLayout[m_pVertexState->bufferCount];
    std::list<RenderVBOLayout *> vertexStates = pRenderObjectShader->GetVBOLayouts();
    int nIndex = 0;
    for (auto it = vertexStates.begin(); it != vertexStates.end(); it++)
    {
        RenderVBOLayout *pRenderVBOLayout = *it;
        layout[nIndex] = pRenderVBOLayout->GetLayout();
        nIndex++;
    }
    m_pVertexState->buffers = layout;
}

void RenderState::InitFragmentState(RenderObjectShader *pRenderObjectShader, std::list<RenderResourceHandle *> targetColorBuffers)
{
    assert(m_pFragmentState == nullptr && m_pFragmentShaderDesc == nullptr);
    RenderShaderProgram *pFragProgram = pRenderObjectShader->GetProgram(RenderShaderProgram::Type::Fragment);

    m_pFragmentState = new wgpu::FragmentState();
    m_pFragmentState->entryPoint = pFragProgram->EntryPoint();
    // module
    m_pFragmentShaderDesc = new wgpu::ShaderModuleWGSLDescriptor();
    m_pFragmentShaderDesc->source = pFragProgram->LoadSource();
    m_pFragmentState->targetCount = targetColorBuffers.size();
    wgpu::ColorTargetState *pColorTargetStates = new wgpu::ColorTargetState[m_pFragmentState->targetCount];
    int nIndex = 0;
    for (auto it = targetColorBuffers.begin(); it != targetColorBuffers.end(); it++)
    {
        wgpu::TextureDescriptor *pTextureDesc = GetRenderResource()->GetTextureDesc(*it);
        pColorTargetStates[nIndex].format = pTextureDesc->format;
        nIndex++;
    }
    m_pFragmentState->targets = pColorTargetStates;
}

void RenderState::InitPrimitiveState()
{
    assert(m_pPrimitiveState == nullptr);
    m_pPrimitiveState = new wgpu::PrimitiveState();
    m_pPrimitiveState->topology = wgpu::PrimitiveTopology::TriangleList;
    m_pPrimitiveState->stripIndexFormat = wgpu::IndexFormat::Undefined;
    m_pPrimitiveState->frontFace = wgpu::FrontFace::CCW;
    m_pPrimitiveState->cullMode = wgpu::CullMode::None;
}

void RenderState::InitDepthStencilState(RenderResourceHandle *pTargetDepthBuffer)
{
    assert(m_pDepthStencilState == nullptr);
    if (pTargetDepthBuffer == nullptr)
        m_pDepthStencilState = nullptr;
    else
    {
        m_pDepthStencilState = new wgpu::DepthStencilState();
        wgpu::TextureDescriptor *pTextureDesc = GetRenderResource()->GetTextureDesc(pTargetDepthBuffer);
        m_pDepthStencilState->format = pTextureDesc->format;
    }
}

void RenderState::InitMultisampleState()
{
    assert(m_pMultisampleState == nullptr);
    m_pMultisampleState = new wgpu::MultisampleState();
}

void RenderState::Cleanup()
{
}
