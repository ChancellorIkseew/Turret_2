#include "input.hpp"
//
#include <SDL3/SDL_events.h>
#include <atomic>
#include "controls.hpp"
#include "engine/io/utf8/utf8.hpp"

void Input::update(const SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        if (event.wheel.y < 0.0f)
            mouseWheelScroll = MouseWheelScroll::up;
        else if (event.wheel.y > 0.0f)
            mouseWheelScroll = MouseWheelScroll::down;
        return;
    }
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        mouseCoord = PixelCoord(event.motion.x, event.motion.y);
        return;
    }
    if (event.type == SDL_EVENT_TEXT_INPUT) {
        symbolJustEntered = utf8::to_char32_t(event.text.text);
        return;
    }

    InputType inputType = InputType::keyboard;
    int code = -1;
    bool pressed = false;
    
    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
        code = event.key.scancode;
        pressed = event.key.down;
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        code = event.button.button;
        pressed = event.button.down;
        inputType = InputType::mouse;
    } else {
        return;
    }

    lastKeyPressed = Binding(code, inputType);
    for (auto& [bindName, binding] : Controls::getBindings()) {
        if (inputType != binding.inputType || code != binding.code)
            continue;
        binding.justTriggered = !binding.active && pressed;
        binding.active = pressed;
    }
}

void Input::reset() {
    mouseWheelScroll = MouseWheelScroll::none;
    symbolJustEntered.reset();
    lastKeyPressed.reset();
    for (auto& [bindName, binding] : Controls::getBindings()) {
        binding.justTriggered = false;
    }
}

bool Input::active(cString bindName) const {
    return Controls::getBindings().contains(bindName) && Controls::getBindings().at(bindName).active;
}
bool Input::jactive(cString bindName) const {
    return Controls::getBindings().contains(bindName) && Controls::getBindings().at(bindName).justTriggered;
}

PixelCoord Input::getMouseCoord() const {
    return mouseCoord;
}
MouseWheelScroll Input::getMouseWheelScroll() const {
    return mouseWheelScroll;
}

std::optional<Binding> Input::getLastKeyPressed() const {
    return lastKeyPressed;
}
std::optional<char32_t> Input::getLastSymbolEntered() const {
    return symbolJustEntered;
}
