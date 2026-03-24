#include "MINGUI/widgets/clickable.hpp"
//
#include "MINGUI/render/render_queue.hpp"

MINGUI

void Clickable::draw(RenderQueue& queue) {
    uint32_t color = 0;
    switch (state) {
    case ButtonState::idle:    color = getPalette().idle;    break;
    case ButtonState::hover:   color = getPalette().hover;   break;
    case ButtonState::checked: color = getPalette().checked; break;
    }
    queue.add(getPosition(), getSize(), color);
}

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
