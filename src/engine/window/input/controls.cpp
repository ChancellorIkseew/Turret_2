#include "controls.hpp"
//
#include "SDL3/SDL_keyboard.h"
#include <TOML/cpptoml.h>
#include <filesystem>
#include "engine/debug/logger.hpp"
#include "utf8/utf8.hpp"

constexpr std::array<cString, 5> mouseCodes{ "LMB", "MidMB", "RMB", "Mouse_4", "Mouse_5" };
static std::filesystem::path controls("res/controls.toml");
static debug::Logger logger("bindings");

static cString getKeyNameCStr(const int code) {
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(code));
}

BindingInfo getBindingInfo(const std::string& keyName) {
    for (int i = 0; i < mouseCodes.size(); ++i) {
        if (keyName == mouseCodes[i])
            return BindingInfo(i + 1, InputType::mouse);
    }
    return BindingInfo(SDL_GetScancodeFromName(keyName.c_str()), InputType::keyboard);
}

std::u32string Controls::getKeyName(const std::string& bindName) {
    return getKeyName(bindings.at(bindName).code);
}
std::u32string Controls::getKeyName(const int code) {
    return utf8::fromConstCharToU32String(SDL_GetScancodeName(static_cast<SDL_Scancode>(code)));
}

void Controls::rebind(const std::string& bindName, const BindingInfo binding) {
    if (bindings.contains(bindName)) {
        if (!bindings.at(bindName).changable)
            return;
        bindings.erase(bindName); // Binding has no copy constructor.
    }  
    bindings.emplace(bindName, Binding(binding));
}

void Controls::writeBindings() {
    std::ofstream fout(controls);
    if (!fout.is_open())
        return;
    auto table = cpptoml::make_table();
    for (const auto& [bindName, binding] : bindings) {
        if (binding.changable)
            table->insert(bindName, getKeyNameCStr(binding.code));
    }
    cpptoml::toml_writer writer(fout, " ");
    writer.visit(*table, false);
}

void Controls::readBindings() {
    const auto table = cpptoml::parse_file(controls.string());
    for (const auto& [bindName, _] : *table) {
        const std::string keyName = table->get_as<std::string>(bindName).value_or("");
        if (!keyName.empty())
            rebind(bindName, getBindingInfo(keyName));
    }
}

void Controls::addBinding(const std::string& bindName, const std::string& keyName) {
    if (bindings.contains(bindName)) {
        logger.error() << "BindName already exists. BindName : " << bindName;
        return;
    }
    const auto info = getBindingInfo(keyName);
    if (info.code == SDL_SCANCODE_UNKNOWN) {
        logger.error() << "Detected as keyboard input type. Incorrect keyName: " << keyName;
        return;
    }
    bindings.emplace(bindName, Binding(info));
}
