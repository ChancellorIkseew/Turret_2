#include "frontend.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/widgets/button.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initSaves(EngineState& state) {
    auto saves = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto load = std::make_unique<Button>(BTN_SIZE, U"Load");

    back->addCallback(std::bind(&Container::close, saves.get()));
    load->addCallback([&]() { state = EngineState::gameplay; });

    saves->addNode(back.release());
    saves->addNode(load.release());
    saves->arrange();
    return saves;
}
