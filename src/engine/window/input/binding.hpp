#pragma once

enum class BindName {
    LMB,
    RMB,
    MidMB,

    Build,
    Demolish,
    Rotate_building,
    Pipette,
    Shoot,
    Control_unit,

    Move_up,
    Move_left,
    Move_down,
    Move_right,

    Pause,
    Cancel,
    Escape,
    Hide_GUI,
    Screenshot,
    Show_FPS,
    Show_atlas,
    Console_cheat
};

enum class InputType {
    keyboard,
    mouse,
};

enum class MouseWheelScroll {
    none = 0,
    up = 1,
    down = -1
};

struct Binding {
    const int code;
    const InputType inputType;
    bool active = false;
    bool justTriggered = false;

    constexpr Binding(const int code, const InputType inputType) :
        code(code), inputType(inputType) {}
    Binding() = default;
    ~Binding() = default;
};
