#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"

std::unique_ptr<Container> frontend::initTimer() {
    auto timer = std::make_unique<Container>(Align::left | Align::up, Orientation::vertical);

    auto startWave = std::make_unique<Button>(200, 50, U"Next wave");
    auto timeToWave = std::make_unique<Label>(U"Next wave in: 10:10");

    timer->addNode(startWave.release());
    timer->addNode(timeToWave.release());
    timer->arrange();
    return timer;
}
