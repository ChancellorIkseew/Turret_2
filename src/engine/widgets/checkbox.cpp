#include "checkbox.hpp"
//
#include "engine/window/input/input.hpp"

void Checkbox::callback() {
    Clickable::callback();
    if (containsMouse() && Input::jactive(LMB)) {
        if (state == ButtonState::hover)
            setState(ButtonState::checked);
        else if (state == ButtonState::checked)
            setState(ButtonState::hover);
    }
}
