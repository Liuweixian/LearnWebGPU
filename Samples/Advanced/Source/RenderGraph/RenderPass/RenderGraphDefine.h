#pragma once
#include <stdio.h>

enum class RenderPassIdx : uint16_t
{
    Invalid,
    DrawObjectPass,
};

enum class GraphPassType : uint8_t
{
    Invalid,
    Draw,
    Compute,
};