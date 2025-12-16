#include "checkbox.hpp"
//
#include "engine/window/input/input.hpp"

void Checkbox::callback(const Input& input) {
    Clickable::callback(input);
    if (containsMouse(input) && input.jactive(LMB)) {
        if (state == ButtonState::hover)
            setState(ButtonState::checked);
        else if (state == ButtonState::checked)
            setState(ButtonState::hover);
    }
}
