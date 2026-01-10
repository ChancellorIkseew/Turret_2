#include "frontend.hpp"
//
#include "engine/settings/settings.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/checkbox.hpp"
#include "engine/widgets/label.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

class FrGUI : public Container {
    Checkbox* customCursor;
    Checkbox* showConsole;
public:
    FrGUI(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto clickable = main->addNode(new Layout(Orientation::vertical));
        customCursor = clickable->addNode(new Checkbox(Settings::gui.customCursor));
        showConsole  = clickable->addNode(new Checkbox(Settings::gui.showConsole));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label(U"custom cursor"));
        labels->addNode(new Label(U"show console"));
        
        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, U"Apply"))->addCallback([&] { applySettings(engine); });

        arrange();
    }

    void applySettings(Engine& engine) {
        Settings::gui.customCursor = customCursor->getValue();
        Settings::gui.showConsole = showConsole->getValue();
        Settings::applySettings(engine);
        Settings::writeSettings();
    }
};

std::unique_ptr<Container> frontend::initGUI(Engine& engine) {
    return std::make_unique<FrGUI>(engine);
}
