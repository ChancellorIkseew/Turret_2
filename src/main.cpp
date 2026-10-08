#include "app.hpp"
#include "engine/debug/logger.hpp"
#include "engine/settings/settings.hpp"
#include "engine/window/input/controls.hpp"

static debug::Logger logger("main");

int main(int argc, char* argv[]) {
    debug::Logger::init("latest_log.txt");

    try {
        Settings::readSettings();
        Controls::readBindings();
        App app("Turret_2.0.17 - pre-alpha", PixelCoord(720, 480));
        Settings::applySettings(app);
        app.run();
    }
    catch (const std::exception& exception) {
        logger.error(exception.what());
        if (Settings::gui.showConsole)
            system("pause");
    }
    
    return 0;
}
