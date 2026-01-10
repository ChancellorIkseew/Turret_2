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

enum class UIColor : uint32_t {
    non_interactive = 0x5A'6D'75'80,
    idle            = 0x00'09'0D'A0,
    hover           = 0x02'2B'3D'A0,
    checked         = 0xA3'BC'C6'A0,
    carrige         = 0xFF'FF'FF'FF
};
