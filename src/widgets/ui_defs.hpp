#pragma once
#include <typeinfo>

using AlignType = uint8_t;

enum class Align : AlignType {
    none   = 0b00000000,
    centre = 0b00000001,
    up     = 0b00000010,
    down   = 0b00000100,
    left   = 0b00001000,
    right  = 0b00010000
};

enum class SizePolicy {
    fixed,
    minimized
};
