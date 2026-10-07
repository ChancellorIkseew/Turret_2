#pragma once
#include "app.hpp"
#include "engine/debug/logger.hpp"
#include "engine/scripting/scripting.hpp"
#include "game/game_session.hpp"

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
    inline Assets* assets;
    inline Camera* camera;
    inline GUI*    gui;
    inline Input*  input;
    inline World*  world;
    inline PlayerController* playerController;
    inline BuiltInScripts* builtInScripts;

    inline void initNewGame(App& app) {
        assets = &app.getAssets();
        camera = &app.getSession().getCamera();
        gui    = &app.getGUI();
        input  = &app.getMainWindow().getInput();
        world  = &app.getSession().getWorld();
        playerController = &app.getSession().getPlayerController();
        builtInScripts   = &app.getSession().getBuiltInScripts();
    }

    [[noreturn]] inline void logAndThrow(const std::string& message) noexcept(false) {
        logger.error(message);
        throw std::runtime_error("");
    }
}
