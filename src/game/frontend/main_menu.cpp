#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initMainMenu(Engine& engine) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto startGame = std::make_unique<Button>(BTN_SIZE, U"Start game");
    auto loadGame  = std::make_unique<Button>(BTN_SIZE, U"Load game");
    auto editor    = std::make_unique<Button>(BTN_SIZE, U"Editor");
    auto settings  = std::make_unique<Button>(BTN_SIZE, U"Settings");
    auto exit      = std::make_unique<Button>(BTN_SIZE, U"Exit game");
    
    startGame->addCallback([&] { engine.getGUI().addOverlaped(frontend::initWorldProperties(engine)); });
    loadGame->addCallback([&] { engine.getGUI().addOverlaped(frontend::initWorldLoading(engine)); });
    //editor->addCallback([&] { state = EngineState::map_editor; });
    settings->addCallback([&] { engine.getGUI().addOverlaped(frontend::initSettings(engine)); });
    exit->addCallback([&] { engine.closeGame(); });

    menu->addNode(startGame.release());
    menu->addNode(loadGame.release());
    menu->addNode(editor.release());
    menu->addNode(settings.release());
    menu->addNode(exit.release());
    
    menu->arrange();
    return menu;
}
