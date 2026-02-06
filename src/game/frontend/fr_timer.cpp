#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/io/utf8/utf8.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/label.hpp"
#include "game/world/world.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

class FrTimer : public Container {
    Engine& engine;
    Label* time;
    Label* mobCount;
    Label* shellCount;
public:
    FrTimer(Engine& engine) : Container(Align::left | Align::up, Orientation::vertical), engine(engine) {
        addNode(new Button(BTN_SIZE, U"Next wave"));
        time = addNode(new Label(U"", false));
        mobCount = addNode(new Label(U"", false));
        shellCount = addNode(new Label(U"", false));
        arrange();
    }

    void callback(UIContext& context) final {
        Container::callback(context);
        time->setText(utf8::to_u32string(engine.getMainWindow().getTime()));
        mobCount->setText(utf8::to_u32string(engine.getSession().getWorld().getMobs().getSoa().mobCount));
        shellCount->setText(utf8::to_u32string(engine.getSession().getWorld().getShells().getSoa().shellCount));
    }
};

std::unique_ptr<Container> frontend::initTimer(Engine& engine) {
    return std::make_unique<FrTimer>(engine);
}
