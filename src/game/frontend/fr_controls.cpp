#include "frontend.hpp"
//
#include "engine/util/sleep.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/controls.hpp"
#include "engine/window/input/input.hpp"
#include "engine/window/input/utf8/utf8.hpp"

constexpr PixelCoord BTN_SIZE(100.0f, 16.0f);

static void rebind(Button* ptr, std::string bindName) {
    std::thread tr([ptr, bindName]() {
        std::optional<Binding> binding = std::nullopt;
        util::sleep(160);
        Input::resetLastKeyPressed();
        while (!binding.has_value()) {
            binding = Input::getLastKeyPressed();
            util::sleep(48);
        }
        ptr->setText(U'[' + Controls::getKeyName(bindName) + U']');
        Controls::rebind(bindName, binding.value());
        });
    tr.detach();
}

std::unique_ptr<Container> frontend::initControls() {
    auto controls = std::make_unique<Container>(Align::centre, Orientation::horizontal);
    auto names = std::make_unique<Layout>(Orientation::vertical);
    auto binds = std::make_unique<Layout>(Orientation::vertical);

    for (const auto& [bindName, binding] : Controls::getBindings()) {
        if (!binding.changable)
            continue;
        auto name = std::make_unique<Label>(utf8::fromConstCharToU32String(bindName.c_str()));
        auto bind = std::make_unique<Button>(BTN_SIZE, U'[' + Controls::getKeyName(bindName) + U']');
        bind->addCallback(std::bind(rebind, bind.get(), bindName));
        names->addNode(name.release());
        binds->addNode(bind.release());
    }

    auto ok = std::make_unique<Button>(BTN_SIZE, U"OK");
    ok->addCallback([container = controls.get()] { container->close(); });

    controls->addNode(names.release());
    controls->addNode(binds.release());
    controls->addNode(ok.release());
    controls->arrange();
    return controls;
}
