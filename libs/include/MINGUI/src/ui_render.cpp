#include "MINGUI/render/ui_render.hpp"
//
#include "engine/render/renderer.hpp"
#include "engine/render/text.hpp"

MINGUI

void RenderQueue::drawAndClear(const Renderer& renderer) noexcept {
    draw(renderer);
    clear();
}

void RenderQueue::draw(const Renderer& renderer) const noexcept {
    for (const auto& rect : mainLayer) {
        renderer.drawRect(rect.color, { rect.position.x, rect.position.y }, { rect.size.x, rect.size.y });
    }
    for (const auto& text : mainLayerTexts) {
        text::drawString(renderer, text.string, { text.position.x, text.position.y });
    }
}

void RenderQueue::clear() noexcept {
    mainLayer.clear();
    mainLayerTexts.clear();
}
