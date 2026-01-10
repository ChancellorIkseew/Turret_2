#include "checkbox.hpp"
//
#include "engine/window/input/input.hpp"

void Checkbox::callback(UIContext& context) {
    Clickable::callback(context);
    if (containsMouse(context.input) && context.input.jactive(LMB)) {
        if (state == ButtonState::hover)
            setState(ButtonState::checked);
        else if (state == ButtonState::checked)
            setState(ButtonState::hover);
    }
}
