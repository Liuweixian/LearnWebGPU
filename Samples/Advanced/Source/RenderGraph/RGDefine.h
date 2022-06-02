#pragma once
#include <stdint.h>

typedef uint16_t RGPassIdx;
enum class RGPassType : uint8_t
{
    Invalid,
    Draw,
    Compute,
};
