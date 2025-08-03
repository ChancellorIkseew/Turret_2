#include "frontend.hpp"
//
#include "engine/gui/gui_util/tile_data.hpp"
#include "engine/widgets/label.hpp"
#include "engine/window/input/utf8/utf8.hpp"
#include "game/world/world.hpp"

constexpr int ROW_SIZE = 6;
constexpr PixelCoord BTN_SIZE(32.0f, 32.0f);

std::unique_ptr<Container> frontend::initHint(TileData& tileData, const ElementRegistry& reg) {
    auto hint = std::make_unique<Container>(Align::right | Align::down, Orientation::vertical);
    
    std::string hintStr;

    switch (tileData.component) {
    case TileComponent::floor:   hintStr = reg.floorTypes.at(tileData.id);   break;
    case TileComponent::overlay: hintStr = reg.overlayTypes.at(tileData.id); break;
    }

    std::u32string hintStrU32 = utf8::fromConstCharToU32String(reg.floorTypes.at(tileData.id).c_str());
    auto label = std::make_unique<Label>(hintStrU32);
    hint->addNode(label.release());
    return hint;
}
