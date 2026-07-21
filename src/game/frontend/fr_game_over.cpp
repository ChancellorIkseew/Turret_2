#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/label.hpp>
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/util/time.hpp"

constexpr Point BTN_SIZE(200.0f, 50.0f);
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
    keys->addNode(new Label("waves defeated"));
    keys->addNode(new Label("time played"));
    //
    vals->addNode(new Label(std::format("{}", wavesDefeated), false));
    vals->addNode(new Label(util::time::timerFormat(secondsPlayed), false));
    //
    return main;
}

std::unique_ptr<Container> frontend::initGameOver(Engine& engine) {
    auto main = std::make_unique<Container>(Align::center, Orientation::vertical);
    main->addNode(new Label("Game over"));
    main->addNode(initStatistics(engine.getSession()).release());
    main->addNode(new Button(BTN_SIZE, "Exit to menu"))->addCallback([&] { engine.openMainMenu(); });
    return main;
}
