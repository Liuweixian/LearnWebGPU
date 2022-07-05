#include "RGRenderState.h"
#include "../GfxDevice.h"

RGRenderState::RGRenderState()
{
    m_pDepthStencilState = nullptr;
}

RGRenderState::~RGRenderState()
{
}

void RGRenderState::Initialize(RGDrawShader *pDrawShader, std::list<RGTextureResHandle *> targetColorBuffers, RGTextureResHandle *pTargetDepthBuffer,
                               std::vector<RGBindEntryGroup> *pBindEntryGroups)
{
    InitBindEntriesState(pBindEntryGroups);
    InitVertexState(pDrawShader);
    InitFragmentState(pDrawShader, targetColorBuffers);
    InitPrimitiveState();
    InitDepthStencilState(pTargetDepthBuffer);
    InitMultisampleState();
}

void RGRenderState::InitVertexState(RGDrawShader *pDrawShader)
{
    RGShaderProgram *pVertexProgram = pDrawShader->GetProgram(RGShaderProgram::Type::Vertex);
    m_VertexState.nextInChain = nullptr;
    m_VertexState.module = GetGfxDevice()->CreateShaderModule(pVertexProgram);
    m_VertexState.entryPoint = pVertexProgram->EntryPoint();
    m_VertexState.constantCount = 0;
    m_VertexState.constants = nullptr;
    std::list<RGVertexLayout *> vertexLayouts = pDrawShader->GetVertexLayouts();
    m_VertexState.bufferCount = (uint32_t)vertexLayouts.size();
    wgpu::VertexBufferLayout *pVBOLayouts = new wgpu::VertexBufferLayout[m_VertexState.bufferCount];
    m_VertexState.buffers = pVBOLayouts;
    int nIndex = 0;
    for (auto it = vertexLayouts.begin(); it != vertexLayouts.end(); it++)
    {
        RGVertexLayout *pVertexLayout = *it;
        pVBOLayouts[nIndex] = pVertexLayout->GetLayout();
        nIndex++;
    }
}

void RGRenderState::InitFragmentState(RGDrawShader *pDrawShader, std::list<RGTextureResHandle *> targetColorBuffers)
{
    RGShaderProgram *pFragProgram = pDrawShader->GetProgram(RGShaderProgram::Type::Fragment);
    m_FragmentState.nextInChain = nullptr;
    m_FragmentState.module = GetGfxDevice()->CreateShaderModule(pFragProgram);
    m_FragmentState.entryPoint = pFragProgram->EntryPoint();
    m_FragmentState.constantCount = 0;
    m_FragmentState.constants = nullptr;
    m_FragmentState.targetCount = (uint32_t)targetColorBuffers.size();
    wgpu::ColorTargetState *pColorTargetStates = new wgpu::ColorTargetState[m_FragmentState.targetCount];
    int nIndex = 0;
    for (auto it = targetColorBuffers.begin(); it != targetColorBuffers.end(); it++)
    {
        wgpu::TextureDescriptor *pTextureDesc = GetResources()->GetTextureDesc(*it);
        pColorTargetStates[nIndex].format = pTextureDesc->format;
        pColorTargetStates[nIndex].writeMask = wgpu::ColorWriteMask::All;
        wgpu::BlendState *pBlendState = new wgpu::BlendState();
        pBlendState->color.operation = wgpu::BlendOperation::Add;
        pBlendState->color.srcFactor = wgpu::BlendFactor::One;
        pBlendState->color.dstFactor = wgpu::BlendFactor::One;
        pBlendState->alpha.operation = wgpu::BlendOperation::Add;
        pBlendState->alpha.srcFactor = wgpu::BlendFactor::One;
        pBlendState->alpha.dstFactor = wgpu::BlendFactor::One;
        pColorTargetStates[nIndex].blend = pBlendState;
        nIndex++;
    }
    m_FragmentState.targets = pColorTargetStates;
}

void RGRenderState::InitBindEntriesState(std::vector<RGBindEntryGroup> *pBindEntryGroups)
{
    uint32_t uBindGroupLayoutCount = (uint32_t)(*pBindEntryGroups).size();
    wgpu::BindGroupLayout bindGroupLayouts[uBindGroupLayoutCount];
    for (int i = 0; i < uBindGroupLayoutCount; i++)
    {
        bindGroupLayouts[i] = (*pBindEntryGroups)[i].GetBindGroupLayout();
    }
    m_PipelineLayout = GetGfxDevice()->CreatePipelineLayout(uBindGroupLayoutCount, bindGroupLayouts);
}

void RGRenderState::InitPrimitiveState()
{
    m_PrimitiveState.topology = wgpu::PrimitiveTopology::TriangleList;
    m_PrimitiveState.stripIndexFormat = wgpu::IndexFormat::Undefined;
    m_PrimitiveState.frontFace = wgpu::FrontFace::CCW;
    m_PrimitiveState.cullMode = wgpu::CullMode::None;
}

void RGRenderState::InitDepthStencilState(RGTextureResHandle *pTargetDepthBuffer)
{
    if (pTargetDepthBuffer == nullptr)
    {
        if (m_pDepthStencilState != nullptr)
            delete m_pDepthStencilState;
        m_pDepthStencilState = nullptr;
        return;
    }
    
    m_pDepthStencilState = new wgpu::DepthStencilState();
    m_pDepthStencilState->nextInChain = nullptr;
    m_pDepthStencilState->format = wgpu::TextureFormat::Undefined;
    m_pDepthStencilState->depthWriteEnabled = false;
    m_pDepthStencilState->depthCompare = wgpu::CompareFunction::Always;
    //m_pDepthStencilState->stencilFront;
    //m_pDepthStencilState->stencilBack;
    m_pDepthStencilState->stencilReadMask = 0xFFFFFFFF;
    m_pDepthStencilState->stencilWriteMask = 0xFFFFFFFF;
    m_pDepthStencilState->depthBias = 0;
    m_pDepthStencilState->depthBiasSlopeScale = 0.0f;
    m_pDepthStencilState->depthBiasClamp = 0.0f;
}

void RGRenderState::InitMultisampleState()
{
    m_MultisampleState.nextInChain = nullptr;
    m_MultisampleState.count = 1;
    m_MultisampleState.mask = 0xFFFFFFFF;
    m_MultisampleState.alphaToCoverageEnabled = false;
}

void RGRenderState::UpdateRenderState(RGMaterial *pMaterial)
{
    uint32_t uIdx = pMaterial->GetIdx();
    auto found = m_RenderPipelines.find(uIdx);
    if (found != m_RenderPipelines.end())
    {
        m_pCurrentRenderPipeline = &(found->second);
        return;
    }
    
    wgpu::RenderPipelineDescriptor renderPipelineDesc;
    renderPipelineDesc.layout = m_PipelineLayout;
    renderPipelineDesc.vertex = m_VertexState;
    renderPipelineDesc.primitive = m_PrimitiveState;
    renderPipelineDesc.depthStencil = m_pDepthStencilState;
    renderPipelineDesc.multisample = m_MultisampleState;
    renderPipelineDesc.fragment = &m_FragmentState;
    wgpu::RenderPipeline renderPipeline = GetGfxDevice()->CreateRenderPipeline(&renderPipelineDesc);
    m_RenderPipelines.insert(std::make_pair(uIdx, renderPipeline));
    m_pCurrentRenderPipeline = &renderPipeline;
    
}
