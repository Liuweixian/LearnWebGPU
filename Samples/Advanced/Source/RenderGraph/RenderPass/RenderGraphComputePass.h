#pragma once
#include "RenderGraphPass.h"
#include "RenderGraphDefine.h"

class RenderGraphComputePass : public RenderGraphPass
{
public:
    RenderGraphComputePass();
    virtual ~RenderGraphComputePass();
    void Execute();
    virtual void Compile() override;
};