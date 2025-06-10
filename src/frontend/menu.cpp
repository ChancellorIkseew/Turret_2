#include "frontend.hpp"
//
#include "widgets/button.hpp"

std::unique_ptr<Container> frontend::initMenu() {
    auto menu = std::make_unique<Container>(Align::centre);

    auto btn = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn2 = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn3 = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn4 = std::make_unique<Button>(PixelCoord(200, 50));

    menu->addNode(btn.release());
    menu->addNode(btn2.release());
    menu->addNode(btn3.release());
    menu->addNode(btn4.release());
    menu->arrange();
    return menu;
}
