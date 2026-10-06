#include "frontend.hpp"
//
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/checkbox.hpp>
#include <MINGUI/widgets/form.hpp>
#include <MINGUI/widgets/icon_button.hpp>
#include <random>
#include "engine/engine.hpp"
#include "engine/io/folders.hpp"
#include "engine/io/parser/validator.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "engine/render/text.hpp"
#include "engine/util/string_util.hpp"
#include "engine/util/time.hpp"
#include "game/generation/generation.hpp"
#include "game/world_saver/gen_preset_saver.hpp"

constexpr uint64_t MAX_SEED = std::numeric_limits<uint64_t>::max();
constexpr Point BTN_SIZE(120, 30);
constexpr Point ICON_SIZE(16, 16);

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
        icons->setMargin(8.f);
        icons->setPadding(8.f);

        frequency->addNode(new Label(tr("Frequency")));
        deposite ->addNode(new Label(tr("Deposite")));
        icons    ->addNode(new Icon(Point(16, 10), nullptr));

        for (const auto& [id, f, d] : overlayPresets) {
            std::string itemName = util::swapPrefix(id, "item_");
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

class OtherWorldSettings : public Layout {
    Form* blockCostMul  = nullptr;
    Form* enemyCountMul = nullptr;
    Form* waveSpacing   = nullptr;
    Checkbox* wavesByTimer = nullptr;
public:
    OtherWorldSettings() : Layout(Orientation::horizontal) {
        auto labels    = addNode(new Layout(Orientation::vertical));
        frontend::useLabelsSpacing(labels);
        labels->addNode(new Label(tr("block cost multiplier")));
        labels->addNode(new Label(tr("enemy count multiplier")));
        labels->addNode(new Label(tr("wave spacing (seconds)")));
        labels->addNode(new Label(tr("launch waves by timer")));
        auto clickable = addNode(new Layout(Orientation::vertical));
        blockCostMul  = clickable->addNode(new Form(1, new Uint8Validator(0, 20)));
        enemyCountMul = clickable->addNode(new Form(1, new Uint8Validator(1, 10)));
        waveSpacing   = clickable->addNode(new Form(180, new Uint64Validator(1, 60 * 7200)));
        wavesByTimer = clickable->addNode(new Checkbox(true));
    }

    void apply(WorldConfig& config) {
        config.blockCostMul = validator::to<int16_t>(blockCostMul->getText()).value_or(1);
        config.enemyCountMul = validator::to<uint32_t>(enemyCountMul->getText()).value_or(1);
        config.ticksPerWave = validator::to<uint64_t>(waveSpacing->getText()).value_or(1) * 60;
        config.wavesByTimer = wavesByTimer->getValue();
    }
};

class FrWorldProperties : public Container {
    IconButton* regenSeed = nullptr;
    Form* seed   = nullptr;
    Form* width  = nullptr;
    Form* height = nullptr;
    OProps* oProps = nullptr;
    OtherWorldSettings* otherSettings = nullptr;
public:
    ~FrWorldProperties() final = default;
    FrWorldProperties(Engine& engine) : Container(Align::center, Orientation::vertical) {
        const Atlas& atlas = engine.getAssets().getAtlas();
        auto main = addNode(new Layout(Orientation::horizontal));

        auto labels = main->addNode(new Layout(Orientation::vertical));
        frontend::useLabelsSpacing(labels);
        labels->addNode(new Label(tr("Seed")));
        labels->addNode(new Label(tr("Width")));
        labels->addNode(new Label(tr("Height")));

        auto forms = main->addNode(new Layout(Orientation::vertical));
        auto seedL = forms->addNode(new Layout(Orientation::horizontal));
        seedL->setPadding(0.f);
        seedL->setMargin(0.f);
        seedL->setPalette(NULL_PALETTE);
        seed = seedL->addNode(new Form(0U, new Uint64Validator(0U, MAX_SEED)));
        regenSeed = seedL->addNode(new IconButton(PixelCoord(20, 20), 2.f, new T1_UITexture(atlas.at("retry_btn"))));
        width  = forms->addNode(new Form(100, new Int32Validator(20, 5000)));
        height = forms->addNode(new Form(100, new Int32Validator(20, 5000)));

        oProps = main->addNode(new OProps(atlas));

        otherSettings = addNode(new OtherWorldSettings());

        auto lower = addNode(new Layout(Orientation::horizontal));
        lower->addNode(new Button(BTN_SIZE, tr("Back")))->addCallback([&] { close(); });
        lower->addNode(new Button(BTN_SIZE, tr("Apply")))->addCallback([&] { createWorld(engine); });

        regenSeed->addCallback([&] { generateSeed(); });
    }
private:
    void generateSeed() {
        std::mt19937_64 randomizer(util::time::getLocalTimeMilliseconds());
        std::uniform_int_distribution<uint64_t> dist;
        seed->setText(std::format("{}", dist(randomizer)));
    }

    void createWorld(Engine& engine) {
        WorldConfig config;
        otherSettings->apply(config);
        config.seed = validator::to<uint64_t>(seed->getText()).value_or(0U);
        config.mapSize = TileCoord(validator::to<int>(width ->getText()).value_or(100),
                                   validator::to<int>(height->getText()).value_or(100));
        WorldProperties properties(config, serializer::loadFloorPreset(io::folders::GENERATION_DEFAULT), oProps->getPresets());
        engine.createWorldInGame(properties);
    }
};

std::unique_ptr<Container> frontend::initWorldProperties(Engine& engine) {
    return std::make_unique<FrWorldProperties>(engine);
}
