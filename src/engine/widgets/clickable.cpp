#include "clickable.hpp"
//
#include "engine/audio/audio.hpp"
#include "engine/window/input/input.hpp"

static const std::string UI_ON_HOVER = "ui_on_hover";

void Clickable::callback(UIContext& context) {
    if (state == ButtonState::hover && !containsMouse(context.input))
        setState(ButtonState::idle);
    else if (state == ButtonState::idle && containsMouse(context.input)) {
        setState(ButtonState::hover);
        context.audio.playUI(UI_ON_HOVER);
    }
}

void Clickable::setState(const ButtonState newState) {
    state = newState;
    switch (state) {
    case ButtonState::idle:    setColor(UIColor::idle);    break;
    case ButtonState::hover:   setColor(UIColor::hover);   break;
    case ButtonState::checked: setColor(UIColor::checked); break;
    }
}
