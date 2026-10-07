#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/layout.hpp>
#include "app.hpp"
#include "engine/gui/gui.hpp"

constexpr Point BTN_SIZE(200, 50);

std::unique_ptr<Container> frontend::initSettings(App& app) {
    auto settings = std::make_unique<Container>(Align::center, Orientation::vertical);

    auto back     = settings->addNode(new Button(BTN_SIZE, tr("Back")));
    auto gameplay = settings->addNode(new Button(BTN_SIZE, tr("Gameplay")));
    auto controls = settings->addNode(new Button(BTN_SIZE, tr("Controls")));
    auto graphics = settings->addNode(new Button(BTN_SIZE, tr("Graphics")));
    auto audio    = settings->addNode(new Button(BTN_SIZE, tr("Audio")));
    auto gui      = settings->addNode(new Button(BTN_SIZE, tr("GUI")));
    auto language = settings->addNode(new Button(BTN_SIZE, tr("Language")));
    
    back    ->addCallback([container = settings.get()] { container->close(); });
    gameplay->addCallback([&] { app.getGUI().addToOverlay(frontend::initGameplay(app)); });
    controls->addCallback([&] { app.getGUI().addToOverlay(frontend::initControls(app)); });
    graphics->addCallback([&] { app.getGUI().addToOverlay(frontend::initGraphics(app)); });
    audio   ->addCallback([&] { app.getGUI().addToOverlay(frontend::initAudio(app)); });
    gui     ->addCallback([&] { app.getGUI().addToOverlay(frontend::initGUI(app)); });
    language->addCallback([&] { app.getGUI().addToOverlay(frontend::initLanguages(app)); });

    return settings;
}
