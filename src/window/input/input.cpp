#include "input.hpp"
//
#include <atomic>

static std::atomic<PixelCoord> mouseCoord;
static std::atomic<MouseWheelScroll> mouseWheelScroll = MouseWheelScroll::none;

void Input::update(const SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        MouseWheelScroll scroll = MouseWheelScroll::none;
        if (event.wheel.y < 0.0f)
            scroll = MouseWheelScroll::up;
        else if (event.wheel.y > 0.0f)
            scroll = MouseWheelScroll::down;
        mouseWheelScroll.store(scroll, std::memory_order_relaxed);
    }

    if (event.type == SDL_EVENT_MOUSE_MOTION)
        mouseCoord.store(PixelCoord(event.motion.x, event.motion.y));

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

    for (auto& [bindName, binding] : bindings) {
        if (inputType != binding.inputType || code != binding.code)
            continue;
        binding.justTriggered = !binding.active && pressed;
        binding.active = pressed;
    }
}

void Input::reset() {
    for (auto& [bindName, binding] : bindings) {
        binding.justTriggered = false;
    }
}

bool Input::active(const BindName bindName) {
    return bindings.contains(bindName) && bindings.at(bindName).active;
}
bool Input::jactive(const BindName bindName) {
    return bindings.contains(bindName) && bindings.at(bindName).justTriggered;
}

PixelCoord Input::getMouseCoord() {
    return mouseCoord.load(std::memory_order_relaxed);
}
MouseWheelScroll Input::getMouseWheelScroll() {
    return mouseWheelScroll.load(std::memory_order_relaxed);
}
