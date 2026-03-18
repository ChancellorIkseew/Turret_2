#include "MINGUI/widgets/clickable.hpp"

MINGUI

void Clickable::callback(UIContext& context) {
    if (context.idled(*this)) {
        setState(ButtonState::idle);
        context.onIdled(*this);
    }
    else if (context.hovered(*this)) {
        setState(ButtonState::hover);
        context.onHovered(*this);
        if (context.clicked(*this))
            context.onClicked(*this);
    }
}
