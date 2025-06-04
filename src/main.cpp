#include "engine/engine.hpp"

int main(int argc, char* argv[]) {
    try {
        Engine engine("Turret_2.0.0");
        engine.run();
    }
    catch(std::exception exception) {

    }

    return 0;
}