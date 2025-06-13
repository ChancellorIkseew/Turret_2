#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/widgets/layout.hpp"

std::unique_ptr<Container> frontend::initSettings() {
    auto settings = std::make_unique<Container>(Align::right | Align::up, Orientation::vertical);

    auto graphics = std::make_unique<Layout>(Orientation::vertical);

    auto fps = std::make_unique<Label>(U"FPS");
    auto fullscreen = std::make_unique<Label>(U"Fullscreen");

    graphics->addNode(fps.release());
    graphics->addNode(fullscreen.release());
    graphics->arrange();

    settings->addNode(graphics.release());
    settings->arrange();
    return settings;
}
