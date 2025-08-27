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
}

std::unique_ptr<Container> frontend::initGraphics(Engine& engine) {
    auto graphics = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto fps = std::make_unique<Layout>(Orientation::horizontal);
    auto fullscreen = std::make_unique<Layout>(Orientation::horizontal);
    auto lower = std::make_unique<Layout>(Orientation::horizontal);

    auto fpsF = std::make_unique<Form>(Settings::display.FPS);
    auto fullscreenC = std::make_unique<Checkbox>(Settings::display.fullscreen);

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto aply = std::make_unique<Button>(BTN_SIZE, U"Aply");
    back->addCallback([container = graphics.get()] { container->close(); });
    aply->addCallback([fps = fpsF.get(), fullscreen = fullscreenC.get(), &engine] { aplySettings(fps, fullscreen, engine); });

    fps->addNode(new Label(U"FPS"));
    fps->addNode(fpsF.release());
    fullscreen->addNode(new Label(U"fullscreen"));
    fullscreen->addNode(fullscreenC.release());

    lower->addNode(back.release());
    lower->addNode(aply.release());

    graphics->addNode(fps.release());
    graphics->addNode(fullscreen.release());
    graphics->addNode(lower.release());
    graphics->arrange();
    return graphics;
}
