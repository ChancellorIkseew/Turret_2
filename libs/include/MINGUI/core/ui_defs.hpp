#pragma once
#include <cstdint>
#include <MINGUI/core/config.hpp>
#include <MINGUI/core/palette.hpp>

START_NAMESPACE_MINGUI

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

enum class GridType : uint8_t {
    from_colls,
    from_rows
};

enum class ButtonState : uint8_t {
    idle,
    hover,
    checked
};

constexpr Palette defaultPalette{
    0x5A'6D'75'80, // nonInteractive
    0x00'09'0D'A0, // idle
    0x02'2B'3D'A0, // hover
    0xA3'BC'C6'A0, // checked
    0xFF'FF'FF'FF  // text
};

// Not 0x00'00'00'00 becuase of null palette check
constexpr Palette transparentPalette{
    0xFF'FF'FF'00, // nonInteractive
    0xFF'FF'FF'00, // idle
    0xFF'FF'FF'00, // hover
    0xFF'FF'FF'00, // checked
    0xFF'FF'FF'00  // text
};

END_NAMESPACE_MINGUI
