#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/checkbox.hpp"
#include "MINGUI/widgets/form.hpp"
#include "MINGUI/widgets/label.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"

constexpr Point BTN_SIZE(120, 30);
constexpr Point FORM_SIZE(28, 20);

class FrAudio : public Container {
    Form* master;
    Form* world;
    Form* ui;
    Form* music;
    Checkbox* toggleSound;
    Checkbox* muteInBakcgroung;
public:
    FrAudio(Engine& engine) : Container(Align::center, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        master = clickable->addNode(new Form(Settings::audio.master, new Uint8Validator(0U, 100U), FORM_SIZE));
        world  = clickable->addNode(new Form(Settings::audio.world,  new Uint8Validator(0U, 100U), FORM_SIZE));
        ui     = clickable->addNode(new Form(Settings::audio.ui,     new Uint8Validator(0U, 100U), FORM_SIZE));
        music  = clickable->addNode(new Form(Settings::audio.music,  new Uint8Validator(0U, 100U), FORM_SIZE));
        toggleSound      = clickable->addNode(new Checkbox(Settings::audio.toggleSound));
        muteInBakcgroung = clickable->addNode(new Checkbox(Settings::audio.muteInBackground));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        frontend::useLabelsSpacing(labels);
        labels->addNode(new Label(tr("master")));
        labels->addNode(new Label(tr("world")));
        labels->addNode(new Label(tr("ui")));
        labels->addNode(new Label(tr("music")));
        labels->addNode(new Label(tr("toggle sound")));
        labels->addNode(new Label(tr("mute in background")));

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, tr("Back")))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, tr("Apply")))->addCallback([&] { applySettings(engine); });
    }

    void applySettings(Engine& engine) {
        using T = decltype(Settings::audio.master);
        Settings::audio.master = validator::to<T>(master->getText()).value_or(50U);
        Settings::audio.world  = validator::to<T>(world ->getText()).value_or(100U);
        Settings::audio.ui     = validator::to<T>(ui    ->getText()).value_or(100U);
        Settings::audio.music  = validator::to<T>(music ->getText()).value_or(100U);
        Settings::audio.toggleSound      = toggleSound     ->getValue();
        Settings::audio.muteInBackground = muteInBakcgroung->getValue();
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initAudio(Engine& engine) {
    return std::make_unique<FrAudio>(engine);
}
