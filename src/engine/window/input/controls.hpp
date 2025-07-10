#pragma once
#include <string>
#include <unordered_map>
#include "binding.hpp"

class Controls {
    static std::unordered_map<std::string, Binding> bindings;
public:
    static const std::unordered_map<std::string, Binding>& getBindings() { return bindings; }
    static std::u32string getKeyName(const std::string& bindName);
    static void writeBindings();
    static void readBindings();
    static void rebind(const std::string& bindName, const Binding binding);
    static void addBinding(const std::string& bindName, const std::string& keyName);
};
