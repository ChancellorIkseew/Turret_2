#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

class FrTimer : public Container {
    Engine& engine;
    Label* time;
public:
    ~FrTimer() final = default;
    FrTimer(Engine& engine) : Container(Align::left | Align::up, Orientation::vertical), engine(engine) {
        addNode(new Button(BTN_SIZE, U"Next wave"));
        time = addNode(new Label(U"", false));
        arrange();
    }

    void callback(const Input& input) final {
        Container::callback(input);
        time->setText(utf8::to_u32string(engine.getMainWindow().getTime()));
    }
};

std::unique_ptr<Container> frontend::initTimer(Engine& engine) {
    return std::make_unique<FrTimer>(engine);
}
