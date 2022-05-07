#pragma once
#include <stdio.h>

enum class RenderPassIdx : uint16_t
{
    Invalid,
    DrawObjectPass,
};

enum class RenderPassType : uint8_t
{
    Invalid,
    Draw,
    Compute,
};