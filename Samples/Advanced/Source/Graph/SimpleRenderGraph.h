#pragma once
#include "../RenderGraph/RenderGraph.h"

class SimpleRenderGraph : public RenderGraph
{
private:
public:
    SimpleRenderGraph();
    ~SimpleRenderGraph();
    virtual bool Execute() override;
    virtual void InitializeComplete() override;
};