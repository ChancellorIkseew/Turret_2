#include "settings.hpp"
//
#include "engine/debug/logger.hpp"
#include "engine/engine.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/window/window.hpp"

static debug::Logger logger("settings");

void Settings::writeSettings() {
    tin::Data data;
    data.emplace("FPS", std::to_string(display.FPS));
    data.emplace("lang", gui.lang);
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
    gui.lang = data.getString("lang").value_or("en_US");
}

void Settings::aplySettings(Engine& engine) {
    engine.getMainWindow().setFPS(display.FPS);
}
