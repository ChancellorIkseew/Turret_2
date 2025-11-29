#include "frontend.hpp"
//
#include "engine/gui/gui_util/tile_data.hpp"
#include "engine/widgets/image_button.hpp"
#include "game/content/indexes.hpp"
#include "game/world/world.hpp"

constexpr int ROW_SIZE = 6;
constexpr PixelCoord BTN_SIZE(32.0f, 32.0f);

std::unique_ptr<Container> frontend::initJEI(TileData& tileData) {
    auto jei = std::make_unique<Container>(Align::right | Align::down, Orientation::vertical);
    int btns = 0;
    auto line = std::make_unique<Layout>(Orientation::horizontal);

    auto addButton = [&](const std::string& name, int id, TileComponent component) {
        auto btn = line->addNode(new ImageButton(BTN_SIZE, name));
        btn->addCallback([id, component, &tileData]() {
            tileData.id = id;
            tileData.component = component;
            });
        btns++;
        if (btns >= ROW_SIZE) {
            jei->addNode(line.release());
            line = std::make_unique<Layout>(Orientation::horizontal);
            btns = 0;
        }
    };

    for (const auto& [floorName, id] : content::Indexes::getFloor()) {
        addButton(floorName, id, TileComponent::floor);
    }
    for (const auto& [overlayName, id] : content::Indexes::getOverlay()) {
        addButton(overlayName, id, TileComponent::overlay);
    }
    for (const auto& [blockName, id] : content::Indexes::getBlocks()) {
        addButton(blockName, id, TileComponent::block);
    }

    tileData.component = TileComponent::floor; // Reset tileData to avoid errors.
    tileData.id = content::Indexes::getFloor().begin()->second;

    if (btns != 0)
        jei->addNode(line.release());
    jei->arrange();
    return jei;
}
