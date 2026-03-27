#include "frontend.hpp"
//
#include "MINGUI/widgets/button.hpp"
#include "MINGUI/widgets/icon.hpp"
#include "MINGUI/widgets/form.hpp"
#include "engine/engine.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "engine/render/text.hpp"
#include "engine/util/string_util.hpp"
#include "game/generation/generation.hpp"
#include "game/world_saver/gen_preset_saver.hpp"

constexpr uint64_t MAX_SEED = std::numeric_limits<uint64_t>::max();
constexpr Point BTN_SIZE(120.0f, 30.0f);
constexpr Point ICON_SIZE(16.0f, 16.0f);

class OProps : public Layout {
    OverlayPresets overlayPresets;
    Layout* icons     = nullptr;
    Layout* frequency = nullptr;
    Layout* deposite  = nullptr;
public:
    OProps(const Atlas& atlas) : Layout(Orientation::horizontal),
        overlayPresets(serializer::loadOverlayPreset(io::folders::GENERATION_DEFAULT)) {
        icons     = addNode(new Layout(Orientation::vertical));
        frequency = addNode(new Layout(Orientation::vertical));
        deposite  = addNode(new Layout(Orientation::vertical));
        icons->setMargin(8.0f);
        icons->setPadding(8.0f);

        frequency->addNode(new Label("Frequency"));
        deposite ->addNode(new Label("Deposite"));
        icons    ->addNode(new Icon(ICON_SIZE, nullptr));

        for (const auto& [id, f, d] : overlayPresets) {
            std::string itemName = util::removePrefix(id, "item_");
            icons->addNode(new Icon(ICON_SIZE, new T1_UITexture(atlas.at(itemName))));
            frequency->addNode(new Form(f, new Int32Validator(0, 10000)));
            deposite->addNode(new Form(d, new Int32Validator(0, 100)));
        }
    }
    //
    const OverlayPresets getPresets() {
        for (int i = 1; i < frequency->getContents().size(); ++i) {
            using T = decltype(overlayPresets[i - 1].frequency);
            const Form* form = static_cast<const Form*>(frequency->getContents()[i].get());
            overlayPresets[i - 1].frequency = validator::to<T>(form->getText()).value_or(0U);
        }
        for (int i = 1; i < deposite->getContents().size(); ++i) {
            using T = decltype(overlayPresets[i - 1].deposite);
            const Form* form = static_cast<const Form*>(deposite->getContents()[i].get());
            overlayPresets[i - 1].deposite = validator::to<T>(form->getText()).value_or(0U);
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
        labels->addNode(new Label("Seed"));
        labels->addNode(new Label("Width"));
        labels->addNode(new Label("Height"));

        auto forms = main->addNode(new Layout(Orientation::vertical));
        seed   = forms->addNode(new Form(0U, new Uint64Validator(0U, MAX_SEED)));
        width  = forms->addNode(new Form(100, new Int32Validator(20, 5000)));
        height = forms->addNode(new Form(100, new Int32Validator(20, 5000)));

        oProps = main->addNode(new OProps(engine.getAssets().getAtlas()));

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, "Back"))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, "Apply"))->addCallback([&] { createWorld(engine); });
    }
private:
    void createWorld(Engine& engine) {
        WorldProperties properties(
            TileCoord(validator::to<int>(width->getText()).value_or(100),
                validator::to<int>(height->getText()).value_or(100)),
            validator::to<uint64_t>(seed->getText()).value_or(0U),
            serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT),
            oProps->getPresets());
        engine.createWorldInGame(properties);
    }
};

std::unique_ptr<Container> frontend::initWorldProperties(Engine& engine) {
    return std::make_unique<FrWorldProperties>(engine);
}
