#include "script_libs.hpp"
//
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

static bool active(const std::string& bindName) {
    return script_libs::input->active(bindName.c_str());
}
static bool jactive(const std::string& bindName) {
    return script_libs::input->jactive(bindName.c_str());
}
static void addBinding(const std::string& bindName, const std::string& keyName) {
    Controls::addBinding(bindName, keyName);
}
static int getMouseWorldPosX() {
    auto position = script_libs::camera->fromScreenToMap(script_libs::input->getMouseCoord());
    return static_cast<int>(position.x);
}
static int getMouseWorldPosY() {
    auto position = script_libs::camera->fromScreenToMap(script_libs::input->getMouseCoord());
    return static_cast<int>(position.y);
}
static int getMouseTileX() {
    auto position = script_libs::camera->fromScreenToMap(script_libs::input->getMouseCoord());
    return t1::tile(position.x);
}
static int getMouseTileY() {
    auto position = script_libs::camera->fromScreenToMap(script_libs::input->getMouseCoord());
    return t1::tile(position.y);
}

void script_libs::registerInput(const ScriptsHandler& scriptsHandler) {
    scriptsHandler.registerFunction("bool input_active(string bindName)", asFunctionPtr(active));
    scriptsHandler.registerFunction("bool input_jactive(string bindName)", asFunctionPtr(jactive));
    scriptsHandler.registerFunction("void input_addBinding(string bindName, string keyName)", asFunctionPtr(addBinding));
    scriptsHandler.registerFunction("int input_getMouseWorldPosX()", asFunctionPtr(getMouseWorldPosX));
    scriptsHandler.registerFunction("int input_getMouseWorldPosY()", asFunctionPtr(getMouseWorldPosY));
    scriptsHandler.registerFunction("int input_getMouseTileX()", asFunctionPtr(getMouseTileX));
    scriptsHandler.registerFunction("int input_getMouseTileY()", asFunctionPtr(getMouseTileY));
}
