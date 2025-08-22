#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/layout.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initSettings(Engine& engine) {
    auto settings = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto controls = std::make_unique<Button>(BTN_SIZE, U"Controls");
    
    back->addCallback([container = settings.get()] { container->close(); });
    controls->addCallback([&] { engine.getGUI().addOverlaped(frontend::initControls()); });

    settings->addNode(back.release());
    settings->addNode(controls.release());
    settings->arrange();
    return settings;
}
