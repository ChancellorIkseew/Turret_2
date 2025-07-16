#pragma once
#include "engine/scripting/scripting.hpp"

namespace script_libs {
    void registerInput(const ScriptsHandler& scriptsHandler);
    void registerUtil(const ScriptsHandler& scriptsHandler);
    void registerWorld(const ScriptsHandler& scriptsHandler);

    static void registerScripts(const ScriptsHandler& scriptsHandler) {
        registerInput(scriptsHandler);
        registerUtil(scriptsHandler);
        registerWorld(scriptsHandler);
    }
}
