#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "engine/widgets/selector.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

constexpr uint32_t INPUT_RELOAD = 160U;
constexpr PixelCoord BTN_SIZE(100.0f, 20.0f);

class FrControls : public Container {
    Engine& engine;
    uint32_t inputReload = 0;
    std::string bindName;
    Selector* bindings = nullptr;
public:
    FrControls(Engine& engine) : Container(Align::centre, Orientation::vertical), engine(engine) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto bindNames = main->addNode(new Layout(Orientation::vertical));
        bindings       = main->addNode(new Selector(Orientation::vertical));

        for (const auto& [bindName, binding] : Controls::getBindings()) {
            if (!binding.changable)
                continue;
            bindNames->addNode(new Label(utf8::to_u32string(bindName)));
            auto btn = bindings->addNode(new Button(BTN_SIZE, U'[' + Controls::getKeyName(bindName) + U']', false));
            btn->addCallback([=, this] { targetBinding(btn, bindName); });
        }

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        //lower->addNode(new Button(BTN_SIZE, U"Apply"))->addCallback([&] { applySettings(engine); });
        //lower->addNode(new Button(BTN_SIZE, U"Reset"))->addCallback([&] { reset(engine); });

        arrange();
    }

    void targetBinding(Button* btn, const std::string& bindName) {
        if (inputReload > 0 || bindings->getTarget().lock())
            return;
        inputReload = INPUT_RELOAD;
        bindings->setTarget(btn);
        this->bindName = bindName;
    }

    void callback(UIContext& context) final {
        Container::callback(context);
        if (inputReload > 0) {
            inputReload -= engine.getMainWindow().getRealFrameDelay();
            return;
        }
        if (!bindings->getTarget().lock() || !context.input.getLastKeyPressed().has_value())
            return;
        inputReload = INPUT_RELOAD;
        Controls::rebind(bindName, context.input.getLastKeyPressed().value());
        bindings->getTarget().lock()->setText(U'[' + Controls::getKeyName(bindName) + U']');
        bindings->resetTarget();
    }
};

std::unique_ptr<Container> frontend::initControls(Engine& engine) {
    return std::make_unique<FrControls>(engine);
}
