#include "MINGUI/render/render_queue.hpp"
//
#include "MINGUI/render/render_bridge.hpp"

MINGUI

void RenderQueue::drawAndClear(RenderBridge& renderBridge) noexcept {
    draw(renderBridge);
    clear();
}

void RenderQueue::drawBatchedAndClear(RenderBridge& renderBridge) noexcept {
    drawBatched(renderBridge);
    clear();
}

void RenderQueue::draw(RenderBridge& renderBridge) const noexcept {
    for (const auto& rect : mainLayer) {
        renderBridge.drawRect(rect);
    }
    for (const auto& text : mainLayerTexts) {
        renderBridge.drawText(text);  
    }
    for (const auto& sprite : mainLayerSprites) {
        renderBridge.drawSprite(sprite);
    }
}

void RenderQueue::drawBatched(RenderBridge& renderBridge) const noexcept {
    renderBridge.drawRectsBatched(mainLayer);
    renderBridge.drawTextsBatched(mainLayerTexts);
    for (const auto& sprite : mainLayerSprites) {
        renderBridge.drawSprite(sprite);
    }
}

void RenderQueue::clear() noexcept {
    mainLayer.clear();
    mainLayerTexts.clear();
    mainLayerSprites.clear();
}
