#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/icon_button.hpp>
#include <MINGUI/widgets/selector.hpp>
#include "app.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "engine/util/time.hpp"
#include "game/game_session.hpp"
#include "game/world/world.hpp"

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
    App& app;
    Label* wave;
    Label* startsIn;
    Label* enemiesRemaining;
    Selector* playback;
    IconButton* pause;
    IconButton* x1;
    IconButton* x2;
    IconButton* x4;
public:
    FrTimer(App& app) : Container(Align::left | Align::up, Orientation::vertical), app(app) {    
        setPadding(12.f);
        const Atlas& atlas = app.getAssets().getAtlas();
        auto bar = addNode(new Layout(Orientation::horizontal));
        bar->setPalette(NULL_PALETTE);
        bar->setPadding(0.f);
        bar->setMargin(24.f);

        auto startWave = bar->addNode(new IconButton(Point(38, 38), 2.0f, new T1_UITexture(atlas.at("start_wave_btn"))));
        startWave->addCallback([&] { app.getSession().startNewWave(); });
        startWave->setPalette(Palette{ .idle = cl::RED, .hover = 0x84'54'54'FF });

        playback = bar->addNode(new Selector(Orientation::horizontal));
        playback->setPadding(1.0f);
        pause = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("pause_btn"))));
        x1    = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x1_btn"))));
        x2    = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x2_btn"))));
        x4    = playback->addNode(new IconButton(ICON_SIZE, 2.0f, new T1_UITexture(atlas.at("x4_btn"))));
        pause->addCallback([&] { pauseWorld(); });
        x1   ->addCallback([&] { setTickSpeed(1); });
        x2   ->addCallback([&] { setTickSpeed(2); });
        x4   ->addCallback([&] { setTickSpeed(4); });

        wave             = addNode(new Label(""));
        startsIn         = addNode(new Label(""));
        enemiesRemaining = addNode(new Label(""));
        wave->setPalette(Palette{ .text = cl::BEIGE });
    }
private:
    void callback(UIContext& context) final {
        Container::callback(context);
        const TimeCount& timeCount = app.getSession().getTimeCount();
        wave->setText(tr("Wave {}", timeCount.getWaveCount()));
        enemiesRemaining->setText(tr("Enemies remaining {}", countEnemies(app.getSession())));
        if (app.getSession().getWorld().getConfig().wavesByTimer) {
            constexpr uint64_t DEFAULT_FPS_TPS = 60;
            startsIn->setText(tr("Starts in {}", util::time::timerFormat(timeCount.getTicksToNextWave() / DEFAULT_FPS_TPS)));
        }
        else
            startsIn->setText(tr("Starts in {}", 0));
        updatePlayback();
        markDirty();
    }

    void updatePlayback() {
        const bool paused = app.getSession().isPausedManually();
        const auto tickSpeed = paused ? 0 : app.getSession().getTickSpeed();
        playback->resetTarget();
        switch (tickSpeed) {
        case 0: playback->setTarget(pause); break;
        case 1: playback->setTarget(x1); break;
        case 2: playback->setTarget(x2); break;
        case 4: playback->setTarget(x4); break;
        }
    }

    void pauseWorld() {
        app.getSession().setPaused(true, app);
    }
    void setTickSpeed(const int speed) {
        app.getSession().setTickSpeed(speed);
        app.getSession().setPaused(false, app);
    }
};

std::unique_ptr<Container> frontend::initTimer(App& app) {
    return std::make_unique<FrTimer>(app);
}
