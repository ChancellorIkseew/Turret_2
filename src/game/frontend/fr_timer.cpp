#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/icon_button.hpp>
#include <MINGUI/widgets/selector.hpp>
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "engine/util/time.hpp"
#include "game/world/world.hpp"

constexpr Point BTN_SIZE(120.0f, 20.0f);
constexpr Point ICON_SIZE(20, 20);

class FrTimer : public Container {
    Engine& engine;
    Label* currentWave;
    Label* timeToWave;
    Selector* playback;
    IconButton* pause;
    IconButton* x1;
    IconButton* x2;
    IconButton* x4;
public:
    FrTimer(Engine& engine) : Container(Align::left | Align::up, Orientation::vertical), engine(engine) {
        addNode(new Button(BTN_SIZE, "start wave"))->addCallback([&] { engine.getSession().startNewWave(); });

        playback = addNode(new Selector(Orientation::horizontal));
        playback->setPadding(1.0f);
        const Atlas& atlas = engine.getAssets().getAtlas();
        pause = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("pause_btn"))));
        x1    = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x1_btn"))));
        x2    = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x2_btn"))));
        x4    = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x4_btn"))));
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
        updatePlayback();
        markDirty();
    }

    void updatePlayback() {
        const bool paused = engine.getSession().isPaused();
        const auto tickSpeed = paused ? 0 : engine.getSession().getTickSpeed();
        playback->resetTarget();
        switch (tickSpeed) {
        case 0: playback->setTarget(pause); break;
        case 1: playback->setTarget(x1); break;
        case 2: playback->setTarget(x2); break;
        case 4: playback->setTarget(x4); break;
        }
    }

    void pauseWorld() {
        engine.getSession().setPaused(true, engine);
    }
    void setTickSpeed(const int speed) {
        engine.getSession().setTickSpeed(speed);
        engine.getSession().setPaused(false, engine);
    }
};

std::unique_ptr<Container> frontend::initTimer(Engine& engine) {
    return std::make_unique<FrTimer>(engine);
}
