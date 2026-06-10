#include "map_drawer.hpp"
//
#include "engine/assets/assets.hpp"
#include "engine/coords/transforms.hpp"
#include "engine/render/atlas.hpp"
#include "engine/render/renderer.hpp"
#include "game/events/events.hpp"
#include "game/player/camera.hpp"
#include "game/world/world_map.hpp"

constexpr PixelCoord BLENDING_AREA(4.0f, 4.0f);
constexpr PixelCoord TILE_SIZE(t1::TILE_F, t1::TILE_F);
constexpr PixelCoord FLOOR_SIZE(t1::TILE_F + 8.0f, t1::TILE_F + 8.0f);

void MapDrawer::updateTextures(const Assets& assets) {
    for (const auto& [name, id] : assets.getIndexes().getFloor()) {
        cachedFloor.emplace(id, std::vector<PixelCoord>());
        floorTextures.emplace(id, assets.getAtlas().at(name));
    }
    for (const auto& [name, id] : assets.getPresets().getOres()) {
        oreTextures.emplace(id.asUint(), assets.getAtlas().at(name));
    }
}

void MapDrawer::cacheFloor(const WorldMap& map) {
    for (auto& [_type, layer] : cachedFloor) {
        layer.clear();
    }
    //
    for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
        for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
            if (map.at(x, y).floor != 0)
                cachedFloor.at(map.at(x, y).floor).push_back(t1::pixel(x, y));
        }
    }
}

void MapDrawer::renderFloor(Renderer& renderer) {
    for (const auto& [tileType, layer] : cachedFloor) {
        if (layer.empty())
            continue;
        const TextureRect textureRect = floorTextures.at(tileType);
        for (const PixelCoord position : layer) {
            renderer.draw(textureRect, position - BLENDING_AREA, FLOOR_SIZE);
        }
    }
}

void MapDrawer::draw(const Camera& camera, Renderer& renderer, const WorldMap& map) {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    if (cashedStart != start || cashedEnd != end || Events::active(Event::map_changed)) {
        cashedStart = start;
        cashedEnd = end;
        cacheFloor(map);
    }

    renderFloor(renderer);
    for (int x = start.x; x < end.x; ++x) {
        for (int y = start.y; y < end.y; ++y) {
            uint8_t ore = map.at(x, y).ore.asUint();
            if (map.at(x, y).ore != OrePresetID(0))
                renderer.draw(oreTextures[ore], t1::pixel(x, y), TILE_SIZE);
        }
    }
}
