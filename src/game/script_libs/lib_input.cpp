#include "script_libs.hpp"
//
#include <ANGEL_SCRIPT/angelscript.h>
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

static bool active(const int bindName) {
    return Input::active(static_cast<BindName>(bindName));
}
static bool jactive(const int bindName) {
    return Input::jactive(static_cast<BindName>(bindName));
}
static BindName addBinding(cString strCode, const InputType inputType) {
    return Controls::addBinding(strCode, inputType);
}

static void registerBindNames(ScriptsHandler& scriptsHandler) {
    auto engine = scriptsHandler.getRaw();
    engine->RegisterEnum("InputType");
    engine->RegisterEnumValue("InputType", "keyboard", static_cast<int>(InputType::keyboard));
    engine->RegisterEnumValue("InputType", "mouse", static_cast<int>(InputType::mouse));
    //
    engine->RegisterEnum("BindName");
    for (const auto& [enumName, stringName] : bindNames) {
        engine->RegisterEnumValue("BindName", stringName, static_cast<int>(enumName));
    }
}

void script_libs::registerInput(ScriptsHandler& scriptsHandler) {
    registerBindNames(scriptsHandler);
    auto engine = scriptsHandler.getRaw();
    engine->RegisterGlobalFunction("bool input_active(BindName bindName)", asFunctionPtr(active), asCALL_CDECL);
    engine->RegisterGlobalFunction("bool input_jactive(BindName bindName)", asFunctionPtr(jactive), asCALL_CDECL);
    //engine->RegisterGlobalFunction("BindName input_addBinding(string strCode, InputType inputType)", asFunctionPtr(jactive), asCALL_CDECL);
}
