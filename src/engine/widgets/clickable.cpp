#include "clickable.hpp"
//
#include "engine/window/input/input.hpp"

void Clickable::callback() {
    if (state == ButtonState::hover && !containsMouse())
        setState(ButtonState::idle);
    else if (state == ButtonState::idle && containsMouse())
        setState(ButtonState::hover);
}

void Clickable::setState(const ButtonState newState) {
    state = newState;
    switch (state) {
    case ButtonState::idle:    setTexture(BTN_IDLE);    break;
    case ButtonState::hover:   setTexture(BTN_HOVER);   break;
    case ButtonState::checked: setTexture(BTN_CHECKED); break;
    }
}
