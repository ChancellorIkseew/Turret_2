#pragma once
#include "engine/scripting/scripting.hpp"

namespace script_libs {
    void registerInput(ScriptsHandler& scriptsHandler);
    void registerUtil(ScriptsHandler& scriptsHandler);

    static void registerScripts(ScriptsHandler& scriptsHandler) {
        registerInput(scriptsHandler);
        registerUtil(scriptsHandler);
    }
}
