#pragma once
#include <string>
#include <unordered_map>
#include "binding.hpp"

class Input;

class Controls {
    friend Input;
    static std::unordered_map<BindName, Binding> bindings;
public:
    static BindName addBinding(cString code, InputType inputType);
    static void writeBindings();
    static void readBindings();
    static void rebind(const BindName bindName, const BindingInfo binding);
    static std::u32string getKeyName(const BindName bindName);
    static std::u32string getKeyName(const int code);
};
