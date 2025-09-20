#pragma once
#include <cstdint>

///@brief For constexpr strings only.
using cString = const char*;

inline constexpr cString LMB("LMB");
inline constexpr cString RMB("RMB");
inline constexpr cString MidMB("MidMB");
inline constexpr cString Arrow_left("Arrow_left");
inline constexpr cString Arrow_right("Arrow_right");
inline constexpr cString Backspace("Backspace");
inline constexpr cString Delete("Delete");
inline constexpr cString Escape("Escape");

inline constexpr cString Build("Build");
inline constexpr cString Demolish("Demolish");
inline constexpr cString Rotate_building("Rotate_building");
inline constexpr cString Pipette("Pipette");
inline constexpr cString Shoot("Shoot");
inline constexpr cString Control_unit("Control_unit");

inline constexpr cString Move_up("Move_up");
inline constexpr cString Move_left("Move_left");
inline constexpr cString Move_down("Move_down");
inline constexpr cString Move_right("Move_right");

inline constexpr cString Pause("Pause");
inline constexpr cString Hide_GUI("Hide_GUI");
inline constexpr cString Screenshot("Screenshot");
inline constexpr cString Show_FPS("Show_FPS");
inline constexpr cString Show_atlas("Show_atlas");
inline constexpr cString Show_hitboxes("Show_hitboxes");
inline constexpr cString Fullscreen("Fullscreen");
inline constexpr cString Console_cheat("Console_cheat");

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
    int code;
    InputType inputType;
    bool changable = true;
    mutable bool active = false;
    mutable bool justTriggered = false;

    constexpr Binding(const int code, const InputType inputType, const bool changable = true) :
        code(code), inputType(inputType), changable(changable) { }
};
