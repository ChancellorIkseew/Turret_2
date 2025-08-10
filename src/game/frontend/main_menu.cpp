#include "frontend.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initMainMenu(WorldProperties& properties, EngineState& state, GUI& gui) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto startGame = std::make_unique<Button>(BTN_SIZE, U"Start game");
    auto loadGame  = std::make_unique<Button>(BTN_SIZE, U"Load game");
    auto editor    = std::make_unique<Button>(BTN_SIZE, U"Editor");
    auto settings  = std::make_unique<Button>(BTN_SIZE, U"Settings");
    auto exit      = std::make_unique<Button>(BTN_SIZE, U"Exit game");
    //state = EngineState::gameplay;
    startGame->addCallback([&] { gui.addOverlaped(frontend::initWorldProperties(properties, state)); });
    loadGame->addCallback([&] { gui.addOverlaped(frontend::initWorldLoading(state)); });
    editor->addCallback([&] { state = EngineState::map_editor; });
    settings->addCallback([&] { gui.addOverlaped(frontend::initSettings(gui)); });
    exit->addCallback([&] { state = EngineState::exit; });

    menu->addNode(startGame.release());
    menu->addNode(loadGame.release());
    menu->addNode(editor.release());
    menu->addNode(settings.release());
    menu->addNode(exit.release());
    
    menu->arrange();
    return menu;
}
