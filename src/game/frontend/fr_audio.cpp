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

class FrAudio : public Container {
    Form* master;
    Form* world;
    Form* ui;
    Form* music;
    Checkbox* toggleSound;
public:
    FrAudio(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        master = clickable->addNode(new Form(Settings::audio.master, new Uint8Validator(0U, 100U), FORM_SIZE));
        world  = clickable->addNode(new Form(Settings::audio.world,  new Uint8Validator(0U, 100U), FORM_SIZE));
        ui     = clickable->addNode(new Form(Settings::audio.ui,     new Uint8Validator(0U, 100U), FORM_SIZE));
        music  = clickable->addNode(new Form(Settings::audio.music,  new Uint8Validator(0U, 100U), FORM_SIZE));
        toggleSound = clickable->addNode(new Checkbox(Settings::audio.toggleSound));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label(U"master"));
        labels->addNode(new Label(U"world"));
        labels->addNode(new Label(U"ui"));
        labels->addNode(new Label(U"music"));
        labels->addNode(new Label(U"toggle sound"));

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, U"Apply"))->addCallback([&] { applySettings(engine); });

        arrange();
    }

    void applySettings(Engine& engine) {
        using T = decltype(Settings::audio.master);
        Settings::audio.master = validator::to<T>(master->getText()).value_or(50U);
        Settings::audio.world  = validator::to<T>(world ->getText()).value_or(100U);
        Settings::audio.ui     = validator::to<T>(ui    ->getText()).value_or(100U);
        Settings::audio.music  = validator::to<T>(music ->getText()).value_or(100U);
        Settings::audio.toggleSound = toggleSound->getValue();;
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initAudio(Engine& engine) {
    return std::make_unique<FrAudio>(engine);
}
