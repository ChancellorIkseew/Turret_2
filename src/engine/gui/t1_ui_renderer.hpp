#pragma once
#include "MINGUI/render/render_bridge.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/text.hpp"

class T1_UIRenderer : public mingui::RenderBridge {
    const Renderer& renderer;
public:
    T1_UIRenderer(const Renderer& renderer) : renderer(renderer) { }
    ~T1_UIRenderer() final = default;
    //
    void drawRect(mingui::Rect rect) final {
        renderer.drawRect(rect.color, { rect.position.x, rect.position.y }, { rect.size.x, rect.size.y });
    }
    void drawText(mingui::Text text) final {
        text::drawString(renderer, text.string, { text.position.x, text.position.y });
    }
    //
    void drawRectsBatched(std::span<const mingui::Rect> rects) final { /* not needded */ }
    void drawTextsBatched(std::span<const mingui::Text> texts) final { /* not needded */ }
};
