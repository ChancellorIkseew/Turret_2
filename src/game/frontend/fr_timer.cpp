#include "frontend.hpp"
//
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initTimer() {
    auto timer = std::make_unique<Container>(Align::left | Align::up, Orientation::vertical);
    timer->addNode(new Button(BTN_SIZE, U"Next wave"));
    timer->addNode(new Label(U"Next wave in: 10:10"));
    timer->arrange();
    return timer;
}
