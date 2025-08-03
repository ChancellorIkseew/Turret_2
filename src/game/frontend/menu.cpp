#include "frontend.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initMenu(EngineState& state, GUI& gui) {
    auto menu = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto save = std::make_unique<Button>(BTN_SIZE, U"Save");
    auto settings = std::make_unique<Button>(BTN_SIZE, U"Settings");
    auto exit = std::make_unique<Button>(BTN_SIZE, U"Exit to menu");

    back->addCallback(std::bind(&Container::close, menu.get()));
    settings->addCallback([&]() { gui.addOverlaped(frontend::initSettings(gui)); });
    exit->addCallback([&]() { state = EngineState::main_menu; });

    menu->addNode(back.release());
    menu->addNode(save.release());
    menu->addNode(settings.release());
    menu->addNode(exit.release());
    menu->arrange();
    return menu;
}
