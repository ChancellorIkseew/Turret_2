#include "button.hpp"
//
#include "engine/window/input/input.hpp"

void Button::callback() {
    Clickable::callback();
    if (containsMouse() && action && Input::jactive(LMB))
        action();
}

void Button::draw() {
    Node::draw();
    label.draw();
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
