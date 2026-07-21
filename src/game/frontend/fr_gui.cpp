#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/checkbox.hpp"
#include "MINGUI/widgets/form.hpp"
#include "MINGUI/widgets/label.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/settings/settings.hpp"

constexpr Point BTN_SIZE(120.0f, 30.0f);
constexpr Point FORM_SIZE(20.0f, 20.0f);

class FrGUI : public Container {
    Form* scale;
    Checkbox* customCursor;
    Checkbox* showConsole;
public:
    FrGUI(Engine& engine) : Container(Align::center, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        scale        = clickable->addNode(new Form(Settings::gui.scale, new Uint8Validator(1U, 8U), FORM_SIZE));
        customCursor = clickable->addNode(new Checkbox(Settings::gui.customCursor));
        showConsole  = clickable->addNode(new Checkbox(Settings::gui.showConsole));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label("GUI scale"));
        labels->addNode(new Label("custom cursor"));
        labels->addNode(new Label("show console"));
        
        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, "Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, "Apply"))->addCallback([&] { applySettings(engine); });
    }

    void applySettings(Engine& engine) {
        using ScaleT = decltype(Settings::gui.scale);
        Settings::gui.scale = validator::to<ScaleT>(scale->getText()).value_or(1U);
        Settings::gui.customCursor = customCursor->getValue();
        Settings::gui.showConsole = showConsole->getValue();
        engine.getGUI().setScale(Settings::gui.scale);
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initGUI(Engine& engine) {
    return std::make_unique<FrGUI>(engine);
}
