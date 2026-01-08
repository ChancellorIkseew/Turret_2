#include "frontend.hpp"
//
#include <cassert>
#include "engine/engine.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

constexpr uint32_t INPUT_RELOAD = 160U;
constexpr PixelCoord BTN_SIZE(100.0f, 20.0f);

class FrControls : public Container {
    Engine& engine;
    uint32_t inputReload = 0;
    Button* btn = nullptr;
    std::string bindName;
public:
    FrControls(Engine& engine) : Container(Align::centre, Orientation::vertical), engine(engine) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto bindNames = main->addNode(new Layout(Orientation::vertical));
        auto bindings  = main->addNode(new Layout(Orientation::vertical));

        for (const auto& [bindName, binding] : Controls::getBindings()) {
            if (!binding.changable)
                continue;
            bindNames->addNode(new Label(utf8::to_u32string(bindName)));
            auto btn = bindings->addNode(new Button(BTN_SIZE, U'[' + Controls::getKeyName(bindName) + U']'));
            btn->addCallback([=] { targetBinding(btn, bindName); });
        }

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        //lower->addNode(new Button(BTN_SIZE, U"Apply"))->addCallback([&] { applySettings(engine); });
        //lower->addNode(new Button(BTN_SIZE, U"Reset"))->addCallback([&] { reset(engine); });

        arrange();
    }

    void targetBinding(Button* btn, const std::string& bindName) {
        assert(!btn);
        if (inputReload > 0 || this->btn)
            return;
        inputReload = INPUT_RELOAD;
        btn->setState(ButtonState::checked);
        this->btn = btn;
        this->bindName = bindName;
    }

    void callback(const Input& input) final {
        Container::callback(input);
        if (inputReload > 0) {
            inputReload -= engine.getMainWindow().getRealFrameDelay();
            return;
        }
        if (!btn || !input.getLastKeyPressed().has_value())
            return;
        inputReload = INPUT_RELOAD;
        Controls::rebind(bindName, input.getLastKeyPressed().value());
        btn->setText(U'[' + Controls::getKeyName(bindName) + U']');
        btn->setState(ButtonState::idle);
        btn = nullptr;
    }
};

std::unique_ptr<Container> frontend::initControls(Engine& engine) {
    return std::make_unique<FrControls>(engine);
}
