#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/layout.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initSettings(Engine& engine) {
    auto settings = std::make_unique<Container>(Align::centre, Orientation::vertical);

    auto back     = settings->addNode(new Button(BTN_SIZE, U"Back"));
    auto controls = settings->addNode(new Button(BTN_SIZE, U"Controls"));
    auto graphics = settings->addNode(new Button(BTN_SIZE, U"Graphics"));
    auto audio    = settings->addNode(new Button(BTN_SIZE, U"Audio"));
    auto gui      = settings->addNode(new Button(BTN_SIZE, U"GUI"));
    auto language = settings->addNode(new Button(BTN_SIZE, U"Language"));
    
    back    ->addCallback([container = settings.get()] { container->close(); });
    controls->addCallback([&] { engine.getGUI().addOverlaped(frontend::initControls(engine)); });
    graphics->addCallback([&] { engine.getGUI().addOverlaped(frontend::initGraphics(engine)); });
    audio   ->addCallback([&] { engine.getGUI().addOverlaped(frontend::initAudio(engine)); });
    gui     ->addCallback([&] { engine.getGUI().addOverlaped(frontend::initGUI(engine)); });
    language->addCallback([&] { engine.getGUI().addOverlaped(frontend::initLanguages(engine)); });

    settings->arrange();
    return settings;
}
