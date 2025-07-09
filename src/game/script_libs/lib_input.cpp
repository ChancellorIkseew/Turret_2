#include "script_libs.hpp"
//
#include <ANGEL_SCRIPT/angelscript.h>
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

static bool active(const std::string& bindName) {
    return Input::active(bindName.c_str());
}
static bool jactive(const std::string& bindName) {
    return Input::jactive(bindName.c_str());
}
static void addBinding(const std::string& bindName, const std::string& keyName) {
    Controls::addBinding(bindName, keyName);
}

void script_libs::registerInput(ScriptsHandler& scriptsHandler) {
    auto engine = scriptsHandler.getRaw();
    engine->RegisterGlobalFunction("bool input_active(BindName bindName)", asFunctionPtr(active), asCALL_CDECL);
    engine->RegisterGlobalFunction("bool input_jactive(BindName bindName)", asFunctionPtr(jactive), asCALL_CDECL);
    //engine->RegisterGlobalFunction("BindName input_addBinding(string bindName, string keyName)", asFunctionPtr(jactive), asCALL_CDECL);
}
