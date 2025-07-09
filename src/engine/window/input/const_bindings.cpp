#include "controls.hpp"
//
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"

std::unordered_map<BindName, Binding> Controls::bindings {
    {BindName::LMB, Binding(SDL_BUTTON_LEFT, InputType::mouse)},
    {BindName::RMB, Binding(SDL_BUTTON_RIGHT, InputType::mouse)},
    {BindName::MidMB, Binding(SDL_BUTTON_MIDDLE, InputType::mouse)},
    {BindName::Arrow_left, Binding(SDL_SCANCODE_LEFT, InputType::keyboard)},
    {BindName::Arrow_right, Binding(SDL_SCANCODE_RIGHT, InputType::keyboard)},
    {BindName::Backspace, Binding(SDL_SCANCODE_BACKSPACE, InputType::keyboard)},
    {BindName::Delete, Binding(SDL_SCANCODE_DELETE, InputType::keyboard)},
    {BindName::Escape, Binding(SDL_SCANCODE_ESCAPE, InputType::keyboard)}
};
