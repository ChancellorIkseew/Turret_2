#pragma once
#include <cstdint>
///@brief For constexpr strings only.
using cString = const char*;

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
    mutable bool released = false;

    constexpr Binding(const int code, const InputType inputType, const bool changable = true) :
        code(code), inputType(inputType), changable(changable) { }
};
