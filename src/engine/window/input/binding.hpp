#pragma once
#include <array>
#include <typeinfo>

enum class BindName : uint8_t {
    LMB,
    RMB,
    MidMB,
    Arrow_left,
    Arrow_right,
    Backspace,
    Delete,

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
    Fullscreen,
    Console_cheat,

    Count
};

using cString = const char*;

inline constexpr std::array<std::pair<BindName, cString>, static_cast<size_t>(BindName::Count)> bindNames {
    std::make_pair(BindName::LMB, "LMB"),
    std::make_pair(BindName::RMB, "RMB"),
    std::make_pair(BindName::MidMB, "MidMB"),
    std::make_pair(BindName::Arrow_left, "Arrow_left"),
    std::make_pair(BindName::Arrow_right, "Arrow_right"),
    std::make_pair(BindName::Backspace, "Backspace"),
    std::make_pair(BindName::Delete, "Delete"),

    std::make_pair(BindName::Build, "Build"),
    std::make_pair(BindName::Demolish, "Demolish"),
    std::make_pair(BindName::Rotate_building, "Rotate_building"),
    std::make_pair(BindName::Pipette, "Pipette"),
    std::make_pair(BindName::Shoot, "Shoot"),
    std::make_pair(BindName::Control_unit, "Control_unit"),

    std::make_pair(BindName::Move_up, "Move_up"),
    std::make_pair(BindName::Move_left, "Move_left"),
    std::make_pair(BindName::Move_down, "Move_down"),
    std::make_pair(BindName::Move_right, "Move_right"),

    std::make_pair(BindName::Pause, "Pause"),
    std::make_pair(BindName::Cancel, "Cancel"),
    std::make_pair(BindName::Escape, "Escape"),
    std::make_pair(BindName::Hide_GUI, "Hide_GUI"),
    std::make_pair(BindName::Screenshot, "Screenshot"),
    std::make_pair(BindName::Show_FPS, "Show_FPS"),
    std::make_pair(BindName::Show_atlas, "Show_atlas"),
    std::make_pair(BindName::Fullscreen, "Fullscreen"),
    std::make_pair(BindName::Console_cheat, "Console_cheat")
};

enum class InputType : uint8_t {
    keyboard,
    mouse,
};

enum class MouseWheelScroll : uint8_t {
    none,
    up,
    down
};

struct Binding {
    const int code;
    const InputType inputType;
    bool active = false;
    bool justTriggered = false;

    constexpr Binding(const int code, const InputType inputType) :
        code(code), inputType(inputType) {}
    Binding() = default;
};
