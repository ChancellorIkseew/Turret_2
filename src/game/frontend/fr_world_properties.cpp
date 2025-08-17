#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/label.hpp"
#include "game/generation/generation.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

static void createWorld(Engine& engine) {
    WorldProperties properties(TileCoord(50, 50), 0);
    engine.createWorldInGame(properties);
}

std::unique_ptr<Container> frontend::initWorldProperties(Engine& engine) {
    auto propsForm = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto parameters = std::make_unique<Layout>(Orientation::horizontal);
    auto lower = std::make_unique<Layout>(Orientation::horizontal);
    auto labels = std::make_unique<Layout>(Orientation::vertical);
    auto forms = std::make_unique<Layout>(Orientation::vertical);

    auto widthL = std::make_unique<Label>(U"Width");
    auto heightL = std::make_unique<Label>(U"Height");
    auto widthF = std::make_unique<Form>();
    auto heightF = std::make_unique<Form>();
    
    labels->addNode(widthL.release());
    labels->addNode(heightL.release());
    forms->addNode(widthF.release());
    forms->addNode(heightF.release());

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto aply = std::make_unique<Button>(BTN_SIZE, U"Aply");
    back->addCallback([container = propsForm.get()] { container->close(); });
    aply->addCallback([&] { createWorld(engine); });

    parameters->addNode(labels.release());
    parameters->addNode(forms.release());
    lower->addNode(back.release());
    lower->addNode(aply.release());

    propsForm->addNode(parameters.release());
    propsForm->addNode(lower.release());
    propsForm->arrange();
    return propsForm;
}
