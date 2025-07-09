#pragma once
#include <string>
#include <unordered_map>
#include "binding.hpp"

class Input;

class Controls {
    friend Input;
    static std::unordered_map<std::string, Binding> bindings;
public:
    static const std::unordered_map<std::string, Binding>& getBindings() { return bindings; }
    static void addBinding(const std::string& bindName, const std::string& keyName);
    static void writeBindings();
    static void readBindings();
    static void rebind(const std::string& bindName, const BindingInfo binding);
    static std::u32string getKeyName(const std::string& bindName);
    static std::u32string getKeyName(const int code);
};
