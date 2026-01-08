#include "button.hpp"
//
#include "engine/window/input/input.hpp"

void Button::callback(const Input& input) {
    Clickable::callback(input);
    if (containsMouse(input) && action && input.jactive(LMB))
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
