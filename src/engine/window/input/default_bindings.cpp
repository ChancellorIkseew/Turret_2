#include "controls.hpp"
//
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_mouse.h"

std::unordered_map<std::string, Binding> Controls::bindings = {
    {LMB, Binding(SDL_BUTTON_LEFT, InputType::mouse, false)},
    {RMB, Binding(SDL_BUTTON_RIGHT, InputType::mouse, false)},
    {MidMB, Binding(SDL_BUTTON_MIDDLE, InputType::mouse, false)},
    {Arrow_left, Binding(SDL_SCANCODE_LEFT, InputType::keyboard, false)},
    {Arrow_right, Binding(SDL_SCANCODE_RIGHT, InputType::keyboard, false)},
    {Backspace, Binding(SDL_SCANCODE_BACKSPACE, InputType::keyboard, false)},
    {Delete, Binding(SDL_SCANCODE_DELETE, InputType::keyboard, false)},
    {Escape, Binding(SDL_SCANCODE_ESCAPE, InputType::keyboard, false)},

    {Build, Binding(SDL_BUTTON_LEFT, InputType::mouse)},
    {Rotate_building, Binding(SDL_SCANCODE_R, InputType::keyboard)},
    {Pipette, Binding(SDL_SCANCODE_Q, InputType::keyboard)},
    {Demolish, Binding(SDL_SCANCODE_LCTRL, InputType::keyboard)},

    {Control_unit, Binding(SDL_SCANCODE_V, InputType::keyboard)},
    {Shoot, Binding(SDL_BUTTON_LEFT, InputType::mouse)},

    {Move_up, Binding(SDL_SCANCODE_W, InputType::keyboard)},
    {Move_left, Binding(SDL_SCANCODE_A, InputType::keyboard)},
    {Move_down, Binding(SDL_SCANCODE_S, InputType::keyboard)},
    {Move_right, Binding(SDL_SCANCODE_D, InputType::keyboard)},

    {Pause, Binding(SDL_SCANCODE_SPACE, InputType::keyboard)},
    {Hide_GUI, Binding(SDL_SCANCODE_F1, InputType::keyboard)},
    {Screenshot, Binding(SDL_SCANCODE_F2, InputType::keyboard)},
    {Show_FPS, Binding(SDL_SCANCODE_F3, InputType::keyboard)},
    {Show_atlas, Binding(SDL_SCANCODE_F4, InputType::keyboard)},
    {Show_hitboxes, Binding(SDL_SCANCODE_F5, InputType::keyboard)},
    {Show_console, Binding(SDL_SCANCODE_F10, InputType::keyboard)},
    {Fullscreen, Binding(SDL_SCANCODE_F11, InputType::keyboard)}
};
