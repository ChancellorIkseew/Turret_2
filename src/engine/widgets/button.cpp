#include "button.hpp"
//
#include "engine/render/text.hpp"
#include "engine/window/input/input.hpp"

void Button::callback() {
    if (action && containsMouse() && Input::jactive(BindName::LMB))
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
