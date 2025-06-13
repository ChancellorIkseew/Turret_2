#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/window/window.hpp"

std::unique_ptr<Container> frontend::initMenu(MainWindow& mainWindow) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto exit = std::make_unique<Button>(200, 50, U"Exit game");
    auto btn2 = std::make_unique<Button>(200, 50, U"Settings");
    auto btn3 = std::make_unique<Button>(200, 50, U"Exit to menu");
    auto btn4 = std::make_unique<Button>(200, 50, U"Save game");

    exit->addCallback([&]() { mainWindow.close(); });

    menu->addNode(exit.release());
    menu->addNode(btn2.release());
    menu->addNode(btn3.release());
    menu->addNode(btn4.release());
    menu->arrange();
    return menu;
}
