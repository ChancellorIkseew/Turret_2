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

static std::unique_ptr<Layout> initGenerationProps() {
    auto genProps = std::make_unique<Layout>(Orientation::horizontal);
    auto resources = genProps->addNode(new Layout(Orientation::vertical));
    auto frequency = genProps->addNode(new Layout(Orientation::vertical));
    auto deposite  = genProps->addNode(new Layout(Orientation::vertical));
    frequency->addNode(new Label(U"Frequency"));
    deposite ->addNode(new Label(U"Deposite"));

    genProps->arrange();
    return genProps;
}

std::unique_ptr<Container> frontend::initWorldProperties(Engine& engine) {
    auto propsForm = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto parameters = propsForm->addNode(new Layout(Orientation::horizontal));
    auto lower      = propsForm->addNode(new Layout(Orientation::horizontal));
    auto labels = parameters->addNode(new Layout(Orientation::vertical));
    auto forms  = parameters->addNode(new Layout(Orientation::vertical));
    parameters->addNode(initGenerationProps().release());

    labels->addNode(new Label(U"Seed"));
    labels->addNode(new Label(U"Width"));
    labels->addNode(new Label(U"Height"));
    auto seedF =   forms->addNode(new Form(0U, new Uint64Validator(0U, std::numeric_limits<uint64_t>::max())));
    auto widthF =  forms->addNode(new Form(100, new Int32Validator(20, 5000)));
    auto heightF = forms->addNode(new Form(100, new Int32Validator(20, 5000)));

    auto back = lower->addNode(new Button(BTN_SIZE, U"Back"));
    auto aply = lower->addNode(new Button(BTN_SIZE, U"Aply"));
    back->addCallback([container = propsForm.get()] { container->close(); });
    aply->addCallback([=, &engine] { createWorld(engine, seedF, widthF, heightF); });

    propsForm->arrange();
    return propsForm;
}
