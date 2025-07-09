#include "controls.hpp"
//
#include "SDL3/SDL_keyboard.h"
#include <TOML/cpptoml.h>
#include <filesystem>
#include "engine/debug/logger.hpp"
#include "utf8/utf8.hpp"

static std::filesystem::path controls("res/controls.toml");
static debug::Logger logger("bindings");

std::u32string Controls::getKeyName(const BindName bindName) {
    return getKeyName(bindings.at(bindName).code);
}
std::u32string Controls::getKeyName(const int code) {
    return utf8::fromConstCharToU32String(SDL_GetScancodeName(static_cast<SDL_Scancode>(code)));
}

void Controls::rebind(const BindName bindName, const BindingInfo binding) {
    if (bindings.contains(bindName))
        bindings.erase(bindName); // Binding has no copy constructor.
    bindings.emplace(bindName, Binding(binding.code, binding.inputType));
}

void Controls::writeBindings() {
    std::ofstream fout(controls);
    if (!fout.is_open())
        return;
    auto table = cpptoml::make_table();
    for (const auto& [enumName, strName] : bindNames) {
        if (bindings.contains(enumName)) {
            getKeyName(enumName);
            table->insert(strName, "keyName");
        }
    }
    cpptoml::toml_writer writer(fout, " ");
    writer.visit(*table, false);
}

static BindName getKeyEnumName(std::string name) {
    return static_cast<BindName>(SDL_GetScancodeFromName(name.c_str()));
}

void Controls::readBindings() {
    const auto& table = cpptoml::parse_file(controls.string());
    for (const auto& [strBindName, strKeyName] : *table) {
        const auto bindName = getKeyEnumName(strBindName);
        if (bindings.contains(bindName)) {
            /*
            bindings.erase(bindName);
            bindings.emplace(bindName, );
            */
        }
    }
}

BindName Controls::addBinding(cString strCode, InputType inputType) {
    int code = SDL_GetScancodeFromName(strCode);
    if (code == SDL_SCANCODE_UNKNOWN) {
        logger.error() << "Incorrect scancode name: " << strCode;
        return BindName::Count;
    }
    for (BindName i = BindName::Reserved_0; i <= BindName::Reserved_20; ++i) {
        if (bindings.contains(i))
            continue;
        bindings.emplace(i, Binding(code, inputType));
        return i;
    }
    logger.error() << "No more reserved bindName avaliable.";
    return BindName::Count;
}
