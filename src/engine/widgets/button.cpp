#include "button.hpp"

MINGUI

void Button::callback(UIContext& context) {
    Clickable::callback(context);
    if (context.clicked(*this) && action)
        action();
}

void Button::draw(const Renderer& renderer) {
    Node::draw(renderer);
    label.draw(renderer);
}

void Button::setPosition(const PixelCoord position) {
    Node::setPosition(position);
    centerText();
}

void Button::setText(const std::u32string& name) {
    label.setText(name);
    centerText();
}

void Button::centerText() {
    PixelCoord margin = (getSize() - label.getSize()) / 2;
    label.setPosition(getPosition() + margin);
}
