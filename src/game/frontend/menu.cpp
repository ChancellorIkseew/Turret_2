#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/engine_state.hpp"

std::unique_ptr<Container> frontend::initMenu(EngineState& state) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto save = std::make_unique<Button>(200, 50, U"Save");
    auto settings = std::make_unique<Button>(200, 50, U"Settings");
    auto exit = std::make_unique<Button>(200, 50, U"Exit to menu");

    exit->addCallback([&]() { state = EngineState::main_menu; });

    menu->addNode(save.release());
    menu->addNode(settings.release());
    menu->addNode(exit.release());
    menu->arrange();
    return menu;
}
