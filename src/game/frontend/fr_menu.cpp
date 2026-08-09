#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"

constexpr Point BTN_SIZE(200, 50);

std::unique_ptr<Container> frontend::initMenu(Engine& engine) {
    auto menu = std::make_unique<Container>(Align::center, Orientation::vertical);

    auto back     = menu->addNode(new Button(BTN_SIZE, tr("Back")));
    auto save     = menu->addNode(new Button(BTN_SIZE, tr("Save")));
    auto settings = menu->addNode(new Button(BTN_SIZE, tr("Settings")));
    auto exit     = menu->addNode(new Button(BTN_SIZE, tr("Exit to menu")));

    back    ->addCallback([container = menu.get()] { container->close(); });
    save    ->addCallback([&] { engine.getGUI().addToOverlay(frontend::initWorldSaving(engine)); });
    settings->addCallback([&] { engine.getGUI().addToOverlay(frontend::initSettings(engine)); });
    exit    ->addCallback([&] { engine.openMainMenu(); });

    return menu;
}
