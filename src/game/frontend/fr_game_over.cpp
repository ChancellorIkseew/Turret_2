#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/label.hpp>
#include "engine/engine.hpp"
#include "engine/game_session.hpp"

constexpr Point BTN_SIZE(200.0f, 50.0f);

std::unique_ptr<Container> frontend::initGameOver(Engine& engine) {
    auto main = std::make_unique<Container>(Align::centre, Orientation::vertical);
    main->addNode(new Label("Game over"));
    main->addNode(new Label("Statistics"));
    main->addNode(new Label("(there will be statistics)"));
    auto exit = main->addNode(new Button(BTN_SIZE, "Exit to menu"));
    exit->addCallback([&] { engine.openMainMenu(); });
    return main;
}
