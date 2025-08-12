#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initMenu(Engine& engine) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto save = std::make_unique<Button>(BTN_SIZE, U"Save");
    auto settings = std::make_unique<Button>(BTN_SIZE, U"Settings");
    auto exit = std::make_unique<Button>(BTN_SIZE, U"Exit to menu");

    back->addCallback([container = menu.get()] { container->close(); });
    save->addCallback([&] { engine.getGUI().addOverlaped(frontend::initWorldSaving()); });
    settings->addCallback([&] { engine.getGUI().addOverlaped(frontend::initSettings(engine)); });
    exit->addCallback([&] { engine.openMainMenu(); });

    menu->addNode(back.release());
    menu->addNode(save.release());
    menu->addNode(settings.release());
    menu->addNode(exit.release());
    menu->arrange();
    return menu;
}
