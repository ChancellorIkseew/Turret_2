#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/input.hpp"
#include <thread>
#include "engine/util/sleep.hpp"

constexpr PixelCoord BTN_SIZE(100.0f, 16.0f);

static std::u32string toU32string(const char* str) {
    std::u32string result;
    while (*str != '\0') {
        result.push_back(static_cast<unsigned char>(*str));
        ++str;
    }
    return result;
}

static void rebind(Button* ptr, BindName bindName) {
    std::thread tr([ptr, bindName]() {
        std::optional<BindingInfo> binding = std::nullopt;
        util::sleep(160);
        Input::resetLastKeyPressed();
        while (!binding.has_value()) {
            binding = Input::getLastKeyPressed();
            util::sleep(48);
        }
        ptr->setText(U'[' + Input::getKeyName(binding.value().code) + U']');
        Input::rebind(bindName, binding.value());
        });
    tr.detach();
}

std::unique_ptr<Container> frontend::initControls() {
    auto controls = std::make_unique<Container>(Align::centre, Orientation::horizontal);
    auto names = std::make_unique<Layout>(Orientation::vertical);
    auto binds = std::make_unique<Layout>(Orientation::vertical);

    for (const auto& [enumName, strName] : bindNames) {
        auto name = std::make_unique<Label>(toU32string(strName));
        auto bind = std::make_unique<Button>(BTN_SIZE, U'[' + Input::getKeyName(enumName) + U']');
        bind->addCallback(std::bind(rebind, bind.get(), enumName));
        names->addNode(name.release());
        binds->addNode(bind.release());
    }

    controls->addNode(names.release());
    controls->addNode(binds.release());
    controls->arrange();
    return controls;
}
