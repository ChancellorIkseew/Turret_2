#include "frontend.hpp"
//
#include <MINGUI/widgets/icon.hpp>
#include <MINGUI/widgets/label.hpp>
#include "app.hpp"
#include "engine/gui/t1_ui_renderer.hpp"
#include "engine/util/string_util.hpp"
#include "game/game_session.hpp"
#include "game/world/world.hpp"

template<class Map, class ID>
const std::string& getNameByID(const Map& findMap, ID id) {
    for (const auto& [name, mappedID] : findMap) {
        if (mappedID == id) return name;
    }
    static const std::string empty;
    return empty;
}

constexpr Point ICON_SIZE(16, 16);

class FrHint : public Container {
    Icon*  icon  = nullptr;
    Label* label = nullptr;
    Label* position = nullptr;
    App& app;
public:
    FrHint(App& app) : Container(Align::down | Align::center, Orientation::horizontal), app(app) {
        icon = addNode(new Icon(ICON_SIZE, nullptr));
        label = addNode(new Label(""));
        position = addNode(new Label(""));
    }

    void callback(UIContext& context) final {
        const PixelCoord mousePosition = app.getMainWindow().getInput().getMouseCoord();
        const Camera& camera = app.getSession().getCamera();
        const WorldMap& map = app.getSession().getWorld().getMap();
        const auto& presets = app.getAssets().getPresets();

        const TileCoord targetTile = t1::tile(camera.fromScreenToMap(mousePosition));
        if (!map.tileExists(targetTile))
            return;
        const uint8_t index = map.at(targetTile).ore.asUint();
        std::string trimedName;
        if (index != 0) {
            const OrePreset& orePreset = presets.getOre(OrePresetID(index));
            trimedName = util::removePrefix(getNameByID(presets.getOres(), OrePresetID(index)), "ore_");
            icon->setTexture(new T1_UITexture(presets.getItem(orePreset.item).textureRect));
        }
        else {
            const FloorPreset& floorPreset = presets.getFloor(FloorPresetID(index));
            trimedName = util::removePrefix(getNameByID(presets.getFloors(), FloorPresetID(index)), "floor_");
            icon->setTexture(new T1_UITexture(floorPreset.textureRect));
        }
        label->setText(tr(trimedName));
        position->setText(std::format("X:{} Y:{}", targetTile.x, targetTile.y));
        markDirty();
    }
};

std::unique_ptr<Container> frontend::initHint(App& app) {
    return std::make_unique<FrHint>(app);
}
