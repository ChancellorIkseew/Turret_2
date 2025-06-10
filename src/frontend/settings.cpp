#include "frontend.hpp"
//
#include "widgets/button.hpp"

std::unique_ptr<Container> frontend::initSettings() {
    auto settings = std::make_unique<Container>(Align::right | Align::up);

    auto exit = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn2 = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn3 = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn4 = std::make_unique<Button>(PixelCoord(200, 50));

    settings->addNode(exit.release());
    settings->addNode(btn2.release());
    settings->addNode(btn3.release());
    settings->addNode(btn4.release());
    settings->arrange();
    return settings;
}
