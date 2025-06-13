#include "input.hpp"

std::unordered_map<BindName, Binding> Input::bindings {
    {BindName::LMB, Binding(SDL_BUTTON_LEFT, InputType::mouse)},
    {BindName::RMB, Binding(SDL_BUTTON_RIGHT, InputType::mouse)},
    {BindName::MidMB, Binding(SDL_BUTTON_MIDDLE, InputType::mouse)},

    {BindName::Build, Binding(SDL_BUTTON_LEFT, InputType::mouse)},
    {BindName::Rotate_building, Binding(SDL_SCANCODE_R, InputType::keyboard)},
    {BindName::Pipette, Binding(SDL_SCANCODE_Q, InputType::keyboard)},
    {BindName::Demolish, Binding(SDL_SCANCODE_LCTRL, InputType::keyboard)},

    {BindName::Control_unit, Binding(SDL_SCANCODE_V, InputType::keyboard)},
    {BindName::Shoot, Binding(SDL_BUTTON_LEFT, InputType::mouse)},

    {BindName::Move_up, Binding(SDL_SCANCODE_W, InputType::keyboard)},
    {BindName::Move_left, Binding(SDL_SCANCODE_A, InputType::keyboard)},
    {BindName::Move_down, Binding(SDL_SCANCODE_S, InputType::keyboard)},
    {BindName::Move_right, Binding(SDL_SCANCODE_D, InputType::keyboard)},

    {BindName::Pause, Binding(SDL_SCANCODE_SPACE, InputType::keyboard)},
    {BindName::Cancel, Binding(SDL_SCANCODE_ESCAPE, InputType::keyboard)},
    {BindName::Escape, Binding(SDL_SCANCODE_ESCAPE, InputType::keyboard)},
    {BindName::Hide_GUI, Binding(SDL_SCANCODE_F1, InputType::keyboard)},
    {BindName::Screenshot, Binding(SDL_SCANCODE_F2, InputType::keyboard)},
    {BindName::Show_FPS, Binding(SDL_SCANCODE_F3, InputType::keyboard)},
    {BindName::Show_atlas, Binding(SDL_SCANCODE_F4, InputType::keyboard)},
    {BindName::Console_cheat, Binding(SDL_SCANCODE_RALT, InputType::keyboard)}
};
