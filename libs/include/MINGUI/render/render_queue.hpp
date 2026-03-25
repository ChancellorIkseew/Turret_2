#pragma once
#include <vector>
#include "MINGUI/core/ui_context.hpp"
#include "MINGUI/render/primitives.hpp"

START_NAMESPACE_MINGUI

class RenderBridge;

class RenderQueue {
    std::vector<Rect> mainLayer;
    std::vector<Text> mainLayerTexts;
    std::vector<Sprite> mainLayerSprites;
public:
    void add(const Point position, const Point size, const uint32_t color) {
        mainLayer.emplace_back(position, size, color);
    }
    void add(const Point position, const std::u32string_view string, const uint32_t color) {
        mainLayerTexts.emplace_back(position, string, color);
    }
    void add(const Point position, const Point size, TextureBridge* textureBridge) {
        mainLayerSprites.emplace_back(position, size, textureBridge);
    }

    void drawBatchedAndClear(RenderBridge& renderBridge) noexcept;
    void drawAndClear(RenderBridge& renderBridge)        noexcept;
    void drawBatched(RenderBridge& renderBridge)   const noexcept;
    void draw(RenderBridge& renderBridge)          const noexcept;
    void clear() noexcept;
};

END_NAMESPACE_MINGUI
