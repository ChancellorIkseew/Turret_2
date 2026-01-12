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

MapDrawer::MapDrawer(const Assets& assets) {
    for (const auto& [name, id] : assets.getIndexes().getFloor()) {
        cachedFloor.emplace(id, std::vector<PixelCoord>());
        floorTextures.emplace(id, assets.getAtlas().at(name));
    }
    for (const auto& [name, id] : assets.getIndexes().getOverlay()) {
        cachedOverlay.emplace(id, std::vector<PixelCoord>());
        overlayTextures.emplace(id, assets.getAtlas().at(name));
    }
    atlasSize = assets.getAtlas().getSize();
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

void MapDrawer::cacheOverlay(const WorldMap& map) {
    for (auto& [_type, layer] : cachedOverlay) {
        layer.clear();
    }
    //
    for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
        for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
            if (map.at(x, y).overlay != 0)
                cachedOverlay.at(map.at(x, y).overlay).push_back(t1::pixel(x, y));
        }
    }
}

void MapDrawer::renderLayer(
    const Renderer& renderer,
    const std::map<uint8_t, std::vector<PixelCoord>>& cachedLayer,
    const std::map<uint8_t, Texture>& textures,
    const PixelCoord tileSize,
    const PixelCoord translation) {

    for (const auto& [tileType, layer] : cachedLayer) {
        if (layer.empty())
            continue;

        positions.clear();
        uvs.clear();
        indexCache.clear();
        
        const SDL_FRect& uvRect = textures.at(tileType).rect;
        float u0 = uvRect.x / atlasSize.x;
        float v0 = uvRect.y / atlasSize.y;
        float u1 = (uvRect.x + uvRect.w) / atlasSize.x;
        float v1 = (uvRect.y + uvRect.h) / atlasSize.y;
        
        for (const PixelCoord position : layer) {
            float x = static_cast<float>(position.x - translation.x);
            float y = static_cast<float>(position.y - translation.y);
            float w = static_cast<float>(tileSize.x);
            float h = static_cast<float>(tileSize.y);

            int baseIdx = static_cast<int>(positions.size() / 2);

            positions.insert(positions.end(), {
                x, y,        // TL
                x + w, y,    // TR
                x, y + h,    // BL
                x + w, y + h // BR
                });

            uvs.insert(uvs.end(), { u0, v0, u1, v0, u0, v1, u1, v1 });

            indexCache.insert(indexCache.end(), {
                baseIdx + 0, baseIdx + 1, baseIdx + 2,
                baseIdx + 2, baseIdx + 1, baseIdx + 3
                });
        }

        renderer.drawBatched(positions.data(), uvs.data(), indexCache.data(),
            static_cast<int>(positions.size() / 2), static_cast<int>(indexCache.size()));
    }
}

void MapDrawer::draw(const Camera& camera, const Renderer& renderer, const WorldMap& map) {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    if (cashedStart != start || cashedEnd != end || Events::active(Event::map_changed)) {
        cashedStart = start;
        cashedEnd = end;
        cacheFloor(map);
        cacheOverlay(map);
    }
    renderLayer(renderer, cachedFloor, floorTextures, FLOOR_SIZE, camera.getTranslation() + BLENDING_AREA);
    renderLayer(renderer, cachedOverlay, overlayTextures, TILE_SIZE, camera.getTranslation());
    drawStructures(camera, map);
}

void MapDrawer::drawStructures(const Camera& camera, const WorldMap& map) {
    const TileCoord start = camera.getStartTile();
    const TileCoord end = camera.getEndTile();
    for (int x = cashedStart.x; x < cashedEnd.x; ++x) {
        for (int y = cashedStart.y; y < cashedEnd.y; ++y) {
            if (map.at(x, y).block)
                map.at(x, y).block->draw();
        }
    }
}
