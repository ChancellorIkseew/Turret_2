#pragma once
#include "MINGUI/render/render_bridge.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/text.hpp"

struct T1_UITexture : mingui::TextureBridge {
    TextureRect textureRect;
public:
    T1_UITexture(const TextureRect textureRect) : textureRect(textureRect) {}
    ~T1_UITexture() final = default;
};

class T1_UIRenderer : public mingui::RenderBridge {
    Renderer& renderer;
    const TextureRect nodeBase;
    const float scale = 1;
public:
    T1_UIRenderer(Renderer& renderer, TextureRect nodeBase) : renderer(renderer), nodeBase(nodeBase) {}
    ~T1_UIRenderer() final = default;
    //
    void drawRect(mingui::Rect rect) final {
        const PixelCoord position(rect.position.x, rect.position.y);
        const PixelCoord size(rect.size.x, rect.size.y);
        constexpr PixelCoord ORIGIN(0.f, 0.f);
        constexpr float ANGLE_RAD = 0;
        renderer.draw(nodeBase, position, size, ORIGIN, ANGLE_RAD, rect.color);
    }
    void drawText(mingui::Text text) final {
        text::drawString(renderer, text.string, { text.position.x, text.position.y });
    }
    void drawSprite(mingui::Sprite sprite) final {
        const TextureRect textureRect = static_cast<T1_UITexture*>(sprite.textureBridge)->textureRect;
        const PixelCoord position(sprite.position.x, sprite.position.y);
        const PixelCoord size(sprite.size.x, sprite.size.y);
        constexpr PixelCoord ORIGIN(0.f, 0.f);
        constexpr float ANGLE_RAD = 0;
        renderer.draw(textureRect, position, size, ORIGIN, ANGLE_RAD);
    }
    //
    void drawRectsBatched(std::span<const mingui::Rect> rects) final { /* not needded */ }
    void drawTextsBatched(std::span<const mingui::Text> texts) final { /* not needded */ }
    //
    void setScale(const float scale) final {
        renderer.setView(scale, PixelCoord(0.f, 0.f));
    }
};
