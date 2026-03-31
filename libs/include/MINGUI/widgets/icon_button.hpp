#pragma once
#include <MINGUI/widgets/abstract_button.hpp>
#include <MINGUI/widgets/icon.hpp>

START_NAMESPACE_MINGUI

class IconButton : public AbstractButton {
    Icon icon;
public:
    IconButton(const Point size, const float padding, TextureBridge* texture) :
        AbstractButton(size), icon(size - Point(padding, padding) * 2, texture) {}
    ~IconButton() final = default;
    void draw(RenderQueue& queue) final;
    void setPosition(const Point position) final;
};

END_NAMESPACE_MINGUI
