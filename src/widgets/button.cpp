#include "button.hpp"
//
#include "render/text.hpp"
#include "window/input/input.hpp"

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
    PixelCoord margin = (getSize() - label.getSize()) / 2;
    label.setPosition(getPosition() + margin);
}
