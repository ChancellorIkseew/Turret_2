#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/label.hpp>
#include "app.hpp"
#include "engine/settings/localization.hpp"
#include "engine/util/time.hpp"
#include "game/game_session.hpp"

constexpr Point BTN_SIZE(200, 50);
constexpr uint64_t DEFAULT_FPS_TPS = 60;

static std::unique_ptr<Layout> initStatistics(GameSession& session) {
    const TimeCount& timeCount = session.getTimeCount();
    const uint64_t secondsPlayed = timeCount.getTickCount() / DEFAULT_FPS_TPS;
    const int wavesDefeated = std::max(0, static_cast<int>(timeCount.getWaveCount()) - 1);
    //
    auto main = std::make_unique<Layout>(Orientation::horizontal);
    auto keys = main->addNode(new Layout(Orientation::vertical));
    auto vals = main->addNode(new Layout(Orientation::vertical));
    //
    keys->addNode(new Label(tr("waves defeated")));
    keys->addNode(new Label(tr("time played")));
    //
    vals->addNode(new Label(std::format("{}", wavesDefeated)));
    vals->addNode(new Label(util::time::timerFormat(secondsPlayed)));
    //
    return main;
}

std::unique_ptr<Container> frontend::initGameOver(App& app) {
    auto main = std::make_unique<Container>(Align::center, Orientation::vertical);
    main->addNode(new Label(tr("Game over")));
    main->addNode(initStatistics(app.getSession()).release());
    main->addNode(new Button(BTN_SIZE, tr("Exit to menu")))->addCallback([&] { app.openMainMenu(); });
    return main;
}
