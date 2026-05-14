#pragma once
#include "MINGUI/render/render_bridge.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/text.hpp"

struct T1_UITexture : mingui::TextureBridge {
    Texture texture;
public:
    T1_UITexture(const Texture texture) : texture(texture) {}
    ~T1_UITexture() final = default;
};

class T1_UIRenderer : public mingui::RenderBridge {
    Renderer& renderer;
    const float scale = 1;
public:
    T1_UIRenderer(Renderer& renderer) : renderer(renderer) { }
    ~T1_UIRenderer() final = default;
    //
    void drawRect(mingui::Rect rect) final {
        renderer.drawRect(rect.color, { rect.position.x, rect.position.y }, { rect.size.x, rect.size.y });
    }
    void drawText(mingui::Text text) final {
        text::drawString(renderer, text.string, { text.position.x, text.position.y });
    }
    void drawSprite(mingui::Sprite sprite) final {
        const Texture texture = static_cast<T1_UITexture*>(sprite.textureBridge)->texture;
        renderer.drawFast(texture, { sprite.position.x, sprite.position.y }, { sprite.size.x, sprite.size.y });
    }
    //
    void drawRectsBatched(std::span<const mingui::Rect> rects) final { /* not needded */ }
    void drawTextsBatched(std::span<const mingui::Text> texts) final { /* not needded */ }
    //
    void setScale(const float scale) final {
        renderer.setScale(scale);
    }
};
