#include "frontend.hpp"
//
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/checkbox.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/label.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);
constexpr PixelCoord FORM_SIZE(28.0f, 20.0f);

class FrGraphics : public Container {
    Form* fps;
    Checkbox* fullscreen;
    Checkbox* inertia;
public:
    FrGraphics(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        fps        = clickable->addNode(new Form(Settings::display.FPS, new Uint32Validator(15U, 240U), FORM_SIZE));
        fullscreen = clickable->addNode(new Checkbox(Settings::display.fullscreen));
        inertia    = clickable->addNode(new Checkbox(Settings::gameplay.cameraInertia));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label(U"FPS"));
        labels->addNode(new Label(U"fullscreen"));
        labels->addNode(new Label(U"camera inertia"));

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, U"Apply"))->addCallback([&] { applySettings(engine); });

        arrange();
    }

    void applySettings(Engine& engine) {
        Settings::display.FPS = validator::toUint32(fps->getText()).value_or(60U);
        Settings::display.fullscreen = fullscreen->getValue();
        Settings::gameplay.cameraInertia = inertia->getValue();
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initGraphics(Engine& engine) {
    return std::make_unique<FrGraphics>(engine);
}
