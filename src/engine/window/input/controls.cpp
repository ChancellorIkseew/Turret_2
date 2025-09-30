#include "controls.hpp"
//
#include <SDL3/SDL_keyboard.h>
#include <array>
#include <filesystem>
#include "engine/debug/logger.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/utf8/utf8.hpp"

constexpr std::array<cString, 6> MOUSE_CODES{ "_", LMB, MidMB, RMB, "Mouse_4", "Mouse_5"};
static std::filesystem::path controls = "controls.tin";
static debug::Logger logger("controls");

static cString getKeyNameCStr(const Binding binding) {
    if (binding.inputType == InputType::mouse)
        return MOUSE_CODES[binding.code];
    return SDL_GetScancodeName(static_cast<SDL_Scancode>(binding.code));
}

static Binding getBinding(const std::string& keyName) {
    for (int code = 0; code < MOUSE_CODES.size(); ++code) {
        if (keyName == MOUSE_CODES[code])
            return Binding(code, InputType::mouse);
    }
    return Binding(SDL_GetScancodeFromName(keyName.c_str()), InputType::keyboard);
}

std::u32string Controls::getKeyName(const std::string& bindName) {
    return utf8::to_u32String(getKeyNameCStr(bindings.at(bindName)));
}

void Controls::rebind(const std::string& bindName, const Binding binding) {
    bindings.insert_or_assign(bindName, binding);
}

void Controls::writeBindings() {
    tin::Data data;
    for (const auto& [bindName, binding] : bindings) {
        if (binding.changable)
            data.emplace(bindName, getKeyNameCStr(binding));
    }
    tin::write(controls, data);
}

void Controls::readBindings() {
    tin::Data data = tin::read(controls);
    if (data.empty()) {
        writeBindings();
        logger.info() << "Saved file with default settings. File: " << controls;
        return;
    }
    for (const auto& [bindName, keyName] : data) {
        rebind(bindName, getBinding(keyName));
    }
}

void Controls::addBinding(const std::string& bindName, const std::string& keyName) {
    if (bindings.contains(bindName)) {
        logger.error() << "BindName already exists. BindName : " << bindName;
        return;
    }
    const auto binding = getBinding(keyName);
    if (binding.code == SDL_SCANCODE_UNKNOWN) {
        logger.error() << "Detected as keyboard input type. Incorrect keyName: " << keyName;
        return;
    }
    bindings.emplace(bindName, binding);
}
