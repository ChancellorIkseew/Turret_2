#include "frontend.hpp"
//
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/layout.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initSettings(GUI& gui) {
    auto settings = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto controls = std::make_unique<Button>(BTN_SIZE, U"Contorls");
    
    back->addCallback(std::bind(&Container::close, settings.get()));
    controls->addCallback([&]() { gui.addOverlaped(frontend::initControls()); });

    settings->addNode(back.release());
    settings->addNode(controls.release());
    settings->arrange();
    return settings;
}
