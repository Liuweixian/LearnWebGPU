#pragma once
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "webgpu/webgpu_cpp.h"
#include "../RGDefine.h"

class RenderMaterial
{
public:
    RenderMaterial(RGPassIdx passIdx);
    virtual ~RenderMaterial();
private:
    RGPassIdx m_uPassIdx;
};
