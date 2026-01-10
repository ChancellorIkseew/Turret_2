#include "clickable.hpp"
//
#include "engine/window/input/input.hpp"

void Clickable::callback(const Input& input) {
    if (state == ButtonState::hover && !containsMouse(input))
        setState(ButtonState::idle);
    else if (state == ButtonState::idle && containsMouse(input))
        setState(ButtonState::hover);
}

void Clickable::setState(const ButtonState newState) {
    state = newState;
    switch (state) {
    case ButtonState::idle:    setColor(UIColor::idle);    break;
    case ButtonState::hover:   setColor(UIColor::hover);   break;
    case ButtonState::checked: setColor(UIColor::checked); break;
    }
}
