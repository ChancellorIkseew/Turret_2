#include "frontend.hpp"
//
#include <mutex>
#include "engine/engine.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/util/sleep.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"

static std::mutex mutex;
constexpr PixelCoord BTN_SIZE(100.0f, 16.0f);

static void rebind(Input& input, Button* btn, std::string bindName) { //TODO: refactor to avoid multithreading
    std::lock_guard<std::mutex> guarg(mutex);
    btn->setState(ButtonState::checked);
    std::thread tr([=, &input]() {
        std::optional<Binding> binding = std::nullopt;
        util::sleep(160);
        input.resetLastKeyPressed();
        while (!binding.has_value()) {
            binding = input.getLastKeyPressed();
            util::sleep(48);
        }
        if (!btn)
            return;
        Controls::rebind(bindName, binding.value());
        btn->setText(U'[' + Controls::getKeyName(bindName) + U']');
        btn->setState(ButtonState::idle);
        });
    tr.detach();
    util::sleep(160);
}

std::unique_ptr<Container> frontend::initControls(Engine& engine) {
    auto controls = std::make_unique<Container>(Align::centre, Orientation::horizontal);
    auto bindNames = controls->addNode(new Layout (Orientation::vertical));
    auto bindings  = controls->addNode(new Layout (Orientation::vertical));

    for (const auto& [bindName, binding] : Controls::getBindings()) {
        if (!binding.changable)
            continue;
        bindNames->addNode(new Label(utf8::to_u32string(bindName)));
        auto btn = bindings->addNode(new Button(BTN_SIZE, U'[' + Controls::getKeyName(bindName) + U']'));
        btn->addCallback([=, &engine] { rebind(engine.getMainWindow().getInput(), btn, bindName); });
    }

    auto ok = controls->addNode(new Button(BTN_SIZE, U"OK"));
    ok->addCallback([container = controls.get()] { Controls::writeBindings(); container->close(); });

    controls->arrange();
    return controls;
}
