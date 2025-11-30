#include "settings.hpp"
//
#include "engine/debug/console.hpp"
#include "engine/debug/logger.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/window/window.hpp"

using namespace t1_sp;
static debug::Logger logger("settings");

void Settings::writeSettings() {
    tin::Data data;
    data.emplace("FPS", std::to_string(display.FPS));
    data.emplace("fullscreen", std::to_string(display.fullscreen));
    //
    data.emplace("camera_inertia", std::to_string(gameplay.cameraInertia));
    data.emplace("pause_on_world_open", std::to_string(gameplay.pauseOnWorldOpen));
    data.emplace("show_hitboxes", std::to_string(gameplay.showHitboxes));
    //
    data.emplace("lang", gui.lang);
    data.emplace("custom_cursor", std::to_string(gui.customCursor));
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
    gameplay.cameraInertia = data.getBool("camera_inertia").value_or(true);
    gameplay.pauseOnWorldOpen = data.getBool("pause_on_world_open").value_or(false);
    gameplay.showHitboxes = data.getBool("show_hitboxes").value_or(false);
    //
    gui.lang = data.getString("lang").value_or("en_US");
    gui.customCursor = data.getBool("custom_cursor").value_or(true);
    gui.showConsole = data.getBool("show_console").value_or(false);
}

void Settings::aplySettings(Engine& engine) { 
    engine.getMainWindow().setFPS(display.FPS);
    engine.getMainWindow().setFullscreen(display.fullscreen);
    // "camera_inertia" not imlemented
    // "pause_on_world_open": implemented in engine.cpp
    // "show_hitboxes" implemented in mobs_system.cpp
    GUI::loadLangTranslations(gui.lang);
    engine.getMainWindow().setCursor(gui.customCursor ? CursorType::arrow : CursorType::OS_default);
    debug::Console::setVisible(Settings::gui.showConsole);
}
