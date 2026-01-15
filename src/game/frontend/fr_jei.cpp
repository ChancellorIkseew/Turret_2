#include "frontend.hpp"
//
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/gui.hpp"
#include "engine/widgets/image_button.hpp"
#include "game/world/world.hpp"

enum class TileComponent : uint8_t { floor, overlay, block };
constexpr int ROW_SIZE = 6;
constexpr PixelCoord BTN_SIZE(32.0f, 32.0f);

struct TileData {
    TileComponent component = TileComponent::floor;
    uint8_t id = 0U;
};

class JEI : public Container {
    Engine& engine;
    TileData tileData;
public:
    JEI(Engine& engine) : Container(Align::right | Align::down, Orientation::vertical), engine(engine) {
        int btnsCount = 0;
        auto line = std::make_unique<Layout>(Orientation::horizontal);

        for (const auto& [floorName, id] : engine.getAssets().getIndexes().getFloor()) {
            addButton(floorName, id, TileComponent::floor, btnsCount, line);
        }
        for (const auto& [overlayName, id] : engine.getAssets().getIndexes().getOverlay()) {
            addButton(overlayName, id, TileComponent::overlay, btnsCount, line);
        }

        tileData.id = engine.getAssets().getIndexes().getFloor().begin()->second; // Reset tileData to avoid errors.

        if (btnsCount != 0)
            addNode(line.release());
        arrange();
    }

    void addButton(const std::string& name, int id, TileComponent component, int& btnsCount, std::unique_ptr<Layout>& line) {
        auto btn = line->addNode(new ImageButton(BTN_SIZE, engine.getAssets().getAtlas().at(name)));
        btn->addCallback([id, component, this]() {
            tileData.id = id;
            tileData.component = component;
            });
        btnsCount++;
        if (btnsCount >= ROW_SIZE) {
            this->addNode(line.release());
            line = std::make_unique<Layout>(Orientation::horizontal);
            btnsCount = 0;
        }
    }

    void callback(UIContext& context) override {
        Container::callback(context);
        if (!context.input.active(Build) || engine.getGUI().ownsMouse())
            return;
        WorldMap& map = engine.getSession().getWorld().getMap();
        const TileCoord tile = t1::tile(engine.getSession().getCamera().fromScreenToMap(context.input.getMouseCoord()));
        switch (tileData.component) {
        case TileComponent::floor:   map.placeFloor(tile, tileData.id);   break;
        case TileComponent::overlay: map.placeOverlay(tile, tileData.id); break;
        case TileComponent::block: break;
        }
    }
};

std::unique_ptr<Container> frontend::initJEI(Engine& engine) {
    return std::make_unique<JEI>(engine);
}
