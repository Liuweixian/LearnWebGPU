#pragma once
#include "../../RenderGraph/RenderGraph.h"

class DrawCubeRenderGraph : public RenderGraph
{
public:
    enum class PassIdx : uint16_t
    {
        Invalid,
        DrawCubePass,
    };
    
    DrawCubeRenderGraph();
    ~DrawCubeRenderGraph();
    virtual void Initialize() override;
    
private:
    void InitializeScene();
    void InitializePass();
};
