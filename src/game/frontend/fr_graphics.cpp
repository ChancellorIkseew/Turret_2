#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/checkbox.hpp"
#include "MINGUI/widgets/form.hpp"
#include "MINGUI/widgets/label.hpp"
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"

constexpr Point BTN_SIZE(120.0f, 30.0f);
constexpr Point FORM_SIZE(28.0f, 20.0f);
constexpr Point BIG_FORM_SIZE(52.0f, 20.0f);

class FrGraphics : public Container {
    Form* fps;
    Checkbox* vSync;
    Checkbox* fullscreen;
    Checkbox* inertia;
public:
    FrGraphics(Engine& engine) : Container(Align::center, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        fps        = clickable->addNode(new Form(Settings::display.fps, new Uint32Validator(15U, 240U), FORM_SIZE));
        vSync      = clickable->addNode(new Checkbox(Settings::display.vSync));
        fullscreen = clickable->addNode(new Checkbox(Settings::display.fullscreen));
        inertia    = clickable->addNode(new Checkbox(Settings::gameplay.cameraInertia));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label("FPS|TPS"));
        labels->addNode(new Label("V-Sync"));
        labels->addNode(new Label("fullscreen"));
        labels->addNode(new Label("camera inertia"));

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, "Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, "Apply"))->addCallback([&] { applySettings(engine); });
    }

    void applySettings(Engine& engine) {
        Settings::display.fps = validator::to<decltype(Settings::display.fps)>(fps->getText()).value_or(60U);
        Settings::display.vSync = vSync->getValue();
        Settings::display.fullscreen = fullscreen->getValue();
        Settings::gameplay.cameraInertia = inertia->getValue();
        engine.getSession().getCamera().toggleInertia(Settings::gameplay.cameraInertia);
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initGraphics(Engine& engine) {
    return std::make_unique<FrGraphics>(engine);
}
