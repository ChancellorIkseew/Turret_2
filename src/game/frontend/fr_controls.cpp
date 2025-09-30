#include "frontend.hpp"
//
#include "engine/io/utf8/utf8.hpp"
#include "engine/util/sleep.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

constexpr PixelCoord BTN_SIZE(100.0f, 16.0f);

static void rebind(Button* btn, std::string bindName) {
    btn->setState(ButtonState::checked);
    std::thread tr([=]() {
        std::optional<Binding> binding = std::nullopt;
        util::sleep(160);
        Input::resetLastKeyPressed();
        while (!binding.has_value()) {
            binding = Input::getLastKeyPressed();
            util::sleep(48);
        }
        Controls::rebind(bindName, binding.value());
        btn->setText(U'[' + Controls::getKeyName(bindName) + U']');
        btn->setState(ButtonState::idle);
        });
    tr.detach();
}

std::unique_ptr<Container> frontend::initControls() {
    auto controls = std::make_unique<Container>(Align::centre, Orientation::horizontal);
    auto names = controls->addNode(new Layout (Orientation::vertical));
    auto binds = controls->addNode(new Layout (Orientation::vertical));

    for (const auto& [bindName, binding] : Controls::getBindings()) {
        if (!binding.changable)
            continue;
        names->addNode(new Label(utf8::to_u32String(bindName)));
        auto btn = binds->addNode(new Button(BTN_SIZE, U'[' + Controls::getKeyName(bindName) + U']'));
        btn->addCallback(std::bind(rebind, btn, bindName));
    }

    auto ok = controls->addNode(new Button(BTN_SIZE, U"OK"));
    ok->addCallback([container = controls.get()] { Controls::writeBindings(); container->close(); });

    controls->arrange();
    return controls;
}
