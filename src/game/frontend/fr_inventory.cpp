#include "frontend.hpp"
//
#include <MINGUI/core/utf8.hpp>
#include <MINGUI/widgets/grid_layout.hpp>
#include "engine/engine.hpp"
#include "engine/gui/gui.hpp"
#include "engine/gui/t1_ui_renderer.hpp"


constexpr int ROW_SIZE = 6;

class FrInvSlot : public Node {
    std::u32string count;
    std::unique_ptr<TextureBridge> item;
    ItemPresetID itemID;
public:
    FrInvSlot(TextureBridge* item, const ItemPresetID itemID, const uint32_t count) :
        item(item), itemID(itemID), count(formatCount(count)) { setSize(Point(48.f, 16.f)); }
    void callback(UIContext& context) final {/*empty*/}
    void draw(RenderQueue& queue) final {
        static constexpr Point ICON_SIZE = Point(16.f, 16.f);
        static constexpr Point OFFSET = Point(16.f, 0.f);
        queue.add(getPosition(), ICON_SIZE, item.get());
        queue.add(getPosition() + OFFSET, count, 0xFF'FF'FF'FF);
    }
    //
    ItemPresetID getItemID() const { return itemID; }
    void setItemCount(const uint64_t value) {
        count = formatCount(value);
    }
    static std::u32string formatCount(const uint64_t value) {
        if (value < 1000)
            return utf8::to_u32string(std::format("{}", value));
        if (value < 1'000'000)
            return utf8::to_u32string(std::format("{}K", value / 1000));
        return utf8::to_u32string(std::format("{}M", value / 1'000'000));
    }
};

class FrInventory : public Container {
    Engine& engine;
public:
    FrInventory(Engine& engine) : Container(Align::center | Align::up, Orientation::vertical),
        engine(engine) {
        auto grid = addNode(new GridLayout(GridType::from_rows, ROW_SIZE));
        grid->setPalette(transparentPalette);

        for (const auto& [name, id] : engine.getAssets().getPresets().getItems()) {
            const TextureRect textureRect = engine.getAssets().getAtlas().at(name);
            grid->addNode(new FrInvSlot(new T1_UITexture(textureRect), id, 0));
        }
    }

};

std::unique_ptr<Container> frontend::initInventory(Engine& engine) {
    return std::make_unique<FrInventory>(engine);
}
