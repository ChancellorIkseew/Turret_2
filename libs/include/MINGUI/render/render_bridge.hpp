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
    //
    virtual void setScale(const float scale) = 0;
};

END_NAMESPACE_MINGUI
