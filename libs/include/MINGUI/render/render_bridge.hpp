#pragma once
#include <span>
#include <MINGUI/core/config.hpp>
#include <MINGUI/render/primitives.hpp>

START_NAMESPACE_MINGUI

class RenderBridge {
public:
    virtual ~RenderBridge() = default;
    //
    virtual void drawRect(const Rect rect) = 0;
    virtual void drawText(const Text text) = 0;
    virtual void drawSprite(const Sprite sprite) = 0;
    virtual void drawRectsBatched(const std::span<const Rect> rects) = 0;
    virtual void drawTextsBatched(const std::span<const Text> texts) = 0;
};

END_NAMESPACE_MINGUI
