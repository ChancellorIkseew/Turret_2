#pragma once
#include "engine/engine.hpp"
#include "engine/scripting/scripting.hpp"

namespace script_libs {
    void registerInput(const ScriptsHandler& scriptsHandler);
    void registerUtil(const ScriptsHandler& scriptsHandler);
    void registerWorld(const ScriptsHandler& scriptsHandler);

    inline void registerScripts(const ScriptsHandler& scriptsHandler) {
        registerInput(scriptsHandler);
        registerUtil(scriptsHandler);
        registerWorld(scriptsHandler);
    }

    inline Camera* camera;
    inline GUI*    gui;
    inline Input*  input;
    inline World*  world;

    inline void initNewGame(Engine& engine) {
        camera = &engine.getCamera();
        gui    = &engine.getGUI();
        input  = &engine.getMainWindow().getInput();
        world  = &engine.getWorld();
    }
}
