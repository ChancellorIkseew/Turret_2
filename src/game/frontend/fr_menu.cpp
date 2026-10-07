#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include "app.hpp"
#include "engine/gui/gui.hpp"

constexpr Point BTN_SIZE(200, 50);

std::unique_ptr<Container> frontend::initMenu(App& app) {
    auto menu = std::make_unique<Container>(Align::center, Orientation::vertical);

    auto back     = menu->addNode(new Button(BTN_SIZE, tr("Back")));
    auto save     = menu->addNode(new Button(BTN_SIZE, tr("Save")));
    auto settings = menu->addNode(new Button(BTN_SIZE, tr("Settings")));
    auto exit     = menu->addNode(new Button(BTN_SIZE, tr("Exit to menu")));

    back    ->addCallback([container = menu.get()] { container->close(); });
    save    ->addCallback([&] { app.getGUI().addToOverlay(frontend::initWorldSaving(app)); });
    settings->addCallback([&] { app.getGUI().addToOverlay(frontend::initSettings(app)); });
    exit    ->addCallback([&] { app.openMainMenu(); });

    return menu;
}
