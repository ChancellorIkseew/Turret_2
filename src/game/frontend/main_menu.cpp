#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initMainMenu(Engine& engine) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto startGame = menu->addNode(new Button(BTN_SIZE, U"Start game"));
    auto loadGame  = menu->addNode(new Button(BTN_SIZE, U"Load game"));
    auto editor    = menu->addNode(new Button(BTN_SIZE, U"Editor"));
    auto settings  = menu->addNode(new Button(BTN_SIZE, U"Settings"));
    auto exit      = menu->addNode(new Button(BTN_SIZE, U"Exit game"));
    
    startGame->addCallback([&] { engine.getGUI().addOverlaped(frontend::initWorldProperties(engine)); });
    loadGame->addCallback([&] { engine.getGUI().addOverlaped(frontend::initWorldLoading(engine)); });
    //editor->addCallback([&] { state = EngineState::map_editor; });
    settings->addCallback([&] { engine.getGUI().addOverlaped(frontend::initSettings(engine)); });
    exit->addCallback([&] { engine.closeGame(); });
    
    menu->arrange();
    return menu;
}
