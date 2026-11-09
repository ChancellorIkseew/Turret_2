#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>

constexpr Point BTN_SIZE(120, 30);

std::unique_ptr<Container> frontend::initConfirmDialog(const std::string& message, std::function<void()> onConfirm) {
    auto box = std::make_unique<Container>(Align::center, Orientation::vertical);
    box->addNode(new Label(message));
    auto buttons = box->addNode(new Layout(Orientation::horizontal));
    auto yes = buttons->addNode(new Button(BTN_SIZE, tr("Yes")));
    auto no = buttons->addNode(new Button(BTN_SIZE, tr("No")));
    yes->addCallback([container = box.get(), onConfirm = std::move(onConfirm)] {
        onConfirm();
        container->close();
        });
    no->addCallback([container = box.get()] { container->close(); });
    return box;
}
