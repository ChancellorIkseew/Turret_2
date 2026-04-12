#include "frontend.hpp"
//
#include <MINGUI/widgets/grid_layout.hpp>
#include <MINGUI/widgets/icon.hpp>
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/gui.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "game/world/world.hpp"

class JEI;
enum class TileComponent : uint8_t { floor, overlay, block };
constexpr int ROW_SIZE = 6;
constexpr Point BTN_SIZE(32.0f, 32.0f);

struct TileData {
    TileComponent component = TileComponent::floor;
    uint8_t id = 0U;
};

class JEISlot : public mingui::Icon {
    TileData tileData;
    JEI* jei;
public:
    JEISlot(const Point size, TextureBridge* texture, JEI* jei, TileData tileData) :
        Icon(size, texture), jei(jei), tileData(tileData) {
    }
    ~JEISlot() final = default;
    void callback(UIContext& context) final;
};

class JEI : public Container {
    Engine& engine;
    TileData tileData;
    BlockRot rotation = up;
public:
    JEI(Engine& engine) : Container(Align::right | Align::down, Orientation::vertical), engine(engine) {
        auto grid = addNode(new GridLayout(GridType::from_rows, ROW_SIZE));
        grid->setPalette(transparentPalette);

        for (const auto& [floorName, id] : engine.getAssets().getIndexes().getFloor()) {
            addButton(floorName, id, TileComponent::floor, grid);
        }
        for (const auto& [overlayName, id] : engine.getAssets().getIndexes().getOverlay()) {
            addButton(overlayName, id, TileComponent::overlay, grid);
        }
        for (const auto& [blockName, id] : engine.getAssets().getPresets().getBlocks()) {
            addButton(blockName, id.asUint(), TileComponent::block, grid);
        }

        tileData.id = engine.getAssets().getIndexes().getFloor().begin()->second; // Reset tileData to avoid errors.
    }

    void addButton(const std::string& name, int id, TileComponent component, GridLayout* grid) {
        const Texture texture = engine.getAssets().getAtlas().at(name);
        grid->addNode(new JEISlot(BTN_SIZE, new T1_UITexture(texture), this, TileData(component, id)));
    }

    void callback(UIContext& context) override {
        Container::callback(context);
        const Input& input = engine.getMainWindow().getInput();
        if (input.jactive(Rotate_building))
            rotation = static_cast<BlockRot>((rotation + 1) % 4);
        if (!input.active(Build) || engine.getGUI().ownsMouse())
            return;
        GameSession& session = engine.getSession();
        WorldMap& map = session.getWorld().getMap();
        const TileCoord tile = t1::tile(session.getCamera().fromScreenToMap(input.getMouseCoord()));
        switch (tileData.component) {
        case TileComponent::floor:   map.placeFloor(tile, tileData.id);   break;
        case TileComponent::overlay: map.placeOverlay(tile, tileData.id); break;
        case TileComponent::block: {
            const TeamID teamID = session.getPlayerController().getPlayerTeam()->getID();
            session.getBuiltInScripts().placeBlock(BlockPresetID(tileData.id), tile, teamID, rotation);
            break;
        }
        }
    }

    void setTileData(const TileData tileData) {
        this->tileData = tileData;
    }
};

void JEISlot::callback(UIContext& context){
    if (context.clicked(*this))
        jei->setTileData(tileData);
}

std::unique_ptr<Container> frontend::initJEI(Engine& engine) {
    return std::make_unique<JEI>(engine);
}
