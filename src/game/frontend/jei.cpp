#include "frontend.hpp"
//
#include "engine/parser/list_parser.hpp"
#include "engine/widgets/image_button.hpp"
#include "game/world/world.hpp"

constexpr PixelCoord BTN_SIZE(32.0f, 32.0f);

std::unique_ptr<Container> frontend::initJEI(TileData& tileData) {
    auto jei = std::make_unique<Container>(Align::right | Align::down, Orientation::vertical);
    int btns = 0;
    uint8_t id = 0;
    list::Data data = list::read("res/tiles.list").value();
    auto line = std::make_unique<Layout>(Orientation::horizontal);
    for (const auto& tileName : data) {
        auto btn = std::make_unique<ImageButton>(BTN_SIZE, tileName);
        btn->addCallback([id, &tileData]() { tileData.id = id; tileData.component = TileComponent::floor; });
        line->addNode(btn.release());
        ++id;
        ++btns;
        if (btns > 4) {
            btns = 0;
            jei->addNode(line.release());
            line = std::make_unique<Layout>(Orientation::horizontal);
        }
    }
    if (btns != 0)
        jei->addNode(line.release());
    jei->arrange();
    return jei;
}
