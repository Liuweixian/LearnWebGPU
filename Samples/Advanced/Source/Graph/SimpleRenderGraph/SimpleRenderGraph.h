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
    enum class PassIdx : uint16_t
    {
        Invalid,
        DrawObjectPass,
    };
};
