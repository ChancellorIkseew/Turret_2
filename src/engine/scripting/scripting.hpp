#pragma once

class asIScriptEngine;
class asIScriptContext;
class asIScriptFunction;

class ScriptsHandler {
    asIScriptEngine* engine;
    asIScriptContext* context;
    asIScriptFunction* mainLoop = nullptr;
public:
    ScriptsHandler();
    ~ScriptsHandler();

    asIScriptEngine* getRaw() const { return engine; }
    void execute() const;
    void load();
};
