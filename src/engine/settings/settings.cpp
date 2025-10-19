#include "settings.hpp"
//
#include "engine/debug/console.hpp"
#include "engine/debug/logger.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/window/window.hpp"

static debug::Logger logger("settings");

void Settings::writeSettings() {
    tin::Data data;
    data.emplace("FPS", std::to_string(display.FPS));
    data.emplace("fullscreen", std::to_string(display.fullscreen));
    //
    data.emplace("show_hitboxes", std::to_string(gameplay.showHitboxes));
    //
    data.emplace("lang", gui.lang);
    data.emplace("show_console", std::to_string(gui.showConsole));
    tin::write("settings.tin", data);
}

void Settings::readSettings() {
    tin::Data data = tin::read("settings.tin");
    if (data.empty()) {
        writeSettings();
        logger.info() << "Saved file with default settings. File: settings.tin";
        return;
    }
    display.FPS = data.getUint32("FPS").value_or(60U);
    display.fullscreen = data.getBool("fullscreen").value_or(false);
    //
    gameplay.showHitboxes = data.getBool("show_hitboxes").value_or(false);
    gameplay.cameraInertia = data.getBool("camera_inertia").value_or(true);
    //
    gui.lang = data.getString("lang").value_or("en_US");
    gui.showConsole = data.getBool("show_console").value_or(false);
}

void Settings::aplySettings(Engine& engine) { 
    engine.getMainWindow().setFPS(display.FPS);
    engine.getMainWindow().setFullscreen(display.fullscreen);
    //
    // TODO implement "show hitboxes"
    //
    GUI::loadLangTranslations(gui.lang);
    debug::Console::setVisible(Settings::gui.showConsole);
}
