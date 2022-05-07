#pragma once
#include "../RenderGraph/RenderGraph.h"

class SimpleRenderGraph : public RenderGraph
{
private:
public:
    SimpleRenderGraph();
    ~SimpleRenderGraph();
    virtual void InitializeComplete() override;
private:
};