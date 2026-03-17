#include "ui_render.hpp"
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
        renderer.drawRect(rect.color, rect.position, rect.size);
    }
    for (const auto& text : mainLayerTexts) {
        text::drawString(renderer, text.string, text.position);
    }
}

void RenderQueue::clear() noexcept {
    mainLayer.clear();
    mainLayerTexts.clear();
}
