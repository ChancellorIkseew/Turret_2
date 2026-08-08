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

static int countEnemies(GameSession& session) {
    const auto& teamIDs = session.getWorld().getMobs().getSoa().teamID;
    const TeamID playerTeamID = session.getPlayerController().getPlayerTeamID();
    int enemiesCount = 0;
    for (const TeamID teamID : teamIDs) {
        if (teamID != playerTeamID)
            ++enemiesCount;
    }
    return enemiesCount;
}

class FrTimer : public Container {
    Engine& engine;
    Label* wave;
    Label* startsIn;
    Label* enemiesRemaining;
    Selector* playback;
    IconButton* pause;
    IconButton* x1;
    IconButton* x2;
    IconButton* x4;
public:
    FrTimer(Engine& engine) : Container(Align::left | Align::up, Orientation::vertical), engine(engine) {
        addNode(new Button(BTN_SIZE, tr("start wave")))->addCallback([&] { engine.getSession().startNewWave(); });

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

        wave             = addNode(new Label(tr("wave")));
        startsIn         = addNode(new Label(tr("starts in")));
        enemiesRemaining = addNode(new Label(tr("enemies remaining")));
    }
private:
    void callback(UIContext& context) final {
        Container::callback(context);
        constexpr uint64_t DEFAULT_FPS_TPS = 60;
        const auto waveCount = engine.getSession().getTimeCount().getWaveCount();
        const auto ticksToWave = engine.getSession().getTimeCount().getTicksToNextWave();
        wave->setText(tr("wave {}", waveCount));
        startsIn->setText(tr("starts in {}", util::time::timerFormat(ticksToWave / DEFAULT_FPS_TPS)));
        enemiesRemaining->setText(tr("enemies remaining {}", countEnemies(engine.getSession())));
        updatePlayback();
        markDirty();
    }

    void updatePlayback() {
        const bool paused = engine.getSession().isPausedManually();
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
