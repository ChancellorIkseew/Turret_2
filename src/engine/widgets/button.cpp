#include "button.hpp"
//
#include "engine/window/input/input.hpp"

void Button::callback() {
    if (state == ButtonState::hover && !containsMouse())
        setState(ButtonState::idle);
    else if (state == ButtonState::idle && containsMouse())
        setState(ButtonState::hover);
    //
    if (state == ButtonState::hover && action && Input::jactive(LMB))
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

void Button::setState(const ButtonState newState) {
    state = newState;
    switch (state) {
    case ButtonState::idle:  setTexture(BTN_IDLE);  break;
    case ButtonState::hover: setTexture(BTN_HOVER); break;
    }
}
