#include "frontend.hpp"
//
#include <MINGUI/render/render_queue.hpp>
#include <MINGUI/widgets/button.hpp>
#include <MINGUI/widgets/icon.hpp>
#include "engine/engine.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "engine/settings/localization.hpp"

class FrInvSlot : public Node {
    std::string count;
    std::unique_ptr<TextureBridge> item;
public:
    FrInvSlot(TextureBridge* item, const int64_t count) :
        item(item), count(std::format("{}", count)) {
        setSize(Point(48, 16));
    }
    void callback(UIContext& context) final {/*empty*/ }
    void draw(RenderQueue& queue) final {
        constexpr Point GLYPH_SIZE = Point(8, 16);
        constexpr Point ICON_SIZE = Point(16, 16);
        constexpr Point OFFSET = Point(16, 0);
        queue.add(getPosition(), ICON_SIZE, item.get());
        queue.add(getPosition() + OFFSET, GLYPH_SIZE, count, 0xFF'FF'FF'FF);
    }
};

class FrBlockInfo: public Container {
public:
    FrBlockInfo(Engine& engine, const uint8_t blockPresetID) : Container(Align::center, Orientation::vertical) {
        setPadding(6.f);
        const BlockPresetID presetID = BlockPresetID(blockPresetID);
        const BlockPreset preset = engine.getAssets().getPresets().getBlock(presetID);

        std::string blockName;
        for (auto& [name, id] : engine.getAssets().getPresets().getBlocks()) {
            if (id == presetID) {
                blockName = name;
                break;
            }
        }

        addNode(new Icon(Point(32, 32), new T1_UITexture(engine.getAssets().getAtlas().at(blockName))));
        addNode(new Label(tr(blockName)));
        addNode(new Label(tr("Size: {}x{}", preset.size, preset.size)));
        addNode(new Label(tr("Build time: {} ticks", preset.buildTime)));
        addNode(new Label(tr("Durability: {}", preset.maxHealth)));
        addNode(new Label(tr("Cost:")));

        auto ingridients = addNode(new Layout(Orientation::horizontal));
        ingridients->setPalette(NULL_PALETTE);
        for (const auto ing : preset.recipe) {
            if (ing.itemID.asUint() == 0)
                break;
            if (ing.amount < 1)
                continue;
            const ItemPreset& preset = engine.getAssets().getPresets().getItem(ing.itemID);
            ingridients->addNode(new FrInvSlot(new T1_UITexture(preset.textureRect), ing.amount));
        }

        addNode(new Button(Point(260, 30), tr("Back")))->addCallback([&] { close(); });
    }

    void draw(RenderQueue& queue) final {
        queue.add(Point(0, 0), Point(4000, 4000), DEFAULT_PALETTE.idle);
        Container::draw(queue);
    }
};

std::unique_ptr<Container> frontend::initBlockInfo(Engine& engine, const uint8_t blockPresetID) {
    return std::make_unique<FrBlockInfo>(engine, blockPresetID);
}
