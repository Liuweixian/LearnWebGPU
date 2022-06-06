#pragma once
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "webgpu/webgpu_cpp.h"
#include "../RGDefine.h"

class RGMaterial
{
public:
    RGMaterial(RGPassIdx passIdx);
    virtual ~RGMaterial();
    uint32_t GetIdx()
    {
        return 0;
    }

private:
    RGPassIdx m_uPassIdx;
    //BlendState
    //ColorMask
};
