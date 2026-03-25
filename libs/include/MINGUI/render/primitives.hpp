#pragma once
#include <cstdint>
#include <string_view>
#include <MINGUI/core/config.hpp>
#include <MINGUI/core/point.hpp>

START_NAMESPACE_MINGUI

struct Rect {
    Point position;
    Point size;
    uint32_t color;
};

struct Text {
    Point position;
    std::u32string_view string;
    uint32_t color;
};

struct TextureBridge {
    virtual ~TextureBridge() = default;
};

struct Sprite {
    Point position;
    Point size;
    TextureBridge* textureBridge;
};

END_NAMESPACE_MINGUI
