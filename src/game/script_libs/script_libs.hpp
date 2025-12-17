#pragma once
#include "engine/debug/logger.hpp"
#include "engine/engine.hpp"
#include "engine/scripting/scripting.hpp"

namespace script_libs {
    void registerInput(const ScriptsHandler& scriptsHandler);
    void registerPlayer(const ScriptsHandler& scriptsHandler);
    void registerUtil(const ScriptsHandler& scriptsHandler);
    void registerWorld(const ScriptsHandler& scriptsHandler);

    inline void registerScripts(const ScriptsHandler& scriptsHandler) {
        registerInput(scriptsHandler);
        registerPlayer(scriptsHandler);
        registerUtil(scriptsHandler);
        registerWorld(scriptsHandler);
    }

    inline debug::Logger logger("scripts_libs");
    inline Camera* camera;
    inline GUI*    gui;
    inline Input*  input;
    inline World*  world;
    inline PlayerController* playerController;

    inline void initNewGame(Engine& engine) {
        camera = &engine.getCamera();
        gui    = &engine.getGUI();
        input  = &engine.getMainWindow().getInput();
        world  = &engine.getWorld();
        playerController = &engine.getPlayerController();
    }

    [[noreturn]] inline void logAndThrow(const std::string& message) noexcept(false) {
        logger.error() << message;
        throw std::runtime_error("");
    }
}
