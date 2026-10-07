#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/label.hpp>
#include <MINGUI/widgets/selector.hpp>
#include "app.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

constexpr uint32_t INPUT_RELOAD = 160U;
constexpr Point BTN_SIZE(100, 20);

class FrControls : public Container {
    App& app;
    uint64_t inputReload = 0;
    std::string bindName;
    Selector* bindings = nullptr;
public:
    FrControls(App& app) : Container(Align::center, Orientation::vertical), app(app) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto bindNames = main->addNode(new Layout(Orientation::vertical));
        bindings       = main->addNode(new Selector(Orientation::vertical));
        frontend::useLabelsSpacing(bindNames);

        for (const auto& [bindName, binding] : Controls::getBindings()) {
            if (!binding.changable)
                continue;
            bindNames->addNode(new Label(bindName));
            auto btn = bindings->addNode(new Button(BTN_SIZE, '[' + Controls::getKeyName(bindName) + ']'));
            btn->addCallback([=, this] { targetBinding(btn, bindName); });
        }

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, tr("Back")))->addCallback([&] { close(); Controls::writeBindings(); });
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
            inputReload -= app.getMainWindow().getRealFrameDelayMs();
            return;
        }
        const std::optional<Binding> lastKey = app.getMainWindow().getInput().getLastKeyPressed();
        if (!bindings->getTarget().lock() || !lastKey.has_value())
            return;
        inputReload = INPUT_RELOAD;
        Controls::rebind(bindName, lastKey.value());
        Button* button = static_cast<Button*>(bindings->getTarget().lock().get());
        button->setText('[' + Controls::getKeyName(bindName) + ']');
        bindings->resetTarget();
        markDirty();
    }
};

std::unique_ptr<Container> frontend::initControls(App& app) {
    return std::make_unique<FrControls>(app);
}
