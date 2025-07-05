#include "frontend.hpp"
//
#include "engine/tile_data.hpp"
#include "engine/widgets/image_button.hpp"
#include "game/world/world.hpp"

constexpr int ROW_SIZE = 4;
constexpr PixelCoord BTN_SIZE(32.0f, 32.0f);

std::unique_ptr<Container> frontend::initJEI(TileData& tileData, const ElementRegistry& reg) {
    auto jei = std::make_unique<Container>(Align::right | Align::down, Orientation::vertical);
    int btns = 0;
    auto line = std::make_unique<Layout>(Orientation::horizontal);

    auto addButton = [&](const std::string& name, int id, TileComponent component) {
        auto btn = std::make_unique<ImageButton>(BTN_SIZE, name);
        btn->addCallback([id, component, &tileData]() {
            tileData.id = id;
            tileData.component = component;
            });
        line->addNode(btn.release());
        btns++;
        if (btns > ROW_SIZE) {
            jei->addNode(line.release());
            line = std::make_unique<Layout>(Orientation::horizontal);
            btns = 0;
        }
    };

    for (const auto& [id, floorName] : reg.floorTypes) {
        addButton(floorName, id, TileComponent::floor);
    }
    for (const auto& [id, overlayName] : reg.overlayTypes) {
        addButton(overlayName, id, TileComponent::overlay);
    }

    if (btns != 0)
        jei->addNode(line.release());
    jei->arrange();
    return jei;
}
