#include "frontend.hpp"
//
#include "widgets/button.hpp"
#include "window/window.hpp"

std::unique_ptr<Container> frontend::initMenu(MainWindow& mainWindow) {
    auto menu = std::make_unique<Container>(Align::centre);

    auto exit = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn2 = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn3 = std::make_unique<Button>(PixelCoord(200, 50));
    auto btn4 = std::make_unique<Button>(PixelCoord(200, 50));

    exit->addCallback([&]() { mainWindow.close(); });

    menu->addNode(exit.release());
    menu->addNode(btn2.release());
    menu->addNode(btn3.release());
    menu->addNode(btn4.release());
    menu->arrange();
    return menu;
}
