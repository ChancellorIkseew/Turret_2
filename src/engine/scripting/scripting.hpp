#pragma once
#include <ANGEL_SCRIPT/angelscript.h>
#include "config.hpp"

using cString = const char*;

class ScriptsHandler {
    asIScriptEngine* engine;
    asIScriptContext* context;
    asIScriptFunction* mainLoop = nullptr;
public:
    ScriptsHandler();
    ~ScriptsHandler();

    void registerFunction(cString declAS, asSFuncPtr functionPtr) const;
    void execute() const;
    void load();
private:
    t1_disable_copy_and_move(ScriptsHandler)
};
