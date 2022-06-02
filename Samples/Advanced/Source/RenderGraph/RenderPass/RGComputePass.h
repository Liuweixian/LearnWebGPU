#pragma once
#include "RGPass.h"

class RGComputePass : public RGPass
{
public:
    RGComputePass(RGPassIdx uPassIdx);
    virtual ~RGComputePass();
    void Execute();
    virtual void Compile() override;
};
