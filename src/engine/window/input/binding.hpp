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
    Escape,

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
    Hide_GUI,
    Screenshot,
    Show_FPS,
    Show_atlas,
    Fullscreen,
    Console_cheat,

    Reserved_0,
    Reserved_1,
    Reserved_2,
    Reserved_3,
    Reserved_4,
    Reserved_5,
    Reserved_6,
    Reserved_7,
    Reserved_8,
    Reserved_9,
    Reserved_10,
    Reserved_11,
    Reserved_12,
    Reserved_13,
    Reserved_14,
    Reserved_15,
    Reserved_16,
    Reserved_17,
    Reserved_18,
    Reserved_19,
    Reserved_20,

    Count
};

static inline BindName operator++(BindName bindName) {
    return static_cast<BindName>(static_cast<uint8_t>(bindName) + 1U);
}

using cString = const char*;

inline constexpr std::array<std::pair<BindName, cString>, static_cast<size_t>(BindName::Count)> bindNames {
    std::make_pair(BindName::LMB, "LMB"),
    std::make_pair(BindName::RMB, "RMB"),
    std::make_pair(BindName::MidMB, "MidMB"),
    std::make_pair(BindName::Arrow_left, "Arrow_left"),
    std::make_pair(BindName::Arrow_right, "Arrow_right"),
    std::make_pair(BindName::Backspace, "Backspace"),
    std::make_pair(BindName::Delete, "Delete"),
    std::make_pair(BindName::Escape, "Escape"),

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
    std::make_pair(BindName::Hide_GUI, "Hide_GUI"),
    std::make_pair(BindName::Screenshot, "Screenshot"),
    std::make_pair(BindName::Show_FPS, "Show_FPS"),
    std::make_pair(BindName::Show_atlas, "Show_atlas"),
    std::make_pair(BindName::Fullscreen, "Fullscreen"),
    std::make_pair(BindName::Console_cheat, "Console_cheat"),

    std::make_pair(BindName::Reserved_0, "Reserved_0"),
    std::make_pair(BindName::Reserved_1, "Reserved_1"),
    std::make_pair(BindName::Reserved_2, "Reserved_2"),
    std::make_pair(BindName::Reserved_3, "Reserved_3"),
    std::make_pair(BindName::Reserved_4, "Reserved_4"),
    std::make_pair(BindName::Reserved_5, "Reserved_5"),
    std::make_pair(BindName::Reserved_6, "Reserved_6"),
    std::make_pair(BindName::Reserved_7, "Reserved_7"),
    std::make_pair(BindName::Reserved_8, "Reserved_8"),
    std::make_pair(BindName::Reserved_9, "Reserved_9"),
    std::make_pair(BindName::Reserved_10, "Reserved_10"),
    std::make_pair(BindName::Reserved_11, "Reserved_11"),
    std::make_pair(BindName::Reserved_12, "Reserved_12"),
    std::make_pair(BindName::Reserved_13, "Reserved_13"),
    std::make_pair(BindName::Reserved_14, "Reserved_14"),
    std::make_pair(BindName::Reserved_15, "Reserved_15"),
    std::make_pair(BindName::Reserved_16, "Reserved_16"),
    std::make_pair(BindName::Reserved_17, "Reserved_17"),
    std::make_pair(BindName::Reserved_18, "Reserved_18"),
    std::make_pair(BindName::Reserved_19, "Reserved_19"),
    std::make_pair(BindName::Reserved_20, "Reserved_20"),
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
};

struct BindingInfo {
    int code;
    InputType inputType;
    constexpr BindingInfo(const int code, const InputType inputType) :
        code(code), inputType(inputType) {}
};
