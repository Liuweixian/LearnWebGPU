#pragma once
#include "../../RenderGraph/RenderGraph.h"

class DrawTriangleRenderGraph : public RenderGraph
{
public:
    enum class PassIdx : uint16_t
    {
        Invalid,
        DrawTrianglePass,
    };
    
    DrawTriangleRenderGraph();
    ~DrawTriangleRenderGraph();
    virtual void Initialize() override;
    
private:
    void InitializeScene();
    void InitializePass();
};
