#include "settings.hpp"
//
#include "engine/debug/console.hpp"
#include "engine/debug/logger.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/window/window.hpp"

Settings::_Audio Settings::audio;
Settings::_Display Settings::display;
Settings::_Gameplay Settings::gameplay;
Settings::_GUI Settings::gui;
static debug::Logger logger("settings");

#define key_val(value) #value, value

void Settings::writeSettings() {
    tin::Data data;
    data.emplace(key_val(audio.master));
    data.emplace(key_val(audio.world));
    data.emplace(key_val(audio.ui));
    data.emplace(key_val(audio.music));
    data.emplace(key_val(audio.toggleSound));
    data.emplace(key_val(audio.muteInBackground));
    //
    data.emplace(key_val(display.FPS));
    data.emplace(key_val(display.ySinc));
    data.emplace(key_val(display.fullscreen));
    //
    data.emplace(key_val(gameplay.cameraInertia));
    data.emplace(key_val(gameplay.pauseInBackground));
    data.emplace(key_val(gameplay.pauseOnWorldOpen));
    data.emplace(key_val(gameplay.showHitboxes));
    data.emplace(key_val(gameplay.showParticles));
    data.emplace(key_val(gameplay.maxParticles));
    //
    data.emplace(key_val(gui.lang));
    data.emplace(key_val(gui.scale));
    data.emplace(key_val(gui.customCursor));
    data.emplace(key_val(gui.showConsole));
    tin::write("settings.tin", data);
}

void Settings::readSettings() {
    const tin::Data data = tin::read("settings.tin");
    data.get(key_val(audio.master), 50U);
    data.get(key_val(audio.world), 100U);
    data.get(key_val(audio.ui), 100U);
    data.get(key_val(audio.music), 100U);
    data.get(key_val(audio.toggleSound), true);
    data.get(key_val(audio.muteInBackground), true);
    //
    data.get(key_val(display.FPS), 60U);
    data.get(key_val(display.ySinc), true);
    data.get(key_val(display.fullscreen), false);
    //
    data.get(key_val(gameplay.cameraInertia), true);
    data.get(key_val(gameplay.pauseInBackground), false);
    data.get(key_val(gameplay.pauseOnWorldOpen), false);
    data.get(key_val(gameplay.showHitboxes), false);
    data.get(key_val(gameplay.showParticles), true);
    data.get(key_val(gameplay.maxParticles), 10000U);
    //
    data.get(key_val(gui.lang), std::string("en_US"));
    data.get(key_val(gui.scale), 1U);
    data.get(key_val(gui.customCursor), true);
    data.get(key_val(gui.showConsole), false);
    //
    if (data.empty()) {
        writeSettings();
        logger.info() << "Saved file with default settings. File: settings.tin";
    }
}

void Settings::applySettings(Engine& engine) {
    float master = static_cast<float>(audio.master) / 100.f;
    engine.getAssets().getAudio().setMasterVolume(audio.toggleSound ? master : 0.0f);
    engine.getAssets().getAudio().setWorldVolume(static_cast<float>(audio.world) / 100.f);
    engine.getAssets().getAudio().setUIVolume(static_cast<float>(audio.ui) / 100.f);
    engine.getAssets().getAudio().setMusicVolume(static_cast<float>(audio.music) / 100.f);
    engine.getAssets().getAudio().updateVolume();
    // "muteInBackground" implemented in game_session.cpp
    //
    engine.getMainWindow().setFPS(display.FPS);
    engine.getMainWindow().getRenderer().setYSincMode(display.ySinc ? YSincMode::adaptive : YSincMode::immediate);
    engine.getMainWindow().setFullscreen(display.fullscreen);
    // "cameraInertia" not imlemented
    // "pauseOnWorldOpen" implemented in engine.cpp
    // "showHitboxes" implemented in mobs_system.cpp
    // "lang" implemented in gui and main_canvas
    // "guiScale" implemented in gui.cpp and fr_gui.cpp
    engine.getMainWindow().getCursor().setType(gui.customCursor ? CursorType::arrow : CursorType::OS_default);
    debug::Console::setVisible(Settings::gui.showConsole);
}
