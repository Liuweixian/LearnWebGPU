#pragma once
#include "../../RenderGraph/RenderGraph.h"

class SimpleRenderGraph : public RenderGraph
{
public:
    enum class PassIdx : uint16_t
    {
        Invalid,
        DrawObjectPass,
    };
    
    SimpleRenderGraph();
    ~SimpleRenderGraph();
    virtual void Initialize() override;
    
private:
    void InitializeScene();
    void InitializePass();
};
