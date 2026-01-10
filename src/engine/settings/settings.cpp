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
    data.emplace("volume_master", std::to_string(audio.master));
    data.emplace("volume_world",  std::to_string(audio.world));
    data.emplace("volume_ui",     std::to_string(audio.ui));
    data.emplace("volume_music",  std::to_string(audio.music));
    data.emplace("toggle_sound",  std::to_string(audio.toggleSound));
    //
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
    audio.master = data.getUint16("volume_master").value_or(50U);
    audio.world  = data.getUint16("volume_world").value_or(100U);
    audio.ui     = data.getUint16("volume_ui").value_or(100U);
    audio.music  = data.getUint16("volume_music").value_or(100U);
    audio.toggleSound = data.getBool("toggle_sound").value_or(true);
    //
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

void Settings::applySettings(Engine& engine) {
    float master = static_cast<float>(audio.master) / 100.f;
    engine.getAssets().getAudio().setMasterVolume(audio.toggleSound ? master : 0.0f);
    engine.getAssets().getAudio().setWorldVolume(static_cast<float>(audio.world) / 100.f);
    engine.getAssets().getAudio().setUIVolume(static_cast<float>(audio.ui) / 100.f);
    engine.getAssets().getAudio().setMusicVolume(static_cast<float>(audio.music) / 100.f);
    engine.getAssets().getAudio().updateVolume();
    //
    engine.getMainWindow().setFPS(display.FPS);
    engine.getMainWindow().setFullscreen(display.fullscreen);
    // "camera_inertia" not imlemented
    // "pause_on_world_open": implemented in engine.cpp
    // "show_hitboxes" implemented in mobs_system.cpp
    GUI::loadLangTranslations(gui.lang);
    engine.getMainWindow().getCursor().setType(gui.customCursor ? CursorType::arrow : CursorType::OS_default);
    debug::Console::setVisible(Settings::gui.showConsole);
}
