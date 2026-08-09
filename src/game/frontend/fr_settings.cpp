#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/layout.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"

constexpr Point BTN_SIZE(200, 50);

std::unique_ptr<Container> frontend::initSettings(Engine& engine) {
    auto settings = std::make_unique<Container>(Align::center, Orientation::vertical);

    auto back     = settings->addNode(new Button(BTN_SIZE, tr("Back")));
    auto gameplay = settings->addNode(new Button(BTN_SIZE, tr("Gameplay")));
    auto controls = settings->addNode(new Button(BTN_SIZE, tr("Controls")));
    auto graphics = settings->addNode(new Button(BTN_SIZE, tr("Graphics")));
    auto audio    = settings->addNode(new Button(BTN_SIZE, tr("Audio")));
    auto gui      = settings->addNode(new Button(BTN_SIZE, tr("GUI")));
    auto language = settings->addNode(new Button(BTN_SIZE, tr("Language")));
    
    back    ->addCallback([container = settings.get()] { container->close(); });
    gameplay->addCallback([&] { engine.getGUI().addToOverlay(frontend::initGameplay(engine)); });
    controls->addCallback([&] { engine.getGUI().addToOverlay(frontend::initControls(engine)); });
    graphics->addCallback([&] { engine.getGUI().addToOverlay(frontend::initGraphics(engine)); });
    audio   ->addCallback([&] { engine.getGUI().addToOverlay(frontend::initAudio(engine)); });
    gui     ->addCallback([&] { engine.getGUI().addToOverlay(frontend::initGUI(engine)); });
    language->addCallback([&] { engine.getGUI().addToOverlay(frontend::initLanguages(engine)); });

    return settings;
}
