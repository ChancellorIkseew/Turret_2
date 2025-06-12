#include "frontend.hpp"
//
#include "widgets/button.hpp"

std::unique_ptr<Container> frontend::initSettings() {
    auto settings = std::make_unique<Container>(Align::right | Align::up);

    auto exit = std::make_unique<Button>(200, 50, U"Exit game");
    auto btn2 = std::make_unique<Button>(200, 50, U"Settings");
    auto btn3 = std::make_unique<Button>(200, 50, U"Exit to menu");
    auto btn4 = std::make_unique<Button>(200, 50, U"Save game");

    settings->addNode(exit.release());
    settings->addNode(btn2.release());
    settings->addNode(btn3.release());
    settings->addNode(btn4.release());
    settings->arrange();
    return settings;
}
