#include "frontend.hpp"
//
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/checkbox.hpp"
#include "engine/widgets/label.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

static void aplySettings(Checkbox* customCursor, Checkbox* showConsole, Engine& engine) {
    Settings::gui.customCursor = customCursor->getValue();
    Settings::gui.showConsole = showConsole->getValue();
    Settings::aplySettings(engine);
    Settings::writeSettings();
}

std::unique_ptr<Container> frontend::initGUI(Engine& engine) {
    auto gui = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto customCursor = gui->addNode(new Layout(Orientation::horizontal));
    auto showConsole  = gui->addNode(new Layout(Orientation::horizontal));
    auto lower        = gui->addNode(new Layout(Orientation::horizontal));

    customCursor->addNode(new Label(U"custom cursor"));
    showConsole ->addNode(new Label(U"show console"));
    auto customCursorC = customCursor->addNode(new Checkbox(Settings::gui.customCursor));
    auto showConsoleC  = showConsole->addNode(new Checkbox(Settings::gui.showConsole));

    auto back = lower->addNode(new Button(BTN_SIZE, U"Back"));
    auto aply = lower->addNode(new Button(BTN_SIZE, U"Aply"));
    back->addCallback([container = gui.get()] { container->close(); });
    aply->addCallback([=, &engine] { aplySettings(customCursorC, showConsoleC, engine); });

    gui->arrange();
    return gui;
}