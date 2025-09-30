#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/tin_parser.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "engine/widgets/label.hpp"
#include "game/generation/generation.hpp"

constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);

class OProps : public Layout {
    std::vector<OverlayPreset> overlayPresets;
    Layout* labels = nullptr;
    Layout* frequency = nullptr;
    Layout* deposite = nullptr;
public:
    OProps() : Layout(Orientation::horizontal) {
        labels = addNode(new Layout(Orientation::vertical));
        frequency = addNode(new Layout(Orientation::vertical));
        deposite = addNode(new Layout(Orientation::vertical));

        frequency->addNode(new Label(U"Frequency"));
        deposite->addNode(new Label(U"Deposite"));
        tin::Data data = tin::read(io::folders::GENERATION / "overlay_default.tin");
        for (const auto& [strID, _] : data) {
            const auto preset = data.getTileCoord(strID).value_or(TileCoord(0, 0));
            const uint8_t id = validator::toUint8(strID).value_or(0U);
            overlayPresets.emplace_back(id, 0, 0);
            labels->addNode(new Label(utf8::to_u32string(id)));
            frequency->addNode(new Form(preset.x, new Int32Validator(0, 10000)));
            deposite->addNode(new Form(preset.y, new Int32Validator(0, 100)));
        }
        arrange();
    }
    ~OProps() final = default;
    //
    const std::vector<OverlayPreset> getPresets() {
        for (int i = 1; i < frequency->getContents().size(); ++i) {
            const Form* form = static_cast<const Form*>(frequency->getContents()[i].get());
            overlayPresets[i - 1].frequency = validator::toInt32(form->getText()).value_or(0U);
        }
        for (int i = 1; i < deposite->getContents().size(); ++i) {
            const Form* form = static_cast<const Form*>(deposite->getContents()[i].get());
            overlayPresets[i - 1].deposite = validator::toInt32(form->getText()).value_or(0U);
        }
        return overlayPresets;
    }
};

static void createWorld(Engine& engine, Form* _seed, Form* width, Form* height, OProps* oprops) {
    TileCoord mapSize;
    uint64_t seed = validator::toUint64(_seed->getText()).value_or(0U);
    mapSize.x = validator::toInt32(width->getText()).value_or(100);
    mapSize.y = validator::toInt32(height->getText()).value_or(100);
    WorldProperties properties(mapSize, seed, oprops->getPresets());
    engine.createWorldInGame(properties);
}

std::unique_ptr<Container> frontend::initWorldProperties(Engine& engine) {
    auto propsForm = std::make_unique<Container>(Align::centre, Orientation::vertical);
    auto parameters = propsForm->addNode(new Layout(Orientation::horizontal));
    auto lower      = propsForm->addNode(new Layout(Orientation::horizontal));
    auto labels = parameters->addNode(new Layout(Orientation::vertical));
    auto forms  = parameters->addNode(new Layout(Orientation::vertical));
    auto oProps = parameters->addNode(new OProps());

    labels->addNode(new Label(U"Seed"));
    labels->addNode(new Label(U"Width"));
    labels->addNode(new Label(U"Height"));
    auto seedF =   forms->addNode(new Form(0U, new Uint64Validator(0U, std::numeric_limits<uint64_t>::max())));
    auto widthF =  forms->addNode(new Form(100, new Int32Validator(20, 5000)));
    auto heightF = forms->addNode(new Form(100, new Int32Validator(20, 5000)));

    auto back = lower->addNode(new Button(BTN_SIZE, U"Back"));
    auto aply = lower->addNode(new Button(BTN_SIZE, U"Aply"));
    back->addCallback([container = propsForm.get()] { container->close(); });
    aply->addCallback([=, &engine] { createWorld(engine, seedF, widthF, heightF, oProps); });

    propsForm->arrange();
    return propsForm;
}
