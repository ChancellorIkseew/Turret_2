#include "checkbox.hpp"
//
#include "engine/window/input/input.hpp"

void Checkbox::callback() {
    if (state == ButtonState::hover && !containsMouse())
        setState(ButtonState::idle);
    else if (state == ButtonState::idle && containsMouse())
        setState(ButtonState::hover);
    //
    if (containsMouse() && Input::jactive(LMB)) {
        if (state == ButtonState::hover)
            setState(ButtonState::checked);
        else if (state == ButtonState::checked)
            setState(ButtonState::hover);
    }
}

void Checkbox::setState(const ButtonState newState) {
    state = newState;
    switch (state) {
    case ButtonState::idle:    setTexture(BTN_IDLE);  break;
    case ButtonState::hover:   setTexture(BTN_HOVER); break;
    case ButtonState::checked: setTexture(BTN_CHECKED); break;
    }
}
