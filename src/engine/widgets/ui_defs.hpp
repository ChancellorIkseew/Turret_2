#pragma once
#include <cstdint>

using AlignType = uint8_t;

enum class Align : AlignType {
    none   = 0b00000000,
    centre = 0b00000001,
    up     = 0b00000010,
    down   = 0b00000100,
    left   = 0b00001000,
    right  = 0b00010000
};

static inline Align operator|(const Align a, const Align b) {
    return static_cast<Align>(static_cast<AlignType>(a) | static_cast<AlignType>(b));
}

enum class Orientation : uint8_t {
    vertical,
    horizontal
};

enum class SizePolicy : uint8_t {
    fixed,
    minimized
};

enum class ButtonState : uint8_t {
    idle,
    hover,
    checked
};
