#pragma once
#include "RenderGraphPass.h"

class RenderGraphComputePass : public RenderGraphPass
{
public:
    RenderGraphComputePass(RGPassIdx uPassIdx);
    virtual ~RenderGraphComputePass();
    void Execute();
    virtual void Compile() override;
};
