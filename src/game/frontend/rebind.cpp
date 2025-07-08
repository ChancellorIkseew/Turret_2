#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/input.hpp"
#include <thread>

constexpr PixelCoord BTN_SIZE(100.0f, 16.0f);

static std::u32string toU32string(const char* str) {
    std::u32string result;
    while (*str != '\0') {
        result.push_back(static_cast<unsigned char>(*str));
        ++str;
    }
    return result;
}

std::unique_ptr<Container> frontend::initRebind() {
    auto rebind = std::make_unique<Container>(Align::centre, Orientation::horizontal);
    auto names = std::make_unique<Layout>(Orientation::vertical);
    auto binds = std::make_unique<Layout>(Orientation::vertical);

    for (const auto& [enumName, strName] : bindNames) {
        auto name = std::make_unique<Label>(toU32string(strName));
        auto bind = std::make_unique<Button>(BTN_SIZE, U'[' + Input::getKeyName(enumName) + U']');
        bind->addCallback([ptr = bind.get(), enumName]() {
            Input::resetLastKeyPressed();
            std::thread tr([ptr, enumName]() {
                std::optional<Binding> binding = std::nullopt;
                while (!binding.has_value()) {
                    if (Input::getLastKeyPressed().has_value())
                        binding.emplace(Input::getLastKeyPressed().value());
                }
                ptr->setText(U"100");
                Input::rebind(enumName, binding.value());
                });
            tr.detach();
            });
        names->addNode(name.release());
        binds->addNode(bind.release());
    }

    rebind->addNode(names.release());
    rebind->addNode(binds.release());
    rebind->arrange();
    return rebind;
}
