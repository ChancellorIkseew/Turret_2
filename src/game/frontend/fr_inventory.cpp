#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/gui.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "game/world/world.hpp"

class FrInvSlot : public Node {
    std::string count;
    std::unique_ptr<TextureBridge> item;
    ItemPresetID itemID;
public:
    FrInvSlot(TextureBridge* item, const ItemPresetID itemID, const int64_t count) :
        item(item), itemID(itemID), count(formatCount(count)) { setSize(Point(48, 16)); }
    void callback(UIContext& context) final {/*empty*/}
    void draw(RenderBridge& renderBridge) final {
        constexpr Point GLYPH_SIZE = Point(8, 16);
        constexpr Point ICON_SIZE = Point(16, 16);
        constexpr Point OFFSET = Point(16, 0);
        renderBridge.drawSprite(Sprite(getPosition(), ICON_SIZE, item.get()));
        renderBridge.drawText(Text(getPosition() + OFFSET, GLYPH_SIZE, count, 0xFF'FF'FF'FF));
    }
    //
    ItemPresetID getItemID() const { return itemID; }
    void setItemCount(const int64_t value) {
        count = formatCount(value);
    }
    static std::string formatCount(const int64_t value) {
        if (value < 1000)
            return std::format("{}", value);
        if (value < 1'000'000)
            return std::format("{}K", value / 1000);
        return std::format("{}M", value / 1'000'000);
    }
};

class FrInventory : public Container {
    Engine& engine;
public:
    FrInventory(Engine& engine) : Container(Align::center | Align::up, Orientation::horizontal_grid),
        engine(engine) {
        setCollRowLimit(6);
        setPadding(6.f);
        for (const auto& [name, id] : engine.getAssets().getPresets().getItems()) {
            const TextureRect textureRect = engine.getAssets().getAtlas().at(name);
            addNode(new FrInvSlot(new T1_UITexture(textureRect), id, 0));
        }
    }

    void callback(UIContext& context) {
        const TeamID playerTeamID = engine.getSession().getPlayerController().getPlayerTeamID();
        Team* platerTeam = engine.getSession().getWorld().getTeams().getTeamByID(playerTeamID);
        Inventory& inventory = platerTeam->getInventory();
        for (const auto& node : getContents()) {
            auto slot = static_pointer_cast<FrInvSlot>(node);
            slot->setItemCount(inventory.resources[slot->getItemID().asUint()]);
        }
    }
};

std::unique_ptr<Container> frontend::initInventory(Engine& engine) {
    return std::make_unique<FrInventory>(engine);
}
