#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include "app.hpp"
#include "engine/gui/gui.hpp"

constexpr Point BTN_SIZE(200, 50);

std::unique_ptr<Container> frontend::initMainMenu(App& app) {
    auto menu = std::make_unique<Container>(Align::center, Orientation::vertical);

    auto startGame = menu->addNode(new Button(BTN_SIZE, tr("Start game")));
    auto loadGame  = menu->addNode(new Button(BTN_SIZE, tr("Load game")));
    auto editor    = menu->addNode(new Button(BTN_SIZE, tr("Editor")));
    auto settings  = menu->addNode(new Button(BTN_SIZE, tr("Settings")));
    auto exit      = menu->addNode(new Button(BTN_SIZE, tr("Exit game")));
    
    startGame->addCallback([&] { app.getGUI().addToOverlay(frontend::initWorldProperties(app)); });
    loadGame ->addCallback([&] { app.getGUI().addToOverlay(frontend::initWorldLoading(app)); });
    editor   ->addCallback([&] { app.createWorldInEditor(); });
    settings ->addCallback([&] { app.getGUI().addToOverlay(frontend::initSettings(app)); });
    exit     ->addCallback([&] { app.closeGame(); });
    
    return menu;
}
