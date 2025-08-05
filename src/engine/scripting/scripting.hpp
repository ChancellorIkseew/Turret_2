#pragma once
#include <ANGEL_SCRIPT/angelscript.h>

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
};
