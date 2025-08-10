#include "frontend.hpp"
//
#include "engine/engine_state.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/label.hpp"
#include "game/generation/generation.hpp"

constexpr PixelCoord BTN_SIZE(200.0f, 50.0f);

static void createWorld(WorldProperties& properties, EngineState& state, Container* propsForm) {
    //properties = ...
    state = EngineState::gameplay;
    propsForm->close();
}

std::unique_ptr<Container> frontend::initWorldProperties(WorldProperties& properties, EngineState& state) {
    auto propsForm = std::make_unique<Container>(Align::centre, Orientation::horizontal);
    auto frequancy = std::make_unique<Layout>(Orientation::vertical);
    auto deposite = std::make_unique<Layout>(Orientation::vertical);

    auto startWave = std::make_unique<Label>(U"frequancy");
    auto timeToWave = std::make_unique<Form>();
    

    frequancy->addNode(startWave.release());
    frequancy->addNode(timeToWave.release());


    auto f1 = std::make_unique<Label>(U"deposite");
    auto f2 = std::make_unique<Label>(U"text");

    deposite->addNode(f1.release());
    deposite->addNode(f2.release());

    auto aply = std::make_unique<Button>(BTN_SIZE, U"Aply");
    aply->addCallback(std::bind_front(createWorld, properties, state, propsForm.get()));

    propsForm->addNode(frequancy.release());
    propsForm->addNode(deposite.release());
    propsForm->arrange();
    return propsForm;
}
