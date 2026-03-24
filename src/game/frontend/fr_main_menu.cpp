#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"

constexpr Point BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initMainMenu(Engine& engine) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto startGame = menu->addNode(new Button(BTN_SIZE, "Start game"));
    auto loadGame  = menu->addNode(new Button(BTN_SIZE, "Load game"));
    auto editor    = menu->addNode(new Button(BTN_SIZE, "Editor"));
    auto settings  = menu->addNode(new Button(BTN_SIZE, "Settings"));
    auto exit      = menu->addNode(new Button(BTN_SIZE, "Exit game"));
    
    startGame->addCallback([&] { engine.getGUI().addToOverlay(frontend::initWorldProperties(engine)); });
    loadGame ->addCallback([&] { engine.getGUI().addToOverlay(frontend::initWorldLoading(engine)); });
    editor   ->addCallback([&] { engine.createWorldInEditor(); });
    settings ->addCallback([&] { engine.getGUI().addToOverlay(frontend::initSettings(engine)); });
    exit     ->addCallback([&] { engine.closeGame(); });
    
    return menu;
}
