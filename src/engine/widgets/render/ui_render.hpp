#pragma once
#include <vector>
#include <string_view>
#include "../ui_context.hpp"

class Renderer;

START_NAMESPACE_MINGUI

struct Rect {
    Point position;
    Point size;
    uint32_t color;
};

struct Text {
    Point position;
    std::u32string_view string;
};

class RenderQueue {
    std::vector<Rect> mainLayer;
    std::vector<Text> mainLayerTexts;
public:
    void add(const Point position, const Point size, const uint32_t color) {
        mainLayer.emplace_back(position, size, color);
    }
    void add(const Point position, std::u32string_view string) {
        mainLayerTexts.emplace_back(position, string);
    }

    void drawAndClear(const Renderer& renderer) noexcept;
    void draw(const Renderer& renderer) const noexcept;
    void clear() noexcept;
};

END_NAMESPACE_MINGUI
