#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/checkbox.hpp"
#include "MINGUI/widgets/label.hpp"
#include "engine/settings/settings.hpp"

constexpr Point BTN_SIZE(120.0f, 30.0f);

class FrGameplay : public Container {
    Checkbox* pauseInBackground;
    Checkbox* pauseOnWorldOpen;
public:
    FrGameplay(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        pauseInBackground = clickable->addNode(new Checkbox(Settings::gameplay.pauseInBackground));
        pauseOnWorldOpen  = clickable->addNode(new Checkbox(Settings::gameplay.pauseOnWorldOpen));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label("pause in background"));
        labels->addNode(new Label("pause on world open"));

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, "Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, "Apply"))->addCallback([&] { applySettings(engine); });
    }

    void applySettings(Engine& engine) {
        Settings::gameplay.pauseInBackground = pauseInBackground->getValue();
        Settings::gameplay.pauseOnWorldOpen  = pauseOnWorldOpen ->getValue();
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initGameplay(Engine& engine) {
    return std::make_unique<FrGameplay>(engine);
}
