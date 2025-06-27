#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/window/window.hpp"
#include "engine/engine_state.hpp"

std::unique_ptr<Container> frontend::initMainMenu(EngineState& state) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto startGame = std::make_unique<Button>(200, 50, U"Start game");
    auto editor    = std::make_unique<Button>(200, 50, U"Editor");
    auto settings  = std::make_unique<Button>(200, 50, U"Settings");
    auto exit      = std::make_unique<Button>(200, 50, U"Exit game");

    startGame->addCallback([&]() { state = EngineState::gameplay; });
    editor->addCallback([&]() { state = EngineState::map_editor; });
    //settings->addCallback([&]() { mainWindow.close(); });
    exit->addCallback([&]() { state = EngineState::exit; });

    menu->addNode(startGame.release());
    menu->addNode(editor.release());
    menu->addNode(settings.release());
    menu->addNode(exit.release());
    
    menu->arrange();
    return menu;
}
