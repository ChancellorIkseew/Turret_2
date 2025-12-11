#include "input.hpp"
//
#include <SDL3/SDL_events.h>
#include <atomic>
#include "controls.hpp"
#include "engine/io/utf8/utf8.hpp"

static SDL_Window* window;
static std::atomic<std::optional<Binding>> lastKeyPressed;
static std::atomic<std::optional<char32_t>> symbolJustEntered;
static std::atomic<PixelCoord> mouseCoord;
static std::atomic<MouseWheelScroll> mouseWheelScroll = MouseWheelScroll::none;

void Input::init(SDL_Window* mainWindow) { window = mainWindow; }

void Input::update(const SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        MouseWheelScroll scroll = MouseWheelScroll::none;
        if (event.wheel.y < 0.0f)
            scroll = MouseWheelScroll::up;
        else if (event.wheel.y > 0.0f)
            scroll = MouseWheelScroll::down;
        mouseWheelScroll.store(scroll, std::memory_order_relaxed);
        return;
    }
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        mouseCoord.store(PixelCoord(event.motion.x, event.motion.y));
        return;
    }
    if (event.type == SDL_EVENT_TEXT_INPUT) {
        symbolJustEntered.store(utf8::to_char32_t(event.text.text), std::memory_order_relaxed);
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

    lastKeyPressed.store(Binding(code, inputType), std::memory_order_relaxed);
    for (auto& [bindName, binding] : Controls::getBindings()) {
        if (inputType != binding.inputType || code != binding.code)
            continue;
        binding.justTriggered = !binding.active && pressed;
        binding.active = pressed;
    }
}

void Input::reset() {
    mouseWheelScroll.store(MouseWheelScroll::none, std::memory_order_relaxed);
    for (auto& [bindName, binding] : Controls::getBindings()) {
        binding.justTriggered = false;
    }
}

bool Input::active(cString bindName) {
    return Controls::getBindings().contains(bindName) && Controls::getBindings().at(bindName).active;
}
bool Input::jactive(cString bindName) {
    return Controls::getBindings().contains(bindName) && Controls::getBindings().at(bindName).justTriggered;
}

PixelCoord Input::getMouseCoord() {
    return mouseCoord.load(std::memory_order_relaxed);
}
MouseWheelScroll Input::getMouseWheelScroll() {
    return mouseWheelScroll.load(std::memory_order_relaxed);
}

std::optional<Binding> Input::getLastKeyPressed() {
    return lastKeyPressed.load(std::memory_order_relaxed);
}
void Input::resetLastKeyPressed() {
    lastKeyPressed = std::nullopt;
}
std::optional<char32_t> Input::getLastSymbolEntered() {
    return symbolJustEntered.exchange(std::nullopt, std::memory_order_relaxed);
}
void Input::enableTextEnter(const bool flag) {
    if (flag)
        SDL_StartTextInput(window);
    else
        SDL_StopTextInput(window);
}
bool Input::isTextEnterEnabled() {
    return SDL_TextInputActive(window);
}
