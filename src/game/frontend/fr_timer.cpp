#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/icon_button.hpp>
#include <MINGUI/widgets/selector.hpp>
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "game/world/world.hpp"
#include "engine/util/time.hpp"

constexpr Point BTN_SIZE(120.0f, 20.0f);
constexpr Point ICON_SIZE(20, 20);

class FrTimer : public Container {
    Engine& engine;
    Label* currentWave;
    Label* timeToWave;
    Label* time;
    Label* mobCount;
    Label* shellCount;
public:
    FrTimer(Engine& engine) : Container(Align::left | Align::up, Orientation::vertical), engine(engine) {
        addNode(new Button(BTN_SIZE, "start wave"))->addCallback([&] { engine.getSession().startNewWave(); });

        auto selector = addNode(new Selector(Orientation::horizontal));
        selector->setPadding(1.0f);
        const Atlas& atlas = engine.getAssets().getAtlas();
        auto pause = selector->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("pause_btn"))));
        auto x1    = selector->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x1_btn"))));
        auto x2    = selector->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x2_btn"))));
        auto x4    = selector->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x4_btn"))));
        pause->addCallback([&] { pauseWorld(); });
        x1   ->addCallback([&] { setTickSpeed(1); });
        x2   ->addCallback([&] { setTickSpeed(2); });
        x4   ->addCallback([&] { setTickSpeed(4); });

        auto wave = addNode(new Layout(Orientation::horizontal));
        wave->addNode(new Label("wave :"));
        currentWave = wave->addNode(new Label("", false));

        auto waveIn = addNode(new Layout(Orientation::horizontal));
        waveIn->addNode(new Label("starts in :"));
        timeToWave = waveIn->addNode(new Label("", false));
    }

    void callback(UIContext& context) final {
        Container::callback(context);
        const auto waveCount = engine.getSession().getTimeCount().getWaveCount();
        const auto fps = engine.getMainWindow().getFPS();
        const auto ticksToWave = engine.getSession().getTimeCount().getTicksToNextWave();
        currentWave->setText(std::to_string(waveCount));
        timeToWave->setText(util::time::timerFormat(ticksToWave / fps));
        markDirty();
    }

    void pauseWorld() {
        const bool paused = engine.getSession().isPaused();
        engine.getSession().setPaused(!paused, engine);
    }
    void setTickSpeed(const int speed) {
        engine.getSession().setTickSpeed(speed);
    }
};

std::unique_ptr<Container> frontend::initTimer(Engine& engine) {
    return std::make_unique<FrTimer>(engine);
}
