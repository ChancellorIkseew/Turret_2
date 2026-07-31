#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/checkbox.hpp"
#include "MINGUI/widgets/form.hpp"
#include "MINGUI/widgets/label.hpp"
#include "MINGUI/widgets/selector.hpp"
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"

constexpr Point BTN_SIZE(120.0f, 30.0f);
constexpr Point S_BTN_SIZE(150.0f, 20.0f);
constexpr Point FORM_SIZE(28.0f, 20.0f);
constexpr Point BIG_FORM_SIZE(52.0f, 20.0f);

class FrGraphics : public Container {
    Form* fps;
    Checkbox* fullscreen;
    Checkbox* inertia;
    Checkbox* vingette;
    Selector* vSync;
    Button* off;
    Button* synchronized;
    Button* adaptive;
public:
    FrGraphics(Engine& engine) : Container(Align::center, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        fps        = clickable->addNode(new Form(Settings::display.fps, new Uint32Validator(15U, 240U), FORM_SIZE));
        fullscreen = clickable->addNode(new Checkbox(Settings::display.fullscreen));
        inertia    = clickable->addNode(new Checkbox(Settings::gameplay.cameraInertia));
        vingette   = clickable->addNode(new Checkbox(Settings::gameplay.vingette));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label("FPS|TPS"));
        labels->addNode(new Label("fullscreen"));
        labels->addNode(new Label("camera inertia"));
        labels->addNode(new Label("vingette"));

        vSync = main->addNode(new Selector(Orientation::vertical));
        vSync->setMargin(2.0f);
        vSync->addNode(new Label("V-Sync mode"));
        off          = vSync->addNode(new Button(S_BTN_SIZE, "off"));
        synchronized = vSync->addNode(new Button(S_BTN_SIZE, "synchronized"));
        adaptive     = vSync->addNode(new Button(S_BTN_SIZE, "adaptive"));
        switch (Settings::display.vSyncMode) {
        case VSyncMode::immediate:    vSync->setTarget(off);          break;
        case VSyncMode::synchronized: vSync->setTarget(synchronized); break;
        case VSyncMode::adaptive:     vSync->setTarget(adaptive);     break;
        }

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, "Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, "Apply"))->addCallback([&] { applySettings(engine); });
    }

    void applySettings(Engine& engine) {
        Settings::display.fps = validator::to<decltype(Settings::display.fps)>(fps->getText()).value_or(60U);
        Settings::display.fullscreen = fullscreen->getValue();
        Settings::gameplay.cameraInertia = inertia->getValue();
        Settings::gameplay.vingette = vingette->getValue();
        engine.getSession().getCamera().toggleInertia(Settings::gameplay.cameraInertia);
        if (vSync->isTarget(off))
            Settings::display.vSyncMode = VSyncMode::immediate;
        if (vSync->isTarget(synchronized))
            Settings::display.vSyncMode = VSyncMode::synchronized;
        if (vSync->isTarget(adaptive))
            Settings::display.vSyncMode = VSyncMode::adaptive;
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initGraphics(Engine& engine) {
    return std::make_unique<FrGraphics>(engine);
}
