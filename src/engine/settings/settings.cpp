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
    data.emplace("volume_master", audio.master);
    data.emplace("volume_world",  audio.world);
    data.emplace("volume_ui",     audio.ui);
    data.emplace("volume_music",  audio.music);
    data.emplace("toggle_sound",  audio.toggleSound);
    //
    data.emplace("FPS",        display.FPS);
    data.emplace("fullscreen", display.fullscreen);
    //
    data.emplace("camera_inertia",      gameplay.cameraInertia);
    data.emplace("pause_on_world_open", gameplay.pauseOnWorldOpen);
    data.emplace("show_hitboxes",       gameplay.showHitboxes);
    //
    data.emplace("lang",          gui.lang);
    data.emplace("custom_cursor", gui.customCursor);
    data.emplace("show_console",  gui.showConsole);
    tin::write("settings.tin", data);
}

void Settings::readSettings() {
    tin::Data data = tin::read("settings.tin");
    if (data.empty()) {
        writeSettings();
        logger.info() << "Saved file with default settings. File: settings.tin";
        return;
    }
    data.get("volume_master", audio.master, 50U);
    data.get("volume_world",  audio.world, 100U);
    data.get("volume_ui",     audio.world, 100U);
    data.get("volume_music",  audio.world, 100U);
    data.get("toggle_sound",  audio.toggleSound, false);
    //
    data.get("FPS",        display.FPS, 60U);
    data.get("fullscreen", display.fullscreen, false);
    //
    data.get("camera_inertia",      gameplay.cameraInertia, true);
    data.get("pause_on_world_open", gameplay.pauseOnWorldOpen, false);
    data.get("show_hitboxes",       gameplay.showHitboxes, false);
    //
    data.get("lang",          gui.lang, std::string("en_US"));
    data.get("custom_cursor", gui.customCursor, true);
    data.get("show_console",  gui.showConsole, false);
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
