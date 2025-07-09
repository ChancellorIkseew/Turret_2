#include "controls.hpp"
//
#include <SDL3/SDL_keyboard.h>
#include <TOML/cpptoml.h>
#include <array>
#include <filesystem>
#include "engine/debug/logger.hpp"
#include "utf8/utf8.hpp"

constexpr std::array<cString, 6> MOUSE_CODES{ "_", LMB, MidMB, RMB, "Mouse_4", "Mouse_5"};
static std::filesystem::path controls("res/controls.toml");
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
    return utf8::fromConstCharToU32String(getKeyNameCStr(bindings.at(bindName)));
}

void Controls::rebind(const std::string& bindName, const Binding binding) {
    bindings.insert_or_assign(bindName, binding);
}

void Controls::writeBindings() {
    std::ofstream fout(controls);
    if (!fout.is_open()) {
        logger.error() << "Could not open file to write. File: " << controls;
        return;
    }  
    auto table = cpptoml::make_table();
    for (const auto& [bindName, binding] : bindings) {
        if (binding.changable)
            table->insert(bindName, getKeyNameCStr(binding));
    }
    cpptoml::toml_writer writer(fout, " ");
    writer.visit(*table, false);
    logger.info() << "Writen file: " << controls;
}

void Controls::readBindings() {
    try {
        const auto table = cpptoml::parse_file(controls.string());
        for (const auto& [bindName, _] : *table) {
            const std::string keyName = table->get_as<std::string>(bindName).value_or("");
            if (!keyName.empty())
                rebind(bindName, getBinding(keyName));
        }
        logger.info() << "Readen file: " << controls;
    }
    catch (cpptoml::parse_exception exception) {
        logger.error() << exception.what();
        logger.info() << "Trying to save file with default settings. File: " << controls;
        writeBindings();
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
