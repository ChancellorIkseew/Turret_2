#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/render/text.hpp"
#include "engine/widgets/button.hpp"
#include "engine/widgets/form.hpp"
#include "game/generation/generation.hpp"
#include "game/world_saver/gen_preset_saver.hpp"

constexpr uint64_t MAX_SEED = std::numeric_limits<uint64_t>::max();
constexpr PixelCoord BTN_SIZE(120.0f, 30.0f);
constexpr PixelCoord ICON_SIZE(0.0f, 20.0f);

class OProps : public Layout {
    OverlayPresets overlayPresets;
    Layout* labels    = nullptr;
    Layout* frequency = nullptr;
    Layout* deposite  = nullptr;
public:
    OProps() : Layout(Orientation::horizontal),
        overlayPresets(serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT)) {
        labels    = addNode(new Layout(Orientation::vertical));
        frequency = addNode(new Layout(Orientation::vertical));
        deposite  = addNode(new Layout(Orientation::vertical));

        frequency->addNode(new Label(U"Frequency"));
        deposite ->addNode(new Label(U"Deposite"));
        labels   ->addNode(new Label(U""/*empty label*/));

        for (const auto& [id, f, d] : overlayPresets) {
            std::u32string str;
            str += text::getCustomSymbol(id);
            labels->addNode(new Label(str, false))->setSize(ICON_SIZE);
            frequency->addNode(new Form(f, new Int32Validator(0, 10000)));
            deposite->addNode(new Form(d, new Int32Validator(0, 100)));
        }
        arrange();
    }
    //
    const OverlayPresets getPresets() {
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

class FrWorldProperties : public Container {
    Form* seed   = nullptr;
    Form* width  = nullptr;
    Form* height = nullptr;
    OProps* oProps = nullptr;
public:
    ~FrWorldProperties() final = default;
    FrWorldProperties(Engine& engine) : Container(Align::centre, Orientation::vertical) {
        auto main = addNode(new Layout(Orientation::horizontal));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        labels->addNode(new Label(U"Seed"));
        labels->addNode(new Label(U"Width"));
        labels->addNode(new Label(U"Height"));

        auto forms = main->addNode(new Layout(Orientation::vertical));
        seed   = forms->addNode(new Form(0U, new Uint64Validator(0U, MAX_SEED)));
        width  = forms->addNode(new Form(100, new Int32Validator(20, 5000)));
        height = forms->addNode(new Form(100, new Int32Validator(20, 5000)));

        oProps = main->addNode(new OProps());

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, U"Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, U"Apply"))->addCallback([&] { createWorld(engine); });

        arrange();
    }
private:
    void createWorld(Engine& engine) {
        WorldProperties properties(
            TileCoord(validator::toInt32(width->getText()).value_or(100),
                validator::toInt32(height->getText()).value_or(100)),
            validator::toUint64(seed->getText()).value_or(0U),
            serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT),
            oProps->getPresets());
        engine.createWorldInGame(properties);
    }
};

std::unique_ptr<Container> frontend::initWorldProperties(Engine& engine) {
    return std::make_unique<FrWorldProperties>(engine);
}
