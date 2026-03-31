#include "frontend.hpp"
//
#include <MINGUI/widgets/icon_button.hpp>
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/label.hpp>
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "game/world/world.hpp"
#include "engine/util/time.hpp"

constexpr Point BTN_SIZE(200.0f, 50.0f);
constexpr Point ICON_SIZE(18, 18);

class FrTimer : public Container {
    Engine& engine;
    Label* currentWave;
    Label* timeToWave;
    Label* time;
    Label* mobCount;
    Label* shellCount;
public:
    FrTimer(Engine& engine) : Container(Align::left | Align::up, Orientation::vertical), engine(engine) {
        addNode(new Button(BTN_SIZE, "Next wave"))->addCallback([&] { engine.getSession().startNewWave(); });
        addNode(new IconButton(ICON_SIZE, 1.0f, new T1_UITexture(engine.getAssets().getAtlas().at("pause"))));
        currentWave = addNode(new Label("", false));
        timeToWave = addNode(new Label("", false));
        time = addNode(new Label("", false));
        mobCount = addNode(new Label("", false));
        shellCount = addNode(new Label("", false));
    }

    void callback(UIContext& context) final {
        Container::callback(context);
        const auto waveCount = engine.getSession().getTimeCount().getWaveCount();
        const auto fps = engine.getMainWindow().getFPS();
        const auto ticksToWave = engine.getSession().getTimeCount().getTicksToNextWave();
        currentWave->setText(std::to_string(waveCount));
        timeToWave->setText(util::time::timerFormat(ticksToWave / fps));
        time->setText(std::to_string(engine.getMainWindow().getTime()));
        mobCount->setText(std::to_string(engine.getSession().getWorld().getMobs().getSoa().mobCount));
        shellCount->setText(std::to_string(engine.getSession().getWorld().getShells().getSoa().shellCount));
        markDirty();
    }
};

std::unique_ptr<Container> frontend::initTimer(Engine& engine) {
    return std::make_unique<FrTimer>(engine);
}
