#pragma once
#include <cstdint>
#include "ui_config.hpp"

START_NAMESPACE_MINGUI

struct Palette {
    uint32_t nonInteractive;
    uint32_t idle;
    uint32_t hover;
    uint32_t checked;
    uint32_t text;

    constexpr bool isValid() const noexcept {
        return nonInteractive + idle + hover + checked + text != 0;
    }
};

constexpr Palette NULL_PALETTE{ 0, 0, 0, 0, 0 };

END_NAMESPACE_MINGUI
