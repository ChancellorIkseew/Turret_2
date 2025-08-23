#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/label.hpp"
#include "game/generation/generation.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

static void createWorld(Engine& engine, Form* _seed, Form* width, Form* height) {
    TileCoord mapSize;
    uint64_t seed = validator::toUint64(_seed->getText()).value_or(0U);
    mapSize.x = validator::toInt32(width->getText()).value_or(100);
    mapSize.y = validator::toInt32(height->getText()).value_or(100);
    WorldProperties properties(mapSize, seed);

    engine.createWorldInGame(properties);
}

std::unique_ptr<Container> frontend::initWorldProperties(Engine& engine) {
    auto propsForm = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto parameters = std::make_unique<Layout>(Orientation::horizontal);
    auto lower = std::make_unique<Layout>(Orientation::horizontal);
    auto labels = std::make_unique<Layout>(Orientation::vertical);
    auto forms = std::make_unique<Layout>(Orientation::vertical);

    auto seedL = std::make_unique<Label>(U"Seed");
    auto widthL = std::make_unique<Label>(U"Width");
    auto heightL = std::make_unique<Label>(U"Height");
    auto seedF = std::make_unique<Form>();
    auto widthF = std::make_unique<Form>();
    auto heightF = std::make_unique<Form>();

    auto back = std::make_unique<Button>(BTN_SIZE, U"Back");
    auto aply = std::make_unique<Button>(BTN_SIZE, U"Aply");
    back->addCallback([container = propsForm.get()] { container->close(); });
    aply->addCallback([&, width = widthF.get(),
                          height = heightF.get(),
                          seed = seedF.get()] { createWorld(engine, seed, width, height); });

    labels->addNode(seedL.release());
    labels->addNode(widthL.release());
    labels->addNode(heightL.release());
    forms->addNode(seedF.release());
    forms->addNode(widthF.release());
    forms->addNode(heightF.release());

    parameters->addNode(labels.release());
    parameters->addNode(forms.release());
    lower->addNode(back.release());
    lower->addNode(aply.release());

    propsForm->addNode(parameters.release());
    propsForm->addNode(lower.release());
    propsForm->arrange();
    return propsForm;
}
