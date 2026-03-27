#include "frontend.hpp"
//
#include "MINGUI/widgets/icon.hpp"
#include "MINGUI/widgets/label.hpp"
#include "engine/engine.hpp"
#include "engine/game_session.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "engine/util/string_util.hpp"
#include "game/world/world.hpp"

constexpr Point ICON_SIZE(16.0f, 16.0f);

class FrHint : public Container {
    Icon*  icon  = nullptr;
    Label* label = nullptr;
    Engine& engine;
public:
    FrHint(Engine& engine) : Container(Align::right | Align::centre, Orientation::horizontal), engine(engine) {
        icon = addNode(new Icon(ICON_SIZE, nullptr));
        label = addNode(new Label(""));
    }

    void callback(UIContext& context) final {
        const PixelCoord mousePosition = engine.getMainWindow().getInput().getMouseCoord();
        const Camera& camera = engine.getSession().getCamera();
        const WorldMap& map = engine.getSession().getWorld().getMap();
        const auto& assets = engine.getAssets();

        const TileCoord targetTile = t1::tile(camera.fromScreenToMap(mousePosition));
        if (!map.tileExists(targetTile))
            return;
        const uint8_t index = map.at(targetTile).overlay;
        std::string trimedName;
        if (index != 0) {
            const std::string& overlayName = assets.getIndexes().getOverlayByIndex(index);
            trimedName = util::removePrefix(overlayName, "overlay_");
            icon->setTexture(new T1_UITexture(assets.getAtlas().at("item_" + trimedName)));
        }
        else {
            const uint8_t floorIndex = map.at(targetTile).floor;
            const std::string& floorName = assets.getIndexes().getFloorByIndex(floorIndex);
            trimedName = util::removePrefix(floorName, "floor_");
            icon->setTexture(new T1_UITexture(assets.getAtlas().at(floorName)));
        }
        label->setText(trimedName);
        markDirty();
    }
};

std::unique_ptr<Container> frontend::initHint(Engine& engine) {
    return std::make_unique<FrHint>(engine);
}
