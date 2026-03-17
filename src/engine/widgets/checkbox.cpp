#include "checkbox.hpp"

MINGUI

void Checkbox::callback(UIContext& context) {
    Clickable::callback(context);
    if (context.clicked(*this)) {
        if (state == ButtonState::hover)
            setState(ButtonState::checked);
        else /* ButtonState::checked */
            setState(ButtonState::hover);
    }
}
