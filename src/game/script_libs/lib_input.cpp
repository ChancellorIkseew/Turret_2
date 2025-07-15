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

void script_libs::registerInput(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("bool input_active(string bindName)", asFunctionPtr(active));
    scriptsHandler.registerFunction("bool input_jactive(string bindName)", asFunctionPtr(jactive));
    scriptsHandler.registerFunction("void input_addBinding(string bindName, string keyName)", asFunctionPtr(addBinding));
}
