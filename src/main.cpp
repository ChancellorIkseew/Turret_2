#include "engine/debug/logger.hpp"
#include "engine/engine.hpp"
#include "engine/settings/settings.hpp"
#include "engine/settings/localization.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/io/folders.hpp"

static debug::Logger logger("main");

int main(int argc, char* argv[]) {
    debug::Logger::init("latest_log.txt");

    try {
        Settings::readSettings();
        Localization::load(tin::read(io::folders::LANG / (Settings::gui.lang + ".tin")).release());
        Controls::readBindings();
        Engine engine("Turret_2.0.11 - pre-alpha", PixelCoord(720, 480));
        Settings::applySettings(engine);
        engine.run();
    }
    catch (const std::exception& exception) {
        logger.error() << exception.what();
        if (Settings::gui.showConsole)
            system("pause");
    }

    return 0;
}
