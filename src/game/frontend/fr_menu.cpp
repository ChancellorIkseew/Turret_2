#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initMenu(Engine& engine) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back     = menu->addNode(new Button(BTN_SIZE, U"Back"));
    auto save     = menu->addNode(new Button(BTN_SIZE, U"Save"));
    auto settings = menu->addNode(new Button(BTN_SIZE, U"Settings"));
    auto exit     = menu->addNode(new Button(BTN_SIZE, U"Exit to menu"));

    back    ->addCallback([container = menu.get()] { container->close(); });
    save    ->addCallback([&] { engine.getGUI().addOverlaped(frontend::initWorldSaving(engine)); });
    settings->addCallback([&] { engine.getGUI().addOverlaped(frontend::initSettings(engine)); });
    exit    ->addCallback([&] { engine.openMainMenu(); });

    menu->arrange();
    return menu;
}
