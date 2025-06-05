#include "engine/engine.hpp"
#include "debug/logger.hpp"

static debug::Logger logger("main");

int main(int argc, char* argv[]) {
    debug::Logger::init("latest_log.txt");

    try {
        Engine engine("Turret_2.0.0");
        engine.run();
    }
    catch(const std::exception& exception) {
        logger.error() << exception.what();
    }

    return 0;
}
