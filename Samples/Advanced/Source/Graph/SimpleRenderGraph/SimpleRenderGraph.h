#pragma once
#include "../../RenderGraph/RenderGraph.h"

class SimpleRenderGraph : public RenderGraph
{
private:
    void InitializeScene();
    void InitializePass();
public:
    SimpleRenderGraph();
    ~SimpleRenderGraph();
    virtual void Initialize() override;
private:
};
