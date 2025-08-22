#include "engine/debug/logger.hpp"
#include "engine/engine.hpp"
#include "engine/settings/settings.hpp"
#include "engine/window/input/controls.hpp"

static debug::Logger logger("main");

int main(int argc, char* argv[]) {
    debug::Logger::init("latest_log.txt");

    try {
        Settings::readSettings();
        Controls::readBindings();
        Engine engine("Turret_2.0.0");
        engine.getMainWindow().setFPS(Settings::display.FPS);
        engine.run();
    }
    catch (const std::exception& exception) {
        logger.error() << exception.what();
    }

    return 0;
}
