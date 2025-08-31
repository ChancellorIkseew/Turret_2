#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/checkbox.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/utf8/utf8.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

static void aplySettings(Form* fps, Checkbox* fullscreen, Engine& engine) {
    Settings::display.FPS = validator::toUint32(fps->getText()).value_or(60U);
    Settings::display.fullscreen = fullscreen->getValue();
    Settings::aplySettings(engine);
    Settings::writeSettings();
}

std::unique_ptr<Container> frontend::initGraphics(Engine& engine) {
    auto graphics = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto fps        = graphics->addNode(new Layout(Orientation::horizontal));
    auto fullscreen = graphics->addNode(new Layout(Orientation::horizontal));
    auto lower      = graphics->addNode(new Layout(Orientation::horizontal));

    fps       ->addNode(new Label(U"FPS"));
    fullscreen->addNode(new Label(U"fullscreen"));
    auto fpsF        = fps->addNode(new Form(Settings::display.FPS));
    auto fullscreenC = fullscreen->addNode(new Checkbox(Settings::display.fullscreen));

    auto back = lower->addNode(new Button(BTN_SIZE, U"Back"));
    auto aply = lower->addNode(new Button(BTN_SIZE, U"Aply"));
    back->addCallback([container = graphics.get()] { container->close(); });
    aply->addCallback([=, &engine] { aplySettings(fpsF, fullscreenC, engine); });

    graphics->arrange();
    return graphics;
}
