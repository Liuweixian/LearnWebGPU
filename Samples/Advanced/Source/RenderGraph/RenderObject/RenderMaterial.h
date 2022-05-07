#pragma once
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "webgpu/webgpu_cpp.h"
#include "../RenderPass/RenderPassDefine.h"

class RenderMaterial
{
public:
    RenderMaterial(RenderPassIdx passIdx);
    virtual ~RenderMaterial();
private:
    RenderPassIdx m_ePassIdx;
};