#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>

constexpr Point BTN_SIZE(120, 30);

std::unique_ptr<Container> frontend::initMessageBox(App& app, const std::string& message) {
    auto box = std::make_unique<Container>(Align::center, Orientation::vertical);
    box->addNode(new Label(message));
    auto back = box->addNode(new Button(BTN_SIZE, tr("Back")));
    back->addCallback([container = box.get()] { container->close(); });
    return box;
}
